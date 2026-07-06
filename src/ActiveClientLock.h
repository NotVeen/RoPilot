#pragma once

#include <windows.h>

namespace ActiveClientLock {
    // Finds the current RobloxPlayerBeta.exe and locks it from being modified
    void LockClient();

    // Releases the lock on the Roblox executable
    void UnlockClient();
}
