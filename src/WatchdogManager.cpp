#include "WatchdogManager.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>

namespace fs = std::filesystem;

WatchdogManager& WatchdogManager::GetInstance() {
    static WatchdogManager instance;
    return instance;
}

void WatchdogManager::SetLaunchCallback(std::function<void(const std::string& cookie)> callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_launchCallback = callback;
}

void WatchdogManager::SetStatusUpdateCallback(std::function<void(const std::string& cookie, int status, int countdown)> callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_statusUpdateCallback = callback;
}

void WatchdogManager::SetToastCallback(std::function<void(const std::string& message, bool isError)> callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_toastCallback = callback;
}

void WatchdogManager::OnAccountLaunched(const std::string& cookie, const std::string& username, const std::string& userId, DWORD pid) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_deliberatelyStopped.erase(cookie);

    AccountWatchdogState& state = m_states[cookie];
    state.Cookie = cookie;
    state.Username = username;
    state.UserId = userId;
    state.ProcessId = pid;
    state.Status = WatchdogStatus::Monitoring;
    state.CountdownSeconds = 0;
    state.LaunchTime = std::chrono::steady_clock::now();
    state.LogFilePath = "";
    state.LastLogReadOffset = 0;
}

void WatchdogManager::OnAccountJoinedGame(const std::string& cookie) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_states.find(cookie);
    if (it != m_states.end()) {
        it->second.RetryCount = 0;
    }
}

void WatchdogManager::MarkDeliberatelyStopped(const std::string& cookie) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_deliberatelyStopped.insert(cookie);
    auto it = m_states.find(cookie);
    if (it != m_states.end()) {
        it->second.Status = WatchdogStatus::Idle;
        it->second.CountdownSeconds = 0;
        it->second.RetryCount = 0;
        it->second.ProcessId = 0;
    }
}

bool WatchdogManager::IsDeliberatelyStopped(const std::string& cookie) {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_deliberatelyStopped.find(cookie) != m_deliberatelyStopped.end();
}

void WatchdogManager::ClearDeliberatelyStopped(const std::string& cookie) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_deliberatelyStopped.erase(cookie);
}

void WatchdogManager::CancelRejoin(const std::string& cookie) {
    std::function<void(const std::string&, int, int)> updateCb;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_deliberatelyStopped.insert(cookie);
        auto it = m_states.find(cookie);
        if (it != m_states.end()) {
            it->second.Status = WatchdogStatus::Idle;
            it->second.CountdownSeconds = 0;
            it->second.RetryCount = 0;
            it->second.ProcessId = 0;
        }
        updateCb = m_statusUpdateCallback;
    }
    if (updateCb) {
        updateCb(cookie, 0, 0);
    }
}

void WatchdogManager::DisableAll() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& pair : m_states) {
        m_deliberatelyStopped.insert(pair.first);
        pair.second.Status = WatchdogStatus::Idle;
        pair.second.CountdownSeconds = 0;
        pair.second.RetryCount = 0;
    }
}

bool WatchdogManager::IsRejoining(const std::string& cookie, int& outCountdown) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_states.find(cookie);
    if (it != m_states.end() && it->second.Status == WatchdogStatus::Rejoining) {
        outCountdown = it->second.CountdownSeconds;
        return true;
    }
    outCountdown = 0;
    return false;
}

int WatchdogManager::GetAccountStatus(const std::string& cookie) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_states.find(cookie);
    if (it != m_states.end() && it->second.Status == WatchdogStatus::Rejoining) {
        return 5;
    }
    return 0;
}

