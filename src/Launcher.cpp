#include "Launcher.h"
#include "RobloxAPI.h"
#include <windows.h>
#include <shlobj.h>
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace Launcher {

    HANDLE g_hMutex = NULL;

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


        // Construct arguments
        std::string args;
        if (placeId.empty()) {
            args = "\"roblox-player:1+launchmode:play+gameinfo:" + ticket + "\"";
        } else {
            args = "\"roblox-player:1+launchmode:play+gameinfo:" + ticket + "+placelauncherurl:https%3A%2F%2Fassetgame.roblox.com%2Fgame%2FPlaceLauncher.ashx%3Frequest%3DRequestGame%26placeId%3D" + placeId + "%26isPlayTogetherGame%3Dfalse\"";
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

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }

    void InitializeMultiInstance() {
        if (!g_hMutex) {
            g_hMutex = CreateMutexA(NULL, TRUE, "ROBLOX_singletonMutex");
        }
    }

    void ShutdownMultiInstance() {
        if (g_hMutex) {
            ReleaseMutex(g_hMutex);
            CloseHandle(g_hMutex);
            g_hMutex = NULL;
        }
    }
}