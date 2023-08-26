#pragma once

// https://www.amd.com/system/files/TechDocs/42301_15h_Mod_00h-0Fh_BKDG.pdf
// 
// Values for Zen 3:
// MSR 0xc001102b (Combined Unit Configuration 3)
// 0x2000cc14 - disable prefetchers (default value: 0x2000CC15)
// 
// MSR 0xC0011022 Data Cache Configuration (DC_CFG)
// 0x40150A000 - disable hardware prefetches for software and disable DC prefetches (default value: 0x401500000)

#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <numeric>
#include <print>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "winring0/OlsDef.h"

using MSRAddr = uint64_t;
using MSRValue = uint64_t;
using MSRConfig = std::vector<std::pair<MSRAddr, MSRValue>>;

inline const std::unordered_map<std::string_view, MSRConfig> msr_configs{
    {
        "19h", {
            {0xC0011022, 0x40150A000},
            {0xc001102b, 0x2000cc14},
        },
    },
};


class MSREnvironment {
public:
    explicit MSREnvironment(const std::string_view cpu_model, const uint32_t threads)
        : cpu_model{ cpu_model }, threads{ threads } {
        if (!msr_configs.contains(cpu_model)) {
            std::println("{} CPU family not supported by benchmark", cpu_model);
            return;
        }

        winring0_dll = LoadLibraryA("WinRing0x64.dll");
        if (winring0_dll == nullptr) {
            std::println("Failed to load WinRing0x64.dll");
            return;
        }

        if (invokeDLL<bool>("InitializeOls") == false) {
            std::println("Failed to initialize WinRing0x64.dll");
            return;
        }

        if (const auto status = invokeDLL<DWORD>("GetDllStatus"); status != OLS_DLL_NO_ERROR) {
            std::println("Failed to initialize WinRing0x64.dll: error = {:d}", status);
            return;
        }

        // should never happen, but check anyway just in case
        if (!invokeDLL<bool>("IsMsr")) {
            throw std::runtime_error("MSR is not supported by current CPU");
        }
        
        if (!saveMSR()) {
            std::println("Failed to read MSR");
            return;
        }

        ok = true;
    }

    bool isOk() const {
        return ok;
    }

    void setMSR() {
        if (!ok) {
            return;
        }

        for (auto t = 0; t < threads; ++t) {
            auto tmask = 1 << t;
            SetThreadAffinityMask(GetCurrentThread(), tmask);

            for (auto [msr, value] : msr_configs.at(cpu_model)) {
                DWORD eax{ static_cast<DWORD>(value & 0xffffffff) };
                DWORD edx{ static_cast<DWORD>(value >> 32) };

                if (invokeDLL<bool, DWORD, DWORD, DWORD>("Wrmsr", msr, eax, edx) == false) {
                    std::println("Failed to set MSR");
                    ok = false;
                    return;
                }
            }
        }
    }

    void restoreMSR() {
        // for saved config
        auto step = saved_config.size() / threads;
        auto t = 0; 

        for (auto [msr, value] : saved_config) {
            auto tmask = 1 << (t++ / step);  
            SetThreadAffinityMask(GetCurrentThread(), tmask);

            DWORD eax{ static_cast<DWORD>(value & 0xffffffff) };
            DWORD edx{ static_cast<DWORD>(value >> 32) };

            if (invokeDLL<bool, DWORD, DWORD, DWORD>("Wrmsr", msr, eax, edx) == false) {
                throw std::runtime_error("Failed to restore MSR");
            }

            //std::println("MSR Register (0x{:x}) restored to 0x{:x}", msr, value);
        };
    }

    ~MSREnvironment() {
        restoreMSR();
        invokeDLL<void>("DeinitializeOls");

        if (winring0_dll != nullptr) {
            FreeLibrary(winring0_dll);
            winring0_dll = nullptr;
        }
    }
private:
    template <typename Ret, typename ...Args>
    Ret invokeDLL(const std::string_view fname, Args ...args) {
        using Function = Ret(*)(Args...);

        Function fptr = (Function)GetProcAddress(winring0_dll, fname.data());
        if (!fptr) {
            return Ret{};
        }

        if constexpr (std::is_same_v<void, Ret>) {
            fptr(args...);
        }
        else {
            const auto res = fptr(args...);
            return res;
        }
    }

    bool saveMSR() {
        constexpr auto all_mask = (std::numeric_limits<size_t>::max)(); // parentheses because of <Windows.h> max macro

        for (auto t = 0; t < threads; ++t) {
            auto tmask = 1 << t;
            SetThreadAffinityMask(GetCurrentThread(), tmask);

            for (auto [msr, _] : msr_configs.at(cpu_model)) {
                DWORD eax{}, edx{};

                if (invokeDLL<bool, DWORD, PDWORD, PDWORD>("Rdmsr", msr, &eax, &edx) == false) {
                    return false;
                }

                saved_config.emplace_back(msr, (static_cast<uint64_t>(edx) << 32) | eax);
                //std::println("MSR Register (0x{:x}) = 0x{:x}", msr, saved_config.back().second);
            }
        }

        return true;
    }

    std::vector<std::pair<MSRAddr, MSRValue>> saved_config{};
    HMODULE winring0_dll{ nullptr };
    std::string_view cpu_model{};
    bool ok{ false };
    uint32_t threads{ 0 };
};