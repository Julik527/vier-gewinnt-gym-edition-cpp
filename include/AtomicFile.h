/**
 * @file include/AtomicFile.h
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
 * @brief Schreibt Spielstand- und Highscore-Dateien über eine temporäre Datei.
 * @details writeSafely(name, writer) führt die Schreibfunktion aus, prüft den Stream und ersetzt danach das Ziel.
 * @par Abhängigkeiten
 * C++17-Dateisystem und unter Windows MoveFileExW; keine SFML-Abhängigkeit.
 * @par Wichtige Randbedingungen
 * Ein fehlgeschlagener Schreibvorgang liefert false. Kein Versprechen absoluter Stromausfallsicherheit.
 */

#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace gym {
template <typename Writer>
bool writeSafely(const std::string& name, Writer writer) {
    namespace fs = std::filesystem;
    // std::filesystem::u8path interpretiert auch Windows-Pfade mit Umlauten richtig.
    const fs::path target = fs::u8path(name);
    static std::atomic<unsigned long long> sequence{0};

    // Each attempt has a fresh timestamp, random nonce and in-process counter.
    // This avoids the deterministic .tmp collision between independent launches.
    fs::path temporary;
    bool available = false;
    for (int attempt = 0; attempt < 12; ++attempt) {
        temporary = target;
        temporary += "." + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count())
            + "." + std::to_string(std::random_device{}())
            + "." + std::to_string(sequence.fetch_add(1)) + ".tmp";
        std::error_code error;
        if (!fs::exists(temporary, error) && !error) {
            available = true;
            break;
        }
    }
    if (!available) return false;

    // Fehlgeschlagene Schreibversuche sollen keine *.tmp-Dateien zurücklassen.
    const auto cleanup = [&temporary]() {
        std::error_code ignored;
        fs::remove(temporary, ignored);
    };
    try {
        std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
        if (!output) return false;
        writer(output);
        output.flush();
        const bool good = static_cast<bool>(output);
        output.close();
        if (!good || !output) {
            cleanup();
            return false;
        }
    } catch (...) {
        cleanup();
        return false;
    }

#ifdef _WIN32
    // Windows: vorhandene Datei ersetzen; WRITE_THROUGH fordert zügiges Schreiben an.
    const bool success = MoveFileExW(temporary.c_str(), target.c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
    std::error_code error;
    // POSIX: Umbenennung im gleichen Dateisystem ersetzt die Zieldatei atomar.
    fs::rename(temporary, target, error);
    const bool success = !error;
#endif
    if (!success) cleanup();
    return success;
}
} // namespace gym
