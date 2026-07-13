#include <windows.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <wrl.h>
#include "AccountManager.h"
#include "Launcher.h"
#include <shellapi.h>
#include <psapi.h>
#include "RobloxAPI.h"
#include "BrowserLogin.h"
#include "HandleCloser.h"
#include "ActiveClientLock.h"
#include "Optimizer.h"
#include "DiscordRPC.h"
#include "UI_Frontend.h"
#include "SettingsManager.h"
#include "Crypto.h"
#include "Updater.h"
#include <json.hpp>
#include <iostream>
#include <regex>
#include <string>
#include <windowsx.h>
#include <fstream>
#include <thread>
#include <atomic>
#include <tlhelp32.h>
#include <sstream>
#include <shlobj.h>
#include <shlwapi.h>

bool IsProcessRunning(DWORD pid) {
    if (pid == 0) return false;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess == NULL) return false;
    DWORD exitCode = 0;
    if (GetExitCodeProcess(hProcess, &exitCode)) {
        CloseHandle(hProcess);
        return exitCode == STILL_ACTIVE;
    }
    CloseHandle(hProcess);
    return false;
}


using namespace Microsoft::WRL;
using json = nlohmann::json;


enum WINDOWCOMPOSITIONATTRIB { WCA_ACCENT_POLICY = 19 };
enum ACCENT_STATE { ACCENT_DISABLED = 0, ACCENT_ENABLE_GRADIENT = 1, ACCENT_ENABLE_TRANSPARENTGRADIENT = 2, ACCENT_ENABLE_BLURBEHIND = 3, ACCENT_ENABLE_ACRYLICBLURBEHIND = 4, ACCENT_INVALID_STATE = 5 };
struct ACCENT_POLICY { ACCENT_STATE AccentState; DWORD AccentFlags; DWORD GradientColor; DWORD AnimationId; };
struct WINDOWCOMPOSITIONATTRIBDATA { WINDOWCOMPOSITIONATTRIB Attrib; PVOID pvData; SIZE_T cbData; };
typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

void ApplyTransparencyMode(HWND hwnd, bool blur, float opacity) {
    // ALWAYS keep WS_EX_LAYERED to prevent DWM frame recalculation bugs (titlebar bug)
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    
    // Set LWA_ALPHA to 255 (fully opaque) so child elements like modals remain solid!
    // The actual background transparency will be handled by DWM composition and WebView2.
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
    
    HMODULE hUser = GetModuleHandle(TEXT("user32.dll"));
    if (hUser) {
        pSetWindowCompositionAttribute setWndComp = (pSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
        if (setWndComp) {
            ACCENT_POLICY policy;
            if (blur) {
                // Windows 10/11 standard blur that works perfectly with WS_EX_LAYERED
                policy = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
            } else {
                // Transparent gradient for opacity without blur. 
                // We set gradient to 0 (fully transparent) because HTML body CSS will handle the actual opacity color!
                policy = { ACCENT_ENABLE_TRANSPARENTGRADIENT, 2, 0, 0 };
            }
            WINDOWCOMPOSITIONATTRIBDATA data = { WCA_ACCENT_POLICY, &policy, sizeof(ACCENT_POLICY) };
            setWndComp(hwnd, &data);
        }
    }
    
    // We MUST force the window to redraw to apply the composition changes smoothly
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME);
}

std::atomic<bool> g_running{true};
bool g_showChangelog = false;
std::vector<std::pair<std::string, bool>> g_toastQueue;
std::mutex g_toastMutex;
bool g_hasShownTrayNotification = false;

#define LOG(x)
#define ERROR_LOG(x)


AccountManager g_accountManager;
SettingsManager g_settingsManager;
std::string g_currentMasterPassword = "";
std::string g_currentMasterSalt = "";
std::string g_pendingUpdateUrl = "";
HWND g_hWnd = NULL;
ComPtr<ICoreWebView2Controller> g_webviewController;
ComPtr<ICoreWebView2> g_webview;
ComPtr<ICoreWebView2Environment> g_webviewEnv;

void UpdateUI();

#define WM_TRAYICON (WM_USER + 1)
NOTIFYICONDATAW g_nid = { 0 };

