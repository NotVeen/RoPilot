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
        m_Settings.HardwareAcceleration = j.value("hardwareAcceleration", true);
        m_Settings.ResourceOptimizer = j.value("resourceOptimizer", false);
        m_Settings.CpuLimiter = j.value("cpuLimiter", false);
        m_Settings.BackgroundCpuLimit = j.value("backgroundCpuLimit", 2);
        m_Settings.LightMode = j.value("lightMode", false);
        m_Settings.FontFamily = j.value("fontFamily", "Segoe UI, sans-serif");
        m_Settings.Language = j.value("language", "en");
        m_Settings.UiScale = j.value("uiScale", 1.0);
        m_Settings.SidebarCollapsed = j.value("sidebarCollapsed", false);
        m_Settings.WindowOpacity = j.value("windowOpacity", 1.0f);
        m_Settings.EnableWindowBlur = j.value("enableWindowBlur", false);
        m_Settings.HideIdentity = j.value("hideIdentity", false);
        m_Settings.EnableDiscordRPC = j.value("enableDiscordRPC", false);
        m_Settings.GlobalPlaceId = j.value("globalPlaceId", "");
        m_Settings.GlobalPrivateServerLink = j.value("globalPrivateServerLink", "");
        m_Settings.HasMasterPassword = j.value("hasMasterPassword", false);
        m_Settings.MasterPasswordHash = j.value("masterPasswordHash", "");
        m_Settings.MasterPasswordSalt = j.value("masterPasswordSalt", "");
        m_Settings.AutoTileOnLaunch = j.value("autoTileOnLaunch", false);
        m_Settings.DefaultTileMode = j.value("defaultTileMode", "auto");
        m_Settings.AutoRejoin = j.value("autoRejoin", false);
        m_Settings.RejoinDelay = j.value("rejoinDelay", 10);
        m_Settings.MaxRejoinRetries = j.value("maxRejoinRetries", 3);
        m_Settings.WebhookEnabled = j.value("webhookEnabled", false);
        m_Settings.WebhookUrl = j.value("webhookUrl", "");
        m_Settings.WebhookNotifyCrash = j.value("webhookNotifyCrash", true);
        m_Settings.WebhookNotifyRejoin = j.value("webhookNotifyRejoin", true);
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
        j["hardwareAcceleration"] = m_Settings.HardwareAcceleration;
        j["resourceOptimizer"] = m_Settings.ResourceOptimizer;
        j["cpuLimiter"] = m_Settings.CpuLimiter;
        j["backgroundCpuLimit"] = m_Settings.BackgroundCpuLimit;
        j["lightMode"] = m_Settings.LightMode;
        j["fontFamily"] = m_Settings.FontFamily;
        j["language"] = m_Settings.Language;
        j["uiScale"] = m_Settings.UiScale;
        j["sidebarCollapsed"] = m_Settings.SidebarCollapsed;
        j["windowOpacity"] = m_Settings.WindowOpacity;
        j["enableWindowBlur"] = m_Settings.EnableWindowBlur;
        j["hideIdentity"] = m_Settings.HideIdentity;
        j["enableDiscordRPC"] = m_Settings.EnableDiscordRPC;
        j["globalPlaceId"] = m_Settings.GlobalPlaceId;
        j["globalPrivateServerLink"] = m_Settings.GlobalPrivateServerLink;
        j["hasMasterPassword"] = m_Settings.HasMasterPassword;
        j["masterPasswordHash"] = m_Settings.MasterPasswordHash;
        j["masterPasswordSalt"] = m_Settings.MasterPasswordSalt;
        j["autoTileOnLaunch"] = m_Settings.AutoTileOnLaunch;
        j["defaultTileMode"] = m_Settings.DefaultTileMode;
        j["autoRejoin"] = m_Settings.AutoRejoin;
        j["rejoinDelay"] = m_Settings.RejoinDelay;
        j["maxRejoinRetries"] = m_Settings.MaxRejoinRetries;
        j["webhookEnabled"] = m_Settings.WebhookEnabled;
        j["webhookUrl"] = m_Settings.WebhookUrl;
        j["webhookNotifyCrash"] = m_Settings.WebhookNotifyCrash;
        j["webhookNotifyRejoin"] = m_Settings.WebhookNotifyRejoin;
        
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

void SettingsManager::HardReset() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_Settings = Settings();
    std::error_code ec;
    std::filesystem::remove(m_FilePath, ec);
    std::filesystem::remove(m_FilePath + ".bak", ec);
}
