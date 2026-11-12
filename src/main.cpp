/**
 * @file src/main.cpp
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
 * @brief Einstiegspunkt des eigenständigen C++17-Konsolenspiels.
 * @details main erstellt Game, ruft run auf und fängt unerwartete std::exception-Fehler ab.
 * @par Abhängigkeiten
 * Game.h und C++17-Standardbibliothek; ausdrücklich keine SFML-/OpenGL-Abhängigkeit.
 * @par Wichtige Randbedingungen
 * Bei einer Exception gibt das Programm eine Fehlermeldung aus und endet mit Exitcode 1.
 */

#include "Game.h"
#include <exception>
#include <iostream>

using namespace std;

// Einstiegspunkt der Schulkonsole: Die gesamte Spiellogik wird über
// Game::run() gesteuert. Die GUI-Programme haben eigene main()-Funktionen.
int main() {
    try {
        Game game;
        game.run();
    // Eine unerwartete Exception beendet das Programm kontrolliert mit Exitcode 1.
    } catch (const exception& ex) {
        cerr << "Programm beendet: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}