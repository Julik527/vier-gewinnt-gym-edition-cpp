/**
 * @file src/gui/GymAudioAssets.h
 * @author Julian Krauß
 * @date 2026-11-12 (Dokumentationsstand)
 * @par Projekt
 * Vier Gewinnt | Gym Edition (C++17)
 * @par Schule
 * Bernd-Blindow-Schule Ravensburg
 * @par Klasse
 * ITA25/9
 * @par Unterricht
 * Programmiertechnik (C++)
 * @par Fachlehrer
 * Dr. Litera
 * @brief Sucht das installierte Musikpaket und bestimmt dessen Variante.
 * @details Prüft acht erwartete .ogg-Dateien im EXE-/Build-Umfeld und erkennt die V4-PROFILE.txt.
 * @par Abhängigkeiten
 * C++17-Dateisystem und Dateiströme; ohne Audiohardware lauffähig.
 * @par Wichtige Randbedingungen
 * Die Prüfung des OggS-Dateikopfs ist kein vollständiger Decoder- oder SHA-256-Test.
 */

#pragma once
#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

namespace gymaudio {

inline constexpr std::array<const char*, 8> requiredOgg{{
    "airy.ogg", "hover.ogg", "pulse.ogg", "sector.ogg",
    "title.ogg", "transmission.ogg", "urgent.ogg", "victory.ogg"
}};

// Erkennungsmerkmal für das separat geprüfte V4-Audioprofil.
// Die Datei allein ersetzt KEINE SHA-256-Verifikation des Installers.
inline bool isGymPhonkV4(const std::filesystem::path& folder) {
    std::ifstream profile(folder / "PROFILE.txt");
    std::string marker;
    return static_cast<bool>(std::getline(profile, marker))
        && marker == "JK_AXIS_GYM_PHONK_V4";
}

struct AssetReport {
    std::filesystem::path folder;
    unsigned found = 0;
    bool complete() const { return found == requiredOgg.size(); }
};

// Schnellprüfung: Datei existiert, ist nicht winzig und beginnt mit OggS.
// Das ist KEIN vollständiger OGG-Decoder- oder Integritätstest.
inline bool isOgg(const std::filesystem::path& path) {
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec)
        || std::filesystem::file_size(path, ec) < 256 || ec)
        return false;
    std::ifstream input(path, std::ios::binary);
    char magic[4]{};
    return input.read(magic, sizeof(magic))
        && std::string(magic, sizeof(magic)) == "OggS";
}

inline AssetReport inspectFolder(std::filesystem::path folder) {
    AssetReport result{std::move(folder), 0};
    for (const auto* name : requiredOgg)
        if (isOgg(result.folder / name)) ++result.found;
    return result;
}

// Zuerst neben der EXE, danach in typischen CMake-/VS-Code-Verzeichnissen
// suchen. Nur 8/8 vorhandene OGG-Header erlauben den Audio-Start.
inline AssetReport inspectAssets(const std::filesystem::path& executable) {
    namespace fs = std::filesystem;
    std::error_code ec;
    const auto dir = fs::absolute(executable, ec).parent_path();
    const auto suffix = fs::path("assets/audio/sci-fi");
    const std::array<fs::path, 5> candidates{{
        dir / suffix,
        fs::current_path(ec) / suffix,
        dir.parent_path() / suffix,
        dir.parent_path().parent_path() / suffix,
        dir.parent_path().parent_path().parent_path() / suffix
    }};
    AssetReport nearest{candidates[0], 0};
    for (const auto& path : candidates) {
        auto report = inspectFolder(path);
        if (report.complete()) return report;
        if (report.found > nearest.found) nearest = std::move(report);
    }
    return nearest;
}

} // namespace gymaudio
