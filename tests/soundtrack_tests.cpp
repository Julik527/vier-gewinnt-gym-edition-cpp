/**
 * @file tests/soundtrack_tests.cpp
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
 * @brief Prüft den früheren C++-Synthesizer ohne Audiohardware.
 * @details Kontrolliert Eigenschaften der von GymSoundtrack.h berechneten Stereo-PCM-Samples.
 * @par Abhängigkeiten
 * GymSoundtrack.h, C++17-Standardbibliothek; kein SFML erforderlich.
 * @par Wichtige Randbedingungen
 * Betrifft das synthetische Legacy-Thema, nicht die Wiedergabe der heutigen Gym-Phonk-V4-OGG-Dateien.
 */

#include "GymSoundtrack.h"
#include <algorithm>
#include <stdexcept>

#include <cmath>
#include <cstdint>
#include <iostream>

using namespace std;

static void require(bool passed, const char* message = "Audio invariant violated") {
    if (!passed) throw runtime_error(message);
}

int main() {
    using namespace gym::audio;
    const auto audio = renderTheme();
    const size_t frames = static_cast<size_t>(
        (static_cast<float>(bars) * 4.f * 60.f / bpm) * sampleRate);
    require(audio.size() == frames * channels);
    require(!audio.empty());
    require(audio.front() == 0);
    std::int64_t totalAbsolute = 0;
    std::int16_t peak = 0;
    bool stereo = false;
    for (size_t i = 0; i < audio.size(); i += channels) {
        const auto left = static_cast<int>(audio[i]);
        const auto right = static_cast<int>(audio[i + 1]);
        totalAbsolute += std::abs(left) + std::abs(right);
        peak = std::max(peak, static_cast<std::int16_t>(std::max(std::abs(left), std::abs(right))));
        stereo |= left != right;
    }
    const double avg = static_cast<double>(totalAbsolute) / audio.size();
    require(avg > 200.0 && avg < 15000.0);
    require(peak < 30000 && peak > 2000);
    require(stereo);
    // Avoid an audible click at the loop boundary.
    require(std::abs(static_cast<int>(audio[audio.size() - 2])) < 450);
    require(std::abs(static_cast<int>(audio.back())) < 450);
    // Synthesis must be deterministic: byte-for-byte same result.
    require(audio == renderTheme());
    cout << "Original stereo soundtrack: " << bars << " bars at "
              << bpm << " BPM, peak " << peak << ", mean " << avg
              << ", loop boundary OK.\n";
}
