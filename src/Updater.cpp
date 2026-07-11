#include "Updater.h"
#include <windows.h>
#include <winhttp.h>
#include "../vendor/json.hpp"
#include <fstream>
#include <iostream>
#include <shellapi.h>
#include <vector>
#include <sstream>

using json = nlohmann::json;

std::string Updater::CURRENT_VERSION = "v1.0.4";

// Simple helper for WinHTTP GET
std::string GetHttpRaw(const std::wstring& domain, const std::wstring& path, const std::wstring& accept) {
    std::string result;
    HINTERNET hSession = WinHttpOpen(L"RoPilot/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return result;

    HINTERNET hConnect = WinHttpConnect(hSession, domain.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (hConnect) {
        LPCWSTR accepts[2] = { accept.c_str(), NULL };
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER, accepts, WINHTTP_FLAG_SECURE);
        
        if (hRequest) {
            if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
                if (WinHttpReceiveResponse(hRequest, NULL)) {
                    DWORD size = 0;
                    DWORD downloaded = 0;
                    do {
                        size = 0;
                        if (!WinHttpQueryDataAvailable(hRequest, &size)) break;
                        if (size == 0) break;
                        char* buf = new char[size + 1];
                        ZeroMemory(buf, size + 1);
                        if (WinHttpReadData(hRequest, (LPVOID)buf, size, &downloaded)) {
                            result.append(buf, downloaded);
                        }
                        delete[] buf;
                    } while (size > 0);
                }
            }
            WinHttpCloseHandle(hRequest);
        }
        WinHttpCloseHandle(hConnect);
    }
    WinHttpCloseHandle(hSession);
    return result;
}

std::vector<int> ParseVersion(std::string v) {
    if (!v.empty() && v[0] == 'v') v = v.substr(1);
    std::vector<int> parts;
    std::stringstream ss(v);
    std::string item;
    while (std::getline(ss, item, '.')) {
        try { parts.push_back(std::stoi(item)); } catch(...) { parts.push_back(0); }
    }
    return parts;
}

bool IsVersionNewer(const std::string& remote, const std::string& local) {
    auto r = ParseVersion(remote);
    auto l = ParseVersion(local);
    size_t len = r.size() > l.size() ? r.size() : l.size();
    for (size_t i = 0; i < len; ++i) {
        int r_val = (i < r.size()) ? r[i] : 0;
        int l_val = (i < l.size()) ? l[i] : 0;
        if (r_val > l_val) return true;
        if (r_val < l_val) return false;
    }
    return false;
}

std::string Updater::CheckForUpdate(std::string& outDownloadUrl) {
    outDownloadUrl = "";
    std::string resp = GetHttpRaw(L"api.github.com", L"/repos/NotVeen/RoPilot/releases/latest", L"application/vnd.github.v3+json");
    if (resp.empty()) return "";

    try {
        json j = json::parse(resp);
        std::string tag = j.value("tag_name", "");
        if (tag.empty() || !IsVersionNewer(tag, CURRENT_VERSION)) {
            return ""; // No update or same/older version
        }
        
        if (j.contains("assets") && j["assets"].is_array()) {
            for (auto& asset : j["assets"]) {
                std::string name = asset.value("name", "");
                if (name.find(".zip") != std::string::npos) {
                    outDownloadUrl = asset.value("browser_download_url", "");
                    
                    // Save changelog to file
                    std::string body = j.value("body", "");
                    std::ofstream clog("changelog.txt");
                    if (clog.is_open()) {
                        clog << body;
                        clog.close();
                    }
                    
                    return tag;
                }
            }
        }
    } catch (...) {
    }
    return "";
}

