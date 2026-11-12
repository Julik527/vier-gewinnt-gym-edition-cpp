/**
 * @file include/GameFileLimits.h
 * @brief Prüft eine defensive Dateigrößengrenze vor dem Laden von GYM1.
 * @details gym::validSaveFileSize kontrolliert Existenz und Größe per std::filesystem, bevor Textdaten eingelesen werden.
 * @par Abhängigkeiten
 * C++17-Standardbibliothek; verwendet von Game und MatchSession.
 * @par Wichtige Randbedingungen
 * Nur Dateien mit 1 bis 65536 Bytes werden zugelassen; die Inhaltsprüfung erfolgt erst beim Parsen.
 */

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
