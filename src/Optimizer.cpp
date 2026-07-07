#include "Optimizer.h"
#include "SettingsManager.h"
#include "Launcher.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_set>
#include <tlhelp32.h>
#include <psapi.h>
#include <intrin.h>

extern SettingsManager g_settingsManager;

namespace Optimizer {

    std::atomic<bool> g_Running{false};
    std::thread g_MonitorThread;
    DWORD g_LastFocusedPid = 0;
    std::unordered_set<DWORD> g_TrimmedPids;
    std::mutex g_TrimMutex;

    static DWORD LogicalCores() {
        DWORD len = 0;
        GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &len);
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) return 0;
        std::vector<BYTE> buffer(len);
        if (!GetLogicalProcessorInformationEx(RelationProcessorCore, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data()), &len))
            return 0;

        DWORD count = 0;
        BYTE* ptr = buffer.data();
        while (ptr < buffer.data() + len) {
            auto* core = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr);
#ifdef _WIN64
            count += static_cast<DWORD>(__popcnt64(core->Processor.GroupMask[0].Mask));
#else
            count += static_cast<DWORD>(__popcnt(core->Processor.GroupMask[0].Mask));
#endif
            ptr += core->Size;
        }
        return count;
    }

    static void SetEfficiencyMode(HANDLE hProcess, bool enable) {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hSnap == INVALID_HANDLE_VALUE) return;

        THREADENTRY32 te{};
        te.dwSize = sizeof(te);

        if (Thread32First(hSnap, &te)) {
            do {
                if (te.th32OwnerProcessID == GetProcessId(hProcess)) {
                    HANDLE hThread = OpenThread(THREAD_SET_INFORMATION, FALSE, te.th32ThreadID);
                    if (hThread) {
                        THREAD_POWER_THROTTLING_STATE tpts{};
                        tpts.Version = THREAD_POWER_THROTTLING_CURRENT_VERSION;
                        tpts.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
                        tpts.StateMask = enable ? THREAD_POWER_THROTTLING_EXECUTION_SPEED : 0;
                        SetThreadInformation(hThread, ThreadPowerThrottling, &tpts, sizeof(tpts));
                        CloseHandle(hThread);
                    }
                }
            } while (Thread32Next(hSnap, &te));
        }
        CloseHandle(hSnap);
    }

    void SetLowestPriority(HANDLE hProcess) {
        if (!hProcess) return;

        DWORD cores = LogicalCores();
        if (cores >= 12) {
            DWORD useCores = std::max<DWORD>(2, cores / 2);
            DWORD_PTR mask = 0;
            for (DWORD i = 0; i < useCores; ++i) mask |= (1ull << i);
            SetProcessAffinityMask(hProcess, mask);
        }

        SetPriorityClass(hProcess, IDLE_PRIORITY_CLASS);
        SetEfficiencyMode(hProcess, true);

        PROCESS_POWER_THROTTLING_STATE pt = {0};
        pt.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
        pt.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
        pt.StateMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
        SetProcessInformation(hProcess, ProcessPowerThrottling, &pt, sizeof(pt));
    }

    void SetHighestPriority(HANDLE hProcess) {
        if (!hProcess) return;

        DWORD cores = LogicalCores();
        if (cores >= 12) {
            DWORD_PTR processMask = 0, systemMask = 0;
            if (GetProcessAffinityMask(GetCurrentProcess(), &processMask, &systemMask)) {
                SetProcessAffinityMask(hProcess, systemMask);
            }
        }

        SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
        SetEfficiencyMode(hProcess, false);

        PROCESS_POWER_THROTTLING_STATE pt = {0};
        pt.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
        pt.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
        pt.StateMask = 0;
        SetProcessInformation(hProcess, ProcessPowerThrottling, &pt, sizeof(pt));
    }

    void TrimWorkingSet(HANDLE hProcess, DWORD pid) {
        std::lock_guard<std::mutex> lock(g_TrimMutex);
        if (g_TrimmedPids.find(pid) == g_TrimmedPids.end()) {
            SetProcessWorkingSetSize(hProcess, (SIZE_T)-1, (SIZE_T)-1);
            EmptyWorkingSet(hProcess);
            g_TrimmedPids.insert(pid);
        }
    }

    static void MonitorLoop() {
        while (g_Running) {
            if (g_settingsManager.GetSettings().ResourceOptimizer) {
                HWND hwnd = GetForegroundWindow();
                DWORD activePid = 0;
                if (hwnd) {
                    GetWindowThreadProcessId(hwnd, &activePid);
                }

                HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (hSnap != INVALID_HANDLE_VALUE) {
                    PROCESSENTRY32W pe{};
                    pe.dwSize = sizeof(pe);
                    if (Process32FirstW(hSnap, &pe)) {
                        do {
                            if (_wcsicmp(pe.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                                HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA | PROCESS_SET_INFORMATION | PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                                if (hProc) {
                                    if (pe.th32ProcessID == activePid) {
                                        if (g_LastFocusedPid != activePid) {
                                            SetHighestPriority(hProc);
                                            std::lock_guard<std::mutex> lock(g_TrimMutex);
                                            g_TrimmedPids.erase(pe.th32ProcessID);
                                        }
                                    } else {
                                        // It is in the background
                                        SetLowestPriority(hProc);
                                        TrimWorkingSet(hProc, pe.th32ProcessID);
                                    }
                                    CloseHandle(hProc);
                                }
                            }
                        } while (Process32NextW(hSnap, &pe));
                    }
                    CloseHandle(hSnap);
                }

                if (activePid != 0 && g_LastFocusedPid != activePid) {
                    g_LastFocusedPid = activePid;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }

    void Start() {
        if (!g_Running) {
            g_Running = true;
            g_MonitorThread = std::thread(MonitorLoop);
        }
    }

    void Stop() {
        g_Running = false;
        if (g_MonitorThread.joinable()) {
            g_MonitorThread.join();
        }
    }

} // namespace Optimizer
