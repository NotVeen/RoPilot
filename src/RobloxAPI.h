#pragma once
#include <string>
#include <vector>

namespace RobloxAPI {
    struct GameInfo {
        std::string PlaceId;
        std::string Name;
        std::string ThumbnailUrl;
    };

    struct UserInfo {
        std::string Username;
        std::string DisplayName;
        long long UserId = 0;
        std::string ThumbnailUrl;
    };

    struct GameSearchResult {
        long long UniverseId = 0;
        long long RootPlaceId = 0;
        std::string Name;
        long long PlayerCount = 0;
        long long Upvotes = 0;
        std::string ThumbnailUrl;
    };

    std::string HttpRequest(const std::wstring& method, const std::wstring& host, const std::wstring& path, const std::string& cookie, const std::string& extraHeaders = "", const std::string& body = "", std::string* outHeaders = nullptr);

    std::string GetCSRFToken(const std::string& cookie);
    std::string GetAuthTicket(const std::string& cookie, const std::string& csrfToken);

    // Get Presence using own cookie
    bool GetPresence(const std::string& cookie, const std::string& userId, std::string& outJobId, int& outPresenceType);

    UserInfo GetUserInfo(const std::string& cookie);
    GameInfo GetGameInfo(const std::string& cookie, const std::string& placeId);
    std::vector<GameSearchResult> SearchGames(const std::string& query);
}