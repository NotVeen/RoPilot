#include "InstallerQuarantine.h"

namespace fs = std::filesystem;

namespace InstallerQuarantine {

    fs::path GetQuarantineBasePath() {
        char tempDir[MAX_PATH];
        GetTempPathA(MAX_PATH, tempDir);
        return fs::path(tempDir) / "RoPilot_Quarantine";
    }

    fs::path GetRobloxVersionsPath() {
        char localAppData[MAX_PATH];
        if (GetEnvironmentVariableA("LOCALAPPDATA", localAppData, MAX_PATH)) {
            return fs::path(localAppData) / "Roblox" / "Versions";
        }
        return fs::path();
    }

    void QuarantineInstallers() {
        fs::path robloxVersionsPath = GetRobloxVersionsPath();
        if (robloxVersionsPath.empty() || !fs::exists(robloxVersionsPath)) return;

        fs::path quarantineBase = GetQuarantineBasePath();
        if (!fs::exists(quarantineBase)) {
            std::error_code ec;
            fs::create_directory(quarantineBase, ec);
        }

        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(robloxVersionsPath, ec)) {
            if (entry.is_directory()) {
                std::string folderName = entry.path().filename().string();
                if (folderName.find("version-") == 0) {
                    fs::path installerPath = entry.path() / "RobloxPlayerInstaller.exe";
                    if (fs::exists(installerPath)) {
                        std::string versionId = folderName.substr(8);
                        fs::path versionQuarantine = quarantineBase / versionId;
                        
                        if (!fs::exists(versionQuarantine)) {
                            std::error_code ec2;
                            fs::create_directory(versionQuarantine, ec2);
                        }
                        
                        try {
                            fs::rename(installerPath, versionQuarantine / "RobloxPlayerInstaller.exe");
                        } catch (...) {}
                    }
                }
            }
        }
    }

    void RestoreInstallers() {
        fs::path robloxVersionsPath = GetRobloxVersionsPath();
        if (robloxVersionsPath.empty()) return;

        fs::path quarantineBase = GetQuarantineBasePath();
        if (!fs::exists(quarantineBase)) return;

        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(quarantineBase, ec)) {
            if (entry.is_directory()) {
                std::string versionId = entry.path().filename().string();
                fs::path installerPathQ = entry.path() / "RobloxPlayerInstaller.exe";
                
                if (fs::exists(installerPathQ)) {
                    fs::path targetFolder = robloxVersionsPath / ("version-" + versionId);
                    if (fs::exists(targetFolder)) {
                        try {
                            fs::rename(installerPathQ, targetFolder / "RobloxPlayerInstaller.exe");
                        } catch (...) {}
                    }
                }
            }
        }
        
        try {
            fs::remove_all(quarantineBase);
        } catch (...) {}
    }

}
