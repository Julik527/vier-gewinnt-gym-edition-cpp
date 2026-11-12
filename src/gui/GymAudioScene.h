/**
 * @file src/gui/GymAudioScene.h
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
 * @brief Ordnet Spielphasen den OGG-Tracks und Lautstärkefaktoren zu.
 * @details Intro, Menü, Setup, Match, Endphase und Ergebnis werden über Scene und filename/gain ausgewählt.
 * @par Abhängigkeiten
 * Reines C++17; keine Fenster- oder Audiohardware nötig.
 * @par Wichtige Randbedingungen
 * Ab 75 Prozent belegter Felder wird Tense gewählt; die Zuordnung verändert keine Spielregeln.
 */

#pragma once

namespace gymaudio {

enum class Scene { Intro, Menu, Setup, Match, Tense, Results };

constexpr const char* filename(Scene scene) {
    switch (scene) {
        case Scene::Intro:   return "airy.ogg";
        case Scene::Menu:    return "title.ogg";
        case Scene::Setup:   return "sector.ogg";
        case Scene::Match:   return "pulse.ogg";
        case Scene::Tense:   return "urgent.ogg";
        case Scene::Results: return "transmission.ogg";
    }
    return "title.ogg";
}

// Richtwerte zum Angleich der ursprünglichen Sci-Fi-Tracks.
// Faktoren multiplizieren die vom Nutzer gewählte Lautstärke (0 bis 100).
constexpr float gain(Scene scene) {
    switch (scene) {
        case Scene::Intro:   return 0.48f;
        case Scene::Menu:    return 0.86f;
        case Scene::Setup:   return 0.35f;
        case Scene::Match:   return 0.78f;
        case Scene::Tense:   return 0.85f;
        case Scene::Results: return 0.45f;
    }
    return 0.75f;
}

// Eigenständige Pegelfaktoren für Gym Phonk V4. Sie beruhen auf der
// Dateianalyse und ersetzen keinen praktischen Hörtest.
constexpr float gain(Scene scene, bool gymPhonkV4) {
    if (!gymPhonkV4) return gain(scene);
    switch (scene) {
        case Scene::Intro:   return .62f;
        case Scene::Menu:    return .96f;
        case Scene::Setup:   return .61f;
        case Scene::Match:   return .66f;
        case Scene::Tense:   return .61f;
        case Scene::Results: return .63f;
    }
    return .65f;
}

// Ab 75 % belegter Zellen wechselt die Musik zur Spannungsphase.
// Die Schwelle hat keinerlei Einfluss auf Sieg- oder Zugregeln.
constexpr Scene gameplayScene(int occupied, int capacity) {
    return capacity > 0 && occupied >= 0 && occupied * 4 >= capacity * 3
        ? Scene::Tense : Scene::Match;
}

} // namespace gymaudio
