import sys, re

with open('src/RobloxAPI.cpp', 'r', encoding='utf-8') as f:
    cpp = f.read()

old_get_outfits = """                    if (!outfitIds.empty()) {
                        LogDebug("Outfit IDs: " + outfitIds);
                        std::wstring thumbPath = L"/v1/users/outfits?userOutfitIds=" + std::wstring(outfitIds.begin(), outfitIds.end()) + L"&size=150x150&format=Png&isCircular=false";
                        std::string thumbRes = HttpRequest(L"GET", L"thumbnails.roblox.com", thumbPath, cookie, "", "", nullptr);
                        LogDebug("Thumb Res: " + thumbRes);
                        
                        try {
                            auto t = json::parse(thumbRes);
                            std::unordered_map<int, std::string> thumbnails;
                            if (t.contains("data")) {
                                for (const auto& thumb : t["data"]) {
                                    if (thumb["imageUrl"].is_string()) {
                                        thumbnails[thumb["targetId"].get<int>()] = thumb["imageUrl"].get<std::string>();
                                    }
                                }
                            }
                            
                            for (auto& outfit : j["data"]) {
                                int id = outfit["id"].get<int>();
                                if (thumbnails.count(id)) outfit["imageUrl"] = thumbnails[id];
                                else outfit["imageUrl"] = "";
                            }
                        } catch (const std::exception& e) {
                            LogDebug(std::string("Exception parsing thumb: ") + e.what());
                        }
                    }"""

new_get_outfits = """                    if (!outfitIds.empty()) {
                        std::wstring thumbPath = L"/v1/users/outfits?userOutfitIds=" + std::wstring(outfitIds.begin(), outfitIds.end()) + L"&size=150x150&format=Png&isCircular=false";
                        std::string thumbRes = HttpRequest(L"GET", L"thumbnails.roblox.com", thumbPath, cookie, "", "", nullptr);
                        
                        try {
                            auto t = json::parse(thumbRes);
                            std::unordered_map<int, std::string> thumbnails;
                            if (t.contains("data")) {
                                for (const auto& thumb : t["data"]) {
                                    if (thumb["imageUrl"].is_string()) {
                                        thumbnails[thumb["targetId"].get<int>()] = thumb["imageUrl"].get<std::string>();
                                    }
                                }
                            }
                            
                            for (auto& outfit : j["data"]) {
                                int id = outfit["id"].get<int>();
                                if (thumbnails.count(id)) outfit["imageUrl"] = thumbnails[id];
                                else outfit["imageUrl"] = "";
                            }
                        } catch (...) {}
                    }
                    
                    // Also fetch full body avatar
                    std::wstring fullBodyPath = L"/v1/users/avatar?userIds=" + std::wstring(userId.begin(), userId.end()) + L"&size=352x352&format=Png&isCircular=false";
                    std::string fullBodyRes = HttpRequest(L"GET", L"thumbnails.roblox.com", fullBodyPath, cookie, "", "", nullptr);
                    try {
                        auto fb = json::parse(fullBodyRes);
                        if (fb.contains("data") && fb["data"].size() > 0 && fb["data"][0]["imageUrl"].is_string()) {
                            j["fullBodyUrl"] = fb["data"][0]["imageUrl"].get<std::string>();
                        } else {
                            j["fullBodyUrl"] = "";
                        }
                    } catch (...) { j["fullBodyUrl"] = ""; }
"""

cpp = cpp.replace(old_get_outfits, new_get_outfits)

with open('src/RobloxAPI.cpp', 'w', encoding='utf-8') as f:
    f.write(cpp)
