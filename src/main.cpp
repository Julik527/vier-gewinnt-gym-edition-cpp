#include "Game.h"
#include <exception>
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "Programm beendet: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}