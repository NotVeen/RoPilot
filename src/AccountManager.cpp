#include "AccountManager.h"
#include "Crypto.h"
#include "../vendor/json.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <wincrypt.h>

using json = nlohmann::json;

AccountManager::AccountManager(const std::string& filePath) : m_FilePath(filePath) {
    Load();
}

void AccountManager::Load(const std::string& password, const std::string& salt) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_LoadFailed = false;
    if (!password.empty() && !salt.empty()) {
        m_Password = password;
        m_Salt = salt;
    }
    m_Accounts.clear();
    std::ifstream file(m_FilePath, std::ios::binary);
    if (!file.is_open()) return;

    try {
        std::vector<BYTE> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if (buffer.empty()) return;

        std::string jsonStr;
        bool decrypted = false;

        if (!password.empty() && !salt.empty()) {
            try {
                std::string ciphertext((char*)buffer.data(), buffer.size());
                jsonStr = Crypto::DecryptAES(ciphertext, password, salt);
                decrypted = true;
            } catch (...) {
                decrypted = false;
            }
        }

        if (!decrypted) {
            DATA_BLOB DataIn;
            DATA_BLOB DataOut;
            DataIn.pbData = buffer.data();
            DataIn.cbData = (DWORD)buffer.size();

            if (CryptUnprotectData(&DataIn, NULL, NULL, NULL, NULL, 0, &DataOut)) {
                jsonStr = std::string((char*)DataOut.pbData, DataOut.cbData);
                LocalFree(DataOut.pbData);
                decrypted = true;
            }
        }

        if (!decrypted) {
            throw std::runtime_error("Decryption failed");
        }

        json j = json::parse(jsonStr);
        if (j.is_array()) {
            for (const auto& item : j) {
                Account acc;
                acc.Cookie = item.value("Cookie", "");
                acc.Info.Username = item.value("Username", "");
                acc.Info.DisplayName = item.value("DisplayName", "");
                acc.Info.UserId = item.value("UserId", 0LL);
                acc.Info.ThumbnailUrl = item.value("ThumbnailUrl", "");
                acc.Group = item.value("Group", "Ungrouped");
                m_Accounts.push_back(acc);
            }
        } else if (j.is_object()) {
            if (j.contains("accounts") && j["accounts"].is_array()) {
                for (const auto& item : j["accounts"]) {
                    Account acc;
                    acc.Cookie = item.value("Cookie", "");
                    acc.Info.Username = item.value("Username", "");
                    acc.Info.DisplayName = item.value("DisplayName", "");
                    acc.Info.UserId = item.value("UserId", 0LL);
                    acc.Info.ThumbnailUrl = item.value("ThumbnailUrl", "");
                    acc.Group = item.value("Group", "Ungrouped");
                    acc.PlaceId = item.value("PlaceId", "");
                    acc.PrivateServerLink = item.value("PrivateServerLink", "");
                    acc.JoinLowServer = item.value("JoinLowServer", false);
                    acc.LowestGraphics = item.value("LowestGraphics", false);
                    acc.AntiAFK = item.value("AntiAFK", false);
                    acc.ActiveAntiAFK = acc.AntiAFK;
                    acc.FFlagOptimization = item.value("FFlagOptimization", "Default");
                    m_Accounts.push_back(acc);
                }
            }
            if (j.contains("groups") && j["groups"].is_array()) {
                for (const auto& groupName : j["groups"]) {
                    if (groupName.is_string()) {
                        m_Groups.push_back(groupName.get<std::string>());
                    }
                }
            }
            m_GroupConfigs.clear();
            if (j.contains("groupConfigs") && j["groupConfigs"].is_object()) {
                for (auto& el : j["groupConfigs"].items()) {
                    std::string gName = el.key();
                    const auto& gObj = el.value();
                    if (gObj.is_object()) {
                        GroupLaunchConfig cfg;
                        cfg.PlaceId = gObj.value("PlaceId", "");
                        cfg.PrivateServerLink = gObj.value("PrivateServerLink", "");
                        cfg.ForceOverride = gObj.value("ForceOverride", false);
                        cfg.JoinLowServer = gObj.value("JoinLowServer", false);
                        cfg.LowestGraphics = gObj.value("LowestGraphics", false);
                        cfg.AntiAFK = gObj.value("AntiAFK", false);
                        cfg.FFlagOptimization = gObj.value("FFlagOptimization", "Default");
                        m_GroupConfigs[gName] = cfg;
                    }
                }
            }
        }
    } catch (...) {
        file.close();
        if (std::filesystem::exists(m_FilePath + ".bak")) {
            try {
                std::filesystem::copy(m_FilePath + ".bak", m_FilePath, std::filesystem::copy_options::overwrite_existing);
                // Do not call Load() recursively to avoid infinite loop on corrupted backups
                m_LoadFailed = true;
            } catch (...) {
                m_LoadFailed = true;
            }
        } else {
            m_LoadFailed = true;
        }
    }
}

