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
    std::string AccentColor = "";
    std::string FontFamily = "Segoe UI, sans-serif";
    std::string Language = "en";
    double UiScale = 1.0;
    bool SidebarCollapsed = false;
    float WindowOpacity = 1.0f;
    bool EnableWindowBlur = false;
};

class SettingsManager {
public:
    SettingsManager(const std::string& filePath = "settings.json");
    
    void Load();
    void Save();

    Settings GetSettings();
    void SetSettings(const Settings& settings);

private:
    std::string m_FilePath;
    Settings m_Settings;
    std::mutex m_mutex;
};
