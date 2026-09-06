#include "WebhookManager.h"
#include "SettingsManager.h"
#include "AccountManager.h"
#include "RobloxAPI.h"
#include "IconBase64.h"
#include <windows.h>
#include <winhttp.h>
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>

#pragma comment(lib, "winhttp.lib")

extern SettingsManager g_settingsManager;
extern AccountManager g_accountManager;

static const char* WEBHOOK_USERNAME = "RoPilot Webhook";
static bool s_avatarConfigured = false;
static std::string s_lastConfiguredUrl = "";
static std::string s_discordCdnAvatarUrl = "";
static std::mutex s_configMutex;

static std::string GetAvatarUrl(const std::string& userId) {
    if (userId.empty() || userId == "0") return "";
    for (const auto& a : g_accountManager.GetAccounts()) {
        if (std::to_string(a.Info.UserId) == userId && !a.Info.ThumbnailUrl.empty()) {
            return a.Info.ThumbnailUrl;
        }
    }
    // Fallback: query official Roblox Thumbnail API directly
    std::wstring path = L"/v1/users/avatar-headshot?userIds=" + RobloxAPI::s2ws(userId) + L"&size=150x150&format=Png&isCircular=false";
    std::string res = RobloxAPI::HttpRequest(L"GET", L"thumbnails.roblox.com", path, "");
    try {
        auto j = nlohmann::json::parse(res);
        if (j.contains("data") && j["data"].is_array() && !j["data"].empty()) {
            return j["data"][0].value("imageUrl", "");
        }
    } catch (...) {}
    return "";
}

static std::string ResolveGameTitle(const std::string& userId, const std::string& placeId, const std::string& initialName) {
    if (!initialName.empty() && initialName != "Roblox Experience") {
        return initialName;
    }
    if (!userId.empty() && userId != "0") {
        for (const auto& a : g_accountManager.GetAccounts()) {
            if (std::to_string(a.Info.UserId) == userId && !a.GameName.empty() && a.GameName != "Roblox Experience") {
                return a.GameName;
            }
        }
    }
    if (!placeId.empty() && placeId != "0" && placeId != "N/A") {
        std::string resolved = RobloxAPI::GetGameName(placeId, "", "");
        if (!resolved.empty() && resolved != "Roblox Experience") {
            return resolved;
        }
    }
    return "Roblox Experience";
}

WebhookManager& WebhookManager::GetInstance() {
    static WebhookManager instance;
    return instance;
}

static std::wstring ToWide(const std::string& str) {
    if (str.empty()) return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
    if (!wstr.empty() && wstr.back() == L'\0') wstr.pop_back();
    return wstr;
}

static std::string ToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size, nullptr, nullptr);
    if (!str.empty() && str.back() == '\0') str.pop_back();
    return str;
}

