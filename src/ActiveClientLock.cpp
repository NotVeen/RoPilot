#include "ActiveClientLock.h"
#include "Launcher.h"
#include <iostream>

namespace ActiveClientLock {

    HANDLE g_hClientLock = INVALID_HANDLE_VALUE;

    void LockClient() {
        if (g_hClientLock != INVALID_HANDLE_VALUE) {
            return; // Already locked
        }

        std::string robloxExe = Launcher::FindRobloxExecutable();
        if (robloxExe.empty()) {
            return; // Not found, nothing to lock
        }

        // Open handle with FILE_SHARE_READ but NOT FILE_SHARE_WRITE or FILE_SHARE_DELETE
        // This natively prevents any other process (like bootstrappers) from modifying or deleting it
        g_hClientLock = CreateFileA(
            robloxExe.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ, // Allow others to read (and execute), but deny write/delete
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        
        if (g_hClientLock == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to lock active client executable. Error: " << GetLastError() << "\n";
        }
    }

    void UnlockClient() {
        if (g_hClientLock != INVALID_HANDLE_VALUE) {
            CloseHandle(g_hClientLock);
            g_hClientLock = INVALID_HANDLE_VALUE;
        }
    }

}
