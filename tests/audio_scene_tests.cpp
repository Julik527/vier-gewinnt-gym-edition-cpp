/**
 * @file tests/audio_scene_tests.cpp
 * @brief Testet die unabhängige Zuordnung der Musik zu Spielzuständen.
 * @details Prüft Tracknamen, Lautstärkefaktoren für Original und V4 sowie den Tense-Wechsel beim Füllstand.
 * @par Abhängigkeiten
 * GymAudioScene.h und Standardbibliothek; kein Fenster oder Lautsprecher erforderlich.
 * @par Wichtige Randbedingungen
 * Die Tests prüfen Entscheidungen und Grenzwerte, nicht die tatsächliche Wiedergabe.
 */

#include "GymAudioScene.h"
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>

void verify(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

int main() {
    using gymaudio::Scene;
    using gymaudio::filename;
    using gymaudio::gain;
    using gymaudio::gameplayScene;
    try {
        verify(std::string(filename(Scene::Intro)) == "airy.ogg", "intro");
        verify(std::string(filename(Scene::Menu)) == "title.ogg", "menu");
        verify(std::string(filename(Scene::Setup)) == "sector.ogg", "setup");
        verify(std::string(filename(Scene::Match)) == "pulse.ogg", "gameplay");
        verify(std::string(filename(Scene::Tense)) == "urgent.ogg", "tension");
        verify(std::string(filename(Scene::Results)) == "transmission.ogg", "results");
        for (Scene scene : {Scene::Intro, Scene::Menu, Scene::Setup,
                            Scene::Match, Scene::Tense, Scene::Results}) {
            verify(gain(scene) > 0.f && gain(scene) <= 1.f, "safe track gain");
        }
        verify(gain(Scene::Menu, true) > gain(Scene::Tense, true),
               "V4 menu track is quieter at source, needs more compensation");
        for (Scene scene : {Scene::Intro, Scene::Menu, Scene::Setup,
                            Scene::Match, Scene::Tense, Scene::Results})
            verify(gain(scene, true) > 0.f && gain(scene, true) <= 1.f,
                   "V4 gain remains in a safe range");
        verify(gameplayScene(0, 42) == Scene::Match, "early match");
        verify(gameplayScene(31, 42) == Scene::Match, "before threshold");
        verify(gameplayScene(32, 42) == Scene::Tense, "after threshold");
        verify(gameplayScene(0, 0) == Scene::Match, "invalid empty capacity");
        verify(gameplayScene(-1, 42) == Scene::Match, "negative counter");
        std::cout << "Sci-Fi Audio: scenes, gain factors and progress threshold OK.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Sci-Fi Audio test: " << error.what() << '\n';
        return 1;
    }
}