int KillAllRobloxInstances() {
    int count = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32W);
        if (Process32FirstW(hSnapshot, &pe32)) {
            do {
                if (_wcsicmp(pe32.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                    if (hProcess) {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                        count++;
                    }
                }
            } while (Process32NextW(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return count;
}

void SetStartupRegistry(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            char path[MAX_PATH];
            GetModuleFileNameA(NULL, path, MAX_PATH);
            std::string pathStr = std::string("\"") + path + "\"";
            RegSetValueExA(hKey, "RoPilot", 0, REG_SZ, (const BYTE*)pathStr.c_str(), pathStr.length() + 1);
        } else {
            RegDeleteValueA(hKey, "RoPilot");
        }
        RegCloseKey(hKey);
    }
}

void AddTrayIcon(HWND hWnd) {
    g_nid.cbSize = sizeof(NOTIFYICONDATAW);
    g_nid.hWnd = hWnd;
    g_nid.uID = 1;
    g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    g_nid.uCallbackMessage = WM_TRAYICON;
    g_nid.hIcon = (HICON)LoadImageW(NULL, L"res\\icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (!g_nid.hIcon) {
        g_nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
        if (!g_nid.hIcon) g_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }
    wcscpy_s(g_nid.szTip, L"RoPilot");
    Shell_NotifyIconW(NIM_ADD, &g_nid);
}

void RemoveTrayIcon() {
    Shell_NotifyIconW(NIM_DELETE, &g_nid);
    if (g_nid.hIcon) DestroyIcon(g_nid.hIcon);
}


std::wstring s2ws(const std::string& s) {
    if (s.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string ws2s(const std::wstring& w) {
    if (w.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &w[0], (int)w.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &w[0], (int)w.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

void SendStatusMessage(const std::string& msg, bool isError = false);
void SendSettingsData();
void UpdateUI();

uint64_t FileTimeToUInt64(const FILETIME& ft) {
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}

void SendStatusMessage(const std::string& msg, bool isError) {
    if (!g_webview) return;
    std::string escapedMsg = msg;
    size_t pos = 0;
    while ((pos = escapedMsg.find("'", pos)) != std::string::npos) {
        escapedMsg.replace(pos, 1, "\\'");
        pos += 2;
    }
    
    std::string script = "window.showStatus('" + escapedMsg + "', " + (isError ? "true" : "false") + ");";
    g_webview->ExecuteScript(s2ws(script).c_str(), nullptr);
}

void ProcessWebMessage(const std::string& msg) {
    try {
        auto j = json::parse(msg);
        std::string action = j.value("action", "");
        
        if (action == "add_browser") {
            BrowserLogin::OpenLoginBrowser(g_webviewEnv.Get(), [](const std::string& cookie) {
                if (!cookie.empty()) {
                    if (g_accountManager.AddAccount(cookie)) {
                        std::string username = "Account";
                        for (const auto& acc : g_accountManager.GetAccounts()) {
                            if (acc.Cookie == cookie) { username = acc.Info.Username; break; }
                        }
                        SendStatusMessage(username + " added successfully!", false);
                        UpdateUI();
                    } else {
                        SendStatusMessage("Failed to add account. Invalid cookie?", true);
                    }
                } else {
                    SendStatusMessage("Browser login cancelled or failed.", true);
                }
            });
        } 
                else if (action == "add_cookie_bulk") {
            auto cookies = j.value("cookies", json::array());
            int addedCount = 0;
            int failedCount = 0;
            for (const auto& cItem : cookies) {
                std::string cookie = cItem.get<std::string>();
                cookie.erase(0, cookie.find_first_not_of(" \t\r\n"));
                cookie.erase(cookie.find_last_not_of(" \t\r\n") + 1);
                
                if (cookie.empty()) continue;
                if (g_accountManager.AddAccount(cookie)) {
                    addedCount++;
                } else {
                    failedCount++;
                }
            }
            if (addedCount > 0) {
                std::string toastMsg = std::to_string(addedCount) + " account(s) added successfully!";
                if (failedCount > 0) {
                    toastMsg += " (" + std::to_string(failedCount) + " failed)";
                }
                SendStatusMessage(toastMsg, false);
                UpdateUI();
            } else {
                SendStatusMessage("Failed to add accounts. Invalid or expired cookies?", true);
            }
        }
                else if (action == "get_account_overview") {
            std::string cookie = j.value("cookie", "");
            long long userId = 0;
            if (j.contains("userId")) {
                if (j["userId"].is_number()) userId = j["userId"].get<long long>();
                else if (j["userId"].is_string()) {
                    try { userId = std::stoll(j["userId"].get<std::string>()); } catch(...) {}
                }
            }
            if (!cookie.empty() && userId != 0) {
                std::thread([cookie, userId]() {
                    RobloxAPI::AccountOverviewData data = RobloxAPI::GetAccountOverview(cookie, userId);
                    
                    json jOut;
                    jOut["action"] = "account_overview_data";
                    jOut["success"] = true;
                    jOut["userId"] = data.UserId;
                    jOut["username"] = data.Username;
                    jOut["displayName"] = data.DisplayName;
                    jOut["createdDate"] = data.CreatedDate;
                    jOut["robux"] = data.Robux;
                    jOut["isPremium"] = data.IsPremium;
                    
                    std::string payload = jOut.dump();
                    std::string escaped_payload;
                    for (char c : payload) {
                        if (c == '\\') escaped_payload += "\\\\";
                        else if (c == '"') escaped_payload += "\\\"";
                        else if (c == '\'') escaped_payload += "\\\'";
                        else escaped_payload += c;
                    }
                    std::string script = "window.postMessage('" + escaped_payload + "', '*');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(script), 0);
                }).detach();
            }
        }
        else if (action == "change_display_name") {
            std::string cookie = j.value("cookie", "");
            std::string userId = j.value("userId", "");
            std::string newName = j.value("newName", "");
            
            std::thread([cookie, userId, newName]() {
                std::string errorMsg;
                if (RobloxAPI::ChangeDisplayName(cookie, userId, newName, errorMsg)) {
                    std::string js = "if(window.onChangeDisplayNameSuccess) window.onChangeDisplayNameSuccess('" + newName + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                } else {
                    std::string errorId = "";
                    std::string lowerError = errorMsg;
                    for(auto& c : lowerError) c = tolower(c);
                    
                    if (lowerError.find("throttled") != std::string::npos || lowerError.find("too many") != std::string::npos) {
                        errorId = "throttled";
                    } else if (lowerError.find("filter") != std::string::npos || lowerError.find("inappropriate") != std::string::npos || lowerError.find("moderated") != std::string::npos) {
                        errorId = "moderated";
                    }
                    
                    std::string safeErrorMsg = errorMsg;
                    size_t pos = 0;
                    while((pos = safeErrorMsg.find("'", pos)) != std::string::npos) {
                        safeErrorMsg.replace(pos, 1, "\\'");
                        pos += 2;
                    }
                    
                    std::string js = "if(window.onChangeDisplayNameError) window.onChangeDisplayNameError('" + errorId + "', '" + safeErrorMsg + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
                        }).detach();
        }
        else if (action == "get_friends") {
            std::string cookie = j.value("cookie", "");
            std::string userId = j.value("userId", "");
            std::thread([cookie, userId]() {
                std::string outJson;
                if (RobloxAPI::GetFriends(cookie, userId, outJson)) {
                    std::string escaped;
                    for (char c : outJson) {
                        if (c == '\\') escaped += "\\\\";
                        else if (c == '"') escaped += "\\\"";
                        else if (c == '\'') escaped += "\\\'";
                        else escaped += c;
                    }
                    std::string js = "if(window.onReceiveFriends) window.onReceiveFriends('" + escaped + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                } else {
                    std::string js = "if(window.onReceiveFriendsError) window.onReceiveFriendsError();";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
            }).detach();
        }
        else if (action == "get_thumbnails") {
            std::string cookie = j.value("cookie", "");
            std::string userIds = j.value("userIds", "");
            if (!userIds.empty()) {
                std::thread([cookie, userIds]() {
                    std::wstring path = L"/v1/users/avatar-headshot?userIds=" + s2ws(userIds) + L"&size=48x48&format=Png&isCircular=true";
                    std::string res = RobloxAPI::HttpRequest(L"GET", L"thumbnails.roblox.com", path, cookie);
                    if (!res.empty()) {
                        std::string escaped;
                        for (char c : res) {
                            if (c == '\\') escaped += "\\\\";
                            else if (c == '"') escaped += "\\\"";
                            else if (c == '\'') escaped += "\\\'";
                            else escaped += c;
                        }
                        std::string js = "if(window.onReceiveThumbnails) window.onReceiveThumbnails('" + escaped + "');";
                        PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                    }
                }).detach();
            }
        }
        else if (action == "get_users_info") {
            std::string userIdsJson = j.value("userIds", "[]");
            std::thread([userIdsJson]() {
                std::string body = "{\"userIds\":" + userIdsJson + ",\"excludeBannedUsers\":false}";
                std::string res = RobloxAPI::HttpRequest(L"POST", L"users.roblox.com", L"/v1/users", "", "Content-Type: application/json\r\n", body);
                if (!res.empty()) {
                    std::string escaped;
                    for (char c : res) {
                        if (c == '\\') escaped += "\\\\";
                        else if (c == '"') escaped += "\\\"";
                        else if (c == '\'') escaped += "\\\'";
                        else escaped += c;
                    }
                    std::string js = "if(window.onReceiveUsersInfo) window.onReceiveUsersInfo('" + escaped + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
            }).detach();
        }
        else if (action == "unfriend") {
            std::string cookie = j.value("cookie", "");
            std::string friendId = j.value("friendId", "");
            std::string batchId = j.value("batchId", ""); // For tracking bulk request items
            std::thread([cookie, friendId, batchId]() {
                std::string errorMsg;
                if (RobloxAPI::Unfriend(cookie, friendId, errorMsg)) {
                    std::string js = "if(window.onUnfriendSuccess) window.onUnfriendSuccess('" + friendId + "', '" + batchId + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                } else {
                    std::string safeErrorMsg = errorMsg;
                    size_t pos = 0;
                    while((pos = safeErrorMsg.find("'", pos)) != std::string::npos) {
                        safeErrorMsg.replace(pos, 1, "\\'");
                        pos += 2;
                    }
                    std::string js = "if(window.onUnfriendError) window.onUnfriendError('" + friendId + "', '" + batchId + "', '" + safeErrorMsg + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
            }).detach();
        }
        else if (action == "get_recent_games") {
            std::string cookie = j.value("cookie", "");
            std::thread([cookie]() {
                std::string outJson;
                if (RobloxAPI::GetRecentGames(cookie, outJson)) {
                    std::string escaped;
                    for (char c : outJson) {
                        if (c == '\\') escaped += "\\\\";
                        else if (c == '"') escaped += "\\\"";
                        else if (c == '\'') escaped += "\\\'";
                        else escaped += c;
                    }
                    std::string js = "if(window.onReceiveRecentGames) window.onReceiveRecentGames('" + escaped + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                } else {
                    std::string escaped;
                    for (char c : outJson) {
                        if (c == '\\') escaped += "\\\\";
                        else if (c == '"') escaped += "\\\"";
                        else if (c == '\'') escaped += "\\\'";
                        else escaped += c;
                    }
                    std::string js = "if(window.onReceiveRecentGamesError) window.onReceiveRecentGamesError('" + escaped + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
            }).detach();
        }
        else if (action == "get_outfits") {
            std::string cookie = j.value("cookie", "");
            std::string userId = j.value("userId", "");
            std::thread([cookie, userId]() {
                std::string outJson;
                if (RobloxAPI::GetOutfits(cookie, userId, outJson)) {
                    std::string escaped;
                    for (char c : outJson) {
                        if (c == '\\') escaped += "\\\\";
                        else if (c == '"') escaped += "\\\"";
                        else if (c == '\'') escaped += "\\\'";
                        else escaped += c;
                    }
                    std::string js = "if(window.onReceiveOutfits) window.onReceiveOutfits('" + escaped + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                } else {
                    std::string js = "if(window.onReceiveOutfitsError) window.onReceiveOutfitsError();";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
            }).detach();
        }
        else if (action == "get_avatar") {
            std::string cookie = j.value("cookie", "");
            std::string userId = j.value("userId", "");
            std::thread([cookie, userId]() {
                json jOut;
                std::wstring cacheBuster = L"&v=" + std::to_wstring(std::time(nullptr));
                
                std::wstring fullBodyPath = L"/v1/users/avatar?userIds=" + std::wstring(userId.begin(), userId.end()) + L"&size=352x352&format=Png&isCircular=false" + cacheBuster;
                std::string fullBodyRes = RobloxAPI::HttpRequest(L"GET", L"thumbnails.roblox.com", fullBodyPath, cookie, "", "", nullptr);
                try {
                    auto fb = json::parse(fullBodyRes);
                    if (fb.contains("data") && fb["data"].size() > 0 && fb["data"][0]["imageUrl"].is_string() && fb["data"][0].value("state", "") == "Completed") {
                        jOut["fullBodyUrl"] = fb["data"][0]["imageUrl"].get<std::string>();
                    } else { jOut["fullBodyUrl"] = ""; }
                } catch (...) { jOut["fullBodyUrl"] = ""; }
                
                std::wstring headshotPath = L"/v1/users/avatar-headshot?userIds=" + std::wstring(userId.begin(), userId.end()) + L"&size=150x150&format=Png&isCircular=false" + cacheBuster;
                std::string headshotRes = RobloxAPI::HttpRequest(L"GET", L"thumbnails.roblox.com", headshotPath, cookie, "", "", nullptr);
                try {
                    auto hs = json::parse(headshotRes);
                    if (hs.contains("data") && hs["data"].size() > 0 && hs["data"][0]["imageUrl"].is_string() && hs["data"][0].value("state", "") == "Completed") {
                        jOut["headshotUrl"] = hs["data"][0]["imageUrl"].get<std::string>();
                    } else { jOut["headshotUrl"] = ""; }
                } catch (...) { jOut["headshotUrl"] = ""; }
                
                std::string payload = jOut.dump();
                std::string escaped;
                for (char c : payload) {
                    if (c == '\\') escaped += "\\\\";
                    else if (c == '"') escaped += "\\\"";
                    else if (c == '\'') escaped += "\\\'";
                    else escaped += c;
                }
                std::string js = "if(window.onReceiveAvatar) window.onReceiveAvatar('" + escaped + "');";
                PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
            }).detach();
        }
        else if (action == "wear_outfit") {
            std::string cookie = j.value("cookie", "");
            std::string outfitId = j.value("outfitId", "");
            std::thread([cookie, outfitId]() {
                std::string outError;
                if (RobloxAPI::WearOutfit(cookie, outfitId, outError)) {
                    RobloxAPI::UserInfo info = RobloxAPI::GetUserInfo(cookie);
                    if (info.UserId != 0) {
                        g_accountManager.UpdateAccountInfo(cookie, info);
                        g_accountManager.Save();
                    }
                    std::string js = "if(window.onWearOutfitSuccess) window.onWearOutfitSuccess();";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                    PostMessage(g_hWnd, WM_APP + 2, 0, 0);
                } else {
                    std::string safeErrorMsg = outError;
                    size_t pos = 0;
                    while((pos = safeErrorMsg.find("'", pos)) != std::string::npos) {
                        safeErrorMsg.replace(pos, 1, "\\'");
                        pos += 2;
                    }
                    std::string js = "if(window.onWearOutfitError) window.onWearOutfitError('" + safeErrorMsg + "');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                }
            }).detach();
        }
        else if (action == "add_cookie") {
            std::string cookie = j.value("cookie", "");
            if (g_accountManager.AddAccount(cookie)) {
                std::string username = "Account";
                for (const auto& acc : g_accountManager.GetAccounts()) {
                    if (acc.Cookie == cookie) { username = acc.Info.Username; break; }
                }
                SendStatusMessage(username + " added successfully!", false);
                UpdateUI();
            } else {
                SendStatusMessage("Failed to add account. Invalid cookie?", true);
            }
        }
        else if (action == "launch") {
            std::string cookie = j.value("cookie", "");
            std::string placeId = j.value("placeId", "");
            std::string linkCode = j.value("linkCode", "");
            bool forceRejoin = j.value("forceRejoin", false);
            bool joinLowServer = j.value("joinLowServer", false);
            bool lowestGraphics = j.value("lowestGraphics", false);
            std::string fflagOpt = j.value("fflagOptimization", "Default");
            
            bool alreadyRunning = false;
            DWORD existingPid = 0;
            std::string username = "Account";
            for (const auto& acc : g_accountManager.GetAccounts()) {
                if (acc.Cookie == cookie) {
                    username = acc.Info.Username;
                    if (acc.ProcessId != 0 && IsProcessRunning(acc.ProcessId)) {
                        alreadyRunning = true;
                        existingPid = acc.ProcessId;
                    }
                    break;
                }
            }
            if (alreadyRunning) {
                if (forceRejoin && existingPid != 0) {
                    HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, existingPid);
                    if (hProc != NULL) {
                        TerminateProcess(hProc, 0);
                        CloseHandle(hProc);
                        Sleep(500);
                    }
                } else {
                    Launcher::FocusWindow(existingPid);
                    SendStatusMessage(username + " is already running!", true);
                    PostMessage(g_hWnd, WM_APP + 2, 0, 0); // Update UI to reset spinner
                    return;
                }
            }

            g_accountManager.UpdateAccountProcess(cookie, 1, 0);
            PostMessage(g_hWnd, WM_APP + 2, 0, 0);

            std::thread([cookie, placeId, linkCode, username, joinLowServer, lowestGraphics, fflagOpt]() {
                static std::mutex launchMutex;
                std::lock_guard<std::mutex> lock(launchMutex);
                
                std::string jobId = "";
                if (joinLowServer && !placeId.empty()) {
                    jobId = RobloxAPI::GetLowestServer(placeId, cookie);
                }
                
                std::string launchError;
                DWORD outPID = 0;
                if (Launcher::LaunchAccount(cookie, placeId, linkCode, jobId, launchError, outPID, lowestGraphics, fflagOpt)) {
                    g_accountManager.UpdateAccountProcess(cookie, 2, outPID);
                    
                    std::lock_guard<std::mutex> toastLock(g_toastMutex);
                    std::string successMsg = username + " launched successfully!";
                    if (g_settingsManager.GetSettings().Language == "id") {
                        successMsg = username + " berhasil diluncurkan!";
                    }
                    g_toastQueue.push_back({successMsg, false});
                } else {
                    g_accountManager.UpdateAccountProcess(cookie, 0, 0);
                    std::lock_guard<std::mutex> toastLock(g_toastMutex);
                    if (g_settingsManager.GetSettings().Language == "id") {
                        if (launchError == "Could not find RobloxPlayerBeta.exe! Please launch Roblox once.") {
                            launchError = "RobloxPlayerBeta.exe tidak ditemukan! Harap jalankan Roblox setidaknya sekali.";
                        } else if (launchError == "RobloxPlayerBeta.dll was not found!") {
                            launchError = "RobloxPlayerBeta.dll tidak ditemukan!";
                        } else if (launchError == "Failed to get CSRF token.") {
                            launchError = "Gagal mendapatkan token CSRF.";
                        } else if (launchError == "Failed to get Auth Ticket.") {
                            launchError = "Gagal mendapatkan Auth Ticket.";
                        } else if (launchError == "Update timed out or Roblox failed to launch after update.") {
                            launchError = "Waktu update habis atau Roblox gagal diluncurkan setelah update.";
                        }
                    }
                    g_toastQueue.push_back({launchError, true});
                    ERROR_LOG("Launch Error for " << cookie << ": " << launchError);
                }
                PostMessage(g_hWnd, WM_APP + 2, 0, 0);
            }).detach();
        }
        else if (action == "save_account_game") {
            std::string cookie = j.value("cookie", "");
            std::string placeId = j.value("placeId", "");
            std::string psLink = j.value("psLink", "");
            bool joinLowServer = j.value("joinLowServer", false);
            bool lowestGraphics = j.value("lowestGraphics", false);
            bool antiAfk = j.value("antiAfk", false);
            std::string fflagOpt = j.value("fflagOptimization", "Default");
            std::thread([cookie, placeId, psLink, joinLowServer, lowestGraphics, antiAfk, fflagOpt]() {
                g_accountManager.UpdateAccountGame(cookie, placeId, psLink, joinLowServer, lowestGraphics, antiAfk, fflagOpt);
            }).detach();
        }
        else if (action == "close") {
            PostMessage(g_hWnd, WM_CLOSE, 0, 0);
        }
        else if (action == "minimize") {
            ShowWindow(g_hWnd, SW_MINIMIZE);
        }
        else if (action == "kill_all") {
            int killed = KillAllRobloxInstances();
            if (killed > 0) {
                SendStatusMessage("All Roblox instances terminated.", false);
            } else {
                SendStatusMessage("No instance is running.", true);
            }
            UpdateUI();
        }
        else if (action == "maximize") {
            ShowWindow(g_hWnd, IsZoomed(g_hWnd) ? SW_RESTORE : SW_MAXIMIZE);
        }
        else if (action == "drag") {
            ReleaseCapture();
            SendMessage(g_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        }
        else if (action == "resize") {
            int edge = j.value("edge", 0);
            if (edge >= 1 && edge <= 8) {
                ReleaseCapture();
                SendMessage(g_hWnd, WM_SYSCOMMAND, SC_SIZE + edge, 0);
            }
        }
        else if (action == "remove") {
            std::string cookie = j.value("cookie", "");
            cookie.erase(0, cookie.find_first_not_of(" \t\r\n"));
            cookie.erase(cookie.find_last_not_of(" \t\r\n") + 1);
            g_accountManager.RemoveAccount(cookie);
            UpdateUI();
            SendStatusMessage("Account removed.", false);
        }
        else if (action == "get_settings") {
            SendSettingsData();
        }
        else if (action == "save_global_launch") {
            std::string placeId = j.value("placeId", "");
            std::string psLink = j.value("psLink", "");
            Settings s = g_settingsManager.GetSettings();
            s.GlobalPlaceId = placeId;
            s.GlobalPrivateServerLink = psLink;
            g_settingsManager.SetSettings(s);
        }
        else if (action == "save_settings") {
            Settings s = g_settingsManager.GetSettings();
            s.AutoUpdate = j.value("autoUpdate", s.AutoUpdate);
            s.RunOnStartup = j.value("runOnStartup", s.RunOnStartup);
            s.MinimizeToTrayOnClose = j.value("minimizeToTrayOnClose", s.MinimizeToTrayOnClose);
            s.AlwaysOnTop = j.value("alwaysOnTop", s.AlwaysOnTop);
            s.AutoKillOnExit = j.value("autoKillOnExit", s.AutoKillOnExit);
            bool oldHardwareAccel = s.HardwareAcceleration;
            s.HardwareAcceleration = j.value("hardwareAcceleration", s.HardwareAcceleration);
            s.ResourceOptimizer = j.value("resourceOptimizer", s.ResourceOptimizer);
            s.CpuLimiter = j.value("cpuLimiter", s.CpuLimiter);
            s.BackgroundCpuLimit = j.value("backgroundCpuLimit", s.BackgroundCpuLimit);
            s.LightMode = j.value("lightMode", s.LightMode);
            s.AccentColor = j.value("accentColor", s.AccentColor);
            s.FontFamily = j.value("fontFamily", s.FontFamily);
            s.Language = j.value("language", s.Language);
            s.UiScale = j.value("uiScale", s.UiScale);
            s.SidebarCollapsed = j.value("sidebarCollapsed", s.SidebarCollapsed);
            bool oldBlur = s.EnableWindowBlur;
            s.WindowOpacity = j.value("windowOpacity", s.WindowOpacity);
            s.EnableWindowBlur = j.value("enableWindowBlur", s.EnableWindowBlur);
        s.HideIdentity = j.value("hideIdentity", s.HideIdentity);
            
            bool oldDiscordRPC = s.EnableDiscordRPC;
            s.EnableDiscordRPC = j.value("enableDiscordRPC", s.EnableDiscordRPC);
            
            g_settingsManager.SetSettings(s);
            
            if (oldDiscordRPC != s.EnableDiscordRPC) {
                if (s.EnableDiscordRPC) {
                    DiscordRPC::Initialize("1525423648452771942");
                } else {
                    DiscordRPC::Shutdown();
                }
            }
            SetStartupRegistry(s.RunOnStartup);
            
            ApplyTransparencyMode(g_hWnd, s.EnableWindowBlur, s.WindowOpacity);
            
            HWND insertAfter = s.AlwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST;
            SetWindowPos(g_hWnd, insertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            
            bool silent = j.value("silent", false);
            if (oldHardwareAccel != s.HardwareAcceleration) {
                SendStatusMessage("Hardware Acceleration changed. Restart RoPilot to apply.", false);
            } else if (!silent) {
                SendStatusMessage("Settings saved successfully.", false);
            }
        }
        else if (action == "create_master_password") {
            std::string password = j.value("password", "");
            if (!password.empty()) {
                Settings s = g_settingsManager.GetSettings();
                s.HasMasterPassword = true;
                s.MasterPasswordSalt = Crypto::GenerateSalt();
                s.MasterPasswordHash = Crypto::HashPassword(password, s.MasterPasswordSalt);
                g_settingsManager.SetSettings(s);
                
                g_currentMasterPassword = password;
                g_currentMasterSalt = s.MasterPasswordSalt;
                g_accountManager.Save(g_currentMasterPassword, g_currentMasterSalt);
                UpdateUI();
                SendSettingsData();
            }
        }
        else if (action == "verify_master_password") {
            std::string password = j.value("password", "");
            Settings s = g_settingsManager.GetSettings();
            if (s.HasMasterPassword) {
                std::string hash = Crypto::HashPassword(password, s.MasterPasswordSalt);
                if (hash == s.MasterPasswordHash) {
                    g_currentMasterPassword = password;
                    g_currentMasterSalt = s.MasterPasswordSalt;
                    try {
                        g_accountManager.Load(g_currentMasterPassword, g_currentMasterSalt);
                        g_webview->ExecuteScript(L"window.masterPasswordVerified();", nullptr);
                        UpdateUI();
                    } catch (...) {
                        g_webview->ExecuteScript(L"window.showStatus('Failed to decrypt data.', true);", nullptr);
                    }
                } else {
                    g_webview->ExecuteScript(L"window.showStatus('Incorrect password.', true);", nullptr);
                }
            } else {
                g_accountManager.Load();
                g_webview->ExecuteScript(L"window.masterPasswordVerified();", nullptr);
                UpdateUI();
            }
        }
        else if (action == "change_master_password") {
            std::string oldPassword = j.value("oldPassword", "");
            std::string newPassword = j.value("newPassword", "");
            Settings s = g_settingsManager.GetSettings();
            if (s.HasMasterPassword) {
                std::string hash = Crypto::HashPassword(oldPassword, s.MasterPasswordSalt);
                if (hash == s.MasterPasswordHash) {
                    s.MasterPasswordSalt = Crypto::GenerateSalt();
                    s.MasterPasswordHash = Crypto::HashPassword(newPassword, s.MasterPasswordSalt);
                    g_settingsManager.SetSettings(s);
                    
                    g_currentMasterPassword = newPassword;
                    g_currentMasterSalt = s.MasterPasswordSalt;
                    
                    g_accountManager.Save(g_currentMasterPassword, g_currentMasterSalt);
                    SendSettingsData();
                    g_webview->ExecuteScript(L"window.showStatus('Master password updated.', false);", nullptr);
                } else {
                    g_webview->ExecuteScript(L"window.showStatus('Incorrect old password.', true);", nullptr);
                }
            }
        }
        else if (action == "preview_opacity") {
            // We no longer use LWA_ALPHA for previewing opacity because it makes the whole window (including modals) transparent.
            // HTML body CSS variable var(--bg-opacity) handles the realtime preview perfectly without any Win32 API calls!
        }
        else if (action == "create_shortcut") {
            IShellLinkW* psl;
            if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (LPVOID*)&psl))) {
                WCHAR exePath[MAX_PATH];
                GetModuleFileNameW(NULL, exePath, MAX_PATH);
                psl->SetPath(exePath);
                
                WCHAR dirPath[MAX_PATH];
                wcscpy_s(dirPath, exePath);
                PathRemoveFileSpecW(dirPath);
                psl->SetWorkingDirectory(dirPath);
                
                psl->SetDescription(L"RoPilot");
                psl->SetIconLocation(exePath, 0);

                IPersistFile* ppf;
                if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf))) {
                    WCHAR desktopPath[MAX_PATH];
                    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktopPath))) {
                        std::wstring linkPath = std::wstring(desktopPath) + L"\\RoPilot.lnk";
                        if (SUCCEEDED(ppf->Save(linkPath.c_str(), TRUE))) {
                            PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string("window.postMessage({\"action\":\"shortcut_success\"}, '*');"), 0);
                        }
                    }
                    ppf->Release();
                }
                psl->Release();
            }
        }
        else if (action == "check_update") {
            std::thread([]() {
                std::string downloadUrl;
                std::string newVersion = Updater::CheckForUpdate(downloadUrl);
                if (!newVersion.empty() && !downloadUrl.empty()) {
                    g_pendingUpdateUrl = downloadUrl;
                    
                    if (g_settingsManager.GetSettings().AutoUpdate) {
                        PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string("window.postMessage({\"action\":\"start_update\"}, '*');"), 0);
                    } else {
                        json jOut;
                        jOut["action"] = "update_available";
                        jOut["version"] = newVersion;
                        std::string js = "window.postMessage(" + jOut.dump() + ", '*');";
                        PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                    }
                }
            }).detach();
        }
        else if (action == "start_update") {
            if (g_pendingUpdateUrl.empty()) return;
            std::string url = g_pendingUpdateUrl;
            g_pendingUpdateUrl = ""; // Prevent double-trigger
            
            std::thread([url]() {
                std::string zipPath = "update.zip";
                bool success = Updater::DownloadUpdate(url, zipPath, [](int percent) {
                    json jOut;
                    jOut["action"] = "update_progress";
                    jOut["percentage"] = percent;
                    std::string js = "window.postMessage(" + jOut.dump() + ", '*');";
                    PostMessage(g_hWnd, WM_APP + 3, (WPARAM)new std::string(js), 0);
                });
                
                if (success) {
                    Updater::ApplyUpdate(zipPath);
                }
            }).detach();
        }
        else if (action == "update_layout") {
            try {
                auto newAccountsList = j.value("accounts", json::array());
                auto currentAccounts = g_accountManager.GetAccounts();
                std::vector<Account> updatedAccounts;

                for (const auto& item : newAccountsList) {
                    std::string cookie = item.value("cookie", "");
                    std::string group = item.value("group", "Ungrouped");
                    
                    for (auto& acc : currentAccounts) {
                        if (acc.Cookie == cookie) {
                            acc.Group = group;
                            updatedAccounts.push_back(acc);
                            break;
                        }
                    }
                }
                
                for (auto& acc : currentAccounts) {
                    bool found = false;
                    for (auto& u : updatedAccounts) {
                        if (u.Cookie == acc.Cookie) { found = true; break; }
                    }
                    if (!found) updatedAccounts.push_back(acc);
                }

                if (j.contains("groups") && j["groups"].is_array()) {
                    std::vector<std::string> newGroups;
                    for (const auto& g : j["groups"]) {
                        if (g.is_string()) {
                            newGroups.push_back(g.get<std::string>());
                        }
                    }
                    g_accountManager.SetGroups(newGroups);
                }

                g_accountManager.SetAccounts(updatedAccounts);
            } catch(...) {}
        }
        else if (action == "kill") {
            std::string cookie = j.value("cookie", "");
            for (auto& acc : g_accountManager.GetAccounts()) {
                if (acc.Cookie == cookie) {
                    if (acc.ProcessId != 0) {
                        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, acc.ProcessId);
                        if (hProcess) {
                            TerminateProcess(hProcess, 0);
                            CloseHandle(hProcess);
                            g_accountManager.UpdateAccountProcess(cookie, 0, 0);
                            UpdateUI();
                            SendStatusMessage("Instance killed.", false);
                        } else {
                            SendStatusMessage("Failed to kill instance (process not found?).", true);
                            ERROR_LOG("Failed to kill instance for " << cookie);
                        }
                    } else {
                        SendStatusMessage("No active instance found.", true);
                    }
                    break;
                }
            }
        }
        else if (action == "kill_group") {
            std::string targetGroup = j.value("group", "");
            int killed = 0;
            std::string killedNames = "";
            
            for (auto& acc : g_accountManager.GetAccounts()) {
                if (acc.Group == targetGroup && acc.ProcessId != 0) {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, acc.ProcessId);
                    if (hProcess) {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                        g_accountManager.UpdateAccountProcess(acc.Cookie, 0, 0);
                        killed++;
                        if (!killedNames.empty()) killedNames += ", ";
                        killedNames += acc.Info.Username;
                    }
                }
            }
            
            if (killed > 0) {
                UpdateUI();
                SendStatusMessage(targetGroup + " group instances killed.", false);
            } else {
                SendStatusMessage("No active instance found.", true);
            }
        }
    } catch (...) {}
}

