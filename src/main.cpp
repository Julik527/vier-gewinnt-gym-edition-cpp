// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   src/main.cpp
// Aufgabe: Startet das Spiel und faengt unerwartete Fehler ab.
// ------------------------------------------------------------

#include "Game.h"
#include <exception>
#include <iostream>

// Hier beginnt das Programm. Die Klasse Game uebernimmt danach das Spiel.
int main() {
    try {
        Game game;
        game.run();
    // Falls etwas schiefgeht, bekommt man eine lesbare Fehlermeldung.
    } catch (const std::exception& ex) {
        std::cerr << "Programm beendet: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}