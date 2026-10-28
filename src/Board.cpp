// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   src/Board.cpp
// Aufgabe: Verwaltet das Spielfeld, die Spielzuege und die Gewinnerkennung.
// ------------------------------------------------------------

#include "Board.h"
#include <chrono>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>

namespace {
// Die Spielerzahl bestimmt, wie gross das Spielfeld wird.
int rowCountForPlayers(int players) {
    if (players < 2 || players > 4) {
        throw std::invalid_argument("Spieleranzahl muss 2 bis 4 sein.");
    }
    return 6 + 2 * (players - 2);
}
}

// Beim Start sind alle Felder leer. Ein Punkt steht fuer ein freies Feld.
Board::Board(int playerCount)
    : rows_(rowCountForPlayers(playerCount)),
      columns_(rows_ + 1),
      grid_(rows_, std::vector<char>(columns_, '.')) {}

bool Board::inside(int row, int column) const {
    return row >= 0 && row < rows_ && column >= 0 && column < columns_;
}

// Von unten nach oben suchen: Dort landet die naechste Hantel.
int Board::nextFreeRow(int column) const {
    if (column < 0 || column >= columns_) return -1;
    for (int row = rows_ - 1; row >= 0; --row) {
        if (grid_[row][column] == '.') return row;
    }
    return -1;
}

// Ein Zug klappt nur, wenn die Spalte noch Platz hat.
bool Board::drop(int column, char symbol, int& rowPlaced) {
    // '.' is the internal empty-cell marker and cannot represent a token.
    if (symbol == '.') { rowPlaced = -1; return false; }
    rowPlaced = nextFreeRow(column);
    if (rowPlaced == -1) return false;
    grid_[rowPlaced][column] = symbol;
    return true;
}

// Nur die oberste Hantel einer Spalte darf entfernt werden.
bool Board::undo(int row, int column) {
    if (!inside(row, column) || grid_[row][column] == '.') return false;
    for (int r = 0; r < row; ++r) {
        if (grid_[r][column] != '.') return false; // nur oberster Stein
    }
    grid_[row][column] = '.';
    return true;
}

// Zaehlt gleiche Hanteln in genau einer Richtung.
int Board::countDirection(int row, int column, int dRow, int dColumn,
                          char symbol) const {
    int count = 0;
    row += dRow;
    column += dColumn;
    while (inside(row, column) && grid_[row][column] == symbol) {
        ++count;
        row += dRow;
        column += dColumn;
    }
    return count;
}

// Vier gleiche Hanteln in einer Reihe bedeuten einen Sieg.
bool Board::hasWon(int row, int column, char symbol) const {
    if (!inside(row, column) || grid_[row][column] != symbol) return false;
    // Wir pruefen waagerecht, senkrecht und die beiden Diagonalen.
    const int dr[4] = {0, 1, 1, 1};
    const int dc[4] = {1, 0, 1, -1};
    for (int i = 0; i < 4; ++i) {
        int length = 1
            + countDirection(row, column, dr[i], dc[i], symbol)
            + countDirection(row, column, -dr[i], -dc[i], symbol);
        if (length >= 4) return true;
    }
    return false;
}

int Board::tokenCount() const {
    int count = 0;
    for (const auto& row : grid_) {
        for (char cell : row) if (cell != '.') ++count;
    }
    return count;
}

// Remis ist moeglich, wenn kein Feld mehr frei ist.
bool Board::isFull() const {
    return tokenCount() == rows_ * columns_;
}

// Zeichnet das Spielfeld im Terminal, auf Wunsch mit einer Zug-Vorschau.
void Board::print(int previewColumn, char previewSymbol) const {
    int previewRow = (previewColumn == -1) ? -1 : nextFreeRow(previewColumn);
    std::cout << "\n    ";
    for (int col = 0; col < columns_; ++col) {
        std::cout << ' ';
        if (col + 1 < 10) std::cout << ' ';
        std::cout << col + 1 << "   ";
    }
    std::cout << "\n";
    for (int row = 0; row < rows_; ++row) {
        std::cout << "    ";
        for (int col = 0; col < columns_; ++col) {
            char value = grid_[row][col];
            if (row == previewRow && col == previewColumn) value = previewSymbol;
            if (value == '.') std::cout << "|  .  ";
            else std::cout << "|=[" << value << "]= "; // ASCII-Kurzhantel
        }
        std::cout << "|\n";
    }
    std::cout << "    " << std::string(columns_ * 6 + 1, '-') << "\n";
}

// Die Animation zeigt nur Bilder nacheinander. Das eigentliche Feld bleibt unveraendert.
void Board::animateDrop(int column, char symbol, int landingRow) const {
    // Nur Konsolen-Animation. Es wird kein zweiter Stein gespeichert.
    // ANSI-Steuerzeichen funktionieren z. B. in Windows Terminal / VS Code.
    for (int row = 0; row <= landingRow; ++row) {
        std::cout << "\x1b[2J\x1b[H";
        std::cout << "VIER GEWINNT | GYM EDITION | Kurzhantel faellt...\n";
        std::cout << "    ";
        for (int col = 0; col < columns_; ++col) {
            std::cout << ' ';
            if (col + 1 < 10) std::cout << ' ';
            std::cout << col + 1 << "   ";
        }
        std::cout << "\n";
        for (int r = 0; r < rows_; ++r) {
            std::cout << "    ";
            for (int c = 0; c < columns_; ++c) {
                char v = (r == row && c == column) ? symbol : grid_[r][c];
                if (v == '.') std::cout << "|  .  ";
                else std::cout << "|=[" << v << "]= ";
            }
            std::cout << "|\n";
        }
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(65));
    }
}