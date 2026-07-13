#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <mutex>
#include <chrono>
#include "RobloxAPI.h"

struct AnalyticsState {
    uint64_t lastSystemTime = 0;
    uint64_t lastProcessTime = 0;
    double cpuUsage = 0.0;
    double ramUsageMB = 0.0;
    std::chrono::system_clock::time_point launchTime;
    bool hasLaunchTime = false;
};

struct Account {
    std::string Cookie;
    RobloxAPI::UserInfo Info;
    int Status = 0; // 0 = Offline, 1 = Launching, 2 = Online, 3 = In Game, 4 = Invalid Cookie
    std::string JobId = "";
    DWORD ProcessId = 0;
    std::string Group = "Ungrouped";
    std::string PlaceId = "";
    std::string PrivateServerLink = "";
    bool JoinLowServer = false;
    bool LowestGraphics = false;
    bool AntiAFK = false;
    std::string FFlagOptimization = "Default";
    AnalyticsState Analytics;
};

class AccountManager {
public:
    AccountManager(const std::string& filePath = "accounts.json");
    
    void Load(const std::string& password = "", const std::string& salt = "");
    void Save(const std::string& password = "", const std::string& salt = "");

    bool AddAccount(const std::string& cookie);
    void RemoveAccount(const std::string& cookie);
    void SetAccounts(const std::vector<Account>& accounts);
    
    std::vector<Account> GetAccounts();
    std::vector<std::string> GetGroups();
    void SetGroups(const std::vector<std::string>& groups);

    void UpdateAccountProcess(const std::string& cookie, int status, DWORD processId);
    void UpdateAccountAnalytics(const std::string& cookie, const AnalyticsState& analytics);
    void UpdateAccountInfo(const std::string& cookie, const RobloxAPI::UserInfo& info);
    void UpdateAccountGame(const std::string& cookie, const std::string& placeId, const std::string& psLink, bool joinLowServer, bool lowestGraphics, bool antiAfk, const std::string& fflagOpt);

private:
    std::string m_FilePath;
    std::vector<Account> m_Accounts;
    std::vector<std::string> m_Groups;
    std::mutex m_mutex;
    std::string m_Password;
    std::string m_Salt;
    bool m_LoadFailed = false;
};