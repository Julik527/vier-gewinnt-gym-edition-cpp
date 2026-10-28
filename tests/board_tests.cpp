#include "Board.h"
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
void verify(bool condition, const std::string& description) {
    // Never use assert() in tests that must also run in Release / NDEBUG.
    if (!condition) throw std::runtime_error("BOARD TEST FAILED: " + description);
}

int put(Board& board, int column, char symbol) {
    int row = -999;
    verify(board.drop(column, symbol, row), "drop succeeded");
    verify(row >= 0 && row < board.rows(), "landing row inside board");
    return row;
}
}

int main() {
    try {
        Board standard(2), medium(3), large(4);
        verify(standard.rows() == 6 && standard.columns() == 7, "2-player dimensions");
        verify(medium.rows() == 8 && medium.columns() == 9, "3-player dimensions");
        verify(large.rows() == 10 && large.columns() == 11, "4-player dimensions");

        int row = -999;
        verify(!standard.drop(-1, 'R', row) && row == -1, "left invalid column");
        verify(!standard.drop(7, 'R', row) && row == -1, "right invalid column");
        verify(standard.nextFreeRow(-1) == -1, "invalid nextFreeRow");
        verify(standard.nextFreeRow(0) == 5, "gravity on empty board");
        verify(!standard.hasWon(5, 0, 'R'), "no win on empty board");
        verify(standard.tokenCount() == 0 && !standard.isFull(), "empty board counts");

        for (int col = 0; col < 4; ++col) put(standard, col, 'R');
        verify(standard.hasWon(5, 3, 'R'), "horizontal win");
        verify(!standard.hasWon(5, 3, 'B'), "wrong symbol is not winner");
        verify(standard.tokenCount() == 4, "token count after 4 moves");

        Board vertical(2);
        for (int i = 0; i < 4; ++i) put(vertical, 0, 'B');
        verify(vertical.hasWon(2, 0, 'B'), "vertical win at top");
        verify(vertical.hasWon(3, 0, 'B'), "vertical win at middle");
        verify(vertical.nextFreeRow(0) == 1, "vertical gravity");
        verify(!vertical.undo(5, 0), "cannot undo buried stone");
        verify(vertical.undo(2, 0), "undo top stone");
        verify(!vertical.hasWon(3, 0, 'B'), "win removed after undo");

        Board diagDown(2);
        for (int col = 0; col < 4; ++col) {
            for (int support = 0; support < col; ++support) put(diagDown, col, 'B');
            put(diagDown, col, 'R');
        }
        verify(diagDown.hasWon(2, 3, 'R'), "diagonal right-up");
        verify(diagDown.undo(2, 3), "undo diagonal top stone");
        verify(!diagDown.hasWon(2, 3, 'R'), "diagonal win cleared");

        Board diagUp(2);
        for (int index = 0; index < 4; ++index) {
            const int col = 3 - index;
            for (int support = 0; support < index; ++support) put(diagUp, col, 'B');
            put(diagUp, col, 'R');
        }
        verify(diagUp.hasWon(2, 0, 'R'), "diagonal left-up");

        Board fullColumn(2);
        for (int i = 0; i < 6; ++i) put(fullColumn, 0, 'R');
        verify(fullColumn.nextFreeRow(0) == -1, "full column nextFreeRow");
        verify(!fullColumn.drop(0, 'R', row) && row == -1, "reject overfilled column");
        verify(fullColumn.tokenCount() == 6, "full column count");

        Board full(2);
        for (int col = 0; col < full.columns(); ++col) {
            for (int k = 0; k < full.rows(); ++k) put(full, col, 'R');
        }
        verify(full.isFull(), "full board detection");
        verify(full.tokenCount() == 42, "full board stone count");

        std::cout << "Board: 2/3/4 Spieler, Grenzen, Schwerkraft, "
                     "4 Gewinnrichtungen, Undo, volle Spalte, volles Spielfeld OK.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
