#include <ctime>
#include "RobloxAPI.h"

#include <fstream>
void LogWear(const std::string& msg) {
    std::ofstream out("ropilot_wear.log", std::ios::app);
    out << msg << "\n";
}


#include <fstream>
void LogDebug(const std::string& msg) {
    std::ofstream out("ropilot_outfits.log", std::ios::app);
    out << msg << "\n";
}

#include <windows.h>
#include <winhttp.h>
#include "../vendor/json.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <mutex>
#include <ctime>

#pragma comment(lib, "winhttp.lib")

using json = nlohmann::json;

namespace RobloxAPI {

    std::wstring s2ws(const std::string& str) {
        if (str.empty()) return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
    }

    std::string ExtractHeader(const std::string& headers, const std::string& key) {
        std::string lowerHeaders = headers;
        std::transform(lowerHeaders.begin(), lowerHeaders.end(), lowerHeaders.begin(), ::tolower);
        std::string lowerKey = key;
        std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);
        
        size_t pos = lowerHeaders.find(lowerKey + ": ");
        if (pos != std::string::npos) {
            pos += lowerKey.length() + 2;
            size_t end = lowerHeaders.find("\r", pos);
            if (end == std::string::npos) end = lowerHeaders.length();
            return headers.substr(pos, end - pos);
        }
        return "";
    }

    std::string HttpRequest(const std::wstring& method, const std::wstring& host, const std::wstring& path, const std::string& cookie, const std::string& extraHeaders, const std::string& body, std::string* outHeaders) {
        HINTERNET hSession = WinHttpOpen(L"Roblox Account Manager / 1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) return "";

        HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
        if (!hConnect) { WinHttpCloseHandle(hSession); return ""; }

        HINTERNET hRequest = WinHttpOpenRequest(hConnect, method.c_str(), path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
        if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return ""; }

        std::string headers = "";
        if (!cookie.empty()) {
            headers += "Cookie: .ROBLOSECURITY=" + cookie + "\r\n";
        }
        if (!extraHeaders.empty()) {
            headers += extraHeaders;
        }

        std::wstring wHeaders = s2ws(headers);
        
        BOOL bResults = WinHttpSendRequest(hRequest, wHeaders.c_str(), wHeaders.length(), (LPVOID)(body.empty() ? NULL : body.c_str()), body.length(), body.length(), 0);

        std::string response;
        if (bResults) {
            bResults = WinHttpReceiveResponse(hRequest, NULL);
        }

        if (bResults) {
            // Get Headers if requested
            if (outHeaders) {
                DWORD dwSize = 0;
                WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, NULL, &dwSize, WINHTTP_NO_HEADER_INDEX);
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                    std::vector<WCHAR> headerBuffer(dwSize / sizeof(WCHAR));
                    if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, headerBuffer.data(), &dwSize, WINHTTP_NO_HEADER_INDEX)) {
                        int size_needed = WideCharToMultiByte(CP_UTF8, 0, headerBuffer.data(), -1, NULL, 0, NULL, NULL);
                        std::string strOut(size_needed, 0);
                        WideCharToMultiByte(CP_UTF8, 0, headerBuffer.data(), -1, &strOut[0], size_needed, NULL, NULL);
                        *outHeaders = strOut;
                    }
                }
            }

            DWORD dwSize = 0;
            DWORD dwDownloaded = 0;
            do {
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
                if (dwSize == 0) break;

                std::vector<char> buffer(dwSize + 1);
                ZeroMemory(buffer.data(), dwSize + 1);

                if (WinHttpReadData(hRequest, (LPVOID)buffer.data(), dwSize, &dwDownloaded)) {
                    response.append(buffer.data(), dwDownloaded);
                }
            } while (dwSize > 0);
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);

        return response;
    }

    static std::unordered_map<std::string, std::pair<std::string, time_t>> g_csrfCache;
    static std::mutex g_csrfMutex;

    std::string GetCSRFToken(const std::string& cookie) {
        {
            std::lock_guard<std::mutex> lock(g_csrfMutex);
            auto it = g_csrfCache.find(cookie);
            if (it != g_csrfCache.end() && time(nullptr) < it->second.second) {
                return it->second.first;
            }
        }
        
        std::string outHeaders;
        HttpRequest(L"POST", L"auth.roblox.com", L"/v2/logout", cookie, "", "", &outHeaders);
        std::string token = ExtractHeader(outHeaders, "x-csrf-token");
        
        if (!token.empty()) {
            std::lock_guard<std::mutex> lock(g_csrfMutex);
            g_csrfCache[cookie] = {token, time(nullptr) + 300}; // Cache for 5 minutes
        }
        return token;
    }

    std::string GetAuthTicket(const std::string& cookie, const std::string& csrfToken) {
        std::string headers = "x-csrf-token: " + csrfToken + "\r\nReferer: https://www.roblox.com/\r\nContent-Type: application/json\r\n";
        std::string outHeaders;
        HttpRequest(L"POST", L"auth.roblox.com", L"/v1/authentication-ticket", cookie, headers, "", &outHeaders);
        return ExtractHeader(outHeaders, "rbx-authentication-ticket");
    }


    bool ChangeDisplayName(const std::string& cookie, const std::string& userId, const std::string& newName, std::string& outError) {
        std::string csrf = GetCSRFToken(cookie);
        if (csrf.empty()) {
            outError = "Failed to obtain CSRF token.";
            return false;
        }

        std::string headers = "x-csrf-token: " + csrf + "\r\nContent-Type: application/json\r\n";
        std::string body = "{\"newDisplayName\": \"" + newName + "\"}";
        std::wstring path = L"/v1/users/" + std::wstring(userId.begin(), userId.end()) + L"/display-names";
        std::string outHeaders;
        std::string response = HttpRequest(L"PATCH", L"users.roblox.com", path, cookie, headers, body, &outHeaders);

        try {
            if (!response.empty()) {
                auto j = json::parse(response);
                if (j.contains("errors")) {
                    outError = j["errors"][0]["message"].get<std::string>();
                    return false;
                }
            }
        } catch (...) {}
        return true;
    }

            bool GetOutfits(const std::string& cookie, const std::string& userId, std::string& outJson) {
        std::wstring path = L"/v1/users/" + std::wstring(userId.begin(), userId.end()) + L"/outfits";
        std::string outHeaders;
        std::string response = HttpRequest(L"GET", L"avatar.roblox.com", path, cookie, "", "", &outHeaders);

        try {
            if (!response.empty()) {
                auto j = json::parse(response);
                if (j.contains("data")) {
                    auto& data = j["data"];
                    auto it = data.begin();
                    while (it != data.end()) {
                        if (it->contains("isEditable") && !(*it)["isEditable"].get<bool>()) {
                            it = data.erase(it);
                        } else {
                            ++it;
                        }
                    }

                    std::string outfitIds = "";
                    for (const auto& outfit : j["data"]) {
                        if (!outfitIds.empty()) outfitIds += ",";
                        outfitIds += std::to_string(outfit["id"].get<long long>());
                    }
                    
                    if (!outfitIds.empty()) {
                        std::wstring thumbPath = L"/v1/users/outfits?userOutfitIds=" + std::wstring(outfitIds.begin(), outfitIds.end()) + L"&size=150x150&format=Png&isCircular=false";
                        std::string thumbRes = HttpRequest(L"GET", L"thumbnails.roblox.com", thumbPath, cookie, "", "", nullptr);
                        
                        try {
                            auto t = json::parse(thumbRes);
                            std::unordered_map<long long, std::string> thumbnails;
                            if (t.contains("data")) {
                                for (const auto& thumb : t["data"]) {
                                    if (thumb["imageUrl"].is_string()) {
                                        thumbnails[thumb["targetId"].get<long long>()] = thumb["imageUrl"].get<std::string>();
                                    }
                                }
                            }
                            
                            for (auto& outfit : j["data"]) {
                                long long id = outfit["id"].get<long long>();
                                if (thumbnails.count(id)) outfit["imageUrl"] = thumbnails[id];
                                else outfit["imageUrl"] = "";
                            }
                        } catch (...) {}
                    }
                    
                    std::wstring cacheBuster = L"&v=" + std::to_wstring(std::time(nullptr));
                    std::wstring fullBodyPath = L"/v1/users/avatar?userIds=" + std::wstring(userId.begin(), userId.end()) + L"&size=352x352&format=Png&isCircular=false" + cacheBuster;
                    std::string fullBodyRes = HttpRequest(L"GET", L"thumbnails.roblox.com", fullBodyPath, cookie, "", "", nullptr);
                    try {
                        auto fb = json::parse(fullBodyRes);
                        if (fb.contains("data") && fb["data"].size() > 0 && fb["data"][0]["imageUrl"].is_string()) {
                            j["fullBodyUrl"] = fb["data"][0]["imageUrl"].get<std::string>();
                        } else {
                            j["fullBodyUrl"] = "";
                        }
                    } catch (...) { j["fullBodyUrl"] = ""; }
                    
                    std::wstring headshotPath = L"/v1/users/avatar-headshot?userIds=" + std::wstring(userId.begin(), userId.end()) + L"&size=150x150&format=Png&isCircular=false" + cacheBuster;
                    std::string headshotRes = HttpRequest(L"GET", L"thumbnails.roblox.com", headshotPath, cookie, "", "", nullptr);
                    try {
                        auto hs = json::parse(headshotRes);
                        if (hs.contains("data") && hs["data"].size() > 0 && hs["data"][0]["imageUrl"].is_string()) {
                            j["headshotUrl"] = hs["data"][0]["imageUrl"].get<std::string>();
                        } else {
                            j["headshotUrl"] = "";
                        }
                    } catch (...) { j["headshotUrl"] = ""; }
                    
                    outJson = j.dump();
                    return true;
                }
            }
        } catch (...) {}
        outJson = "{}";
        return false;
    }

        bool WearOutfit(const std::string& cookie, const std::string& outfitId, std::string& outError) {
        std::string csrf = GetCSRFToken(cookie);
        if (csrf.empty()) { outError = "Failed to obtain CSRF token."; return false; }
        
        std::wstring detailsPath = L"/v1/outfits/" + std::wstring(outfitId.begin(), outfitId.end()) + L"/details";
        std::string detailsRes = HttpRequest(L"GET", L"avatar.roblox.com", detailsPath, cookie, "", "", nullptr);
        if (detailsRes.empty()) { outError = "Failed to get outfit details."; return false; }
        
        try {
            auto details = json::parse(detailsRes);
            if (details.contains("errors")) {
                if (details["errors"].size() > 0 && details["errors"][0].contains("message") && details["errors"][0]["message"].is_string())
                    outError = details["errors"][0]["message"].get<std::string>();
                else outError = "Failed to get outfit details (error response).";
                return false;
            }
            
            std::string headers = "x-csrf-token: " + csrf + "\r\nContent-Type: application/json\r\n";
            
            // 1. Avatar Type
            if (details.contains("playerAvatarType")) {
                std::string typeBody = "{\"playerAvatarType\":\"" + details["playerAvatarType"].get<std::string>() + "\"}";
                HttpRequest(L"POST", L"avatar.roblox.com", L"/v1/avatar/set-player-avatar-type", cookie, headers, typeBody, nullptr);
            }
            
            // 2. Scales
            if (details.contains("scale") || details.contains("scales")) {
                auto scaleObj = details.contains("scales") ? details["scales"] : details["scale"];
                std::string scaleBody = scaleObj.dump();
                HttpRequest(L"POST", L"avatar.roblox.com", L"/v1/avatar/set-scales", cookie, headers, scaleBody, nullptr);
            }
            
            // 3. Body Colors
            if (details.contains("bodyColors")) {
                std::string colorBody = details["bodyColors"].dump();
                HttpRequest(L"POST", L"avatar.roblox.com", L"/v1/avatar/set-body-colors", cookie, headers, colorBody, nullptr);
            }
            
            // 4. Wearing Assets
            if (details.contains("assets")) {
                std::string assetsBody = "{\"assets\":" + details["assets"].dump() + "}";
                std::string wearRes = HttpRequest(L"POST", L"avatar.roblox.com", L"/v2/avatar/set-wearing-assets", cookie, headers, assetsBody, nullptr);
                
                try {
                    auto j = json::parse(wearRes);
                    if (j.contains("errors")) {
                        if (j["errors"].size() > 0 && j["errors"][0].contains("message") && j["errors"][0]["message"].is_string())
                            outError = j["errors"][0]["message"].get<std::string>();
                        else outError = wearRes;
                        return false;
                    }
                    if (j.contains("success") && !j["success"].get<bool>()) {
                        outError = "Failed to set wearing assets (success=false).";
                        return false;
                    }
                } catch (...) {
                    // Ignore JSON parse errors for wear response if it didn't return an error JSON
                }
            }
            
            return true;
            
        } catch (const std::exception& e) {
            outError = std::string("JSON Exception: ") + e.what();
            return false;
        }
    }


    bool GetPresence(const std::string& cookie, const std::string& userId, std::string& outJobId, int& outPresenceType) {
        std::string csrf = GetCSRFToken(cookie);
        std::string headers = "x-csrf-token: " + csrf + "\r\nContent-Type: application/json\r\n";
        std::string body = "{\"userIds\": [" + userId + "]}";
        std::string outHeaders;
        std::string response = HttpRequest(L"POST", L"presence.roblox.com", L"/v1/presence/users", cookie, headers, body, &outHeaders);
        if (response.empty()) return false;

        try {
            auto j = json::parse(response);
            if (j.contains("userPresences") && j["userPresences"].is_array() && j["userPresences"].size() > 0) {
                auto presence = j["userPresences"][0];
                outPresenceType = presence.value("userPresenceType", 0);
                outJobId = presence.value("gameId", "");
                if (outJobId.empty()) {
                    outJobId = "";
                }
                return true;
            }
        } catch (...) {}
        return false;
    }

    UserInfo GetUserInfo(const std::string& cookie) {
        UserInfo info;
        // 1. Get UserId and Username
        std::string authapi = HttpRequest(L"GET", L"users.roblox.com", L"/v1/users/authenticated", cookie, "", "", nullptr);
        if (!authapi.empty()) {
            try {
                auto j = json::parse(authapi);
                if (j.contains("id")) info.UserId = j["id"].get<long long>();
                if (j.contains("name")) info.Username = j["name"].get<std::string>();
                if (j.contains("displayName")) info.DisplayName = j["displayName"].get<std::string>();
            } catch (...) {}
        }
        
        // 2. Get Avatar Thumbnail
        if (info.UserId != 0) {
            std::wstring path = L"/v1/users/avatar-headshot?userIds=" + std::to_wstring(info.UserId) + L"&size=150x150&format=Png";
            std::string thumbData = HttpRequest(L"GET", L"thumbnails.roblox.com", path, cookie, "", "", nullptr);
            try {
                auto j = json::parse(thumbData);
                if (j.contains("data") && j["data"].is_array() && j["data"].size() > 0) {
                    info.ThumbnailUrl = j["data"][0]["imageUrl"].get<std::string>();
                }
            } catch (...) {}
        }
        
        return info;
    }

    AccountOverviewData GetAccountOverview(const std::string& cookie, long long userId) {
        AccountOverviewData data;
        data.UserId = userId;
        
        std::wstring userPath = L"/v1/users/" + std::to_wstring(userId);
        std::string userApi = HttpRequest(L"GET", L"users.roblox.com", userPath, cookie, "", "", nullptr);
        if (!userApi.empty()) {
            try {
                auto j = json::parse(userApi);
                if (j.contains("name")) data.Username = j["name"].get<std::string>();
                if (j.contains("displayName")) data.DisplayName = j["displayName"].get<std::string>();
                if (j.contains("created")) data.CreatedDate = j["created"].get<std::string>();
            } catch (...) {}
        }
        
        std::wstring econPath = L"/v1/users/" + std::to_wstring(userId) + L"/currency";
        std::string econApi = HttpRequest(L"GET", L"economy.roblox.com", econPath, cookie, "", "", nullptr);
        if (!econApi.empty()) {
            try {
                auto j = json::parse(econApi);
                if (j.contains("robux")) data.Robux = j["robux"].get<long long>();
            } catch (...) {}
        }
        
        std::wstring premiumPath = L"/v1/users/" + std::to_wstring(userId) + L"/validate-membership";
        std::string premiumApi = HttpRequest(L"GET", L"premiumfeatures.roblox.com", premiumPath, cookie, "", "", nullptr);
        if (!premiumApi.empty()) {
            if (premiumApi.find("true") != std::string::npos) data.IsPremium = true;
        }
        
        return data;
    }

    bool GetRecentGames(const std::string& cookie, std::string& outJson) {
        std::string csrf = GetCSRFToken(cookie);
        if (csrf.empty()) {
            outJson = "[{\"error\": \"CSRF_FAILED\"}]";
            return false;
        }

        std::string body = "{\"sessionId\":\"00000000-0000-0000-0000-000000000000\",\"pageType\":\"Home\",\"sessionIdHasBeenUsed\":false}";
        std::string extraHeaders = "Content-Type: application/json\r\nAccept: application/json\r\nX-CSRF-TOKEN: " + csrf + "\r\n";

        std::string res = HttpRequest(L"POST", L"apis.roblox.com", L"/discovery-api/omni-recommendation", cookie, extraHeaders, body);
        
        if (res.empty()) {
            outJson = "[{\"error\": \"EMPTY_RESPONSE\"}]";
            return false;
        }

        try {
            json resJson = json::parse(res);
            json gamesList = json::array();
            
            if (resJson.contains("sorts") && resJson["sorts"].is_array()) {
                for (auto& sort : resJson["sorts"]) {
                    std::string topic = sort.value("topic", "");
                    long long topicId = sort.value("topicId", 0LL);
                    if ((topic == "Continue" || topicId == 100000003) && sort.contains("recommendationList")) {
                        for (auto& rec : sort["recommendationList"]) {
                            long long universeId = rec.value("universeId", 0LL);
                            if (universeId == 0) universeId = rec.value("contentId", 0LL);
                            if (universeId != 0) {
                                json game;
                                game["universeId"] = universeId;
                                gamesList.push_back(game);
                                if (gamesList.size() >= 40) break;
                            }
                        }
                        break;
                    }
                }
            }

            if (!gamesList.empty()) {
                std::string universeIds = "";
                for (size_t i = 0; i < gamesList.size(); ++i) {
                    if (i > 0) universeIds += ",";
                    universeIds += std::to_string(gamesList[i]["universeId"].get<long long>());
                }
                
                // Fetch basic game info including active players
                std::wstring gamesPath = L"/v1/games?universeIds=" + s2ws(universeIds);
                std::string gamesRes = HttpRequest(L"GET", L"games.roblox.com", gamesPath, cookie);
                
                // Fetch upVotes and downVotes
                std::wstring votesPath = L"/v1/games/votes?universeIds=" + s2ws(universeIds);
                std::string votesRes = HttpRequest(L"GET", L"games.roblox.com", votesPath, cookie);
                
                if (!gamesRes.empty()) {
                    try {
                        json gamesJson = json::parse(gamesRes);
                        json votesJson = json::object();
                        if (!votesRes.empty()) {
                            try { votesJson = json::parse(votesRes); } catch (...) {}
                        }
                        
                        if (gamesJson.contains("data") && gamesJson["data"].is_array()) {
                            for (auto& gData : gamesJson["data"]) {
                                long long tUid = gData.value("id", 0LL);
                                for (auto& g : gamesList) {
                                    if (g["universeId"] == tUid) {
                                        g["name"] = gData.value("name", "Unknown");
                                        g["rootPlaceId"] = gData.value("rootPlaceId", 0LL);
                                        g["id"] = g["rootPlaceId"];
                                        g["playing"] = gData.value("playing", 0LL);
                                        
                                        // Calculate like percentage
                                        int likeRatio = 0;
                                        if (votesJson.contains("data") && votesJson["data"].is_array()) {
                                            for (auto& vData : votesJson["data"]) {
                                                if (vData.value("id", 0LL) == tUid) {
                                                    long long up = vData.value("upVotes", 0LL);
                                                    long long down = vData.value("downVotes", 0LL);
                                                    if (up + down > 0) {
                                                        likeRatio = (int)((double)up / (up + down) * 100.0);
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                        g["likeRatio"] = likeRatio;
                                    }
                                }
                            }
                        }
                    } catch (...) {}
                }
                
                json validGames = json::array();
                for (auto& g : gamesList) {
                    if (g.contains("rootPlaceId") && g["rootPlaceId"] != 0) {
                        validGames.push_back(g);
                    }
                }
                gamesList = validGames;
                
                std::wstring path = L"/v1/games/icons?universeIds=" + s2ws(universeIds) + L"&returnPolicy=PlaceHolder&size=150x150&format=Png&isCircular=false";
                std::string thumbRes = HttpRequest(L"GET", L"thumbnails.roblox.com", path, cookie);
                if (!thumbRes.empty()) {
                    try {
                        json thumbJson = json::parse(thumbRes);
                        if (thumbJson.contains("data") && thumbJson["data"].is_array()) {
                            for (auto& thumb : thumbJson["data"]) {
                                long long tUid = thumb.value("targetId", 0LL);
                                std::string tUrl = thumb.value("imageUrl", "");
                                for (auto& g : gamesList) {
                                    if (g["universeId"] == tUid) {
                                        g["thumbnail"] = tUrl;
                                        break;
                                    }
                                }
                            }
                        }
                    } catch (...) {}
                }
            }
            
            outJson = gamesList.dump();
            return true;
        } catch (...) {
            outJson = "[{\"error\": \"RATE_LIMIT_OR_JSON_FAILED\"}]";
            return false;
        }
    }

    bool GetFriends(const std::string& cookie, const std::string& userId, std::string& outJson) {
        std::wstring path = L"/v1/users/" + s2ws(userId) + L"/friends";
        std::string res = HttpRequest(L"GET", L"friends.roblox.com", path, cookie);
        if (res.empty() || res.find("\"data\"") == std::string::npos) {
            outJson = "{\"error\": \"Failed to fetch friends\"}";
            return false;
        }
        outJson = res;
        return true;
    }

    bool Unfriend(const std::string& cookie, const std::string& friendId, std::string& outError) {
        std::string csrf = GetCSRFToken(cookie);
        if (csrf.empty()) {
            outError = "CSRF Token missing";
            return false;
        }
        std::wstring path = L"/v1/users/" + s2ws(friendId) + L"/unfriend";
        std::string res = HttpRequest(L"POST", L"friends.roblox.com", path, cookie, "x-csrf-token: " + csrf + "\r\n", "{}");
        if (res.find("\"errors\"") != std::string::npos) {
            outError = "Failed to unfriend: " + res;
            return false;
        }
        return true;
    }

    std::string GetLowestServer(const std::string& placeId, const std::string& cookie) {
        if (placeId.empty()) return "";
        std::string path = "/v1/games/" + placeId + "/servers/Public?sortOrder=Asc&limit=100";
        std::string response = HttpRequest(L"GET", L"games.roblox.com", s2ws(path), cookie);
        if (response.empty()) return "";

        try {
            json j = json::parse(response);
            if (j.contains("data") && j["data"].is_array()) {
                std::string bestServerId = "";
                int bestPing = 999999;
                int lowestPlaying = 999999;

                for (const auto& server : j["data"]) {
                    if (server.contains("id") && server.contains("playing") && server.contains("maxPlayers")) {
                        int playing = server["playing"].get<int>();
                        int maxPlayers = server["maxPlayers"].get<int>();
                        int ping = server.value("ping", 999999);

                        // Ignore full servers
                        if (playing >= maxPlayers) continue;

                        if (playing < lowestPlaying || (playing == lowestPlaying && ping < bestPing)) {
                            lowestPlaying = playing;
                            bestPing = ping;
                            bestServerId = server["id"].get<std::string>();
                        }
                    }
                }
                return bestServerId;
            }
        } catch (...) {
            // Parsing error
        }
        return "";
    }

}