void SendSettingsData() {
    if (!g_webview) return;
    Settings s = g_settingsManager.GetSettings();
    json jOut;
    jOut["action"] = "settings_data";
    jOut["autoUpdate"] = s.AutoUpdate;
    jOut["runOnStartup"] = s.RunOnStartup;
    jOut["minimizeToTrayOnClose"] = s.MinimizeToTrayOnClose;
    jOut["alwaysOnTop"] = s.AlwaysOnTop;
    jOut["autoKillOnExit"] = s.AutoKillOnExit;
    jOut["hardwareAcceleration"] = s.HardwareAcceleration;
    jOut["resourceOptimizer"] = s.ResourceOptimizer;
    jOut["cpuLimiter"] = s.CpuLimiter;
    jOut["backgroundCpuLimit"] = s.BackgroundCpuLimit;
    jOut["lightMode"] = s.LightMode;
    jOut["accentColor"] = s.AccentColor;
    jOut["fontFamily"] = s.FontFamily;
    jOut["language"] = s.Language;
    jOut["uiScale"] = s.UiScale;
    jOut["sidebarCollapsed"] = s.SidebarCollapsed;
    jOut["windowOpacity"] = s.WindowOpacity;
    jOut["enableWindowBlur"] = s.EnableWindowBlur;
    jOut["hideIdentity"] = s.HideIdentity;
    jOut["enableDiscordRPC"] = s.EnableDiscordRPC;
    jOut["globalPlaceId"] = s.GlobalPlaceId;
    jOut["globalPrivateServerLink"] = s.GlobalPrivateServerLink;
    jOut["hasMasterPassword"] = s.HasMasterPassword;
    std::string js = "window.postMessage(" + jOut.dump() + ", '*');";
    g_webview->ExecuteScript(s2ws(js).c_str(), nullptr);
}

