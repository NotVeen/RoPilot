#include "BrowserLogin.h"
#include <windows.h>
#include <wrl.h>
#include "WebView2.h"
#include <iostream>

using namespace Microsoft::WRL;

namespace BrowserLogin {

    HWND g_hWnd = NULL;
    ComPtr<ICoreWebView2Controller> webviewController;
    ComPtr<ICoreWebView2> webview;
    std::function<void(const std::string&)> g_onCookieFound;

    LRESULT CALLBACK WndProcBrowser(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            case WM_SIZE:
                if (webviewController != nullptr) {
                    RECT bounds;
                    GetClientRect(hWnd, &bounds);
                    webviewController->put_Bounds(bounds);
                }
                break;
            case WM_DESTROY:
                webviewController = nullptr;
                webview = nullptr;
                break;
            default:
                return DefWindowProcW(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    void OpenLoginBrowser(ICoreWebView2Environment* env, std::function<void(const std::string&)> onCookieFound) {
        if (!env) return;
        g_onCookieFound = onCookieFound;

        HINSTANCE hInstance = GetModuleHandle(NULL);
        WNDCLASSEXW wcex = {0};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProcBrowser;
        wcex.hInstance = hInstance;
        wcex.lpszClassName = L"WebView2LoginClass";

        RegisterClassExW(&wcex);

        g_hWnd = CreateWindowW(L"WebView2LoginClass", L"Roblox Login", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

        ShowWindow(g_hWnd, SW_SHOW);
        UpdateWindow(g_hWnd);

        env->CreateCoreWebView2Controller(g_hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                if (controller != nullptr) {
                    webviewController = controller;
                    webviewController->get_CoreWebView2(&webview);

                    RECT bounds;
                    GetClientRect(g_hWnd, &bounds);
                    webviewController->put_Bounds(bounds);

                                // Add NavigationCompleted listener to check for cookies
                                webview->add_NavigationCompleted(
                                    Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                        [](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
                                            
                                            // Get Cookie Manager
                                            ComPtr<ICoreWebView2_2> webview2_2;
                                            if (SUCCEEDED(sender->QueryInterface(IID_PPV_ARGS(&webview2_2)))) {
                                                ComPtr<ICoreWebView2CookieManager> cookieManager;
                                                webview2_2->get_CookieManager(&cookieManager);
                                                
                                                cookieManager->GetCookies(L"https://www.roblox.com", Callback<ICoreWebView2GetCookiesCompletedHandler>(
                                                    [](HRESULT result, ICoreWebView2CookieList* cookieList) -> HRESULT {
                                                        if (!cookieList) return S_OK;
                                                        UINT cookieCount;
                                                        cookieList->get_Count(&cookieCount);
                                                        for (UINT i = 0; i < cookieCount; ++i) {
                                                            ComPtr<ICoreWebView2Cookie> cookie;
                                                            cookieList->GetValueAtIndex(i, &cookie);
                                                            LPWSTR name;
                                                            cookie->get_Name(&name);
                                                            if (wcscmp(name, L".ROBLOSECURITY") == 0) {
                                                                LPWSTR value;
                                                                cookie->get_Value(&value);
                                                                
                                                                // Validate it's a real logged-in cookie
                                                                if (value != nullptr && wcsstr(value, L"_|WARNING") != nullptr) {
                                                                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, value, -1, NULL, 0, NULL, NULL);
                                                                    std::string strOut(size_needed, 0);
                                                                    WideCharToMultiByte(CP_UTF8, 0, value, -1, &strOut[0], size_needed, NULL, NULL);
                                                                    
                                                                    // Trim null terminator
                                                                    if (!strOut.empty() && strOut.back() == '\0') strOut.pop_back();

                                                                    if (g_onCookieFound) {
                                                                        g_onCookieFound(strOut);
                                                                    }
                                                                    
                                                                    CoTaskMemFree(name);
                                                                    CoTaskMemFree(value);
                                                                    
                                                                    // Close window automatically
                                                                    PostMessage(g_hWnd, WM_CLOSE, 0, 0);
                                                                    return S_OK;
                                                                }
                                                                CoTaskMemFree(value);
                                                            }
                                                            CoTaskMemFree(name);
                                                        }
                                                        return S_OK;
                                                    }).Get());
                                            }
                                            return S_OK;
                                        }).Get(), nullptr);

                                // Clear all existing cookies to force a fresh login
                                ComPtr<ICoreWebView2_2> webview2_2;
                                if (SUCCEEDED(webview->QueryInterface(IID_PPV_ARGS(&webview2_2)))) {
                                    ComPtr<ICoreWebView2CookieManager> cookieManager;
                                    webview2_2->get_CookieManager(&cookieManager);
                                    cookieManager->DeleteAllCookies();
                                }

                                webview->Navigate(L"https://www.roblox.com/login");
                }
                return S_OK;
            }).Get());

    }
}
