#pragma once
#include <string>
#include <functional>

#include "WebView2.h"

namespace BrowserLogin {
    // Opens a window with WebView2 navigated to Roblox Login.
    // Calls onCookieFound when .ROBLOSECURITY is detected.
    void OpenLoginBrowser(ICoreWebView2Environment* env, std::function<void(const std::string&)> onCookieFound);
}