void UpdateUI() {
    if (!g_webview) return;
    
    json jRoot = json::object();
    json jArray = json::array();
    for (const auto& acc : g_accountManager.GetAccounts()) {
        json jAcc;
        jAcc["Username"] = acc.Info.Username;
        jAcc["DisplayName"] = acc.Info.DisplayName;
        jAcc["UserId"] = acc.Info.UserId;
        jAcc["ThumbnailUrl"] = acc.Info.ThumbnailUrl;
        jAcc["Cookie"] = acc.Cookie;
        jAcc["Status"] = acc.Status;
        jAcc["JobId"] = acc.JobId;
        jAcc["ProcessId"] = acc.ProcessId;
        jAcc["Group"] = acc.Group;
        jAcc["PlaceId"] = acc.PlaceId;
        jAcc["PrivateServerLink"] = acc.PrivateServerLink;
        jAcc["JoinLowServer"] = acc.JoinLowServer;
        jAcc["LowestGraphics"] = acc.LowestGraphics;
        jAcc["AntiAFK"] = acc.AntiAFK;
        jAcc["FFlagOptimization"] = acc.FFlagOptimization;
        
        jAcc["CpuUsage"] = acc.Analytics.cpuUsage;
        jAcc["RamUsage"] = acc.Analytics.ramUsageMB;
        if (acc.Analytics.hasLaunchTime) {
            auto now = std::chrono::system_clock::now();
            jAcc["RuntimeSeconds"] = std::chrono::duration_cast<std::chrono::seconds>(now - acc.Analytics.launchTime).count();
        } else {
            jAcc["RuntimeSeconds"] = 0;
        }
        
        jArray.push_back(jAcc);
    }
    jRoot["accounts"] = jArray;
    
    json jGroups = json::array();
    for (const auto& g : g_accountManager.GetGroups()) {
        jGroups.push_back(g);
    }
    jRoot["groups"] = jGroups;
    
    
    std::string jsonStr = jRoot.dump();
    std::wstring script = L"window.updateAccounts(String.raw`" + s2ws(jsonStr) + L"`);";
    g_webview->ExecuteScript(script.c_str(), nullptr);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_NCCALCSIZE: {
        if (wParam == TRUE) {
            return 0;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    case WM_NCACTIVATE: {
        // Prevent Windows from automatically repainting the classic titlebar when the window loses/gains focus
        // By passing -1 as lParam, DefWindowProc will process the activation state but SKIP the non-client repaint!
        return DefWindowProc(hWnd, message, wParam, -1);
    }
    case WM_NCPAINT: {
        // Completely suppress any accidental non-client area painting
        return 0;
    }
    case WM_GETMINMAXINFO: {
        MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        if (GetMonitorInfo(hMonitor, &mi)) {
            mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
            mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
            mmi->ptMaxPosition.x = mi.rcWork.left - mi.rcMonitor.left;
            mmi->ptMaxPosition.y = mi.rcWork.top - mi.rcMonitor.top;
        }
        mmi->ptMinTrackSize.x = 940;
        mmi->ptMinTrackSize.y = 500;
        return 0;
    }
    case WM_APP + 2: {
        UpdateUI();
        {
            std::lock_guard<std::mutex> lock(g_toastMutex);
            for(auto& toast : g_toastQueue) {
                SendStatusMessage(toast.first, toast.second);
            }
            g_toastQueue.clear();
        }
        return 0;
    }

    case WM_SIZE: {
        RECT bounds;
        GetClientRect(hWnd, &bounds);
        
        if (bounds.right > 0 && bounds.bottom > 0) {
            HRGN hRgn;
            if (wParam == SIZE_MAXIMIZED) {
                hRgn = CreateRectRgn(0, 0, bounds.right, bounds.bottom);
            } else {
                hRgn = CreateRoundRectRgn(0, 0, bounds.right + 1, bounds.bottom + 1, 16, 16);
            }
            SetWindowRgn(hWnd, hRgn, TRUE);
        }

        if (g_webviewController != nullptr) {
            g_webviewController->put_Bounds(bounds);
        }
        break;
    }
    case WM_CLOSE:
        if (g_settingsManager.GetSettings().MinimizeToTrayOnClose) {
            ShowWindow(hWnd, SW_HIDE);
            if (!g_hasShownTrayNotification) {
                g_hasShownTrayNotification = true;
                g_nid.uFlags = NIF_INFO;
                wcscpy_s(g_nid.szInfo, L"To fully close the app, right-click the RoPilot icon in your system tray and select Exit.");
                wcscpy_s(g_nid.szInfoTitle, L"RoPilot is still running");
                g_nid.dwInfoFlags = NIIF_INFO;
                Shell_NotifyIconW(NIM_MODIFY, &g_nid);
                g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            }
        } else {
            DestroyWindow(hWnd);
        }
        return 0;
    case WM_TRAYICON:
        if (lParam == WM_LBUTTONUP) {
            ShowWindow(hWnd, SW_RESTORE);
            SetForegroundWindow(hWnd);
        } else if (lParam == WM_RBUTTONUP) {
            POINT pt;
            GetCursorPos(&pt);
            HMENU hMenu = CreatePopupMenu();
            InsertMenuW(hMenu, -1, MF_BYPOSITION | MF_STRING, 1001, L"Open");
            InsertMenuW(hMenu, -1, MF_BYPOSITION | MF_STRING, 1002, L"Exit");
            SetForegroundWindow(hWnd);
            int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hWnd, NULL);
            DestroyMenu(hMenu);
            if (cmd == 1001) {
                ShowWindow(hWnd, SW_RESTORE);
                SetForegroundWindow(hWnd);
            } else if (cmd == 1002) {
                DestroyWindow(hWnd);
            }
        }
        return 0;
    case WM_DESTROY:
        if (g_settingsManager.GetSettings().AutoKillOnExit) {
            KillAllRobloxInstances();
        }
        RemoveTrayIcon();
        g_running = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HANDLE g_h773Lock = INVALID_HANDLE_VALUE;

void Apply773Fix() {
    char localAppData[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {
        std::string robloxPath = std::string(localAppData) + "\\Roblox";
        std::string localStoragePath = robloxPath + "\\LocalStorage";
        std::string cookiesPath = localStoragePath + "\\RobloxCookies.dat";
        
        CreateDirectoryA(robloxPath.c_str(), NULL);
        CreateDirectoryA(localStoragePath.c_str(), NULL);
        
        g_h773Lock = CreateFileA(cookiesPath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    try {
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        HANDLE hMutex = CreateMutexW(NULL, TRUE, L"RoPilot_SingleInstance_Mutex");
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            HWND existingWnd = FindWindowW(L"MultiRobloxClass", L"RoPilot");
            if (existingWnd) {
                ShowWindow(existingWnd, SW_RESTORE);
                SetForegroundWindow(existingWnd);
            }
            CloseHandle(hMutex);
            return 0;
        }

        if (lpCmdLine && wcsstr(lpCmdLine, L"--updated") != nullptr) {
            g_showChangelog = true;
        }

        Apply773Fix();

        Launcher::InitializeMultiInstance();

    g_accountManager.Load();
    
    if (g_settingsManager.GetSettings().EnableDiscordRPC) {
        DiscordRPC::Initialize("1525423648452771942");
    }

    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszClassName = L"MultiRobloxClass";
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(1));
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(1));
    RegisterClassExW(&wcex);

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int winW = 1000;
    int winH = 600;
    int xPos = (screenW - winW) / 2;
    int yPos = (screenH - winH) / 2;

    g_hWnd = CreateWindowExW(WS_EX_LAYERED, L"MultiRobloxClass", L"RoPilot", 
        WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME, xPos, yPos, winW, winH, 
        nullptr, nullptr, hInstance, nullptr);

    HWND insertAfter = g_settingsManager.GetSettings().AlwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST;
    SetWindowPos(g_hWnd, insertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    AddTrayIcon(g_hWnd);
    ApplyTransparencyMode(g_hWnd, g_settingsManager.GetSettings().EnableWindowBlur, g_settingsManager.GetSettings().WindowOpacity);
    SetStartupRegistry(g_settingsManager.GetSettings().RunOnStartup);

    BOOL useDarkMode = TRUE;
    HMODULE hDwm = LoadLibraryW(L"dwmapi.dll");
    if (hDwm) {
        typedef HRESULT(WINAPI* DwmSetWindowAttributePtr)(HWND, DWORD, LPCVOID, DWORD);
        DwmSetWindowAttributePtr pDwmSetWindowAttribute = (DwmSetWindowAttributePtr)GetProcAddress(hDwm, "DwmSetWindowAttribute");
        if (pDwmSetWindowAttribute) {
            pDwmSetWindowAttribute(g_hWnd, 20, &useDarkMode, sizeof(useDarkMode));
            pDwmSetWindowAttribute(g_hWnd, 19, &useDarkMode, sizeof(useDarkMode));
            
            #ifndef DWMWA_BORDER_COLOR
            #define DWMWA_BORDER_COLOR 34
            #endif
            DWORD darkColor = 0x00111111;
            pDwmSetWindowAttribute(g_hWnd, DWMWA_BORDER_COLOR, &darkColor, sizeof(darkColor));
            DWORD cornerPref = 2; // DWMWCP_ROUND
            pDwmSetWindowAttribute(g_hWnd, 33, &cornerPref, sizeof(cornerPref));
            
            typedef struct _MARGINS { int cxLeftWidth; int cxRightWidth; int cyTopHeight; int cyBottomHeight; } MARGINS;
            typedef HRESULT(WINAPI* DwmExtendFrameIntoClientAreaPtr)(HWND, const MARGINS*);
            DwmExtendFrameIntoClientAreaPtr extendFrame = (DwmExtendFrameIntoClientAreaPtr)GetProcAddress(hDwm, "DwmExtendFrameIntoClientArea");
            if (extendFrame) {
                MARGINS margins = {-1, -1, -1, -1};
                extendFrame(g_hWnd, &margins);
            }
        }
        FreeLibrary(hDwm);
    }
        
    std::thread([]() {
        int presenceTimer = 0;
        while (g_running) {
            Sleep(1000);
            if (!g_running) break;
            
            bool updated = false;
            auto accounts = g_accountManager.GetAccounts();
            
            if (Launcher::IsAnyRobloxRunning()) {
                ActiveClientLock::LockClient();
            } else {
                ActiveClientLock::UnlockClient();
            }
            
            int activeAccounts = 0;
            
            presenceTimer++;
            bool checkPresence = (presenceTimer % 15 == 0);
            
            for (auto& acc : accounts) {
                if (acc.Status == 2 || acc.Status == 3) {
                    bool isProcessAlive = false;
                    if (acc.ProcessId != 0) {
                        DWORD exitCode = 0;
                        HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, acc.ProcessId);
                        if (hProcess) {
                            if (GetExitCodeProcess(hProcess, &exitCode)) {
                                if (exitCode == STILL_ACTIVE) {
                                    isProcessAlive = true;
                                    activeAccounts++;
                                    
                                    AnalyticsState state = acc.Analytics;
                                    
                                    PROCESS_MEMORY_COUNTERS pmc;
                                    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                                        state.ramUsageMB = (double)pmc.WorkingSetSize / (1024.0 * 1024.0);
                                    }
                                    
                                    FILETIME idleTime, kernelTime, userTime;
                                    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
                                        FILETIME creationTime, processExitTime, processKernelTime, processUserTime;
                                        if (GetProcessTimes(hProcess, &creationTime, &processExitTime, &processKernelTime, &processUserTime)) {
                                            uint64_t sysKernel = FileTimeToUInt64(kernelTime);
                                            uint64_t sysUser = FileTimeToUInt64(userTime);
                                            uint64_t procKernel = FileTimeToUInt64(processKernelTime);
                                            uint64_t procUser = FileTimeToUInt64(processUserTime);
                                            
                                            uint64_t sysTotal = sysKernel + sysUser;
                                            uint64_t procTotal = procKernel + procUser;
                                            
                                            if (state.lastSystemTime != 0 && sysTotal > state.lastSystemTime) {
                                                uint64_t sysDelta = sysTotal - state.lastSystemTime;
                                                uint64_t procDelta = procTotal - state.lastProcessTime;
                                                state.cpuUsage = ((double)procDelta / (double)sysDelta) * 100.0;
                                            }
                                            state.lastSystemTime = sysTotal;
                                            state.lastProcessTime = procTotal;
                                        }
                                    }
                                    g_accountManager.UpdateAccountAnalytics(acc.Cookie, state);
                                    
                                    if (checkPresence) {
                                        std::string jobId;
                                        int presenceType = 0;
                                        if (RobloxAPI::GetPresence(acc.Cookie, std::to_string(acc.Info.UserId), jobId, presenceType)) {
                                            if (presenceType == 2) {
                                                if (acc.Status != 3) {
                                                    g_accountManager.UpdateAccountProcess(acc.Cookie, 3, acc.ProcessId);
                                                }
                                            } else {
                                                if (acc.Status != 2) {
                                                    g_accountManager.UpdateAccountProcess(acc.Cookie, 2, acc.ProcessId);
                                                }
                                            }
                                        }
                                    }
                                    
                                    updated = true;
                                }
                            }
                            CloseHandle(hProcess);
                        }
                    }

                    if (!isProcessAlive) {
                        g_accountManager.UpdateAccountProcess(acc.Cookie, 0, 0);
                        updated = true;
                    }
                }
            }
            
            DiscordRPC::UpdatePresence(activeAccounts);
            
            if (updated) {
                PostMessage(g_hWnd, WM_APP + 2, 0, 0);
            }
        }
    }).detach();

    std::thread([]() {
        while (g_running) {
            auto accounts = g_accountManager.GetAccounts();
            bool anyChanged = false;
            for (const auto& acc : accounts) {
                if (!g_running) break;
                auto info = RobloxAPI::GetUserInfo(acc.Cookie);
                if (info.UserId == 0) {
                    // Invalid cookie detected
                    if (acc.Status != 4) {
                        g_accountManager.UpdateAccountProcess(acc.Cookie, 4, 0);
                        anyChanged = true;
                    }
                } else if (info.ThumbnailUrl != acc.Info.ThumbnailUrl || info.Username != acc.Info.Username || info.DisplayName != acc.Info.DisplayName) {
                    g_accountManager.UpdateAccountInfo(acc.Cookie, info);
                    anyChanged = true;
                }
                Sleep(1000); // Check 1 account per second to avoid rate limiting
            }
            if (anyChanged && g_running) {
                PostMessage(g_hWnd, WM_APP + 2, 0, 0);
            }
            for (int i = 0; i < 300; ++i) {
                if (!g_running) break;
                Sleep(1000);
            }
        }
    }).detach();

    // Anti-AFK Thread
    std::thread([]() {
        struct EnumData {
            DWORD processId;
            HWND hwnd;
        };
        
        while (g_running) {
            for (int i = 0; i < 300; ++i) {
                if (!g_running) return;
                Sleep(1000);
            }
            
            auto accounts = g_accountManager.GetAccounts();
            for (const auto& acc : accounts) {
                if (acc.AntiAFK && acc.ProcessId != 0 && acc.Status == 3) {
                    EnumData data = { acc.ProcessId, NULL };
                    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
                        EnumData* d = (EnumData*)lParam;
                        DWORD pid = 0;
                        GetWindowThreadProcessId(hwnd, &pid);
                        if (pid == d->processId) {
                            char className[256];
                            GetClassNameA(hwnd, className, sizeof(className));
                            if (std::string(className) == "WINDOWSCLIENT") {
                                d->hwnd = hwnd;
                                return FALSE;
                            }
                        }
                        return TRUE;
                    }, (LPARAM)&data);
                    
                    if (data.hwnd != NULL) {
                        HWND originalFg = GetForegroundWindow();
                        bool wasMinimized = IsIconic(data.hwnd);
                        
                        if (wasMinimized) {
                            ShowWindow(data.hwnd, SW_RESTORE);
                            Sleep(80);
                        }

                        // Force Active
                        DWORD fgThread = GetWindowThreadProcessId(originalFg, NULL);
                        DWORD myThread = GetCurrentThreadId();
                        
                        if (fgThread != myThread) {
                            AttachThreadInput(myThread, fgThread, TRUE);
                            SetForegroundWindow(data.hwnd);
                            AttachThreadInput(myThread, fgThread, FALSE);
                        } else {
                            SetForegroundWindow(data.hwnd);
                        }
                        
                        Sleep(80);

                        INPUT inputs[2] = {0};
                        
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_SPACE;
                        inputs[0].ki.wScan = MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC);
                        inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SPACE;
                        inputs[1].ki.wScan = MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC);
                        inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

                        SendInput(1, inputs, sizeof(INPUT));
                        Sleep(50);
                        SendInput(1, inputs + 1, sizeof(INPUT));
                        
                        Sleep(120);

                        if (originalFg != NULL && originalFg != data.hwnd) {
                            fgThread = GetWindowThreadProcessId(originalFg, NULL);
                            if (fgThread != myThread) {
                                AttachThreadInput(myThread, fgThread, TRUE);
                                SetForegroundWindow(originalFg);
                                AttachThreadInput(myThread, fgThread, FALSE);
                            } else {
                                SetForegroundWindow(originalFg);
                            }
                        }

                        if (wasMinimized) {
                            ShowWindow(data.hwnd, SW_MINIMIZE);
                        }
                    }
                }
            }
        }
    }).detach();

    if (!g_hWnd) {
        LOG("Failed to create window");
        return FALSE;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);
    LOG("Window shown, creating WebView2");

    auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
    if (!g_settingsManager.GetSettings().HardwareAcceleration) {
        options->put_AdditionalBrowserArguments(L"--disable-gpu");
    }

    CreateCoreWebView2EnvironmentWithOptions(nullptr, L"main_data", options.Get(),
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                g_webviewEnv = env;
                env->CreateCoreWebView2Controller(g_hWnd, 
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (controller != nullptr) {
                                g_webviewController = controller;
                                g_webviewController->get_CoreWebView2(&g_webview);
                            }
                            ICoreWebView2Controller2* controller2 = nullptr;
                            if (SUCCEEDED(g_webviewController->QueryInterface(IID_PPV_ARGS(&controller2))) && controller2) {
                                COREWEBVIEW2_COLOR color = {0, 0, 0, 0};
                                controller2->put_DefaultBackgroundColor(color);
                                controller2->Release();
                            }


                            ICoreWebView2Settings* settings;
                            if (SUCCEEDED(g_webviewController->get_CoreWebView2(&g_webview)) && g_webview) {
                                if (SUCCEEDED(g_webview->get_Settings(&settings)) && settings) {
                                    settings->put_AreDevToolsEnabled(FALSE);
                                    settings->put_AreDefaultContextMenusEnabled(FALSE);
                                    settings->Release();
                                }
                            }

                            EventRegistrationToken token;
                            g_webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                [](ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                     LPWSTR messageRaw = nullptr;
                                     if (SUCCEEDED(args->TryGetWebMessageAsString(&messageRaw)) && messageRaw) {
                                         ProcessWebMessage(ws2s(messageRaw));
                                         CoTaskMemFree(messageRaw);
                                     } else if (SUCCEEDED(args->get_WebMessageAsJson(&messageRaw)) && messageRaw) {
                                         ProcessWebMessage(ws2s(messageRaw));
                                         CoTaskMemFree(messageRaw);
                                     }
                                    return S_OK;
                                }).Get(), &token);

                            RECT bounds;
                            GetClientRect(g_hWnd, &bounds);
                            g_webviewController->put_Bounds(bounds);

