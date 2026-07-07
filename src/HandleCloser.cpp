#include "HandleCloser.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <string>

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

#define SystemHandleInformation 16
#define ObjectNameInformation 1

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
    USHORT UniqueProcessId;
    USHORT CreatorBackTraceIndex;
    UCHAR ObjectTypeIndex;
    UCHAR HandleAttributes;
    USHORT HandleValue;
    PVOID Object;
    ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;

typedef struct _PUBLIC_OBJECT_NAME_INFORMATION {
    UNICODE_STRING Name;
    WCHAR NameBuffer[1];
} PUBLIC_OBJECT_NAME_INFORMATION, *PPUBLIC_OBJECT_NAME_INFORMATION;

typedef NTSTATUS(NTAPI *NtQuerySystemInformation_t)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

typedef NTSTATUS(NTAPI *NtQueryObject_t)(
    HANDLE Handle,
    ULONG ObjectInformationClass,
    PVOID ObjectInformation,
    ULONG ObjectInformationLength,
    PULONG ReturnLength
);

void HandleCloser::CloseRobloxHandles() {
    HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
    if (!hNtDll) return;
    
    NtQuerySystemInformation_t NtQuerySystemInformation = (NtQuerySystemInformation_t)GetProcAddress(hNtDll, "NtQuerySystemInformation");
    NtQueryObject_t NtQueryObject = (NtQueryObject_t)GetProcAddress(hNtDll, "NtQueryObject");
    
    if (!NtQuerySystemInformation || !NtQueryObject) return;

    // Find all Roblox processes
    std::vector<DWORD> robloxPids;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32W);
        if (Process32FirstW(hSnapshot, &pe32)) {
            do {
                if (_wcsicmp(pe32.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                    robloxPids.push_back(pe32.th32ProcessID);
                }
            } while (Process32NextW(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    
    if (robloxPids.empty()) return;

    ULONG handleInfoSize = 0x10000;
    PVOID handleInfo = malloc(handleInfoSize);
    
    NTSTATUS status;
    while ((status = NtQuerySystemInformation(SystemHandleInformation, handleInfo, handleInfoSize, NULL)) == 0xC0000004) {
        handleInfoSize *= 2;
        handleInfo = realloc(handleInfo, handleInfoSize);
    }
    
    if (!NT_SUCCESS(status)) {
        free(handleInfo);
        return;
    }
    
    PSYSTEM_HANDLE_INFORMATION pSysHandleInfo = (PSYSTEM_HANDLE_INFORMATION)handleInfo;
    
    for (ULONG i = 0; i < pSysHandleInfo->NumberOfHandles; i++) {
        SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = pSysHandleInfo->Handles[i];
        
        bool isRoblox = false;
        for (DWORD pid : robloxPids) {
            if (handle.UniqueProcessId == pid) {
                isRoblox = true;
                break;
            }
        }
        
        if (!isRoblox) continue;
        
        HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, handle.UniqueProcessId);
        if (!hProcess) continue;
        
        HANDLE dupHandle = NULL;
        if (DuplicateHandle(hProcess, (HANDLE)handle.HandleValue, GetCurrentProcess(), &dupHandle, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
            ULONG nameInfoSize = 0x1000;
            PVOID nameInfo = malloc(nameInfoSize);
            
            if (NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, nameInfo, nameInfoSize, NULL))) {
                PPUBLIC_OBJECT_NAME_INFORMATION pNameInfo = (PPUBLIC_OBJECT_NAME_INFORMATION)nameInfo;
                if (pNameInfo->Name.Buffer && pNameInfo->Name.Length > 0) {
                    std::wstring name(pNameInfo->Name.Buffer, pNameInfo->Name.Length / sizeof(WCHAR));
                    if (name.find(L"ROBLOX_singletonEvent") != std::wstring::npos) {
                        HANDLE nullHandle = NULL;
                        DuplicateHandle(hProcess, (HANDLE)handle.HandleValue, NULL, &nullHandle, 0, FALSE, DUPLICATE_CLOSE_SOURCE);
                    }
                }
            }
            free(nameInfo);
            CloseHandle(dupHandle);
        }
        CloseHandle(hProcess);
    }
    
    free(handleInfo);
}

bool HandleCloser::CloseProcessRobloxHandle(DWORD targetPid) {
    HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
    if (!hNtDll) return false;
    
    NtQuerySystemInformation_t NtQuerySystemInformation = (NtQuerySystemInformation_t)GetProcAddress(hNtDll, "NtQuerySystemInformation");
    NtQueryObject_t NtQueryObject = (NtQueryObject_t)GetProcAddress(hNtDll, "NtQueryObject");
    
    if (!NtQuerySystemInformation || !NtQueryObject) return false;

    ULONG handleInfoSize = 0x10000;
    PVOID handleInfo = malloc(handleInfoSize);
    
    NTSTATUS status;
    while ((status = NtQuerySystemInformation(SystemHandleInformation, handleInfo, handleInfoSize, NULL)) == 0xC0000004) {
        handleInfoSize *= 2;
        handleInfo = realloc(handleInfo, handleInfoSize);
    }
    
    if (!NT_SUCCESS(status)) {
        free(handleInfo);
        return false;
    }
    
    PSYSTEM_HANDLE_INFORMATION pSysHandleInfo = (PSYSTEM_HANDLE_INFORMATION)handleInfo;
    bool foundAndClosed = false;
    
    for (ULONG i = 0; i < pSysHandleInfo->NumberOfHandles; i++) {
        SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = pSysHandleInfo->Handles[i];
        
        if (handle.UniqueProcessId != targetPid) continue;
        
        HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, handle.UniqueProcessId);
        if (!hProcess) continue;
        
        HANDLE dupHandle = NULL;
        if (DuplicateHandle(hProcess, (HANDLE)handle.HandleValue, GetCurrentProcess(), &dupHandle, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
            ULONG nameInfoSize = 0x1000;
            PVOID nameInfo = malloc(nameInfoSize);
            
            if (NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, nameInfo, nameInfoSize, NULL))) {
                PPUBLIC_OBJECT_NAME_INFORMATION pNameInfo = (PPUBLIC_OBJECT_NAME_INFORMATION)nameInfo;
                if (pNameInfo->Name.Buffer && pNameInfo->Name.Length > 0) {
                    std::wstring name(pNameInfo->Name.Buffer, pNameInfo->Name.Length / sizeof(WCHAR));
                    if (name.find(L"ROBLOX_singletonEvent") != std::wstring::npos) {
                        HANDLE nullHandle = NULL;
                        DuplicateHandle(hProcess, (HANDLE)handle.HandleValue, NULL, &nullHandle, 0, FALSE, DUPLICATE_CLOSE_SOURCE);
                        foundAndClosed = true;
                    }
                }
            }
            free(nameInfo);
            CloseHandle(dupHandle);
        }
        CloseHandle(hProcess);
        if (foundAndClosed) break;
    }
    
    free(handleInfo);
    return foundAndClosed;
}
