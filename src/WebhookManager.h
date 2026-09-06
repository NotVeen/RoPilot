#pragma once
#include <string>
#include <functional>
#include <cstdint>
#include "../vendor/json.hpp"

class WebhookManager {
public:
    static WebhookManager& GetInstance();

    // Event alerts
    void SendCrashAlert(const std::string& username, const std::string& userId,
                        const std::string& gameName, const std::string& placeId,
                        const std::string& reason, int cooldownSec, int retryCount, int maxRetries);

    void SendRejoinAlert(const std::string& username, const std::string& userId,
                         const std::string& gameName, const std::string& placeId,
                         int attempt, int maxRetries, bool success);

    void SendTestWebhook(const std::string& webhookUrl, std::function<void(bool success, const std::string& message)> callback = nullptr);

    // Generic asynchronous delivery
    void PostAsync(const std::string& webhookUrl, const nlohmann::json& payload,
                   std::function<void(bool success, const std::string& message)> callback = nullptr);

    void EnsureWebhookConfigured(const std::string& webhookUrl);
    bool PatchSync(const std::string& webhookUrl, const std::string& jsonString, std::string& outResponse, int& outStatusCode);

private:
    WebhookManager() = default;
    ~WebhookManager() = default;
    WebhookManager(const WebhookManager&) = delete;
    WebhookManager& operator=(const WebhookManager&) = delete;

    bool PostSync(const std::string& webhookUrl, const std::string& jsonString, std::string& outResponse, int& outStatusCode);
};
