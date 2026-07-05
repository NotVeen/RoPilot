#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <mutex>
#include "RobloxAPI.h"

struct Account {
    std::string Cookie;
    RobloxAPI::UserInfo Info;
    int Status = 0; // 0 = Offline, 1 = In Game, 2 = Loading
    std::string JobId = "";
    DWORD ProcessId = 0;
    std::string Group = "Ungrouped";
};

class AccountManager {
public:
    AccountManager(const std::string& filePath = "accounts.json");
    
    void Load();
    void Save();

    bool AddAccount(const std::string& cookie);
    void RemoveAccount(const std::string& cookie);
    void SetAccounts(const std::vector<Account>& accounts);
    
    std::vector<Account> GetAccounts();
    std::vector<std::string> GetGroups();
    void SetGroups(const std::vector<std::string>& groups);
    void UpdateAccountPresence(const std::string& cookie, int status, const std::string& jobId);
    void UpdateAccountProcess(const std::string& cookie, int status, DWORD processId);
    void UpdateAccountInfo(const std::string& cookie, const RobloxAPI::UserInfo& info);

private:
    std::string m_FilePath;
    std::vector<Account> m_Accounts;
    std::vector<std::string> m_Groups;
    std::mutex m_mutex;
};