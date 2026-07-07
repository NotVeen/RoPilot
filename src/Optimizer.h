#pragma once

#include <windows.h>
#include <vector>

namespace Optimizer {

    void Start();
    void Stop();

    // Utility functions for manual triggering
    void SetLowestPriority(HANDLE hProcess);
    void SetHighestPriority(HANDLE hProcess);

} // namespace Optimizer