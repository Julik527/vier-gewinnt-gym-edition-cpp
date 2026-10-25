#include "Board.h"
#include <cassert>
#include <iostream>

static void place(Board& board, int column, char symbol) {
    int row = -1;
    assert(board.drop(column, symbol, row));
}

int main() {
    Board a(2);
    assert(a.rows() == 6 && a.columns() == 7);
    Board b(3);
    assert(b.rows() == 8 && b.columns() == 9);
    Board c(4);
    assert(c.rows() == 10 && c.columns() == 11);
    int row;
    assert(!a.drop(-1, 'R', row));
    assert(!a.drop(7, 'R', row));
    for (int col = 0; col < 4; ++col) {
        assert(a.drop(col, 'R', row));
    }
    assert(a.hasWon(row, 3, 'R')); // horizontal
    Board vertical(2);
    for (int i = 0; i < 4; ++i) place(vertical, 0, 'B');
    assert(vertical.hasWon(2, 0, 'B')); // 4 Steine in Spalte 1 (Reihen 2 bis 5)
    assert(vertical.hasWon(3, 0, 'B')); // auch die benachbarte Position meldet vier
    Board diagA(2);
    for (int col = 0; col < 4; ++col) {
        for (int support = 0; support < col; ++support) place(diagA, col, 'B');
        place(diagA, col, 'R');
    }
    assert(diagA.hasWon(2, 3, 'R'));
    Board diagB(2);
    for (int index = 0; index < 4; ++index) {
        int col = 3 - index;
        for (int support = 0; support < index; ++support) place(diagB, col, 'B');
        place(diagB, col, 'R');
    }
    assert(diagB.hasWon(2, 0, 'R'));
    assert(diagA.undo(2, 3));
    assert(!diagA.hasWon(2, 3, 'R'));
    std::cout << "Board Tests erfolgreich: Dimensionen, Eingaben, "
                 "waagrecht, senkrecht, diagonal, Undo.\n";
}