std::string html = GetUIHtml();
Settings s = g_settingsManager.GetSettings();
if (s.LightMode) {
    size_t pos = html.find("<html lang=\"en\">");
    if (pos != std::string::npos) {
        html.replace(pos, 16, "<html lang=\"en\" data-theme=\"light\">");
    } else {
        pos = html.find("<html>");
        if (pos != std::string::npos) {
            html.replace(pos, 6, "<html data-theme=\"light\">");
        }
    }
}

if (s.SidebarCollapsed) {
    size_t pos = html.find("class=\"sidebar\" id=\"sidebar\"");
    if (pos != std::string::npos) {
        html.replace(pos, 28, "class=\"sidebar collapsed\" id=\"sidebar\"");
    }
}

std::string extraStyle = "<style>:root {";
if (!s.AccentColor.empty()) {
    extraStyle += "--accent-color: " + s.AccentColor + ";";
    extraStyle += "--accent-tint-hover: " + s.AccentColor + "26;";
    extraStyle += "--accent-tint-active: " + s.AccentColor + "40;";
}
if (!s.FontFamily.empty()) {
    extraStyle += "--font-family: " + s.FontFamily + ";";
}
    extraStyle += "--ui-scale: " + std::to_string(s.UiScale) + ";";
extraStyle += "}</style>";

