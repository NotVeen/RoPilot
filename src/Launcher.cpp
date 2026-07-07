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
#include "ActiveClientLock.h"
#include "AccountManager.h"

extern AccountManager g_accountManager;

namespace fs = std::filesystem;

namespace Launcher {

    HANDLE g_hMutex = NULL;
    HANDLE g_hEvent = NULL;

    std::string FindRobloxExecutable() {
        char localPath[MAX_PATH];
        char progPath[MAX_PATH];
        std::vector<fs::path> pathsToCheck;
        
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localPath))) {
            pathsToCheck.push_back(fs::path(localPath) / "Roblox" / "Versions");
            pathsToCheck.push_back(fs::path(localPath) / "Fishstrap" / "Versions");
            pathsToCheck.push_back(fs::path(localPath) / "Bloxstrap" / "Versions");
        }
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, progPath))) {
            pathsToCheck.push_back(fs::path(progPath) / "Roblox" / "Versions");
        }

        for (const auto& robloxPath : pathsToCheck) {
            if (fs::exists(robloxPath)) {
                for (const auto& entry : fs::directory_iterator(robloxPath)) {
                    if (entry.is_directory()) {
                        fs::path exePath = entry.path() / "RobloxPlayerBeta.exe";
                        if (fs::exists(exePath)) {
                            return exePath.string();
                        }
                        fs::path launcherPath = entry.path() / "RobloxPlayerLauncher.exe";
                        if (fs::exists(launcherPath)) {
                            return launcherPath.string();
                        }
                    }
                }
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

    bool LaunchAccount(const std::string& cookie, const std::string& placeId, std::string& outError, DWORD& outPID) {
        std::string robloxPath = FindRobloxExecutable();
        if (robloxPath.empty()) {
            outError = "Could not find RobloxPlayerBeta.exe! Please launch Roblox once.";
            return false;
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
        if (!placeId.empty()) {
            uri += "+placelauncherurl:https%3A%2F%2Fassetgame.roblox.com%2Fgame%2FPlaceLauncher.ashx%3Frequest%3DRequestGame%26placeId%3D" + placeId + "%26isPlayTogetherGame%3Dfalse";
        }

        ActiveClientLock::UnlockClient();

        // Smart Update Checker
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
            // Fall back to bootstrapper to allow updates
            ShellExecuteA(NULL, "open", uri.c_str(), NULL, NULL, SW_SHOWNORMAL);
            outPID = 0; // Temporarily 0
            
            // Spawn background thread to find the newly launched process ID and update AccountManager
            std::thread([cookie]() {
                for (int i = 0; i < 30; i++) {
                    Sleep(1000); // Check every second for up to 30 seconds
                    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                    if (hSnapshot != INVALID_HANDLE_VALUE) {
                        PROCESSENTRY32W pe32;
                        pe32.dwSize = sizeof(PROCESSENTRY32W);
                        if (Process32FirstW(hSnapshot, &pe32)) {
                            do {
                                if (_wcsicmp(pe32.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                                    // Found a running instance, assume it's the one we just launched
                                    g_accountManager.UpdateAccountProcess(cookie, 2, pe32.th32ProcessID);
                                    CloseHandle(hSnapshot);
                                    return; // Exit thread
                                }
                            } while (Process32NextW(hSnapshot, &pe32));
                        }
                        CloseHandle(hSnapshot);
                    }
                }
            }).detach();
            
            return true;
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

        if (!CreateProcessA(NULL, cmdLineMutable.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
            return false;
        }

        outPID = pi.dwProcessId;

        // Wait up to 15 seconds for the new process to initialize and create its singleton mutex.
        // Once it creates the mutex, we immediately close it, freeing the way for the NEXT instance to launch safely!
        for (int i = 0; i < 150; i++) {
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
        // Multi-instance is now handled via HandleCloser dynamically.
        // We do not need to hold the singleton handles in RoPilot.
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