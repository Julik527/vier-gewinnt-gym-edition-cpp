/**
 * @file include/Player.h
 * @brief Definiert einen Spieler mit Namen und Spielsymbol.
 * @details Player stellt einen Konstruktor sowie lesende Getter bereit; die Daten selbst bleiben privat.
 * @par Abhängigkeiten
 * C++17-Standardbibliothek; verwendet in Konsole und Grafikversionen.
 * @par Wichtige Randbedingungen
 * Die Validierung von Namen und Symbol-Eindeutigkeit erfolgt bei der Spieleranlage, nicht im Player-Konstruktor.
 */

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