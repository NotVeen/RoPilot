import sys

with open('src/main.cpp', 'r', encoding='utf-8') as f:
    main_code = f.read()

new_handlers = """            }).detach();
        }
        else if (action == "get_outfits") {
            std::string cookie = j.value("cookie", "");
            std::string userId = j.value("userId", "");
            
            std::thread([cookie, userId]() {
                std::string outJson;
                if (RobloxAPI::GetOutfits(cookie, userId, outJson)) {
                    std::string escaped_payload;
                    for (char c : outJson) {
                        if (c == '\\\\') escaped_payload += "\\\\\\\\";
                        else if (c == '"') escaped_payload += "\\\\\\\"";
                        else if (c == '\\'') escaped_payload += "\\\\\\'";
                        else escaped_payload += c;
                    }
                    std::string js = "if(window.onReceiveOutfits) window.onReceiveOutfits('" + escaped_payload + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                } else {
                    std::string js = "if(window.onReceiveOutfitsError) window.onReceiveOutfitsError();";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
            }).detach();
        }
        else if (action == "wear_outfit") {
            std::string cookie = j.value("cookie", "");
            std::string outfitId = j.value("outfitId", "");
            
            std::thread([cookie, outfitId]() {
                std::string outError;
                if (RobloxAPI::WearOutfit(cookie, outfitId, outError)) {
                    // Trigger avatar refresh by updating account thumbnail
                    g_accountManager.RefreshAccountAvatar(cookie);
                    
                    std::string js = "if(window.onWearOutfitSuccess) window.onWearOutfitSuccess();";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                    
                    // Also trigger main UI update
                    PostMessage(g_hWnd, WM_APP + 2, 0, 0); // UpdateUI
                } else {
                    std::string safeErrorMsg = outError;
                    size_t pos = 0;
                    while((pos = safeErrorMsg.find("'", pos)) != std::string::npos) {
                        safeErrorMsg.replace(pos, 1, "\\\\'");
                        pos += 2;
                    }
                    std::string js = "if(window.onWearOutfitError) window.onWearOutfitError('" + safeErrorMsg + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
            }).detach();
        }"""

main_code = main_code.replace("            }).detach();\n        }", new_handlers)

with open('src/main.cpp', 'w', encoding='utf-8') as f:
    f.write(main_code)