void AccountManager::Save(const std::string& password, const std::string& salt) {
    if (m_LoadFailed) return;
    
    if (!password.empty() && !salt.empty()) {
        m_Password = password;
        m_Salt = salt;
    }
    
    std::string usePwd = m_Password;
    std::string useSalt = m_Salt;

    std::vector<Account> accsCopy;
    std::vector<std::string> groupsCopy;
    std::map<std::string, GroupLaunchConfig> groupConfigsCopy;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        accsCopy = m_Accounts;
        groupsCopy = m_Groups;
        groupConfigsCopy = m_GroupConfigs;
    }

    json j = json::object();
    json accountsArray = json::array();
    for (const auto& acc : accsCopy) {
        json item;
        item["Cookie"] = acc.Cookie;
        item["Username"] = acc.Info.Username;
        item["DisplayName"] = acc.Info.DisplayName;
        item["UserId"] = acc.Info.UserId;
        item["ThumbnailUrl"] = acc.Info.ThumbnailUrl;
        item["Group"] = acc.Group;
        item["PlaceId"] = acc.PlaceId;
        item["PrivateServerLink"] = acc.PrivateServerLink;
        item["JoinLowServer"] = acc.JoinLowServer;
        item["LowestGraphics"] = acc.LowestGraphics;
        item["AntiAFK"] = acc.AntiAFK;
        item["FFlagOptimization"] = acc.FFlagOptimization;
        accountsArray.push_back(item);
    }
    j["accounts"] = accountsArray;
    
    json groupsArray = json::array();
    for (const auto& g : groupsCopy) {
        groupsArray.push_back(g);
    }
    j["groups"] = groupsArray;

    json groupConfigsObj = json::object();
    for (const auto& pair : groupConfigsCopy) {
        json item;
        item["PlaceId"] = pair.second.PlaceId;
        item["PrivateServerLink"] = pair.second.PrivateServerLink;
        item["ForceOverride"] = pair.second.ForceOverride;
        item["JoinLowServer"] = pair.second.JoinLowServer;
        item["LowestGraphics"] = pair.second.LowestGraphics;
        item["AntiAFK"] = pair.second.AntiAFK;
        item["FFlagOptimization"] = pair.second.FFlagOptimization;
        groupConfigsObj[pair.first] = item;
    }
    j["groupConfigs"] = groupConfigsObj;

    std::string jsonStr = j.dump(4);

    if (!usePwd.empty() && !useSalt.empty()) {
        try {
            std::string encrypted = Crypto::EncryptAES(jsonStr, usePwd, useSalt);
            std::ofstream file(m_FilePath, std::ios::binary);
            if (file.is_open()) {
                file.write(encrypted.c_str(), encrypted.size());
                file.close();
                try {
                    std::filesystem::copy(m_FilePath, m_FilePath + ".bak", std::filesystem::copy_options::overwrite_existing);
                } catch (...) {}
            }
        } catch (...) {}
    } else {
        DATA_BLOB DataIn;
        DATA_BLOB DataOut;
        DataIn.pbData = (BYTE*)jsonStr.c_str();
        DataIn.cbData = (DWORD)jsonStr.length();

        if (CryptProtectData(&DataIn, L"RoPilot Encrypted Accounts", NULL, NULL, NULL, 0, &DataOut)) {
            std::ofstream file(m_FilePath, std::ios::binary);
            if (file.is_open()) {
                file.write((char*)DataOut.pbData, DataOut.cbData);
                file.close();
                try {
                    std::filesystem::copy(m_FilePath, m_FilePath + ".bak", std::filesystem::copy_options::overwrite_existing);
                } catch (...) {}
            }
            LocalFree(DataOut.pbData);
        }
    }
}

