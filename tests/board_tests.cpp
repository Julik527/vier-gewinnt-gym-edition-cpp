// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   tests/board_tests.cpp
// Aufgabe: Testet das Spielfeld mit automatischen Prüfungen.
// ------------------------------------------------------------

#include "Board.h"
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <string>

namespace {
// Wenn ein Test falsch ist, soll das Testprogramm sichtbar scheitern.
void verify(bool condition, const std::string& description) {
    // Diese Prüfung muss auch im Release-Build funktionieren.
    if (!condition) throw std::runtime_error("BOARD TEST FAILED: " + description);
}

// Spart im Test wiederholte Zeilen beim Einwerfen einer Hantel.
int put(Board& board, int column, char symbol) {
    int row = -999;
    verify(board.drop(column, symbol, row), "drop succeeded");
    verify(row >= 0 && row < board.rows(), "landing row inside board");
    return row;
}
}

// Hier werden die wichtigen Regeln des Spielfelds durchprobiert.
int main() {
    try {
        // Die Feldgröße soll zur Zahl der Spieler passen.
        Board standard(2), medium(3), large(4);
        verify(standard.rows() == 6 && standard.columns() == 7, "2-player dimensions");
        verify(medium.rows() == 8 && medium.columns() == 9, "3-player dimensions");
        verify(large.rows() == 10 && large.columns() == 11, "4-player dimensions");

        for (int invalidPlayers : {0, 1, 5, 10000}) {
            bool rejected = false;
            try { Board invalid(invalidPlayers); }
            catch (const std::invalid_argument&) { rejected = true; }
            verify(rejected, "invalid player count rejected");
        }

        int row = -999;
        verify(!standard.drop(-1, 'R', row) && row == -1, "left invalid column");
        verify(!standard.drop(0, '.', row) && row == -1, "reject empty-cell token");
        verify(!standard.drop(7, 'R', row) && row == -1, "right invalid column");
        verify(standard.nextFreeRow(-1) == -1, "invalid nextFreeRow");
        verify(standard.nextFreeRow(0) == 5, "gravity on empty board");
        verify(!standard.hasWon(5, 0, 'R'), "no win on empty board");
        verify(standard.tokenCount() == 0 && !standard.isFull(), "empty board counts");

        for (int col = 0; col < 4; ++col) put(standard, col, 'R');
        verify(standard.hasWon(5, 3, 'R'), "horizontal win");
        verify(standard.winningCells(5, 3, 'R').size() == 4, "highlight 4 horizontal");
        verify(standard.winningCells(5, 3, 'B').empty(), "wrong symbol not highlighted");
        verify(!standard.hasWon(5, 3, 'B'), "wrong symbol is not winner");
        verify(standard.tokenCount() == 4, "token count after 4 moves");

        // Vier Steine in einer Spalte müssen als Sieg erkannt werden.
        Board vertical(2);
        for (int i = 0; i < 4; ++i) put(vertical, 0, 'B');
        verify(vertical.hasWon(2, 0, 'B'), "vertical win at top");
        verify(vertical.winningCells(2, 0, 'B').size() == 4, "highlight 4 vertical");
        verify(vertical.hasWon(3, 0, 'B'), "vertical win at middle");
        verify(vertical.nextFreeRow(0) == 1, "vertical gravity");
        verify(!vertical.undo(5, 0), "cannot undo buried stone");
        verify(vertical.undo(2, 0), "undo top stone");
        verify(!vertical.hasWon(3, 0, 'B'), "win removed after undo");

        // Eine Diagonale entsteht durch unterschiedlich hohe Stapel.
        Board diagDown(2);
        for (int col = 0; col < 4; ++col) {
            for (int support = 0; support < col; ++support) put(diagDown, col, 'B');
            put(diagDown, col, 'R');
        }
        verify(diagDown.hasWon(2, 3, 'R'), "diagonal right-up");
        verify(diagDown.winningCells(2, 3, 'R').size() == 4, "highlight diagonal");
        verify(diagDown.undo(2, 3), "undo diagonal top stone");
        verify(!diagDown.hasWon(2, 3, 'R'), "diagonal win cleared");

        Board diagUp(2);
        for (int index = 0; index < 4; ++index) {
            const int col = 3 - index;
            for (int support = 0; support < index; ++support) put(diagUp, col, 'B');
            put(diagUp, col, 'R');
        }
        verify(diagUp.hasWon(2, 0, 'R'), "diagonal left-up");

        // In eine volle Spalte darf kein weiterer Stein fallen.
        Board fullColumn(2);
        for (int i = 0; i < 6; ++i) put(fullColumn, 0, 'R');
        verify(fullColumn.nextFreeRow(0) == -1, "full column nextFreeRow");
        verify(!fullColumn.drop(0, 'R', row) && row == -1, "reject overfilled column");
        verify(fullColumn.tokenCount() == 6, "full column count");

        // Am Ende prüfen wir auch, ob ein ganz volles Feld erkannt wird.
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