std::string WatchdogManager::FindLogFile(const std::string& userId, const std::chrono::steady_clock::time_point& launchTime) {
    char* localAppData = nullptr;
    size_t len = 0;
    if (_dupenv_s(&localAppData, &len, "LOCALAPPDATA") != 0 || !localAppData) {
        return "";
    }
    std::string logsDir = std::string(localAppData) + "\\Roblox\\logs";
    free(localAppData);

    std::error_code ec;
    if (!fs::exists(logsDir, ec)) {
        return "";
    }

    std::string bestPath = "";
    fs::file_time_type bestTime;
    bool found = false;

    // First try: find recent log file matching userId
    for (const auto& entry : fs::directory_iterator(logsDir, ec)) {
        if (!entry.is_regular_file(ec)) continue;
        std::string filename = entry.path().filename().string();
        if (filename.find("_Player_") == std::string::npos || filename.find(".log") == std::string::npos) {
            continue;
        }

        auto writeTime = entry.last_write_time(ec);
        if (!found || writeTime > bestTime) {
            bestTime = writeTime;
            bestPath = entry.path().string();
            found = true;
        }
    }

    return bestPath;
}

bool WatchdogManager::CheckLogForDisconnect(AccountWatchdogState& state) {
    if (state.LogFilePath.empty()) {
        state.LogFilePath = FindLogFile(state.UserId, state.LaunchTime);
        if (state.LogFilePath.empty()) return false;
    }

    HANDLE hFile = CreateFileA(
        state.LogFilePath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        CloseHandle(hFile);
        return false;
    }

    uint64_t currentSize = (uint64_t)fileSize.QuadPart;
    if (currentSize <= state.LastLogReadOffset) {
        CloseHandle(hFile);
        return false;
    }

    // Read up to last 16KB or diff
    uint64_t readStart = state.LastLogReadOffset;
    if (currentSize - readStart > 16384) {
        readStart = currentSize - 16384;
    }

    LARGE_INTEGER seekPos;
    seekPos.QuadPart = readStart;
    SetFilePointerEx(hFile, seekPos, NULL, FILE_BEGIN);

    DWORD bytesToRead = (DWORD)(currentSize - readStart);
    std::vector<char> buffer(bytesToRead + 1, 0);
    DWORD bytesRead = 0;

    bool disconnectFound = false;
    if (ReadFile(hFile, buffer.data(), bytesToRead, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string text(buffer.data(), bytesRead);

        if (text.find("[FLog::Network] Connection lost") != std::string::npos ||
            text.find("Disconnect event received") != std::string::npos ||
            text.find("Lost connection to the game server") != std::string::npos ||
            text.find("DisconnectionNotification") != std::string::npos ||
            text.find("Error: Disconnected") != std::string::npos ||
            text.find("[FLog::Network] Client:Disconnect") != std::string::npos) {
            disconnectFound = true;
        }
    }

    state.LastLogReadOffset = currentSize;
    CloseHandle(hFile);
    return disconnectFound;
}

struct EnumCrashWindowsData {
    DWORD targetPid;
    bool found;
};

static BOOL CALLBACK EnumWindowsCrashProc(HWND hwnd, LPARAM lParam) {
    auto* data = reinterpret_cast<EnumCrashWindowsData*>(lParam);
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == data->targetPid && IsWindowVisible(hwnd)) {
        char title[256] = {0};
        char className[256] = {0};
        GetWindowTextA(hwnd, title, sizeof(title));
        GetClassNameA(hwnd, className, sizeof(className));

        std::string sTitle(title);
        std::string sClass(className);
        if (sTitle.find("Roblox Crash") != std::string::npos ||
            sTitle.find("An unexpected error occurred") != std::string::npos ||
            (sClass == "#32770" && sTitle.find("Roblox") != std::string::npos)) {
            data->found = true;
            return FALSE;
        }
    }
    return TRUE;
}

bool WatchdogManager::CheckCrashDialog(DWORD pid) {
    if (pid == 0) return false;
    EnumCrashWindowsData data = { pid, false };
    EnumWindows(EnumWindowsCrashProc, (LPARAM)&data);
    return data.found;
}

