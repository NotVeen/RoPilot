@echo off
echo Building Multi-Roblox Account Manager...
if not exist "build" mkdir build

rc.exe /fo build\app.res res\app.rc

cl.exe /EHsc /MD /O2 /std:c++17 /DUNICODE /D_UNICODE /I vendor\webview2\include /I vendor src\main.cpp src\AccountManager.cpp src\Launcher.cpp src\RobloxAPI.cpp src\BrowserLogin.cpp src\HandleCloser.cpp build\app.res /link winhttp.lib ole32.lib advapi32.lib shell32.lib user32.lib gdi32.lib crypt32.lib vendor\WebView2Loader.dll.lib /SUBSYSTEM:WINDOWS /OUT:RoPilot.exe /pdb:build\RoPilot.pdb

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

move *.obj build\ >nul 2>&1
echo Build successful!