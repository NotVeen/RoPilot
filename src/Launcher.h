#pragma once
#include <string>
#include <windows.h>

namespace Launcher {
    // Finds the latest RobloxPlayerBeta.exe in %LOCALAPPDATA%\Roblox\Versions
    std::string FindRobloxExecutable();

    // Checks if any Roblox instance is currently running
    bool IsAnyRobloxRunning();

    void ApplyLowestGraphicsSettings(bool lowestGraphics);
    void ApplyFFlags(const std::string& robloxPath, const std::string& fflagOpt);

    // Fetches the ticket and launches the game
    bool LaunchAccount(const std::string& cookie, const std::string& placeId, const std::string& linkCode, const std::string& jobId, std::string& outError, DWORD& outPID, bool lowestGraphics = false, const std::string& fflagOpt = "Default");

    // Creates the Mutex to allow multiple instances (call once at app startup)
    void InitializeMultiInstance();
    
    // Releases the Mutex
    void ShutdownMultiInstance();

    // Brings the process window to the foreground
    void FocusWindow(DWORD processId);
}