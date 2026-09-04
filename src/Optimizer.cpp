#include "Optimizer.h"
#include "SettingsManager.h"
#include "Launcher.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_set>
#include <unordered_map>
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
    std::unordered_map<DWORD, HANDLE> g_JobObjects;
    std::mutex g_JobMutex;

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

        // Use BELOW_NORMAL_PRIORITY_CLASS to allow background instances to keep alive network & UI
        // without starving foreground applications. Never use IDLE_PRIORITY_CLASS for online games.
        SetPriorityClass(hProcess, BELOW_NORMAL_PRIORITY_CLASS);
    }

    void SetHighestPriority(HANDLE hProcess) {
        if (!hProcess) return;

        DWORD_PTR processMask = 0, systemMask = 0;
        if (GetProcessAffinityMask(hProcess, &processMask, &systemMask)) {
            SetProcessAffinityMask(hProcess, systemMask);
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
        // Do NOT call EmptyWorkingSet: it forces all texture buffers and Luau VM pages
        // to disk/pagefile, purging in-game UI textures, breaking buttons, and causing severe page faults.
    }

    void SetProcessCpuLimit(HANDLE hProcess, DWORD pid, int limitPercent) {
        std::lock_guard<std::mutex> lock(g_JobMutex);
        HANDLE hJob = NULL;
        auto it = g_JobObjects.find(pid);
        if (it == g_JobObjects.end()) {
            hJob = CreateJobObjectW(NULL, NULL);
            if (hJob) {
                if (AssignProcessToJobObject(hJob, hProcess)) {
                    g_JobObjects[pid] = hJob;
                } else {
                    CloseHandle(hJob);
                    hJob = NULL;
                }
            }
        } else {
            hJob = it->second;
        }

        if (hJob) {
            JOBOBJECT_CPU_RATE_CONTROL_INFORMATION jcrc{};
            jcrc.ControlFlags = JOB_OBJECT_CPU_RATE_CONTROL_ENABLE | JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP;
            jcrc.CpuRate = limitPercent * 100; // 10000 is 100%
            if (jcrc.CpuRate > 10000) jcrc.CpuRate = 10000;
            if (jcrc.CpuRate < 100) jcrc.CpuRate = 100;
            
            SetInformationJobObject(hJob, JobObjectCpuRateControlInformation, &jcrc, sizeof(jcrc));
        }
    }

    void CleanupJobs() {
        std::lock_guard<std::mutex> lock(g_JobMutex);
        for (auto it = g_JobObjects.begin(); it != g_JobObjects.end(); ) {
            DWORD exitCode;
            HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, it->first);
            if (hProc) {
                if (GetExitCodeProcess(hProc, &exitCode) && exitCode != STILL_ACTIVE) {
                    CloseHandle(it->second);
                    it = g_JobObjects.erase(it);
                } else {
                    ++it;
                }
                CloseHandle(hProc);
            } else {
                CloseHandle(it->second);
                it = g_JobObjects.erase(it);
            }
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
                                            SetProcessCpuLimit(hProc, pe.th32ProcessID, 100);
                                            std::lock_guard<std::mutex> lock(g_TrimMutex);
                                            g_TrimmedPids.erase(pe.th32ProcessID);
                                        }
                                    } else {
                                        // Check process uptime: grant startup grace period (35s) so assets and UI load smoothly
                                        bool isStartingUp = false;
                                        FILETIME ftCreation{}, ftExit{}, ftKernel{}, ftUser{};
                                        if (GetProcessTimes(hProc, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
                                            FILETIME ftNow{};
                                            GetSystemTimeAsFileTime(&ftNow);
                                            ULARGE_INTEGER createTime, nowTime;
                                            createTime.LowPart = ftCreation.dwLowDateTime;
                                            createTime.HighPart = ftCreation.dwHighDateTime;
                                            nowTime.LowPart = ftNow.dwLowDateTime;
                                            nowTime.HighPart = ftNow.dwHighDateTime;

                                            uint64_t uptimeSec = (nowTime.QuadPart - createTime.QuadPart) / 10000000ULL;
                                            if (uptimeSec < 35) {
                                                isStartingUp = true;
                                            }
                                        }

                                        if (isStartingUp) {
                                            SetProcessCpuLimit(hProc, pe.th32ProcessID, 100);
                                        } else {
                                            SetLowestPriority(hProc);
                                            SetProcessCpuLimit(hProc, pe.th32ProcessID, g_settingsManager.GetSettings().CpuLimiter ? g_settingsManager.GetSettings().BackgroundCpuLimit : 100);
                                            TrimWorkingSet(hProc, pe.th32ProcessID);
                                        }
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
            CleanupJobs();

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
