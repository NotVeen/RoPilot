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

    std::wstring s2ws(const std::string& str);
    std::string GetCSRFToken(const std::string& cookie);
    std::string GetAuthTicket(const std::string& cookie, const std::string& csrfToken);
    bool ChangeDisplayName(const std::string& cookie, const std::string& userId, const std::string& newName, std::string& outError);
    bool GetOutfits(const std::string& cookie, const std::string& userId, std::string& outJson);
    std::string GetLowestServer(const std::string& placeId, const std::string& cookie);
    bool WearOutfit(const std::string& cookie, const std::string& outfitId, std::string& outError);
    bool GetRecentGames(const std::string& cookie, std::string& outJson);

    bool GetFriends(const std::string& cookie, const std::string& userId, std::string& outJson);
    bool Unfriend(const std::string& cookie, const std::string& friendId, std::string& outError);

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