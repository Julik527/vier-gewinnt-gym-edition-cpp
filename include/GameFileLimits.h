// GYM1-Spielstände besitzen höchstens 10x11 Felder (4 Spieler).
// Die Grenze von 64 KiB ist eine defensive Obergrenze, nicht die erwartete
// Größe einer normalen Datei. So werden extreme Eingaben früh abgewiesen.
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