size_t headPos = html.find("</head>");
if (headPos != std::string::npos) {
    html.insert(headPos, extraStyle);
}

g_webview->NavigateToString(s2ws(html).c_str());

                            g_webview->add_NavigationCompleted(Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                [](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
                                    UpdateUI();
                                    
                                    if (g_showChangelog) {
                                        std::ifstream clog("changelog.txt");
                                        if (clog.is_open()) {
                                            std::stringstream buffer;
                                            buffer << clog.rdbuf();
                                            std::string changelog = buffer.str();
                                            clog.close();
                                            remove("changelog.txt");
                                            
                                            json jOut;
                                            jOut["action"] = "show_changelog";
                                            jOut["content"] = changelog;
                                            jOut["version"] = Updater::CURRENT_VERSION;
                                            std::string js = "window.postMessage(" + jOut.dump() + ", '*');";
                                            g_webview->ExecuteScript(s2ws(js).c_str(), nullptr);
                                        }
                                        g_showChangelog = false;
                                    }
                                    
                                    if (Launcher::IsAnyRobloxRunning()) {
                                        g_webview->ExecuteScript(L"if(window.showKillAllPrompt) window.showKillAllPrompt();", nullptr);
                                    }
                                    
                                    return S_OK;
                                }).Get(), &token);

                            return S_OK;
                        }).Get());
                return S_OK;
            }).Get());

        MSG msg;
        Optimizer::Start();
        while (GetMessage(&msg, nullptr, 0, 0)) {
            if (msg.message == WM_APP + 3) {
                std::string* js = (std::string*)msg.wParam;
                if (g_webview) {
                    g_webview->ExecuteScript(s2ws(*js).c_str(), nullptr);
                }
                delete js;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        Optimizer::Stop();
        ActiveClientLock::UnlockClient();
        DiscordRPC::Shutdown();
        Launcher::ApplyFFlags(Launcher::FindRobloxExecutable(), "Default");
        CoUninitialize();
        return (int)msg.wParam;
    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Exception", MB_OK | MB_ICONERROR);
        return -1;
    } catch (...) {
        MessageBoxA(NULL, "Unknown exception", "Exception", MB_OK | MB_ICONERROR);
        return -1;
    }
}