void WatchdogManager::TriggerDisconnect(AccountWatchdogState& state, int rejoinDelay, int maxRetries, bool autoRejoinEnabled, const std::string& language) {
    // Terminate hanging process if still running
    if (state.ProcessId != 0) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, state.ProcessId);
        if (hProcess) {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
        state.ProcessId = 0;
    }

    if (autoRejoinEnabled && state.RetryCount < maxRetries) {
        state.RetryCount++;
        state.Status = WatchdogStatus::Rejoining;
        state.CountdownSeconds = (rejoinDelay > 0) ? rejoinDelay : 10;

        if (m_statusUpdateCallback) {
            m_statusUpdateCallback(state.Cookie, 5, state.CountdownSeconds);
        }

        if (m_toastCallback) {
            std::string toastMsg;
            if (language == "id") {
                toastMsg = state.Username + " terputus. Rejoining in " + std::to_string(state.CountdownSeconds) + "s (" + std::to_string(state.RetryCount) + "/" + std::to_string(maxRetries) + ")";
            } else {
                toastMsg = state.Username + " disconnected. Rejoining in " + std::to_string(state.CountdownSeconds) + "s (" + std::to_string(state.RetryCount) + "/" + std::to_string(maxRetries) + ")";
            }
            m_toastCallback(toastMsg, false);
        }
    } else {
        if (state.RetryCount >= maxRetries && m_toastCallback) {
            std::string toastMsg;
            if (language == "id") {
                toastMsg = state.Username + " gagal auto-rejoin setelah " + std::to_string(maxRetries) + " percobaan";
            } else {
                toastMsg = state.Username + " auto-rejoin failed after " + std::to_string(maxRetries) + " attempts";
            }
            m_toastCallback(toastMsg, true);
        }
        state.Status = WatchdogStatus::Idle;
        state.RetryCount = 0;
        state.CountdownSeconds = 0;

        if (m_statusUpdateCallback) {
            m_statusUpdateCallback(state.Cookie, 0, 0);
        }
    }
}

void WatchdogManager::Tick(int rejoinDelay, int maxRetries, bool autoRejoinEnabled, const std::string& language) {
    std::vector<std::string> cookiesToLaunch;
    std::function<void(const std::string&, int, int)> statusCb;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tickCounter++;
        statusCb = m_statusUpdateCallback;

        for (auto& pair : m_states) {
            AccountWatchdogState& state = pair.second;

            if (state.Status == WatchdogStatus::Monitoring) {
                if (state.ProcessId != 0) {
                    bool alive = false;
                    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, state.ProcessId);
                    if (hProcess) {
                        DWORD exitCode = 0;
                        if (GetExitCodeProcess(hProcess, &exitCode) && exitCode == STILL_ACTIVE) {
                            alive = true;
                        }
                        CloseHandle(hProcess);
                    }

                    if (!alive) {
                        if (m_deliberatelyStopped.find(state.Cookie) != m_deliberatelyStopped.end()) {
                            state.Status = WatchdogStatus::Idle;
                            state.ProcessId = 0;
                            if (statusCb) statusCb(state.Cookie, 0, 0);
                        } else {
                            TriggerDisconnect(state, rejoinDelay, maxRetries, autoRejoinEnabled, language);
                        }
                    } else {
                        // Alive: check crash dialog or log disconnect every 2 seconds
                        if (m_tickCounter % 2 == 0) {
                            if (CheckCrashDialog(state.ProcessId) || CheckLogForDisconnect(state)) {
                                TriggerDisconnect(state, rejoinDelay, maxRetries, autoRejoinEnabled, language);
                            }
                        }
                    }
                }
            } else if (state.Status == WatchdogStatus::Rejoining) {
                state.CountdownSeconds--;
                if (state.CountdownSeconds > 0) {
                    if (statusCb) {
                        statusCb(state.Cookie, 5, state.CountdownSeconds);
                    }
                } else {
                    state.Status = WatchdogStatus::Idle;
                    state.CountdownSeconds = 0;
                    cookiesToLaunch.push_back(state.Cookie);
                }
            }
        }
    }

    for (const auto& cookie : cookiesToLaunch) {
        if (m_launchCallback) {
            m_launchCallback(cookie);
        }
    }
}
