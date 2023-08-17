#pragma once

#include <cstdlib>
#include <cstdint>

#include <algorithm>
#include <charconv>
#include <fstream>
#include <string>
#include <format>
#include <optional>
#include <unordered_set>
#include <string_view>

struct MemoryInfo {
    uint32_t channels{}; // Works as a multiplier.
    uint32_t speed{}; // In MT/s (equal to clock speed * 2)
    double bandwidth{}; // In GB/s. 
    std::string error{};

    // For greater robustness Win32_PhysicalMemory object should be used (https://learn.microsoft.com/en-us/windows/win32/cimwin32prov/win32-physicalmemory)
    // but for simplicity it is done by parsing output of `wmic` command.
    explicit MemoryInfo() {
        constexpr std::string_view tmp_file{ "tmp_file.out" };
        const auto cmd{ std::format("wmic memorychip get Speed, BankLabel > {:s}", tmp_file.data()) };
        
        // If command fails, no info about RAM could be retrieved
        // This command should produce output that consists of two columns, where first row is a header row
        // and subsequent rows are data rows which values are separated by double space
        if (std::system(cmd.c_str()) > 0) {
            error = "Memory info command execution failed";
            return;
        }

        // Read the output from the generated file
        std::ifstream input_file(tmp_file.data());
        if (!input_file.is_open()) {
            error = "Memory info data read failed ";
            std::remove(tmp_file.data());
            return;
        }

        // removes all but last '\0' chars; it seems that every other char is surrounded by NUL
        // trims right whitespaces
        const auto sanitize = [](std::string& s) {
            s.erase(std::remove(s.begin(), s.end(), '\0'), s.end());
            const auto pos{ s.find_last_not_of(" \r\t\n") };
            if (pos == std::string::npos) {
                return;
            }
            s = s.substr(0, pos + 1);
        };


        // cuts strings at double space and returns before and after string
        const auto cut = [](const std::string& s) -> std::optional<std::pair<std::string_view, std::string_view>> {
            const auto pos = s.find("  ");
            if (pos == std::string::npos) {
                return std::nullopt;
            }

            return std::make_pair(std::string_view{ s.begin(), s.begin() + pos }, std::string_view{ s.begin() + pos + 2, s.end() });
        };

        // Parse output
        std::unordered_set<std::string> unique_labels{};
        std::string line{};
        bool skipped{ false };
        uint32_t parsed_speed{};

        while (std::getline(input_file, line)) {
            // always skip header line
            if (!skipped) {
                skipped = true;
                continue;
            }

            // eof
            if (line.size() == 0 || line.size() == 1 && line[0] == '\0') {
                break;
            }

            sanitize(line);
            const auto splitted = cut(line);
            if (!splitted.has_value()) {
                error = "Parsing error";
                break;
            }

            const auto& [label, speedStr] { splitted.value() };
            unique_labels.insert(std::string{ label.begin(), label.end() });
            std::from_chars(speedStr.data(), speedStr.data() + speedStr.size(), parsed_speed);
        }

        channels = unique_labels.size(); // May not be the best way to determine # of channels, but works for me. Would need to test more.
        speed = parsed_speed;
        bandwidth = static_cast<double>(speed * channels * 8) / 1'000; /* Typical DataWidth is 64 bits thus `* 8`. */

        // Delete tmp file
        input_file.close();
        std::remove(tmp_file.data());
    }
};
