// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   include/Player.h
// Aufgabe: Speichert den Namen und das Spielsymbol eines Spielers.
// ------------------------------------------------------------

#pragma once
#include <string>
#include <utility>

// Ein Spieler besitzt einen Namen und ein eindeutiges Symbol.
class Player {
private:
    // Der Name wird spaeter im Menue und beim Highscore angezeigt.
    std::string name_;
    // Das einzelne Zeichen stellt die Hantel auf dem Feld dar.
    char symbol_;
public:
    // Ein neuer Spieler bekommt direkt Namen und Symbol.
    Player(std::string name, char symbol)
        : name_(std::move(name)), symbol_(symbol) {}

    // Nur lesen: Andere Klassen sollen die Daten nicht direkt aendern.
    const std::string& getName() const { return name_; }
    char getSymbol() const { return symbol_; }
};