bool AccountManager::AddAccount(const std::string& cookie) {
    std::string cleanCookie = cookie;
    cleanCookie.erase(0, cleanCookie.find_first_not_of(" \t\r\n\"'"));
    cleanCookie.erase(cleanCookie.find_last_not_of(" \t\r\n\"'") + 1);
    if (cleanCookie.find("%") != std::string::npos) {
        std::string dec;
        for (size_t i = 0; i < cleanCookie.length(); ++i) {
            if (cleanCookie[i] == '%' && i + 2 < cleanCookie.length()) {
                int hexVal = 0;
                if (sscanf(cleanCookie.substr(i + 1, 2).c_str(), "%x", &hexVal) == 1) {
                    dec += static_cast<char>(hexVal);
                    i += 2;
                    continue;
                }
            }
            dec += cleanCookie[i];
        }
        cleanCookie = dec;
    }
    if (cleanCookie.rfind(".ROBLOSECURITY=", 0) == 0) {
        cleanCookie = cleanCookie.substr(15);
    }
    if (cleanCookie.empty()) return false;

    RobloxAPI::UserInfo info = RobloxAPI::GetUserInfo(cleanCookie);
    if (info.UserId == 0) {
        // Retry with a brief delay in case of session replication lag (especially after browser login)
        Sleep(600);
        info = RobloxAPI::GetUserInfo(cleanCookie);
    }
    if (info.UserId == 0) {
        Sleep(1000);
        info = RobloxAPI::GetUserInfo(cleanCookie);
    }
    if (info.UserId == 0) return false; // Invalid cookie

    bool exists = false;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& acc : m_Accounts) {
            if (acc.Info.UserId == info.UserId) {
                acc.Cookie = cleanCookie;
                acc.Info = info;
                acc.Status = 0;
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            Account newAcc;
            newAcc.Cookie = cleanCookie;
            newAcc.Info = info;
            m_Accounts.push_back(newAcc);
        }
    }
    
    Save();
    return true;
}

void AccountManager::RemoveAccount(const std::string& cookie) {
    bool removed = false;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = std::remove_if(m_Accounts.begin(), m_Accounts.end(), [&](const Account& a) {
            return a.Cookie == cookie;
        });
        if (it != m_Accounts.end()) {
            m_Accounts.erase(it, m_Accounts.end());
            removed = true;
        }
    }
    if (removed) {
        Save();
    }
}

std::vector<Account> AccountManager::GetAccounts() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_Accounts;
}

void AccountManager::UpdateAccountProcess(const std::string& cookie, int status, DWORD processId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& acc : m_Accounts) {
        if (acc.Cookie == cookie) {
            acc.Status = status;
            acc.ProcessId = processId;
            if (processId != 0 && !acc.Analytics.hasLaunchTime) {
                acc.Analytics.launchTime = std::chrono::system_clock::now();
                acc.Analytics.hasLaunchTime = true;
            }
            if (status == 0) {
                acc.ActiveAntiAFK = false;
                acc.Analytics.hasLaunchTime = false;
                acc.Analytics.cpuUsage = 0.0;
                acc.Analytics.ramUsageMB = 0.0;
                acc.Analytics.lastSystemTime = 0;
                acc.Analytics.lastProcessTime = 0;
            }
            break;
        }
    }
}

