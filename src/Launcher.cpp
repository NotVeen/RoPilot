#include "Launcher.h"
#include "RobloxAPI.h"
#include "HandleCloser.h"
#include "Updater.h"
#include <windows.h>
#include <shlobj.h>
#include <filesystem>
#include <vector>
#include <thread>
#include <tlhelp32.h>
#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <mutex>
#include "ActiveClientLock.h"
#include "AccountManager.h"
#include "../vendor/json.hpp"

extern AccountManager g_accountManager;

namespace fs = std::filesystem;

namespace Launcher {

    HANDLE g_hMutex = NULL;
    HANDLE g_hEvent = NULL;
    std::mutex g_launchMutex;

    struct EnumData {
        DWORD dwProcessId;
        HWND hWnd;
    };

    BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
        EnumData* data = (EnumData*)lParam;
        DWORD processId = 0;
        GetWindowThreadProcessId(hwnd, &processId);
        if (data->dwProcessId == processId && IsWindowVisible(hwnd)) {
            data->hWnd = hwnd;
            return FALSE;
        }
        return TRUE;
    }

    bool HasWindow(DWORD processId) {
        EnumData data = { processId, NULL };
        EnumWindows(EnumWindowsProc, (LPARAM)&data);
        return data.hWnd != NULL;
    }

    void FocusWindow(DWORD processId) {
        EnumData data = { processId, NULL };
        EnumWindows(EnumWindowsProc, (LPARAM)&data);
        if (data.hWnd != NULL) {
            if (IsIconic(data.hWnd)) {
                ShowWindow(data.hWnd, SW_RESTORE);
            }
            SetForegroundWindow(data.hWnd);
        }
    }

    static std::string ExtractPathFromCommand(const std::string& cmd) {
        if (cmd.empty()) return "";
        std::string path;
        if (cmd.front() == '"') {
            size_t endQuote = cmd.find('"', 1);
            if (endQuote != std::string::npos) {
                path = cmd.substr(1, endQuote - 1);
            }
        } else {
            size_t space = cmd.find(' ');
            if (space != std::string::npos) {
                path = cmd.substr(0, space);
            } else {
                path = cmd;
            }
        }
        return path;
    }

    static std::string GetRegistryProtocolCommand() {
        HKEY hKey = NULL;
        char buffer[1024] = {0};
        DWORD bufferSize = sizeof(buffer);
        
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Classes\\roblox-player\\shell\\open\\command", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            if (RegQueryValueExA(hKey, NULL, NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                RegCloseKey(hKey);
                return std::string(buffer);
            }
            RegCloseKey(hKey);
        }

        bufferSize = sizeof(buffer);
        if (RegOpenKeyExA(HKEY_CLASSES_ROOT, "roblox-player\\shell\\open\\command", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            if (RegQueryValueExA(hKey, NULL, NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                RegCloseKey(hKey);
                return std::string(buffer);
            }
            RegCloseKey(hKey);
        }

        return "";
    }

    static std::string FindExecutableInVersions(const fs::path& versionsPath) {
        std::error_code ec;
        if (!fs::exists(versionsPath, ec) || !fs::is_directory(versionsPath, ec)) {
            return "";
        }

        std::string bestExe;
        fs::file_time_type bestTime;
        bool found = false;

        try {
            for (const auto& entry : fs::directory_iterator(versionsPath, fs::directory_options::skip_permission_denied, ec)) {
                if (ec) break;
                if (entry.is_directory(ec)) {
                    fs::path exePath = entry.path() / "RobloxPlayerBeta.exe";
                    if (fs::exists(exePath, ec)) {
                        auto writeTime = fs::last_write_time(exePath, ec);
                        if (!found || (!ec && writeTime > bestTime)) {
                            bestTime = writeTime;
                            bestExe = exePath.string();
                            found = true;
                        }
                    }
                }
            }
        } catch (...) {}

        if (found) return bestExe;

        // Check if RobloxPlayerBeta.exe is directly in versionsPath
        fs::path directExe = versionsPath / "RobloxPlayerBeta.exe";
        if (fs::exists(directExe, ec)) {
            return directExe.string();
        }

        // Fallback: check RobloxPlayerLauncher.exe if RobloxPlayerBeta.exe not found
        try {
            for (const auto& entry : fs::directory_iterator(versionsPath, fs::directory_options::skip_permission_denied, ec)) {
                if (ec) break;
                if (entry.is_directory(ec)) {
                    fs::path launcherPath = entry.path() / "RobloxPlayerLauncher.exe";
                    if (fs::exists(launcherPath, ec)) {
                        return launcherPath.string();
                    }
                }
            }
        } catch (...) {}

        return "";
    }

    static std::string FindRobloxCandidateInFolder(const fs::path& folder) {
        std::error_code ec;
        if (!fs::exists(folder, ec)) return "";

        // Check if folder contains Versions directory
        fs::path vPath = folder / "Versions";
        std::string exe = FindExecutableInVersions(vPath);
        if (!exe.empty()) return exe;

        // Check directly in folder
        fs::path directExe = folder / "RobloxPlayerBeta.exe";
        if (fs::exists(directExe, ec)) return directExe.string();

        return "";
    }

    std::string FindRobloxExecutable() {
        char localPath[MAX_PATH];
        char progPath[MAX_PATH];
        char prog64Path[MAX_PATH];

        std::string localAppDirStr;
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localPath))) {
            localAppDirStr = localPath;
        }

        // 1. Prioritize known custom bootstrappers in %LOCALAPPDATA%
        if (!localAppDirStr.empty()) {
            static const std::vector<std::string> knownBootstrappers = {
                "Fishstrap", "Bloxstrap", "Froststrap", "Noxstrap", "Voidstrap"
            };
            for (const auto& bsName : knownBootstrappers) {
                fs::path bsFolder = fs::path(localAppDirStr) / bsName;
                std::string cand = FindRobloxCandidateInFolder(bsFolder);
                if (!cand.empty()) {
                    return cand;
                }
            }
        }

        // 2. Check Registry Protocol Handler (if registered to a custom bootstrapper)
        std::string regCmd = GetRegistryProtocolCommand();
        if (!regCmd.empty()) {
            std::string regExe = ExtractPathFromCommand(regCmd);
            std::error_code ec;
            if (!regExe.empty() && fs::exists(regExe, ec)) {
                fs::path p(regExe);
                
                // Determine if this path belongs to the standard "Roblox" folder or a custom bootstrapper
                bool isInsideRobloxFolder = false;
                for (const auto& part : p) {
                    if (_stricmp(part.string().c_str(), "Roblox") == 0) {
                        isInsideRobloxFolder = true;
                        break;
                    }
                }

                // If registry points to a custom bootstrapper outside the "Roblox" folder
                if (!isInsideRobloxFolder) {
                    if (_stricmp(p.filename().string().c_str(), "RobloxPlayerBeta.exe") == 0) {
                        return p.string();
                    }
                    std::string cand = FindRobloxCandidateInFolder(p.parent_path());
                    if (!cand.empty()) {
                        return cand;
                    }
                }
            }
        }

        // 3. Dynamic search in %LOCALAPPDATA%: scan all other folders EXCEPT "Roblox"
        if (!localAppDirStr.empty()) {
            std::error_code ec;
            fs::path localAppDir(localAppDirStr);
            try {
                fs::directory_iterator it(localAppDir, ec);
                fs::directory_iterator end;
                while (!ec && it != end) {
                    try {
                        if (it->is_directory(ec) && !ec) {
                            std::string folderName = it->path().filename().string();
                            if (_stricmp(folderName.c_str(), "Roblox") != 0) {
                                std::string cand = FindRobloxCandidateInFolder(it->path());
                                if (!cand.empty()) {
                                    return cand;
                                }
                            }
                        }
                    } catch (...) {}
                    ec.clear();
                    it.increment(ec);
                }
            } catch (...) {}
        }

        // 4. Fallback: if RobloxPlayerBeta.exe is only found in default "Roblox" folder
        std::vector<fs::path> defaultRobloxPaths;
        if (!localAppDirStr.empty()) {
            defaultRobloxPaths.push_back(fs::path(localAppDirStr) / "Roblox");
        }
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, progPath))) {
            defaultRobloxPaths.push_back(fs::path(progPath) / "Roblox");
        }
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, prog64Path))) {
            defaultRobloxPaths.push_back(fs::path(prog64Path) / "Roblox");
        }

        for (const auto& robloxDir : defaultRobloxPaths) {
            std::string cand = FindRobloxCandidateInFolder(robloxDir);
            if (!cand.empty()) {
                return cand;
            }
        }

        return "";
    }

    bool IsAnyRobloxRunning() {
        bool running = false;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32W pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32W);
            if (Process32FirstW(hSnapshot, &pe32)) {
                do {
                    if (_wcsicmp(pe32.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                        running = true;
                        break;
                    }
                } while (Process32NextW(hSnapshot, &pe32));
            }
            CloseHandle(hSnapshot);
        }
        return running;
    }

    void ApplyLowestGraphicsSettings(bool lowestGraphics) {
        char localPath[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localPath))) {
            fs::path settingsPath = fs::path(localPath) / "Roblox" / "GlobalBasicSettings_13.xml";
            if (fs::exists(settingsPath)) {
                std::ifstream inFile(settingsPath);
                if (inFile.is_open()) {
                    std::stringstream buffer;
                    buffer << inFile.rdbuf();
                    std::string content = buffer.str();
                    inFile.close();

                    if (lowestGraphics) {
                        content = std::regex_replace(content, std::regex("<token name=\"SavedQualityLevel\">[^<]*</token>"), "<token name=\"SavedQualityLevel\">1</token>");
                    } else {
                        content = std::regex_replace(content, std::regex("<token name=\"SavedQualityLevel\">[^<]*</token>"), "<token name=\"SavedQualityLevel\">0</token>");
                    }

                    std::ofstream outFile(settingsPath, std::ios::trunc);
                    if (outFile.is_open()) {
                        outFile << content;
                        outFile.close();
                    }
                }
            }
        }
    }

    void ApplyFFlags(const std::string& robloxPath, const std::string& fflagOpt) {
        if (robloxPath.empty()) return;
        try {
            fs::path rp(robloxPath);
            fs::path clientSettingsDir = rp.parent_path() / "ClientSettings";
            if (!fs::exists(clientSettingsDir)) {
                fs::create_directory(clientSettingsDir);
            }
            
            fs::path settingsFile = clientSettingsDir / "ClientAppSettings.json";
            std::string content = "{}";

            if (fflagOpt == "Medium") {
                content = R"({
  "DFIntDebugFRMQualityLevelOverride": "4",
  "FIntRenderShadowIntensity": "150",
  "DFFlagTextureQualityOverrideEnabled": "True",
  "DFIntTextureQualityOverride": "2",
  "FIntDebugTextureManagerSkipMips": "0",
  "FIntFRMMaxGrassDistance": "100",
  "DFIntMaxParticleSpriteCount": "500",
  "FFlagDisablePostFx": "False",
  "DFFlagDebugRenderForceTechnologyVoxel": "False"
})";
            } else if (fflagOpt == "Low") {
                content = R"({
  "DFIntDebugFRMQualityLevelOverride": "2",
  "FIntRenderShadowIntensity": "0",
  "DFFlagTextureQualityOverrideEnabled": "True",
  "DFIntTextureQualityOverride": "1",
  "FIntDebugTextureManagerSkipMips": "2",
  "FIntFRMMaxGrassDistance": "25",
  "FIntFRMMinGrassDistance": "25",
  "DFIntMaxParticleSpriteCount": "50",
  "FFlagDisablePostFx": "True",
  "DFFlagDebugRenderForceTechnologyVoxel": "True"
})";
            } else if (fflagOpt == "Potato") {
                content = R"({
  "DFIntDebugFRMQualityLevelOverride": "1",
  "FIntRenderShadowIntensity": "0",
  "FFlagDebugDisableShadows": "True",
  "DFFlagTextureQualityOverrideEnabled": "True",
  "DFIntTextureQualityOverride": "0",
  "DFIntTextureCompositorActiveJobs": "0",
  "FIntDebugTextureManagerSkipMips": "8",
  "FIntFRMMaxGrassDistance": "0",
  "FIntFRMMinGrassDistance": "0",
  "DFIntMaxParticleSpriteCount": "0",
  "DFIntMaxParticleMeshCount": "0",
  "FIntEmitterMaxSpawnedPerFrame": "0",
  "FFlagDisablePostFx": "True",
  "DFFlagDebugRenderForceTechnologyVoxel": "True",
  "FFlagDebugSkyGray": "True"
})";
            }

            std::ofstream outFile(settingsFile, std::ios::trunc);
            if (outFile.is_open()) {
                outFile << content;
                outFile.close();
            }
        } catch(...) {}
    }

    bool LaunchAccount(const std::string& cookie, const std::string& placeId, const std::string& linkCode, const std::string& jobId, std::string& outError, DWORD& outPID, bool lowestGraphics, const std::string& fflagOpt, std::string* outResolvedPlaceId) {
        std::lock_guard<std::mutex> lock(g_launchMutex);
        ApplyLowestGraphicsSettings(lowestGraphics);
        
        std::string robloxPath = FindRobloxExecutable();
        if (robloxPath.empty()) {
            outError = "Could not find RobloxPlayerBeta.exe! Please launch Roblox once.";
            return false;
        }

        std::filesystem::path dllPath = std::filesystem::path(robloxPath).parent_path() / "RobloxPlayerBeta.dll";
        if (!std::filesystem::exists(dllPath)) {
            outError = "RobloxPlayerBeta.dll was not found!";
            return false;
        }

        ApplyFFlags(robloxPath, fflagOpt);

        if (!IsAnyRobloxRunning()) {
            char localPath[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localPath))) {
                fs::path rbxDir = fs::path(localPath) / "Roblox";
                fs::path walPath = rbxDir / "rbx-storage.db-wal";
                fs::path shmPath = rbxDir / "rbx-storage.db-shm";
                std::error_code ec;
                if (fs::exists(walPath, ec)) fs::remove(walPath, ec);
                if (fs::exists(shmPath, ec)) fs::remove(shmPath, ec);
            }
        }

        std::string csrf = RobloxAPI::GetCSRFToken(cookie);
        if (csrf.empty()) {
            outError = "Failed to get CSRF token.";
            return false;
        }

        std::string ticket = RobloxAPI::GetAuthTicket(cookie, csrf);
        if (ticket.empty()) {
            outError = "Failed to get Auth Ticket.";
            return false;
        }

        std::string uri = "roblox-player:1+launchmode:play+gameinfo:" + ticket;
        std::string localPlaceId = placeId;
        std::string extractedLinkCode = linkCode;
        bool isShareLink = false;

        if (!extractedLinkCode.empty()) {
            size_t gamesPos = extractedLinkCode.find("/games/");
            if (gamesPos != std::string::npos) {
                size_t idStart = gamesPos + 7;
                size_t idEnd = extractedLinkCode.find('/', idStart);
                if (idEnd == std::string::npos) idEnd = extractedLinkCode.find('?', idStart);
                std::string extractedId = (idEnd != std::string::npos) ? extractedLinkCode.substr(idStart, idEnd - idStart) : extractedLinkCode.substr(idStart);
                if (!extractedId.empty()) {
                    if (!localPlaceId.empty() && localPlaceId != extractedId) {
                        outError = "MISMATCH_PLACE_ID";
                        ActiveClientLock::UnlockClient();
                        return false;
                    }
                    if (localPlaceId.empty()) {
                        localPlaceId = extractedId;
                    }
                }
            }
        }

        if (!localPlaceId.empty() || !extractedLinkCode.empty()) {
            std::string resolveUrl = "";
            size_t pos = extractedLinkCode.find("privateServerLinkCode=");
            if (pos != std::string::npos) {
                extractedLinkCode = extractedLinkCode.substr(pos + 22);
                size_t ampersand = extractedLinkCode.find('&');
                if (ampersand != std::string::npos) {
                    extractedLinkCode = extractedLinkCode.substr(0, ampersand);
                }
            } else {
                pos = extractedLinkCode.find("share?code=");
                if (pos != std::string::npos) {
                    extractedLinkCode = extractedLinkCode.substr(pos + 11);
                    size_t ampersand = extractedLinkCode.find('&');
                    if (ampersand != std::string::npos) {
                        extractedLinkCode = extractedLinkCode.substr(0, ampersand);
                    }
                    isShareLink = true;
                }
            }

            if (isShareLink && !extractedLinkCode.empty()) {
                std::string resolveApi = "/sharelinks/v1/resolve-link";
                std::string extraHeaders = "X-CSRF-TOKEN: " + csrf + "\r\nContent-Type: application/json\r\n";
                std::string postData = "{\"linkId\":\"" + extractedLinkCode + "\",\"linkType\":\"Server\"}";
                std::string dummyHeaders;
                std::string response = RobloxAPI::HttpRequest(L"POST", L"apis.roblox.com", RobloxAPI::s2ws(resolveApi), cookie, extraHeaders, postData, &dummyHeaders);
                
                try {
                    size_t statusPos = response.find("\"status\":\"");
                    if (statusPos != std::string::npos) {
                        size_t statusEnd = response.find("\"", statusPos + 10);
                        std::string status = response.substr(statusPos + 10, statusEnd - (statusPos + 10));
                        if (status == "Expired") {
                            outError = "Tautan Private Server kedaluwarsa (Share Link Expired).";
                            ActiveClientLock::UnlockClient();
                            return false;
                        }
                    }
                    
                    size_t linkCodePos = response.find("\"linkCode\":\"");
                    if (linkCodePos != std::string::npos) {
                        size_t linkCodeEnd = response.find("\"", linkCodePos + 12);
                        extractedLinkCode = response.substr(linkCodePos + 12, linkCodeEnd - (linkCodePos + 12));
                    } else {
                        outError = "Gagal memproses Share Link (Tidak ditemukan linkCode).";
                        ActiveClientLock::UnlockClient();
                        return false;
                    }

                    size_t pIdPos = response.find("\"placeId\":");
                    if (pIdPos != std::string::npos) {
                        size_t pIdEnd = response.find_first_of(",}", pIdPos + 10);
                        std::string extractedPlaceId = response.substr(pIdPos + 10, pIdEnd - (pIdPos + 10));
                        if (extractedPlaceId != "0" && !extractedPlaceId.empty()) {
                            if (!localPlaceId.empty() && localPlaceId != extractedPlaceId) {
                                outError = "MISMATCH_PLACE_ID";
                                ActiveClientLock::UnlockClient();
                                return false;
                            }
                            localPlaceId = extractedPlaceId;
                        }
                    }
                } catch(...) {
                    outError = "Gagal memproses Share Link.";
                    ActiveClientLock::UnlockClient();
                    return false;
                }
            }

            if (localPlaceId.empty()) {
                outError = "Place ID tidak ditemukan. Harap isi Place ID atau gunakan URL yang valid.";
                ActiveClientLock::UnlockClient();
                return false;
            }

            if (outResolvedPlaceId) {
                *outResolvedPlaceId = localPlaceId;
            }

            if (!extractedLinkCode.empty()) {
                resolveUrl = "/games/" + localPlaceId + "?privateServerLinkCode=" + extractedLinkCode;
                std::string accessCode = "";
                std::string dummyHeaders;
                std::string extraHeaders = "X-CSRF-TOKEN: " + csrf + "\r\nReferer: https://www.roblox.com/games/" + localPlaceId + "/Game\r\n";
                std::string response = RobloxAPI::HttpRequest(L"GET", L"www.roblox.com", RobloxAPI::s2ws(resolveUrl), cookie, extraHeaders, "", &dummyHeaders);
                
                std::smatch match;
                std::regex re("joinPrivateGame\\(\\s*\\d+\\s*,\\s*'([^']+)'");
                if (std::regex_search(response, match, re) && match.size() > 1) {
                    accessCode = match[1].str();
                }

                if (!accessCode.empty()) {
                    uri += "+placelauncherurl:https%3A%2F%2Fassetgame.roblox.com%2Fgame%2FPlaceLauncher.ashx%3Frequest%3DRequestPrivateGame%26placeId%3D" + localPlaceId + "%26accessCode%3D" + accessCode + "%26linkCode%3D" + extractedLinkCode;
                } else {
                    outError = "Tidak memiliki akses ke Private Server (Access Denied).";
                    ActiveClientLock::UnlockClient();
                    return false;
                }
            } else {
                if (!jobId.empty()) {
                    uri += "+placelauncherurl:https%3A%2F%2Fassetgame.roblox.com%2Fgame%2FPlaceLauncher.ashx%3Frequest%3DRequestGameJob%26placeId%3D" + localPlaceId + "%26gameJobId%3D" + jobId + "%26isPlayTogetherGame%3Dfalse";
                } else {
                    uri += "+placelauncherurl:https%3A%2F%2Fassetgame.roblox.com%2Fgame%2FPlaceLauncher.ashx%3Frequest%3DRequestGame%26placeId%3D" + localPlaceId + "%26isPlayTogetherGame%3Dfalse";
                }
            }
        }

        ActiveClientLock::UnlockClient();

        std::string latestVersion = Updater::GetLatestRobloxVersion();
        bool needsUpdate = false;
        
        if (!latestVersion.empty()) {
            try {
                fs::path rp(robloxPath);
                std::string folderName = rp.parent_path().filename().string();
                if (folderName != latestVersion) {
                    needsUpdate = true;
                }
            } catch(...) {}
        }
        
        if (needsUpdate && !IsAnyRobloxRunning()) {
            ShellExecuteA(NULL, "open", uri.c_str(), NULL, NULL, SW_SHOWNORMAL);
            outPID = 0;
            
            for (int i = 0; i < 300; i++) { // Wait up to 5 minutes for the update to complete
                Sleep(1000); 
                HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (hSnapshot != INVALID_HANDLE_VALUE) {
                    PROCESSENTRY32W pe32;
                    pe32.dwSize = sizeof(PROCESSENTRY32W);
                    if (Process32FirstW(hSnapshot, &pe32)) {
                        do {
                            if (_wcsicmp(pe32.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                                DWORD pid = pe32.th32ProcessID;
                                if (HasWindow(pid)) {
                                    outPID = pid;
                                    CloseHandle(hSnapshot);
                                    
                                    for (int j = 0; j < 10; j++) {
                                        if (HandleCloser::CloseProcessRobloxHandle(outPID)) {
                                            break;
                                        }
                                        Sleep(100);
                                    }
                                    
                                    return true;
                                }
                            }
                        } while (Process32NextW(hSnapshot, &pe32));
                    }
                    CloseHandle(hSnapshot);
                }
            }
            
            outError = "Update timed out or Roblox failed to launch after update.";
            return false;
        }

        std::string args;
        if (placeId.empty()) {
            args = "\"" + uri + "\"";
        } else {
            args = "\"" + uri + "\"";
        }
        std::string cmdLine = "\"" + robloxPath + "\" " + args;

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::vector<char> cmdLineMutable(cmdLine.begin(), cmdLine.end());
        cmdLineMutable.push_back('\0');

        fs::path rp(robloxPath);
        std::string workDir = rp.parent_path().string();

        if (!CreateProcessA(NULL, cmdLineMutable.data(), NULL, NULL, FALSE, 0, NULL, workDir.c_str(), &si, &pi)) {
            std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
            return false;
        }

        outPID = pi.dwProcessId;

        for (int i = 0; i < 150; i++) {
            if (HasWindow(outPID)) {
                break;
            }
            Sleep(100);
        }

        for (int i = 0; i < 10; i++) {
            if (HandleCloser::CloseProcessRobloxHandle(outPID)) {
                break;
            }
            Sleep(100);
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }

    void InitializeMultiInstance() {
    }

    void ShutdownMultiInstance() {
        if (g_hMutex) {
            ReleaseMutex(g_hMutex);
            CloseHandle(g_hMutex);
            g_hMutex = NULL;
        }
        if (g_hEvent) {
            CloseHandle(g_hEvent);
            g_hEvent = NULL;
        }
    }
}