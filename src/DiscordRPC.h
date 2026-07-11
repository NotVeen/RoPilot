#pragma once
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <windows.h>

class DiscordRPC {
public:
    static void Initialize(const std::string& clientId);
    static void UpdatePresence(int accountCount);
    static void Shutdown();

private:
    static void ReconnectLoop();
    static bool Connect();
    static void Disconnect();
    static bool SendHandshake();
    static bool SendPayload(int opcode, const std::string& payload);
    
    static std::string m_ClientId;
    static HANDLE m_Pipe;
    static std::atomic<bool> m_IsRunning;
    static std::thread m_ReconnectThread;
    static std::mutex m_Mutex;
    static int m_AccountCount;
    static int64_t m_StartTime;
};