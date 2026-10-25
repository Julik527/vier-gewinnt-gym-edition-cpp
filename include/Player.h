#pragma once
#include <string>
#include <utility>

// Ein Spieler besitzt einen Namen und ein eindeutiges Symbol.
class Player {
private:
    std::string name_;
    char symbol_;
public:
    Player(std::string name, char symbol)
        : name_(std::move(name)), symbol_(symbol) {}

    const std::string& getName() const { return name_; }
    char getSymbol() const { return symbol_; }
};