#pragma once
#include <string>
#include <vector>

namespace RobloxAPI {

    struct UserInfo {
        std::string Username;
        std::string DisplayName;
        long long UserId = 0;
        std::string ThumbnailUrl;
    };

    std::string HttpRequest(const std::wstring& method, const std::wstring& host, const std::wstring& path, const std::string& cookie, const std::string& extraHeaders = "", const std::string& body = "", std::string* outHeaders = nullptr);

    std::string GetCSRFToken(const std::string& cookie);
    std::string GetAuthTicket(const std::string& cookie, const std::string& csrfToken);

    // Get Presence using own cookie
    bool GetPresence(const std::string& cookie, const std::string& userId, std::string& outJobId, int& outPresenceType);

        struct AccountOverviewData {
        std::string Username;
        std::string DisplayName;
        long long UserId = 0;
        std::string CreatedDate;
        long long Robux = 0;
        bool IsPremium = false;
    };

    AccountOverviewData GetAccountOverview(const std::string& cookie, long long userId);

    UserInfo GetUserInfo(const std::string& cookie);}