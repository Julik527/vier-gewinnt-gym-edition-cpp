// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   src/main.cpp
// Aufgabe: Startet das Spiel und fängt unerwartete Fehler ab.
// ------------------------------------------------------------

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