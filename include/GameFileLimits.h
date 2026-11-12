// GYM1 game saves have a bounded schema and a maximum 110-cell board.
// Reject unexpectedly large files before formatted extraction allocates strings.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <system_error>

namespace gym {

inline bool validSaveFileSize(const std::string& path) {
    std::error_code error;
    const auto bytes = std::filesystem::file_size(
        std::filesystem::u8path(path), error);
    constexpr std::uintmax_t maxGym1Bytes = 64 * 1024;
    return !error && bytes > 0 && bytes <= maxGym1Bytes;
}

} // namespace gym
