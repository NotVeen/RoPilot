#include "AccountManager.h"
#include "../vendor/json.hpp"
#include <fstream>
#include <iostream>
#include <mutex>
#include <wincrypt.h>

using json = nlohmann::json;

AccountManager::AccountManager(const std::string& filePath) : m_FilePath(filePath) {
    Load();
}

void AccountManager::Load() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_Accounts.clear();
    std::ifstream file(m_FilePath, std::ios::binary);
    if (!file.is_open()) return;

    try {
        std::vector<BYTE> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if (buffer.empty()) return;

        DATA_BLOB DataIn;
        DATA_BLOB DataOut;
        DataIn.pbData = buffer.data();
        DataIn.cbData = (DWORD)buffer.size();

        std::string jsonStr;

        if (CryptUnprotectData(&DataIn, NULL, NULL, NULL, NULL, 0, &DataOut)) {
            jsonStr = std::string((char*)DataOut.pbData, DataOut.cbData);
            LocalFree(DataOut.pbData);
        } else {
            jsonStr = std::string((char*)buffer.data(), buffer.size());
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
        }
    } catch (...) {}
}

void AccountManager::Save() {
    std::vector<Account> accsCopy;
    std::vector<std::string> groupsCopy;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        accsCopy = m_Accounts;
        groupsCopy = m_Groups;
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
        accountsArray.push_back(item);
    }
    j["accounts"] = accountsArray;
    
    json groupsArray = json::array();
    for (const auto& g : groupsCopy) {
        groupsArray.push_back(g);
    }
    j["groups"] = groupsArray;

    std::string jsonStr = j.dump(4);

    DATA_BLOB DataIn;
    DATA_BLOB DataOut;
    DataIn.pbData = (BYTE*)jsonStr.c_str();
    DataIn.cbData = (DWORD)jsonStr.length();

    if (CryptProtectData(&DataIn, L"RoPilot Encrypted Accounts", NULL, NULL, NULL, 0, &DataOut)) {
        std::ofstream file(m_FilePath, std::ios::binary);
        if (file.is_open()) {
            file.write((char*)DataOut.pbData, DataOut.cbData);
        }
        LocalFree(DataOut.pbData);
    }
}

bool AccountManager::AddAccount(const std::string& cookie) {
    RobloxAPI::UserInfo info = RobloxAPI::GetUserInfo(cookie);
    if (info.UserId == 0) return false; // Invalid cookie

    bool exists = false;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& acc : m_Accounts) {
            if (acc.Info.UserId == info.UserId) {
                acc.Cookie = cookie;
                acc.Info = info;
                acc.Status = 0;
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            Account newAcc;
            newAcc.Cookie = cookie;
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
            } else if (processId == 0) {
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
