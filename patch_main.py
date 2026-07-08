import sys

with open('src/main.cpp', 'r', encoding='utf-8') as f:
    main_code = f.read()

change_display_name_handler = """        else if (action == "change_display_name") {
            std::string cookie = j.value("cookie", "");
            std::string userId = j.value("userId", "");
            std::string newName = j.value("newName", "");
            
            std::string errorMsg;
            if (RobloxAPI::ChangeDisplayName(cookie, userId, newName, errorMsg)) {
                SendStatusMessage("Display Name changed successfully!", false);
                std::string js = "if(window.refreshManageAccount) window.refreshManageAccount();";
                g_webview->ExecuteScript(s2ws(js).c_str(), nullptr);
            } else {
                SendStatusMessage("Failed to change Display Name: " + errorMsg, true);
            }
        }
"""

if 'change_display_name' not in main_code:
    main_code = main_code.replace('        else if (action == "add_cookie") {', change_display_name_handler + '        else if (action == "add_cookie") {')

with open('src/main.cpp', 'w', encoding='utf-8') as f:
    f.write(main_code)
