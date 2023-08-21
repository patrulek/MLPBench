#pragma once

#include <Windows.h>
#include <expected>
#include <type_traits>

#include <bit>

bool ensureMemLockPrivilege() {
    HANDLE hToken{ nullptr };
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return false;
    }

    TOKEN_PRIVILEGES tp{};
    if (!LookupPrivilegeValueA(NULL, "SeLockMemoryPrivilege", &tp.Privileges[0].Luid)) {
        return false;
    }
    
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, 0)) {
        return false;
    }

    CloseHandle(hToken);
    return true;
}

template<typename T>
struct PagedMemory {
    T* ptr;
    uint64_t size;
    uint64_t alignment;
};

using win_error_t = std::invoke_result_t<decltype(GetLastError)>;

template<typename T, size_t Buffer_Size>
std::expected<PagedMemory<T>, win_error_t> allocWithLargePages() {
    static_assert(Buffer_Size > 0 && Buffer_Size);

    if (!ensureMemLockPrivilege()) {
        return std::unexpected(GetLastError());
    }

    // For some reason using VirtualAlloc2 with these parameters: https://stackoverflow.com/a/76448684
    // resulted in error 1450, but letting system to decide itself works just fine.
    auto ptr{ VirtualAlloc(NULL, Buffer_Size, MEM_RESERVE | MEM_COMMIT | MEM_LARGE_PAGES , PAGE_READWRITE) };
    if (ptr == nullptr) {
        return std::unexpected(GetLastError());
    }

    auto mem_alignment = GetLargePageMinimum();
    if (reinterpret_cast<uintptr_t>(ptr) % 1'073'741'824 == 0) {
        mem_alignment = 1'073'741'824;
    }

    return PagedMemory{
        .ptr = reinterpret_cast<T*>(ptr),
        .size = Buffer_Size / sizeof(T),
        .alignment = mem_alignment,
    };
}

template<typename T>
std::expected<bool, win_error_t> dealloc(PagedMemory<T> pm) {
    if (!VirtualFree(reinterpret_cast<LPVOID>(pm.ptr), 0, MEM_RELEASE)) {
        return std::unexpected(GetLastError());
    }
    
    return true;
}