bool Updater::DownloadUpdate(const std::string& url, const std::string& destPath, void (*progressCallback)(int)) {
    // Basic download using WinHTTP but parsing URL to host/path
    // For simplicity, since github uses redirects, WinHttp automatically follows them by default.
    // Parse URL manually:
    std::string urlStr = url;
    if (urlStr.find("https://") == 0) urlStr = urlStr.substr(8);
    size_t slashPos = urlStr.find("/");
    if (slashPos == std::string::npos) return false;
    
    std::string host = urlStr.substr(0, slashPos);
    std::string path = urlStr.substr(slashPos);

    std::wstring wHost(host.begin(), host.end());
    std::wstring wPath(path.begin(), path.end());

    HINTERNET hSession = WinHttpOpen(L"RoPilot/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return false;

    // We must enable redirects explicitly if needed, but it's enabled by default.
    HINTERNET hConnect = WinHttpConnect(hSession, wHost.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    bool success = false;
    if (hConnect) {
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wPath.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
        if (hRequest) {
            // Enable auto-redirect handling
            DWORD dwOption = WINHTTP_OPTION_REDIRECT_POLICY_ALWAYS;
            WinHttpSetOption(hRequest, WINHTTP_OPTION_REDIRECT_POLICY, &dwOption, sizeof(dwOption));

            if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
                if (WinHttpReceiveResponse(hRequest, NULL)) {
                    
                    DWORD statusCode = 0;
                    DWORD dwSizeCode = sizeof(statusCode);
                    WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &dwSizeCode, WINHTTP_NO_HEADER_INDEX);
                    
                    // If we get a 302, WinHTTP should have followed it, so status should be 200.
                    if (statusCode == 200) {
                        DWORD contentLength = 0;
                        DWORD dwSize = sizeof(contentLength);
                        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &contentLength, &dwSize, WINHTTP_NO_HEADER_INDEX);
                        
                        std::ofstream file(destPath, std::ios::binary);
                        if (file.is_open()) {
                            DWORD totalDownloaded = 0;
                            DWORD size = 0;
                            DWORD downloaded = 0;
                            do {
                                size = 0;
                                if (!WinHttpQueryDataAvailable(hRequest, &size)) break;
                                if (size == 0) break;
                                char* buf = new char[size + 1];
                                if (WinHttpReadData(hRequest, (LPVOID)buf, size, &downloaded)) {
                                    file.write(buf, downloaded);
                                    totalDownloaded += downloaded;
                                    if (contentLength > 0 && progressCallback) {
                                        int percent = (int)(((double)totalDownloaded / contentLength) * 100);
                                        progressCallback(percent);
                                    }
                                }
                                delete[] buf;
                            } while (size > 0);
                            
                            file.close();
                            success = (totalDownloaded > 0);
                        }
                    }
                }
            }
            WinHttpCloseHandle(hRequest);
        }
        WinHttpCloseHandle(hConnect);
    }
    WinHttpCloseHandle(hSession);
    return success;
}

void Updater::ApplyUpdate(const std::string& zipPath) {
    std::string psScriptPath = "updater.ps1";
    std::ofstream file(psScriptPath);
    file << "param($zipPath)\n";
    file << "Start-Sleep -Seconds 2\n";
    file << "Expand-Archive -Force -Path $zipPath -DestinationPath .\n";
    file << "Start-Process 'RoPilot.exe' -ArgumentList '--updated'\n";
    file << "Remove-Item $zipPath -Force\n";
    file << "Remove-Item $MyInvocation.MyCommand.Path -Force\n";
    file.close();

    std::string cmd = "-NoProfile -ExecutionPolicy Bypass -File \"" + psScriptPath + "\" -zipPath \"" + zipPath + "\"";
    
    SHELLEXECUTEINFOA sei = { sizeof(sei) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = NULL;
    sei.lpVerb = "open";
    sei.lpFile = "powershell.exe";
    sei.lpParameters = cmd.c_str();
    sei.lpDirectory = NULL;
    sei.nShow = SW_HIDE;
    
    if (ShellExecuteExA(&sei)) {
        CloseHandle(sei.hProcess);
        ExitProcess(0);
    }
}

std::string Updater::GetLatestRobloxVersion() {
    static std::string cachedVersion = "";
    static DWORD lastFetchTime = 0;
    DWORD now = GetTickCount();

    // Cache for 30 minutes
    if (!cachedVersion.empty() && (now - lastFetchTime < 30 * 60 * 1000)) {
        return cachedVersion;
    }

    std::string resp = GetHttpRaw(L"clientsettingscdn.roblox.com", L"/v2/client-version/WindowsPlayer", L"application/json");
    if (resp.empty()) return cachedVersion;

    try {
        json j = json::parse(resp);
        std::string v = j.value("clientVersionUpload", "");
        if (!v.empty()) {
            cachedVersion = v;
            lastFetchTime = now;
        }
    } catch (...) {}
    
    return cachedVersion;
}

