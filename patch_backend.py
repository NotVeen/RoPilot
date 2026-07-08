import sys

with open('src/RobloxAPI.cpp', 'r', encoding='utf-8') as f:
    api = f.read()

change_display_name_code = """
    bool ChangeDisplayName(const std::string& cookie, const std::string& userId, const std::string& newName, std::string& outError) {
        std::string csrf = GetCSRFToken(cookie);
        if (csrf.empty()) {
            outError = "Failed to obtain CSRF token.";
            return false;
        }

        std::string headers = "x-csrf-token: " + csrf + "\\r\\nContent-Type: application/json\\r\\n";
        std::string body = "{\\"newDisplayName\\": \\"" + newName + "\\"}";
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
"""

if 'ChangeDisplayName' not in api:
    api = api.replace('    bool GetPresence(', change_display_name_code + '\\n    bool GetPresence(')

with open('src/RobloxAPI.cpp', 'w', encoding='utf-8') as f:
    f.write(api)
