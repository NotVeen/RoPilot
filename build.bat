@echo off
echo Building RoPilot...

:: Setup MSVC environment if cl.exe is not in PATH
where cl >nul 2>nul
if %errorlevel% neq 0 (
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars32.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
    )
)
if not exist "build" mkdir build

rc.exe /fo build\app.res res\app.rc

cl.exe /EHsc /MD /O2 /std:c++17 /DUNICODE /D_UNICODE /I vendor\webview2\include /I vendor src\main.cpp src\AccountManager.cpp src\SettingsManager.cpp src\Updater.cpp src\Launcher.cpp src\RobloxAPI.cpp src\BrowserLogin.cpp src\HandleCloser.cpp src\ActiveClientLock.cpp src\Optimizer.cpp src\DiscordRPC.cpp build\app.res /link winhttp.lib ole32.lib advapi32.lib shell32.lib user32.lib gdi32.lib crypt32.lib psapi.lib vendor\WebView2Loader.dll.lib /SUBSYSTEM:WINDOWS /OUT:RoPilot.exe /pdb:build\RoPilot.pdb

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

move *.obj build\ >nul 2>&1
echo Build successful!