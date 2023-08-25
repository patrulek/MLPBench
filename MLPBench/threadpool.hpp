#pragma once

#include <atomic>
#include <array>
#include <thread>

#include <processthreadsapi.h>
#include "cpuinfo.hpp"


void setThreadAffinity(const uint32_t cores, const uint32_t logical_cores, int32_t coreid) {
    auto handle{ GetCurrentThread() };
    while (coreid < 0) {
        coreid += cores;
    }
    
    std::bitset<64> mask;
    if (logical_cores == 2 * cores) {
        mask.set((2 * coreid) % logical_cores, true);
        mask.set((2 * coreid + 1) % logical_cores, true);
    } else {
        mask.set(coreid % cores, true);
    }
    SetThreadAffinityMask(handle, mask.to_ulong());
}

template<uint32_t Threads>
class ThreadPool {

public:
    explicit ThreadPool(const CPUThreadInfo& cpu_info)
        : cpu_info(cpu_info) {}

    // sets function for all threads
    template<typename Function, typename SpecificArg, typename ...Args>
    bool setupFunction(Function&& function, SpecificArg&& arg, Args&&... commons) {
        if (state != 0) {
            return false;
        }

        state = 1;
        std::atomic<uint32_t> counter{ 0 };

        for (uint32_t t = 0; t < threads.size(); ++t) {
            threads[t] = std::thread([&, tid = t, cores = cpu_info.cores, logical_cores = cpu_info.threads]() {
                // set thread affinity in reverse order from last to first core
                setThreadAffinity(cores, logical_cores, cores - 2 - tid);
                counter.fetch_add(1, std::memory_order_relaxed);
                while (state < 2);
                function(arg[tid], commons...);
            });
        }

        // wait for all threads to setup
        while (counter < Threads);
        return true;
    }

    void unlockThreads() {
        state = 2;
    }

    void join() {
        for (auto& thread : threads) {
            thread.join();
        }

        state = 0;
    }

private:

    std::array<std::thread, Threads> threads;
    std::atomic <uint32_t> state{ 0 };
    const CPUThreadInfo& cpu_info;
};
