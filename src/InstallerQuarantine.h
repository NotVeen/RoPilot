#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <filesystem>

namespace InstallerQuarantine {
    // Moves RobloxPlayerInstaller.exe to a temporary location
    void QuarantineInstallers();

    // Restores the quarantined installers to their original locations
    void RestoreInstallers();
}
