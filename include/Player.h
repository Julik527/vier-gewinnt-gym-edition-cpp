// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   include/Player.h
// Aufgabe: Speichert den Namen und das Spielsymbol eines Spielers.
// ------------------------------------------------------------

#pragma once
#include <string>
#include <utility>

// Player ist ein einfaches Datenobjekt. Die Gültigkeit von Namen und die
// Einmaligkeit der Symbole werden bei der Spieleranlage geprüft.
class Player {
private:
    // Der Name wird später im Menü und beim Highscore angezeigt.
    std::string name_;
    // Das einzelne Zeichen stellt die Hantel auf dem Feld dar.
    char symbol_;
public:
    // Ein neuer Spieler bekommt direkt Namen und Symbol.
    Player(std::string name, char symbol)
        : name_(std::move(name)), symbol_(symbol) {}

    // Nur lesen: Andere Klassen sollen die Daten nicht direkt ändern.
    const std::string& getName() const { return name_; }
    char getSymbol() const { return symbol_; }
};