bool WebhookManager::PostSync(const std::string& webhookUrl, const std::string& jsonString, std::string& outResponse, int& outStatusCode) {
    outStatusCode = 0;
    outResponse.clear();

    if (webhookUrl.empty()) {
        outResponse = "Empty webhook URL";
        return false;
    }

    std::string targetUrl = webhookUrl;
    if (targetUrl.find("with_components=true") == std::string::npos) {
        if (targetUrl.find('?') == std::string::npos) {
            targetUrl += "?with_components=true";
        } else {
            targetUrl += "&with_components=true";
        }
    }

    std::wstring wUrl = ToWide(targetUrl);

    URL_COMPONENTS urlComp = { 0 };
    urlComp.dwStructSize = sizeof(urlComp);
    urlComp.dwSchemeLength = (DWORD)-1;
    urlComp.dwHostNameLength = (DWORD)-1;
    urlComp.dwUrlPathLength = (DWORD)-1;
    urlComp.dwExtraInfoLength = (DWORD)-1;

    if (!WinHttpCrackUrl(wUrl.c_str(), (DWORD)wUrl.length(), 0, &urlComp)) {
        outResponse = "Invalid webhook URL format";
        return false;
    }

    std::wstring host(urlComp.lpszHostName, urlComp.dwHostNameLength);
    std::wstring path(urlComp.lpszUrlPath, urlComp.dwUrlPathLength + urlComp.dwExtraInfoLength);
    bool isHttps = (urlComp.nScheme == INTERNET_SCHEME_HTTPS);

    HINTERNET hSession = WinHttpOpen(
        L"RoPilot-Webhook/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );
    if (!hSession) {
        outResponse = "Failed to initialize WinHTTP session";
        return false;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), urlComp.nPort, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        outResponse = "Failed to connect to Discord server";
        return false;
    }

    DWORD reqFlags = isHttps ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"POST",
        path.c_str(),
        nullptr,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        reqFlags
    );

    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        outResponse = "Failed to create HTTP request";
        return false;
    }

    // Set timeout to 10 seconds
    WinHttpSetTimeouts(hRequest, 5000, 5000, 10000, 10000);

    std::wstring headers = L"Content-Type: application/json; charset=utf-8\r\n";
    BOOL sendOk = WinHttpSendRequest(
        hRequest,
        headers.c_str(),
        (DWORD)headers.length(),
        (LPVOID)jsonString.c_str(),
        (DWORD)jsonString.length(),
        (DWORD)jsonString.length(),
        0
    );

    if (!sendOk || !WinHttpReceiveResponse(hRequest, nullptr)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        outResponse = "Failed to receive response from Discord";
        return false;
    }

    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    WinHttpQueryHeaders(
        hRequest,
        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
        WINHTTP_HEADER_NAME_BY_INDEX,
        &statusCode,
        &statusCodeSize,
        WINHTTP_NO_HEADER_INDEX
    );
    outStatusCode = (int)statusCode;

    std::string responseBody;
    DWORD bytesAvailable = 0;
    while (WinHttpQueryDataAvailable(hRequest, &bytesAvailable) && bytesAvailable > 0) {
        std::vector<char> buffer(bytesAvailable + 1, 0);
        DWORD bytesRead = 0;
        if (WinHttpReadData(hRequest, buffer.data(), bytesAvailable, &bytesRead) && bytesRead > 0) {
            responseBody.append(buffer.data(), bytesRead);
        } else {
            break;
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    outResponse = responseBody;
    // Discord returns 200 OK or 204 No Content for successful webhook execution
    return (outStatusCode == 200 || outStatusCode == 204);
}

bool WebhookManager::PatchSync(const std::string& webhookUrl, const std::string& jsonString, std::string& outResponse, int& outStatusCode) {
    outStatusCode = 0;
    outResponse.clear();

    if (webhookUrl.empty()) {
        outResponse = "Empty webhook URL";
        return false;
    }

    std::wstring wUrl = ToWide(webhookUrl);
    URL_COMPONENTS urlComp = { 0 };
    urlComp.dwStructSize = sizeof(urlComp);
    urlComp.dwSchemeLength = (DWORD)-1;
    urlComp.dwHostNameLength = (DWORD)-1;
    urlComp.dwUrlPathLength = (DWORD)-1;
    urlComp.dwExtraInfoLength = (DWORD)-1;

    if (!WinHttpCrackUrl(wUrl.c_str(), (DWORD)wUrl.length(), 0, &urlComp)) {
        outResponse = "Invalid webhook URL format";
        return false;
    }

    std::wstring host(urlComp.lpszHostName, urlComp.dwHostNameLength);
    std::wstring path(urlComp.lpszUrlPath, urlComp.dwUrlPathLength);
    bool isHttps = (urlComp.nScheme == INTERNET_SCHEME_HTTPS);

    HINTERNET hSession = WinHttpOpen(
        L"RoPilot-Webhook/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );
    if (!hSession) {
        outResponse = "Failed to initialize WinHTTP session";
        return false;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), urlComp.nPort, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        outResponse = "Failed to connect to Discord server";
        return false;
    }

    DWORD reqFlags = isHttps ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"PATCH",
        path.c_str(),
        nullptr,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        reqFlags
    );

    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        outResponse = "Failed to create HTTP request";
        return false;
    }

    WinHttpSetTimeouts(hRequest, 5000, 5000, 10000, 10000);

    std::wstring headers = L"Content-Type: application/json; charset=utf-8\r\n";
    BOOL sendOk = WinHttpSendRequest(
        hRequest,
        headers.c_str(),
        (DWORD)headers.length(),
        (LPVOID)jsonString.c_str(),
        (DWORD)jsonString.length(),
        (DWORD)jsonString.length(),
        0
    );

    if (!sendOk || !WinHttpReceiveResponse(hRequest, nullptr)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        outResponse = "Failed to receive response from Discord";
        return false;
    }

    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    WinHttpQueryHeaders(
        hRequest,
        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
        WINHTTP_HEADER_NAME_BY_INDEX,
        &statusCode,
        &statusCodeSize,
        WINHTTP_NO_HEADER_INDEX
    );
    outStatusCode = (int)statusCode;

    std::string responseBody;
    DWORD bytesAvailable = 0;
    while (WinHttpQueryDataAvailable(hRequest, &bytesAvailable) && bytesAvailable > 0) {
        std::vector<char> buffer(bytesAvailable + 1, 0);
        DWORD bytesRead = 0;
        if (WinHttpReadData(hRequest, buffer.data(), bytesAvailable, &bytesRead) && bytesRead > 0) {
            responseBody.append(buffer.data(), bytesRead);
        } else {
            break;
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    outResponse = responseBody;
    return (outStatusCode == 200);
}

void WebhookManager::EnsureWebhookConfigured(const std::string& webhookUrl) {
    if (webhookUrl.empty()) return;
    {
        std::lock_guard<std::mutex> lock(s_configMutex);
        if (s_avatarConfigured && s_lastConfiguredUrl == webhookUrl) {
            return;
        }
    }

    nlohmann::json patchBody;
    patchBody["name"] = WEBHOOK_USERNAME;
    patchBody["avatar"] = "data:image/png;base64," + std::string(ROPILOT_ICON_BASE64);

    std::string response;
    int statusCode = 0;
    bool ok = PatchSync(webhookUrl, patchBody.dump(), response, statusCode);
    if (ok && statusCode == 200) {
        std::lock_guard<std::mutex> lock(s_configMutex);
        s_avatarConfigured = true;
        s_lastConfiguredUrl = webhookUrl;
        try {
            auto j = nlohmann::json::parse(response);
            std::string id = j.value("id", "");
            std::string av = j.value("avatar", "");
            if (!id.empty() && !av.empty()) {
                s_discordCdnAvatarUrl = "https://cdn.discordapp.com/avatars/" + id + "/" + av + ".png";
            }
        } catch (...) {}
    }
}

void WebhookManager::PostAsync(const std::string& webhookUrl, const nlohmann::json& payload,
                              std::function<void(bool success, const std::string& message)> callback) {
    std::string jsonStr = payload.dump();
    std::thread([this, webhookUrl, jsonStr, callback]() {
        std::string response;
        int statusCode = 0;
        bool ok = PostSync(webhookUrl, jsonStr, response, statusCode);
        if (callback) {
            if (ok) {
                callback(true, "Delivered (HTTP " + std::to_string(statusCode) + ")");
            } else {
                std::string err = "Failed (HTTP " + std::to_string(statusCode) + ")";
                if (!response.empty()) {
                    try {
                        auto jErr = nlohmann::json::parse(response);
                        if (jErr.contains("message")) {
                            err += ": " + jErr["message"].get<std::string>();
                        }
                        if (jErr.contains("errors")) {
                            err += " - " + jErr["errors"].dump();
                        }
                    } catch (...) {
                        err += ": " + response.substr(0, 200);
                    }
                }
                callback(false, err);
            }
        }
    }).detach();
}

void WebhookManager::SendCrashAlert(const std::string& username, const std::string& userId,
                                   const std::string& gameName, const std::string& placeId,
                                   const std::string& reason, int cooldownSec, int retryCount, int maxRetries) {
    Settings s = g_settingsManager.GetSettings();
    if (!s.WebhookEnabled || s.WebhookUrl.empty() || !s.WebhookNotifyCrash) return;

    EnsureWebhookConfigured(s.WebhookUrl);

    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    std::string displayGame = ResolveGameTitle(userId, placeId, gameName);
    std::string displayPlace = placeId.empty() ? "N/A" : placeId;
    std::string displayReason = reason.empty() ? "Unexpected client termination / freeze" : reason;
    std::string displayUser = username.empty() ? "Unknown Player" : username;

    // Discord Components V2 structure
    nlohmann::json containerComponents = nlohmann::json::array();

    // 1. Header (Section with accessory if avatar available, otherwise Text Display)
    std::string avatarUrl = GetAvatarUrl(userId);
    if (!avatarUrl.empty()) {
        nlohmann::json sectionComp;
        sectionComp["type"] = 9;
        sectionComp["components"] = nlohmann::json::array({
            {
                {"type", 10},
                {"content", "## :warning: Roblox Crash Detected"}
            },
            {
                {"type", 10},
                {"content", "**Player:** `" + displayUser + "` (" + userId + ")"}
            }
        });
        sectionComp["accessory"] = {
            {"type", 11},
            {"media", {
                {"url", avatarUrl}
            }}
        };
        containerComponents.push_back(sectionComp);
    } else {
        containerComponents.push_back({
            {"type", 10},
            {"content", "## :warning: Roblox Crash Detected\n**Player:** `" + displayUser + "` (" + userId + ")"}
        });
    }

    // 2. Separator (type 14)
    containerComponents.push_back({
        {"type", 14},
        {"divider", true},
        {"spacing", 1}
    });

    // 3. Details Text Display (type 10)
    std::string details =
        "**Experience:** " + displayGame + "\n" +
        "**Place ID:** `" + displayPlace + "`\n" +
        "**Reason:** " + displayReason + "\n" +
        "**Watchdog Action:** Auto-rejoining in " + std::to_string(cooldownSec) + "s (Attempt " +
        std::to_string(retryCount) + "/" + std::to_string(maxRetries) + ")\n" +
        "**Timestamp:** <t:" + std::to_string(now) + ":F>";

    containerComponents.push_back({
        {"type", 10},
        {"content", details}
    });

    // 4. Separator (type 14)
    containerComponents.push_back({
        {"type", 14},
        {"divider", true},
        {"spacing", 1}
    });

    // 5. Action Row with Link Buttons (type 1 & 2)
    nlohmann::json actionRow;
    actionRow["type"] = 1;
    nlohmann::json buttons = nlohmann::json::array();

    if (!userId.empty() && userId != "0") {
        buttons.push_back({
            {"type", 2},
            {"style", 5},
            {"label", "Roblox Profile"},
            {"url", "https://www.roblox.com/users/" + userId + "/profile"}
        });
    }

    if (!placeId.empty() && placeId != "0" && placeId != "N/A") {
        buttons.push_back({
            {"type", 2},
            {"style", 5},
            {"label", "Game Page"},
            {"url", "https://www.roblox.com/games/" + placeId}
        });
    }

    if (!buttons.empty()) {
        actionRow["components"] = buttons;
        containerComponents.push_back(actionRow);
    }

    nlohmann::json payload;
    payload["username"] = WEBHOOK_USERNAME;
    {
        std::lock_guard<std::mutex> lock(s_configMutex);
        if (!s_discordCdnAvatarUrl.empty()) {
            payload["avatar_url"] = s_discordCdnAvatarUrl;
        }
    }
    payload["flags"] = 32768; // IS_COMPONENTS_V2
    payload["components"] = nlohmann::json::array({
        {
            {"type", 17}, // Container
            {"accent_color", 15680324}, // Red (#EF4444)
            {"components", containerComponents}
        }
    });

    PostAsync(s.WebhookUrl, payload, nullptr);
}

void WebhookManager::SendRejoinAlert(const std::string& username, const std::string& userId,
                                     const std::string& gameName, const std::string& placeId,
                                     int attempt, int maxRetries, bool success) {
    Settings s = g_settingsManager.GetSettings();
    if (!s.WebhookEnabled || s.WebhookUrl.empty() || !s.WebhookNotifyRejoin) return;

    EnsureWebhookConfigured(s.WebhookUrl);

    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    std::string displayGame = ResolveGameTitle(userId, placeId, gameName);
    std::string displayPlace = placeId.empty() ? "N/A" : placeId;
    std::string displayUser = username.empty() ? "Unknown Player" : username;

    nlohmann::json containerComponents = nlohmann::json::array();

    // 1. Header (Section with accessory if avatar available, otherwise Text Display)
    std::string headerTitle = success ? "## :white_check_mark: Auto-Rejoin Successful" : "## :x: Auto-Rejoin Failed";
    std::string avatarUrl = GetAvatarUrl(userId);
    if (!avatarUrl.empty()) {
        nlohmann::json sectionComp;
        sectionComp["type"] = 9;
        sectionComp["components"] = nlohmann::json::array({
            {
                {"type", 10},
                {"content", headerTitle}
            },
            {
                {"type", 10},
                {"content", "**Player:** `" + displayUser + "` (" + userId + ")"}
            }
        });
        sectionComp["accessory"] = {
            {"type", 11},
            {"media", {
                {"url", avatarUrl}
            }}
        };
        containerComponents.push_back(sectionComp);
    } else {
        containerComponents.push_back({
            {"type", 10},
            {"content", headerTitle + "\n**Player:** `" + displayUser + "` (" + userId + ")"}
        });
    }

    // 2. Separator
    containerComponents.push_back({
        {"type", 14},
        {"divider", true},
        {"spacing", 1}
    });

    // 3. Details
    std::string details;
    if (success) {
        details =
            "**Experience:** " + displayGame + "\n" +
            "**Place ID:** `" + displayPlace + "`\n" +
            "**Status:** Successfully reconnected and joined server\n" +
            "**Attempt Taken:** " + std::to_string(attempt) + "/" + std::to_string(maxRetries) + "\n" +
            "**Timestamp:** <t:" + std::to_string(now) + ":F>";
    } else {
        details =
            "**Experience:** " + displayGame + "\n" +
            "**Place ID:** `" + displayPlace + "`\n" +
            "**Status:** Maximum retry limit reached (" + std::to_string(maxRetries) + "/" + std::to_string(maxRetries) + ")\n" +
            "**Action:** Account halted by Watchdog to protect account security\n" +
            "**Timestamp:** <t:" + std::to_string(now) + ":F>";
    }

    containerComponents.push_back({
        {"type", 10},
        {"content", details}
    });

    // 4. Separator
    containerComponents.push_back({
        {"type", 14},
        {"divider", true},
        {"spacing", 1}
    });

    // 5. Action Row with Link Buttons
    nlohmann::json actionRow;
    actionRow["type"] = 1;
    nlohmann::json buttons = nlohmann::json::array();

    if (!userId.empty() && userId != "0") {
        buttons.push_back({
            {"type", 2},
            {"style", 5},
            {"label", "Roblox Profile"},
            {"url", "https://www.roblox.com/users/" + userId + "/profile"}
        });
    }

    if (!placeId.empty() && placeId != "0" && placeId != "N/A") {
        buttons.push_back({
            {"type", 2},
            {"style", 5},
            {"label", "Game Page"},
            {"url", "https://www.roblox.com/games/" + placeId}
        });
    }

    if (!buttons.empty()) {
        actionRow["components"] = buttons;
        containerComponents.push_back(actionRow);
    }

    nlohmann::json payload;
    payload["username"] = WEBHOOK_USERNAME;
    {
        std::lock_guard<std::mutex> lock(s_configMutex);
        if (!s_discordCdnAvatarUrl.empty()) {
            payload["avatar_url"] = s_discordCdnAvatarUrl;
        }
    }
    payload["flags"] = 32768; // IS_COMPONENTS_V2
    payload["components"] = nlohmann::json::array({
        {
            {"type", 17}, // Container
            {"accent_color", success ? 1096065 : 14427686}, // Green (#10B981) or Dark Red (#DC2626)
            {"components", containerComponents}
        }
    });

    PostAsync(s.WebhookUrl, payload, nullptr);
}

void WebhookManager::SendTestWebhook(const std::string& webhookUrl, std::function<void(bool success, const std::string& message)> callback) {
    EnsureWebhookConfigured(webhookUrl);

    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    nlohmann::json containerComponents = nlohmann::json::array();

    // 1. Header Text
    containerComponents.push_back({
        {"type", 10},
        {"content", "## :rocket: RoPilot Webhook Connected\nRoPilot Discord Webhook integration is active and operating normally."}
    });

    // 2. Separator
    containerComponents.push_back({
        {"type", 14},
        {"divider", true},
        {"spacing", 1}
    });

    // 3. Details
    std::string details =
        std::string("**Client:** RoPilot Multi-Instance Manager\n") +
        "**Type:** Discord Webhook\n" +
        "**Monitored Events:** Crash Watchdog & Smart Auto-Rejoin\n" +
        "**Status:** Ready to dispatch real-time alerts\n" +
        "**Timestamp:** <t:" + std::to_string(now) + ":F>";

    containerComponents.push_back({
        {"type", 10},
        {"content", details}
    });

    // 4. Separator
    containerComponents.push_back({
        {"type", 14},
        {"divider", true},
        {"spacing", 1}
    });

    // 5. Action Row with Link Button
    containerComponents.push_back({
        {"type", 1},
        {"components", nlohmann::json::array({
            {
                {"type", 2},
                {"style", 5},
                {"label", "RoPilot GitHub"},
                {"url", "https://github.com/NotVeen/RoPilot"}
            }
        })}
    });

    nlohmann::json payload;
    payload["username"] = WEBHOOK_USERNAME;
    {
        std::lock_guard<std::mutex> lock(s_configMutex);
        if (!s_discordCdnAvatarUrl.empty()) {
            payload["avatar_url"] = s_discordCdnAvatarUrl;
        }
    }
    payload["flags"] = 32768; // IS_COMPONENTS_V2
    payload["components"] = nlohmann::json::array({
        {
            {"type", 17},
            {"accent_color", 6514417}, // Indigo (#6366F1)
            {"components", containerComponents}
        }
    });

    PostAsync(webhookUrl, payload, callback);
}
