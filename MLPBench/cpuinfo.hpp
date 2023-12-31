#pragma once

#include <vector>
#include <bitset>
#include <array>
#include <intrin.h>
#include <thread>
#include <string>
#include <expected>

// Based on: https://github.com/cklutz/mcoreinfo/blob/master/sysinfo/CpuCapabilities.cs
// and https://learn.microsoft.com/pl-pl/cpp/intrinsics/cpuid-cpuidex?view=msvc-170
class InstructionSet
{
    // forward declarations
    class InstructionSet_Internal;

public:
    // Hyperthreading Enabled
    static bool HTT() { return CPU_Rep.f_1_EDX_[28]; };
    static std::string Model() { return CPU_Rep.model_; };
    static std::string Family() { return CPU_Rep.family_; };

private:
    static const InstructionSet_Internal CPU_Rep;

    class InstructionSet_Internal
    {
    public:
        InstructionSet_Internal()
            : f_1_EDX_{ 0 }, data_{}, extdata_{}, model_{}
        {
            //int cpuInfo[4] = {-1};
            std::array<int, 4> cpui{};

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
            __cpuid(cpui.data(), 0);
            auto nIds_{ cpui[0] };

            for (int i = 0; i <= nIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                data_.push_back(cpui);
            }

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1)
            {
                f_1_EDX_ = data_[1][3];
                auto eax = data_[1][0];
                auto raw_family = (eax >> 8) & ((1 << 4) - 1);
                auto raw_exfamily = (eax >> 20) & ((1 << 8) - 1);
                family_ = std::format("{:x}h", raw_family + raw_exfamily);
            }

            // Calling __cpuid with 0x80000000 as the function_id argument
            // gets the number of the highest valid extended ID.
            cpui.fill(0);
            __cpuid(cpui.data(), 0x80000000);
            nIds_ = cpui[0];

            char model[0x40];
            memset(model, 0, sizeof(model));

            for (int i = 0x80000000; i <= nIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                extdata_.push_back(cpui);
            }

            // Interpret CPU model string if reported
            if (nIds_ >= 0x80000004)
            {
                memcpy(model, extdata_[2].data(), sizeof(cpui));
                memcpy(model + 16, extdata_[3].data(), sizeof(cpui));
                memcpy(model + 32, extdata_[4].data(), sizeof(cpui));
                model_ = model;
                if (auto pos = model_.find_last_not_of(" \0"); pos != std::string::npos) {
                    model_ = model_.substr(0, pos + 1);
                }
            }
        };

        std::bitset<32> f_1_EDX_;
        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;
        std::string model_;
        std::string family_;
    };
};

// Initialization
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

struct CPUThreadInfo {
    uint32_t cores{ 0 };
    uint32_t threads{ 0 };
    bool htt{ false };
    std::string model{};
    std::string family{};

    explicit CPUThreadInfo()
        : cores(InstructionSet::HTT() ? std::thread::hardware_concurrency() / 2 : std::thread::hardware_concurrency()),
        threads(std::thread::hardware_concurrency()),
        htt(InstructionSet::HTT()),
        model(InstructionSet::Model()),
        family(InstructionSet::Family()) {}

    std::expected<std::pair<uint32_t, uint32_t>, bool> logicalCoresIdxs(const uint32_t phys_core_idx) {
        if (phys_core_idx > cores) {
            return std::unexpected(false);
        }

        // For a hyperthreaded system with physical cores 0, 1, 2, �, N - 1 and logical CPUs 0, 1, 2, �, 2N - 1
        // the physical core with number j is mapped to the logical CPUs with numbers j and j + N
        // https://stackoverflow.com/questions/423761/mapping-logical-processors-to-physical-processors#comment108917251_7324224
        return std::make_pair(phys_core_idx, phys_core_idx + cores);
    }
};