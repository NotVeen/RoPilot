#include "SettingsManager.h"
#include "../vendor/json.hpp"
#include <fstream>

using json = nlohmann::json;

SettingsManager::SettingsManager(const std::string& filePath) : m_FilePath(filePath) {
    Load();
}

void SettingsManager::Load() {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ifstream file(m_FilePath);
    if (!file.is_open()) return;

    try {
        json j;
        file >> j;
        m_Settings.AutoUpdate = j.value("autoUpdate", false);
        m_Settings.RunOnStartup = j.value("runOnStartup", false);
        m_Settings.MinimizeToTrayOnClose = j.value("minimizeToTrayOnClose", true);
        m_Settings.AlwaysOnTop = j.value("alwaysOnTop", false);
        m_Settings.AutoKillOnExit = j.value("autoKillOnExit", false);
    } catch (...) {
        // Failed to parse or read, keep defaults
    }
}

void SettingsManager::Save() {
    std::lock_guard<std::mutex> lock(m_mutex);
    try {
        json j;
        j["autoUpdate"] = m_Settings.AutoUpdate;
        j["runOnStartup"] = m_Settings.RunOnStartup;
        j["minimizeToTrayOnClose"] = m_Settings.MinimizeToTrayOnClose;
        j["alwaysOnTop"] = m_Settings.AlwaysOnTop;
        j["autoKillOnExit"] = m_Settings.AutoKillOnExit;
        
        std::ofstream file(m_FilePath);
        if (file.is_open()) {
            file << j.dump(4);
        }
    } catch (...) {
    }
}

Settings SettingsManager::GetSettings() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_Settings;
}

void SettingsManager::SetSettings(const Settings& settings) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_Settings = settings;
    }
    Save();
}
