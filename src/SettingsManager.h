#pragma once
#include <string>
#include <mutex>

struct Settings {
    bool AutoUpdate = false;
    bool RunOnStartup = false;
    bool MinimizeToTrayOnClose = true;
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
