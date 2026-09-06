#pragma once
#include <string>
#include <mutex>

struct Settings {
    bool AutoUpdate = false;
    bool RunOnStartup = false;
    bool MinimizeToTrayOnClose = true;
    bool AlwaysOnTop = false;
    bool AutoKillOnExit = false;
    bool HardwareAcceleration = true;
    bool ResourceOptimizer = false;
    bool CpuLimiter = false;
    int BackgroundCpuLimit = 2;
    bool LightMode = false;
    std::string FontFamily = "Segoe UI, sans-serif";
    std::string Language = "en";
    double UiScale = 1.0;
    bool SidebarCollapsed = false;
    float WindowOpacity = 1.0f;
    bool EnableWindowBlur = false;
    bool HideIdentity = false;
    bool EnableDiscordRPC = false;
    std::string GlobalPlaceId = "";
    std::string GlobalPrivateServerLink = "";
    bool HasMasterPassword = false;
    std::string MasterPasswordHash = "";
    std::string MasterPasswordSalt = "";
    bool AutoTileOnLaunch = false;
    std::string DefaultTileMode = "auto";
    bool AutoRejoin = false;
    int RejoinDelay = 10;
    int MaxRejoinRetries = 3;
    bool WebhookEnabled = false;
    std::string WebhookUrl = "";
    bool WebhookNotifyCrash = true;
    bool WebhookNotifyRejoin = true;
};

class SettingsManager {
public:
    SettingsManager(const std::string& filePath = "settings.json");
    
    void Load();
    void Save();

    Settings GetSettings();
    void SetSettings(const Settings& settings);
    void HardReset();

private:
    std::string m_FilePath;
    Settings m_Settings;
    std::mutex m_mutex;
};
