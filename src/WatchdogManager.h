#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <mutex>
#include <chrono>
#include <functional>
#include <windows.h>

enum class WatchdogStatus {
    Idle,
    Monitoring,
    Rejoining,
    Failed
};

struct AccountWatchdogState {
    std::string Cookie;
    std::string Username;
    std::string UserId;
    DWORD ProcessId = 0;
    WatchdogStatus Status = WatchdogStatus::Idle;
    int CountdownSeconds = 0;
    int RetryCount = 0;
    std::chrono::steady_clock::time_point LaunchTime;
    std::string LogFilePath = "";
    uint64_t LastLogReadOffset = 0;
};

class WatchdogManager {
public:
    static WatchdogManager& GetInstance();

    void SetLaunchCallback(std::function<void(const std::string& cookie)> callback);
    void SetStatusUpdateCallback(std::function<void(const std::string& cookie, int status, int countdown)> callback);
    void SetToastCallback(std::function<void(const std::string& message, bool isError)> callback);

    void OnAccountLaunched(const std::string& cookie, const std::string& username, const std::string& userId, DWORD pid);
    void OnAccountJoinedGame(const std::string& cookie);
    void MarkDeliberatelyStopped(const std::string& cookie);
    bool IsDeliberatelyStopped(const std::string& cookie);
    void ClearDeliberatelyStopped(const std::string& cookie);
    void CancelRejoin(const std::string& cookie);
    void DisableAll();

    bool IsRejoining(const std::string& cookie, int& outCountdown);
    int GetAccountStatus(const std::string& cookie);

    // Called periodically (e.g. every 1s) from the background monitor thread
    void Tick(int rejoinDelay, int maxRetries, bool autoRejoinEnabled, const std::string& language);

private:
    WatchdogManager() = default;
    ~WatchdogManager() = default;
    WatchdogManager(const WatchdogManager&) = delete;
    WatchdogManager& operator=(const WatchdogManager&) = delete;

    std::string FindLogFile(const std::string& userId, const std::chrono::steady_clock::time_point& launchTime);
    bool CheckLogForDisconnect(AccountWatchdogState& state);
    bool CheckCrashDialog(DWORD pid);
    void TriggerDisconnect(AccountWatchdogState& state, int rejoinDelay, int maxRetries, bool autoRejoinEnabled, const std::string& language);

    std::mutex m_mutex;
    std::map<std::string, AccountWatchdogState> m_states;
    std::set<std::string> m_deliberatelyStopped;
    std::function<void(const std::string& cookie)> m_launchCallback;
    std::function<void(const std::string& cookie, int status, int countdown)> m_statusUpdateCallback;
    std::function<void(const std::string& message, bool isError)> m_toastCallback;
    int m_tickCounter = 0;
};
