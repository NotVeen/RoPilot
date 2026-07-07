#pragma once

#include <windows.h>

class HandleCloser {
public:
    static void CloseRobloxHandles();
    static bool CloseProcessRobloxHandle(DWORD targetPid);
};
