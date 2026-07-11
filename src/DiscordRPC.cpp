#include "DiscordRPC.h"
#include <iostream>
#include <chrono>
#include "../vendor/json.hpp"

using json = nlohmann::json;

std::string DiscordRPC::m_ClientId;
HANDLE DiscordRPC::m_Pipe = INVALID_HANDLE_VALUE;
std::atomic<bool> DiscordRPC::m_IsRunning(false);
std::thread DiscordRPC::m_ReconnectThread;
std::mutex DiscordRPC::m_Mutex;
int DiscordRPC::m_AccountCount = 0;
int64_t DiscordRPC::m_StartTime = 0;

void DiscordRPC::Initialize(const std::string& clientId) {
    if (m_IsRunning) return;
    
    m_ClientId = clientId;
    m_IsRunning = true;
    m_StartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    m_ReconnectThread = std::thread(ReconnectLoop);
}

void DiscordRPC::Shutdown() {
    m_IsRunning = false;
    if (m_ReconnectThread.joinable()) {
        m_ReconnectThread.join();
    }
    Disconnect();
}

void DiscordRPC::UpdatePresence(int accountCount) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_AccountCount = accountCount;
    
    if (m_Pipe == INVALID_HANDLE_VALUE) return;
    
    json payload;
    payload["cmd"] = "SET_ACTIVITY";
    payload["args"]["pid"] = GetCurrentProcessId();
    
    json activity;
    if (accountCount == 0) {
        activity["state"] = "Idling";
    } else {
        activity["state"] = "Managing " + std::to_string(accountCount) + " Account" + (accountCount > 1 ? "s" : "");
    }
    activity["details"] = "RoPilot v1.0.4";
    activity["timestamps"]["start"] = m_StartTime;
    activity["assets"]["large_image"] = "icon";
    activity["assets"]["large_text"] = "RoPilot";
    
    json button;
    button["label"] = "Use RoPilot Now!";
    button["url"] = "https://github.com/NotVeen/RoPilot/releases/latest";
    activity["buttons"] = json::array({button});
    
    payload["args"]["activity"] = activity;
    payload["nonce"] = "1";
    
    uint32_t op = 1;
    std::string payloadStr = payload.dump();
    uint32_t len = payloadStr.length();
    
    // Drain any pending messages from Discord
    DWORD bytesAvail = 0;
    while (PeekNamedPipe(m_Pipe, nullptr, 0, nullptr, &bytesAvail, nullptr) && bytesAvail > 0) {
        char buf[1024];
        DWORD bytesRead;
        ReadFile(m_Pipe, buf, min(bytesAvail, (DWORD)sizeof(buf)), &bytesRead, nullptr);
    }
    
    DWORD bytesWritten;
    if (!WriteFile(m_Pipe, &op, sizeof(op), &bytesWritten, nullptr) ||
        !WriteFile(m_Pipe, &len, sizeof(len), &bytesWritten, nullptr) ||
        !WriteFile(m_Pipe, payloadStr.c_str(), len, &bytesWritten, nullptr)) {
        // Disconnect if write fails
        CloseHandle(m_Pipe);
        m_Pipe = INVALID_HANDLE_VALUE;
    }
}

bool DiscordRPC::Connect() {
    for (int i = 0; i < 10; ++i) {
        std::string pipeName = "\\\\.\\pipe\\discord-ipc-" + std::to_string(i);
        m_Pipe = CreateFileA(pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (m_Pipe != INVALID_HANDLE_VALUE) {
            return true;
        }
    }
    return false;
}

void DiscordRPC::Disconnect() {
    if (m_Pipe != INVALID_HANDLE_VALUE) {
        CloseHandle(m_Pipe);
        m_Pipe = INVALID_HANDLE_VALUE;
    }
}

bool DiscordRPC::SendHandshake() {
    json payload;
    payload["v"] = 1;
    payload["client_id"] = m_ClientId;
    std::string payloadStr = payload.dump();
    
    uint32_t op = 0;
    uint32_t len = payloadStr.length();
    
    DWORD bytesWritten;
    if (!WriteFile(m_Pipe, &op, sizeof(op), &bytesWritten, nullptr)) return false;
    if (!WriteFile(m_Pipe, &len, sizeof(len), &bytesWritten, nullptr)) return false;
    if (!WriteFile(m_Pipe, payloadStr.c_str(), len, &bytesWritten, nullptr)) return false;
    
    // Read the response to complete handshake
    uint32_t readOp, readLen;
    DWORD bytesRead;
    if (ReadFile(m_Pipe, &readOp, sizeof(readOp), &bytesRead, nullptr) && bytesRead == sizeof(readOp)) {
        if (ReadFile(m_Pipe, &readLen, sizeof(readLen), &bytesRead, nullptr) && bytesRead == sizeof(readLen)) {
            if (readLen > 0 && readLen < 10000) { // arbitrary safe limit
                char* buf = new char[readLen + 1];
                ReadFile(m_Pipe, buf, readLen, &bytesRead, nullptr);
                delete[] buf;
            }
        }
    }
    
    return true;
}

void DiscordRPC::ReconnectLoop() {
    while (m_IsRunning) {
        bool needsHandshake = false;
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_Pipe == INVALID_HANDLE_VALUE) {
                if (Connect()) {
                    needsHandshake = true;
                }
            }
        }
        
        if (needsHandshake) {
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (!SendHandshake()) {
                Disconnect();
            }
        }
        
        // Update presence
        UpdatePresence(m_AccountCount);
        
        // Wait 5 seconds
        for (int i = 0; i < 50 && m_IsRunning; ++i) {
            Sleep(100);
        }
    }
}