void AccountManager::SetActiveAntiAFK(const std::string& cookie, bool active) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& acc : m_Accounts) {
        if (acc.Cookie == cookie) {
            acc.ActiveAntiAFK = active;
            break;
        }
    }
}

void AccountManager::UpdateAccountAnalytics(const std::string& cookie, const AnalyticsState& analytics) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& acc : m_Accounts) {
        if (acc.Cookie == cookie) {
            acc.Analytics = analytics;
            break;
        }
    }
}

void AccountManager::SetAccounts(const std::vector<Account>& accounts) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_Accounts = accounts;
    }
    Save();
}

std::vector<std::string> AccountManager::GetGroups() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_Groups;
}

void AccountManager::SetGroups(const std::vector<std::string>& groups) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_Groups = groups;
    }
    Save();
}

void AccountManager::UpdateAccountInfo(const std::string& cookie, const RobloxAPI::UserInfo& info) {
    bool changed = false;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& acc : m_Accounts) {
            if (acc.Cookie == cookie) {
                if (acc.Info.ThumbnailUrl != info.ThumbnailUrl || acc.Info.Username != info.Username || acc.Info.DisplayName != info.DisplayName) {
                    acc.Info = info;
                    changed = true;
                }
                break;
            }
        }
    }
    if (changed) {
        Save();
    }
}

void AccountManager::UpdateAccountGame(const std::string& cookie, const std::string& placeId, const std::string& psLink, bool joinLowServer, bool lowestGraphics, bool antiAfk, const std::string& fflagOpt) {
    bool changed = false;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& acc : m_Accounts) {
            if (acc.Cookie == cookie) {
                if (acc.PlaceId != placeId || acc.PrivateServerLink != psLink || acc.JoinLowServer != joinLowServer || acc.LowestGraphics != lowestGraphics || acc.AntiAFK != antiAfk || acc.FFlagOptimization != fflagOpt) {
                    acc.PlaceId = placeId;
                    acc.PrivateServerLink = psLink;
                    acc.JoinLowServer = joinLowServer;
                    acc.LowestGraphics = lowestGraphics;
                    acc.AntiAFK = antiAfk;
                    if (acc.ProcessId != 0) {
                        acc.ActiveAntiAFK = antiAfk;
                    }
                    acc.FFlagOptimization = fflagOpt;
                    changed = true;
                }
                break;
            }
        }
    }
    if (changed) {
        Save();
    }
}

void AccountManager::HardReset() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_Accounts.clear();
    m_Groups.clear();
    m_GroupConfigs.clear();
    m_Password = "";
    m_Salt = "";
    m_LoadFailed = false;
    std::error_code ec;
    std::filesystem::remove(m_FilePath, ec);
    std::filesystem::remove(m_FilePath + ".bak", ec);
}

std::map<std::string, GroupLaunchConfig> AccountManager::GetGroupConfigs() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_GroupConfigs;
}

void AccountManager::SetGroupConfig(const std::string& groupName, const GroupLaunchConfig& config) {
    if (groupName.empty()) return;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_GroupConfigs[groupName] = config;
        if (config.ForceOverride) {
            for (auto& acc : m_Accounts) {
                if (acc.Group == groupName && acc.ProcessId != 0) {
                    acc.ActiveAntiAFK = config.AntiAFK;
                }
            }
        }
    }
    Save();
}

void AccountManager::DeleteGroupConfig(const std::string& groupName) {
    if (groupName.empty()) return;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_GroupConfigs.erase(groupName);
    }
    Save();
}

void AccountManager::RenameGroupConfig(const std::string& oldName, const std::string& newName) {
    if (oldName.empty() || newName.empty() || oldName == newName) return;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_GroupConfigs.find(oldName);
        if (it != m_GroupConfigs.end()) {
            GroupLaunchConfig cfg = it->second;
            m_GroupConfigs.erase(it);
            m_GroupConfigs[newName] = cfg;
        }
    }
    Save();
}