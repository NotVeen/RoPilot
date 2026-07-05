#include <windows.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <wrl.h>
#include "AccountManager.h"
#include "Launcher.h"
#include "RobloxAPI.h"
#include "BrowserLogin.h"
#include "UI_Frontend.h"
#include <json.hpp>
#include <iostream>
#include <string>
#include <windowsx.h>
#include <fstream>
#include <thread>
#include <atomic>

using namespace Microsoft::WRL;
using json = nlohmann::json;

std::atomic<bool> g_running{true};

std::ofstream debug_log("debug_log.txt", std::ios::trunc);
#define LOG(x) debug_log << x << std::endl;

std::ofstream error_log("error.txt", std::ios::trunc);
#define ERROR_LOG(x) error_log << x << std::endl;


AccountManager g_accountManager;
HWND g_hWnd = NULL;
ComPtr<ICoreWebView2Controller> g_webviewController;
ComPtr<ICoreWebView2> g_webview;
ComPtr<ICoreWebView2Environment> g_webviewEnv;

// Forward declaration
void UpdateUI();

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

void SendStatusMessage(const std::string& msg, bool isError) {
    if (!g_webview) return;
    std::string escapedMsg = msg;
    // Basic escape
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
            // Need to spawn BrowserLogin
            BrowserLogin::OpenLoginBrowser(g_webviewEnv.Get(), [](const std::string& cookie) {
                if (!cookie.empty()) {
                    if (g_accountManager.AddAccount(cookie)) {
                        SendStatusMessage("Account added successfully!", false);
                        UpdateUI();
                    } else {
                        SendStatusMessage("Failed to add account. Invalid cookie?", true);
                    }
                } else {
                    SendStatusMessage("Browser login cancelled or failed.", true);
                }
            });
        } 
        else if (action == "add_cookie") {
            std::string cookie = j.value("cookie", "");
            if (g_accountManager.AddAccount(cookie)) {
                SendStatusMessage("Account added successfully!", false);
                UpdateUI();
            } else {
                SendStatusMessage("Failed to add account. Invalid cookie?", true);
            }
        }
        else if (action == "launch") {
            std::string cookie = j.value("cookie", "");
            std::string placeId = j.value("placeId", "");
            std::string errorMsg;
            DWORD outPID = 0;
            if (Launcher::LaunchAccount(cookie, placeId, errorMsg, outPID)) {
                g_accountManager.UpdateAccountProcess(cookie, 2, outPID);
                UpdateUI();
                SendStatusMessage("Launched successfully!", false);
            } else {
                UpdateUI();
                SendStatusMessage(errorMsg, true);
                ERROR_LOG("Launch Error for " << cookie << ": " << errorMsg);
            }
        }
        else if (action == "close") {
            PostMessage(g_hWnd, WM_CLOSE, 0, 0);
        }
        else if (action == "minimize") {
            ShowWindow(g_hWnd, SW_MINIMIZE);
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
            g_accountManager.RemoveAccount(cookie);
            UpdateUI();
            SendStatusMessage("Account removed.", false);
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
                
                // Keep any missing ones at the end just in case
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
    } catch (...) {}
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
        jAcc["Group"] = acc.Group;
        jArray.push_back(jAcc);
    }
    jRoot["accounts"] = jArray;
    
    json jGroups = json::array();
    for (const auto& g : g_accountManager.GetGroups()) {
        jGroups.push_back(g);
    }
    jRoot["groups"] = jGroups;
    
    
    std::string jsonStr = jRoot.dump();
    FILE* f = fopen("debug_json.txt", "w");
    if(f) {
        fwrite(jsonStr.c_str(), 1, jsonStr.length(), f);
        fclose(f);
    }
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
        return 0;
    }
    case WM_APP + 2: {
        UpdateUI();
        return 0;
    }

    case WM_APP + 1: {
        bool updated = false;
        auto accounts = g_accountManager.GetAccounts();
        for (auto& acc : accounts) {
            if (acc.Status == 1 || acc.Status == 2) {
                bool isProcessAlive = false;
                if (acc.ProcessId != 0) {
                    DWORD exitCode = 0;
                    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, acc.ProcessId);
                    if (hProcess) {
                        if (GetExitCodeProcess(hProcess, &exitCode)) {
                            if (exitCode == STILL_ACTIVE) isProcessAlive = true;
                        }
                        CloseHandle(hProcess);
                    }
                }

                if (!isProcessAlive) {
                    g_accountManager.UpdateAccountProcess(acc.Cookie, 0, 0);
                    updated = true;
                } else {
                    std::string jobId;
                    int presenceType;
                    if (RobloxAPI::GetPresence(acc.Cookie, std::to_string(acc.Info.UserId), jobId, presenceType)) {
                        if (presenceType == 2) { // 2 = InGame
                            if (acc.Status != 1 || acc.JobId != jobId) {
                                g_accountManager.UpdateAccountPresence(acc.Cookie, 1, jobId);
                                updated = true;
                            }
                        }
                    }
                }
            }
        }
        if (updated) {
            UpdateUI();
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
    case WM_DESTROY:
        g_running = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    try {
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        // Initialize Mutex
        Launcher::InitializeMultiInstance();

    // Load Accounts
    g_accountManager.Load();

    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

    g_hWnd = CreateWindowW(L"MultiRobloxClass", L"RoPilot", 
        WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME, xPos, yPos, winW, winH, 
        nullptr, nullptr, hInstance, nullptr);

    // Apply dark mode to the window border
    BOOL useDarkMode = TRUE;
    HMODULE hDwm = LoadLibraryW(L"dwmapi.dll");
    if (hDwm) {
        typedef HRESULT(WINAPI* DwmSetWindowAttributePtr)(HWND, DWORD, LPCVOID, DWORD);
        DwmSetWindowAttributePtr pDwmSetWindowAttribute = (DwmSetWindowAttributePtr)GetProcAddress(hDwm, "DwmSetWindowAttribute");
        if (pDwmSetWindowAttribute) {
            // 20 is for Windows 11 and newer Windows 10 builds, 19 is for older Windows 10 builds
            pDwmSetWindowAttribute(g_hWnd, 20, &useDarkMode, sizeof(useDarkMode));
            pDwmSetWindowAttribute(g_hWnd, 19, &useDarkMode, sizeof(useDarkMode));
            
            // Set window border color directly if available (Windows 11)
            #ifndef DWMWA_BORDER_COLOR
            #define DWMWA_BORDER_COLOR 34
            #endif
            COLORREF darkColor = RGB(42, 42, 42); // match --border-color #2a2a2a
            pDwmSetWindowAttribute(g_hWnd, DWMWA_BORDER_COLOR, &darkColor, sizeof(darkColor));
        }
        FreeLibrary(hDwm);
    }
        
    std::thread([]() {
        while (g_running) {
            Sleep(5000);
            if (!g_running) break;
            PostMessage(g_hWnd, WM_APP + 1, 0, 0);
        }
    }).detach();

    std::thread([]() {
        while (g_running) {
            auto accounts = g_accountManager.GetAccounts();
            bool anyChanged = false;
            for (const auto& acc : accounts) {
                if (!g_running) break;
                auto info = RobloxAPI::GetUserInfo(acc.Cookie);
                if (info.UserId != 0 && (info.ThumbnailUrl != acc.Info.ThumbnailUrl || info.Username != acc.Info.Username || info.DisplayName != acc.Info.DisplayName)) {
                    g_accountManager.UpdateAccountInfo(acc.Cookie, info);
                    anyChanged = true;
                }
                Sleep(1000); // Check 1 account per second to avoid rate limiting
            }
            if (anyChanged && g_running) {
                PostMessage(g_hWnd, WM_APP + 2, 0, 0);
            }
            // Sleep for 30 seconds before next full check
            for (int i = 0; i < 30; ++i) {
                if (!g_running) break;
                Sleep(1000);
            }
        }
    }).detach();

    if (!g_hWnd) {
        LOG("Failed to create window");
        return FALSE;
    }

    // Initial region is handled by WM_SIZE

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);
    LOG("Window shown, creating WebView2");

    CreateCoreWebView2EnvironmentWithOptions(nullptr, L"main_data", nullptr,
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

                            // Disable DevTools and Context Menu
                            ICoreWebView2Settings* settings;
                            if (SUCCEEDED(g_webviewController->get_CoreWebView2(&g_webview)) && g_webview) {
                                if (SUCCEEDED(g_webview->get_Settings(&settings)) && settings) {
                                    settings->put_AreDevToolsEnabled(FALSE);
                                    settings->put_AreDefaultContextMenusEnabled(FALSE);
                                    settings->Release();
                                }
                            }

                            // Setup Message Received
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

                            // Initial Size
                            RECT bounds;
                            GetClientRect(g_hWnd, &bounds);
                            g_webviewController->put_Bounds(bounds);

                            // Load UI
                            g_webview->NavigateToString(s2ws(HTML_CONTENT).c_str());

                            // Populate Accounts after initial load
                            g_webview->add_NavigationCompleted(Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                [](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
                                    UpdateUI();
                                    return S_OK;
                                }).Get(), &token);

                            return S_OK;
                        }).Get());
                return S_OK;
            }).Get());

        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

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