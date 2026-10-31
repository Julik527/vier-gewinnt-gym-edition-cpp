// Aufgabenblatt 51: prueft nur eigens vorbereitete Schul-Vorfuehrdaten.
// Kein Audio, keine GUI und keine Veraenderung echter Spielstaende.
#include "MatchSession.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: school_demo_fixture_tests <2p> <4p>\n";
        return 2;
    }
    MatchSession two;
    if (!two.load(argv[1]) || !two.active()
        || two.players().size() != 2u
        || two.board().rows() != 6 || two.board().columns() != 7
        || two.board().tokenCount() != 6 || two.current() != 0) {
        std::cerr << "Invalid two-player GYM1 classroom fixture.\n";
        return 1;
    }
    Board board = two.board();
    int row = -1;
    if (!board.drop(3, two.players()[two.current()].getSymbol(), row)
        || !board.hasWon(row, 3, 'R')) {
        std::cerr << "The prepared horizontal winning move is wrong.\n";
        return 1;
    }
    MatchSession four;
    if (!four.load(argv[2]) || !four.active()
        || four.players().size() != 4u
        || four.board().rows() != 10 || four.board().columns() != 11
        || four.board().tokenCount() != 8 || four.current() != 0) {
        std::cerr << "Invalid four-player GYM1 classroom fixture.\n";
        return 1;
    }
    std::cout << "Aufgabenblatt 51: beide GYM1-Demo-Spielstaende geprueft.\n";
    return 0;
}
