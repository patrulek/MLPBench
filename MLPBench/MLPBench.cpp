#include <algorithm>
#include <array>
#include <chrono>
#include <random>
#include <vector>
#include <span>
#include <print>
#include <thread>

#include "memoryinfo.hpp"
#include "multithreading.hpp"
#include "virtualmem.hpp"

constexpr size_t Memory_Size{ 2'147'483'648 }; // 2GB
constexpr size_t Elem_Count{ Memory_Size / sizeof(uint64_t) };
constexpr size_t Min_Elem{ 1 };
constexpr size_t Max_Elem{ Elem_Count - 1 };
constexpr uint64_t Array_Sum{ (Min_Elem + Max_Elem) / 2 * (Elem_Count - 1) };
double bwLane1{ 0 };
double bwThreads1{ 0 };
double bwLinear{ 0 };

template <uint64_t Threads = 1, uint64_t Lanes = 1>
void multithreadedRandomAccessSum(std::array<uint64_t, 65>& sums, std::span<uint64_t> memory) {
    static_assert(Threads == 1 || (Threads >= 2 && Threads <= 64 && Threads % 2 == 0));
    static_assert(Lanes == 1 || (Lanes >= 2 && Lanes <= 64 && Lanes % 2 == 0));

    std::vector<std::array<uint64_t, 65>> sums_per_thread;
    sums_per_thread.resize(Threads);

    // setup starting indexes
    uint64_t initial_idx{ 0 };
    const auto thread_size{ memory.size() / Threads };
    const auto step{ memory.size() / Lanes / Threads };
    for (uint32_t thread_id = 0; thread_id < Threads; ++thread_id) {
        uint32_t sums_idx  = 1;
        sums_per_thread[thread_id][sums_idx] = initial_idx;

        for ( sums_idx = sums_idx + 1; sums_idx <= Lanes; ++sums_idx) {
            for (uint32_t j = 0; j < step; ++j) {
                initial_idx = memory[initial_idx];
            }
            sums_per_thread[thread_id][sums_idx] = initial_idx;
        }
    }

    std::array<std::thread, Threads> pool{};

    const auto start_time = std::chrono::high_resolution_clock::now();
    const auto foonc = [&step](std::array<uint64_t, 65>& sums, std::span<uint64_t> memory) {
        for (uint32_t i = 0; i < step; ++i) {
            if constexpr (Lanes > 32) {
                sums[33] = memory[sums[33]];
                sums[34] = memory[sums[34]];
                sums[35] = memory[sums[35]];
                sums[36] = memory[sums[36]];
                sums[37] = memory[sums[37]];
                sums[38] = memory[sums[38]];
                sums[39] = memory[sums[39]];
                sums[40] = memory[sums[40]];
                sums[41] = memory[sums[41]];
                sums[42] = memory[sums[42]];
                sums[43] = memory[sums[43]];
                sums[44] = memory[sums[44]];
                sums[45] = memory[sums[45]];
                sums[46] = memory[sums[46]];
                sums[47] = memory[sums[47]];
                sums[48] = memory[sums[48]];
                sums[49] = memory[sums[49]];
                sums[50] = memory[sums[50]];
                sums[51] = memory[sums[51]];
                sums[52] = memory[sums[52]];
                sums[53] = memory[sums[53]];
                sums[54] = memory[sums[54]];
                sums[55] = memory[sums[55]];
                sums[56] = memory[sums[56]];
                sums[57] = memory[sums[57]];
                sums[58] = memory[sums[58]];
                sums[59] = memory[sums[59]];
                sums[60] = memory[sums[60]];
                sums[61] = memory[sums[61]];
                sums[62] = memory[sums[62]];
                sums[63] = memory[sums[63]];
                sums[64] = memory[sums[64]];
            }

            if constexpr (Lanes > 16) {
                sums[17] = memory[sums[17]];
                sums[18] = memory[sums[18]];
                sums[19] = memory[sums[19]];
                sums[20] = memory[sums[20]];
                sums[21] = memory[sums[21]];
                sums[22] = memory[sums[22]];
                sums[23] = memory[sums[23]];
                sums[24] = memory[sums[24]];
                sums[25] = memory[sums[25]];
                sums[26] = memory[sums[26]];
                sums[27] = memory[sums[27]];
                sums[28] = memory[sums[28]];
                sums[29] = memory[sums[29]];
                sums[30] = memory[sums[30]];
                sums[31] = memory[sums[31]];
                sums[32] = memory[sums[32]];
            }

            if constexpr (Lanes > 8) {
                sums[9] = memory[sums[9]];
                sums[10] = memory[sums[10]];
                sums[11] = memory[sums[11]];
                sums[12] = memory[sums[12]];
                sums[13] = memory[sums[13]];
                sums[14] = memory[sums[14]];
                sums[15] = memory[sums[15]];
                sums[16] = memory[sums[16]];
            }

            if constexpr (Lanes > 4) {
                sums[5] = memory[sums[5]];
                sums[6] = memory[sums[6]];
                sums[7] = memory[sums[7]];
                sums[8] = memory[sums[8]];
            }

            if constexpr (Lanes > 2) {
                sums[3] = memory[sums[3]];
                sums[4] = memory[sums[4]];
            }

            if constexpr (Lanes > 1) {
                sums[2] = memory[sums[2]];
            }

            sums[1] = memory[sums[1]];
        }
    };

    // start all threads
    for (uint32_t t = 1; t < Threads; ++t) {
        pool[t] = std::thread(foonc, std::ref(sums_per_thread[t]), memory);
    }

    foonc(sums_per_thread[0], memory);

    // join all threads
    for (uint32_t t = 1; t < Threads; ++t) {
        pool[t].join();
    }

    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0;
    const auto bandwidth = Memory_Size / elapsed / 1'000'000.0;
    if (Threads == 1 && Lanes == 1) {
        bwLane1 = bandwidth;
    }
    const auto speedup = bandwidth / bwLane1;
    const auto nshit = 1'000'000'000 * elapsed / Elem_Count;
    const auto eff = 100.0 * bandwidth / bwLinear;

    std::println("{:14d}     {:16d}     {:8.3f}     {:9.1f}     {:7.2f}     {:6.1f}     {:9.1f}", Threads, Lanes, elapsed, bandwidth, speedup, nshit, eff);

    for (uint32_t t = 0; t < Threads; ++t) {
        for (uint32_t l = 1; l <= Lanes; ++l) {
            sums[l] += sums_per_thread[t][l];
        }
    }
}

void sattolo(std::span<uint64_t, Elem_Count> memory) {
    std::random_device rd;
    std::mt19937_64 g(rd());

    auto i{ Elem_Count };
    while (i-- > 1) {
        std::uniform_int_distribution<uint64_t> dist{ 0, i - 1 };
        const uint64_t swap_idx{ dist(g) };
        std::swap(memory[i], memory[swap_idx]);
    }
}

// Globals
MemoryInfo mem_info{};
CPUThreadInfo cpu_info{};

void calculateSum(uint64_t& total_sum, const std::span<uint64_t> data) {
    uint64_t sum{ 0 };
    for (const auto& v : data) {
        sum += v;
    }

    total_sum += sum;
}

template<uint32_t Rounds, uint32_t Threads = 1>
uint64_t sequentialAccessSum(std::span<uint64_t, Elem_Count> memory) {
    static_assert(Threads == 1 || (Threads >= 2 && Threads <= 64 && Threads % 2 == 0));

    std::array<std::thread, Threads> pool{};
    constexpr uint64_t step{ Elem_Count / Threads };
    constexpr uint64_t Total_Bytes{ Rounds * Memory_Size };

    auto start_time{ std::chrono::high_resolution_clock::now() };
    uint64_t sum{ 0 };
    for (uint32_t r = 0; r < Rounds; ++r) {
        sum = 0;
        
        // start all threads
        for (uint32_t t = 1; t < Threads; ++t) {
            pool[t] = std::thread(calculateSum, std::ref(sum), memory.subspan(t * step, step));
        }

        calculateSum(sum, memory.subspan(0, step));

        // join all threads
        for (uint32_t t = 1; t < Threads; ++t) {
            pool[t].join();
        }
    }
    auto end_time{ std::chrono::high_resolution_clock::now() };
    auto elapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0 };
    auto bandwidth{ Total_Bytes / elapsed / 1'000'000.0 };
    bwLinear = std::max<double>(bandwidth, bwLinear);

    if (Threads == 1) {
        bwThreads1 = bandwidth;
    }
    const auto speedup = bandwidth / bwThreads1;
    const auto nshit = 1'000'000'000 * elapsed / Elem_Count / Rounds;
    const auto eff = 100.0 * bandwidth / (mem_info.bandwidth * 1'000);

    std::println("{:14d}     {:8.3f}     {:9.1f}     {:7.2f}     {:6.3f}     {:9.1f}", Threads, elapsed, bandwidth, speedup, nshit, eff);
    return sum;
}

int main()
{
    const auto bstart_time{ std::chrono::high_resolution_clock::now() };

    // Print system info
    if (!mem_info.error.empty()) {
        std::println("Couldn't retrieve memory info due to: {:s}", mem_info.error);
    } else {
        std::println("RAM Info:\n * Channels: {:d}\n * Speed [MT/s]: {:d}\n * Theoretical bandwidth: {:.1f}GB/s\n", mem_info.channels, mem_info.speed, mem_info.bandwidth);
    }

    std::println("CPU Info:\n * Physical cores: {:d}\n * Logical cores: {:d}\n * Hyperthreading: {:s}\n", cpu_info.cores, cpu_info.threads, cpu_info.htt);

    // Allocate virtual memory
    std::println("Allocating non-paged (locked) memory of size {:d} bytes with system decided page sizes...", Memory_Size);
    auto data{ allocWithLargePages<uint64_t, Memory_Size>() };
    if (!data.has_value()) {
        std::println("Memory incorrectly allocated: error = {:d}", data.error());
        std::exit(-1);
    }

    std::println("Memory aligned to {:d} bytes: data pointer = 0x{:x}", data.value().alignment, reinterpret_cast<uintptr_t>(data.value().ptr));

    std::span<uint64_t, Elem_Count> data_span(data.value().ptr, data.value().size);

    // Allocate aligned memory
    std::println("Allocating array made of {:d} ({:d} MB) 64-bit words...", Elem_Count, Memory_Size / 1024 / 1024);
    std::span<uint64_t, Elem_Count> memory{ data_span };
    const auto ptr0{ reinterpret_cast<uintptr_t>(&memory[0]) };
    const auto ptr1{ reinterpret_cast<uintptr_t>(&memory[1]) };

    if (ptr0 % data.value().alignment != 0 || ptr1 % data.value().alignment != sizeof(uint64_t)) {
        std::println("Memory incorrectly aligned: &[0] = 0x{:x}, &[1] = 0x{:x}", ptr0, ptr1);
        std::exit(-1);
    }

    // Initialize and shuffle
    std::println("Initializing array...");

    for (int i = 0; i < Elem_Count; ++i) {
        memory[i] = i;
    }

    sattolo(memory);

    // Veryfing data
    std::println("Verifying sequential and random access sums...");
    uint64_t v{ 12312 };
    std::array<uint64_t, 2>test_sum{};
    for (uint32_t i = 0; i < Elem_Count; ++i) {
        test_sum[0] += memory[i];
        test_sum[1] += memory[v];
        v = memory[v];
    }
    if (test_sum[0] != test_sum[1]) {
        std::println("Sequential and random access sum mismatch: {:d} != {:d}", test_sum[0], test_sum[1]);
        std::exit(-1);
    }

    // Calculating bandwidth for sequential access
    constexpr uint32_t Rounds{ 64 };
    constexpr uint32_t Max_Lanes{ 64 };
    std::println("\nCalculating time for sequential access ({:d} rounds)...", Rounds);
    std::println("--------------------------------------------------------------------------------");
    std::println("- # of threads --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -");
    std::println("--------------------------------------------------------------------------------");

    std::array<uint64_t, Max_Lanes + 1> sums{};
    sums[1] = sequentialAccessSum<Rounds, 1>(memory);
    sums[2] = sequentialAccessSum<Rounds, 2>(memory);
    sums[4] = sequentialAccessSum<Rounds, 4>(memory);
    sums[8] = sequentialAccessSum<Rounds, 8>(memory);
    sums[16] = sequentialAccessSum<Rounds, 16>(memory);
    sums[32] = sequentialAccessSum<Rounds, 32>(memory);
    sums[64] = sequentialAccessSum<Rounds, 64>(memory);

    // Calculating bandwidth for random access
    std::println("\nCalculating time for random access (single round)...");
    std::println("-----------------------------------------------------------------------------------------------------");
    std::println("- # of threads --- lanes per thread --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -");
    std::println("-----------------------------------------------------------------------------------------------------");

    // one thread lanes
    multithreadedRandomAccessSum<1, 1>(sums, memory);
    multithreadedRandomAccessSum<1, 2>(sums, memory);
    multithreadedRandomAccessSum<1, 4>(sums, memory);
    multithreadedRandomAccessSum<1, 8>(sums, memory);
    multithreadedRandomAccessSum<1, 16>(sums, memory);
    multithreadedRandomAccessSum<1, 32>(sums, memory);
    multithreadedRandomAccessSum<1, 64>(sums, memory);
    std::println("= ------------------------------------------------------------------------------------------------- =");

    // two thread lanes
    multithreadedRandomAccessSum<2, 1>(sums, memory);
    multithreadedRandomAccessSum<2, 2>(sums, memory);
    multithreadedRandomAccessSum<2, 4>(sums, memory);
    multithreadedRandomAccessSum<2, 8>(sums, memory);
    multithreadedRandomAccessSum<2, 16>(sums, memory);
    multithreadedRandomAccessSum<2, 32>(sums, memory);
    multithreadedRandomAccessSum<2, 64>(sums, memory);
    std::println("= ------------------------------------------------------------------------------------------------- =");
    
    // four thread lanes
    multithreadedRandomAccessSum<4, 1>(sums, memory);
    multithreadedRandomAccessSum<4, 2>(sums, memory);
    multithreadedRandomAccessSum<4, 4>(sums, memory);
    multithreadedRandomAccessSum<4, 8>(sums, memory);
    multithreadedRandomAccessSum<4, 16>(sums, memory);
    multithreadedRandomAccessSum<4, 32>(sums, memory);
    multithreadedRandomAccessSum<4, 64>(sums, memory);
    std::println("= ------------------------------------------------------------------------------------------------- =");

    // eight thread lanes
    multithreadedRandomAccessSum<8, 1>(sums, memory);
    multithreadedRandomAccessSum<8, 2>(sums, memory);
    multithreadedRandomAccessSum<8, 4>(sums, memory);
    multithreadedRandomAccessSum<8, 8>(sums, memory);
    multithreadedRandomAccessSum<8, 16>(sums, memory);
    multithreadedRandomAccessSum<8, 32>(sums, memory);
    multithreadedRandomAccessSum<8, 64>(sums, memory);
    std::println("= ------------------------------------------------------------------------------------------------- =");

    // sixteen thread lanes
    multithreadedRandomAccessSum<16, 1>(sums, memory);
    multithreadedRandomAccessSum<16, 2>(sums, memory);
    multithreadedRandomAccessSum<16, 4>(sums, memory);
    multithreadedRandomAccessSum<16, 8>(sums, memory);
    multithreadedRandomAccessSum<16, 16>(sums, memory);
    multithreadedRandomAccessSum<16, 32>(sums, memory);
    multithreadedRandomAccessSum<16, 64>(sums, memory);
    std::println("= ------------------------------------------------------------------------------------------------- =");

    // thirty-two thread lanes
    multithreadedRandomAccessSum<32, 1>(sums, memory);
    multithreadedRandomAccessSum<32, 2>(sums, memory);
    multithreadedRandomAccessSum<32, 4>(sums, memory);
    multithreadedRandomAccessSum<32, 8>(sums, memory);
    multithreadedRandomAccessSum<32, 16>(sums, memory);
    multithreadedRandomAccessSum<32, 32>(sums, memory);
    multithreadedRandomAccessSum<32, 64>(sums, memory);
    std::println("= ------------------------------------------------------------------------------------------------- =");

    // sixty-four thread lanes
    multithreadedRandomAccessSum<64, 1>(sums, memory);
    multithreadedRandomAccessSum<64, 2>(sums, memory);
    multithreadedRandomAccessSum<64, 4>(sums, memory);
    multithreadedRandomAccessSum<64, 8>(sums, memory);
    multithreadedRandomAccessSum<64, 16>(sums, memory);
    multithreadedRandomAccessSum<64, 32>(sums, memory);
    multithreadedRandomAccessSum<64, 64>(sums, memory);

    uint64_t aggr{ 0 };
    for (const auto s : sums)
        aggr += s;

    dealloc(data.value());

    const auto bend_time{ std::chrono::high_resolution_clock::now() };
    std::println("\nTotal benchmark time: {:.3f}s", std::chrono::duration_cast<std::chrono::milliseconds>(bend_time - bstart_time).count() / 1000.0);

    return static_cast<int>(aggr);
}
