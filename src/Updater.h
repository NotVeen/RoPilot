#pragma once
#include <string>

class Updater {
public:
    static std::string CURRENT_VERSION;
    
    // Checks github for new version. Returns empty string if no update or error.
    static std::string CheckForUpdate(std::string& outDownloadUrl);
    
    // Check Roblox API for the latest version string
    static std::string GetLatestRobloxVersion();
    
    // Downloads update. callback takes (progress_percentage). Returns true if success.
    static bool DownloadUpdate(const std::string& url, const std::string& destPath, void (*progressCallback)(int));
    
    // Applies the downloaded ZIP update via a background script and exits the app.
    static void ApplyUpdate(const std::string& zipPath);
};
