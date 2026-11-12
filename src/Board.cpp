/**
 * @file src/Board.cpp
 * @brief Implementiert Schwerkraft, Zugrücknahme und Gewinnerkennung.
 * @details Das Raster misst 6x7, 8x9 oder 10x11; ein Sieg wird entlang von vier Achsen ab dem letzten Stein geprüft.
 * @par Abhängigkeiten
 * Implementierung der Schnittstelle Board.h; arbeitet ohne SFML oder OpenGL.
 * @par Wichtige Randbedingungen
 * Ungültige Spalten/Symbole setzen keinen Stein; winningCells liefert die komplette erste Gewinnlinie.
 */

#include "Board.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

namespace {
// Die Spielerzahl bestimmt, wie groß das Spielfeld wird.
int rowCountForPlayers(int players) {
    if (players < 2 || players > 4) {
        throw invalid_argument("Spieleranzahl muss 2 bis 4 sein.");
    }
    return 6 + 2 * (players - 2);
}
}

// Beim Start sind alle Felder leer. Ein Punkt steht für ein freies Feld.
Board::Board(int playerCount)
    : rows_(rowCountForPlayers(playerCount)),
      columns_(rows_ + 1),
      grid_(rows_, vector<char>(columns_, '.')) {}

bool Board::inside(int row, int column) const {
    return row >= 0 && row < rows_ && column >= 0 && column < columns_;
}

// Wegen der Schwerkraft belegt eine Hantel das unterste noch freie Feld.
// -1 bedeutet: Spalte existiert nicht oder ist bereits voll.
int Board::nextFreeRow(int column) const {
    if (column < 0 || column >= columns_) return -1;
    for (int row = rows_ - 1; row >= 0; --row) {
        if (grid_[row][column] == '.') return row;
    }
    return -1;
}

// Zuerst Symbol und Spalte validieren; eine ungültige Eingabe ändert
// den gespeicherten Feldzustand nicht.
bool Board::drop(int column, char symbol, int& rowPlaced) {
    // Nur die vier festgelegten Spielersymbole sind erlaubt. Sonstige
    // Steuerzeichen wuerden GYM1-Spielstaende unlesbar machen.
    if (symbol != 'R' && symbol != 'B' && symbol != 'G' && symbol != 'Y') {
        rowPlaced = -1;
        return false;
    }
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

// Zählt nur zusammenhängende gleiche Symbole ab dem NACHBARFELD.
// Das Startfeld wird bei der Gewinnerkennung separat als 1 gezählt.
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
    // Ein leeres Feld darf nicht als Gewinnlinie gewertet werden.
    if (symbol == '.' || !inside(row, column)
        || grid_[row][column] != symbol) return false;
    // Achsen: horizontal (0,1), vertikal (1,0) und beide Diagonalen.
    // Durch Zählen vorwärts und rückwärts erkennt auch ein mittlerer Stein den Sieg.
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

// Liefert die erste zusammenhängende Gewinnlinie als Zeilen-/Spaltenpaare.
// Die GUI nutzt sie zum Markieren, die Gewinnregel selbst bleibt in Board.
vector<pair<int, int>> Board::winningCells(int row, int column,
                                                      char symbol) const {
    if (symbol == '.' || !inside(row, column)
        || grid_[row][column] != symbol) return {};
    const int dr[4] = {0, 1, 1, 1};
    const int dc[4] = {1, 0, 1, -1};
    for (int i = 0; i < 4; ++i) {
        int firstRow = row;
        int firstCol = column;
        while (inside(firstRow - dr[i], firstCol - dc[i])
               && grid_[firstRow - dr[i]][firstCol - dc[i]] == symbol) {
            firstRow -= dr[i];
            firstCol -= dc[i];
        }
        vector<pair<int, int>> line;
        for (int r = firstRow, c = firstCol;
             inside(r, c) && grid_[r][c] == symbol;
             r += dr[i], c += dc[i]) {
            line.emplace_back(r, c);
        }
        // Auch bei 5 oder mehr verbundenen Steinen die ganze Linie anzeigen.
        if (line.size() >= 4) return line;
    }
    return {};
}

int Board::tokenCount() const {
    int count = 0;
    for (const auto& row : grid_) {
        for (char cell : row) if (cell != '.') ++count;
    }
    return count;
}

// Remis ist möglich, wenn kein Feld mehr frei ist.
bool Board::isFull() const {
    return tokenCount() == rows_ * columns_;
}

// Zeichnet das Spielfeld im Terminal, auf Wunsch mit einer Zug-Vorschau.
void Board::print(int previewColumn, char previewSymbol) const {
    int previewRow = (previewColumn == -1) ? -1 : nextFreeRow(previewColumn);
    cout << "\n    ";
    for (int col = 0; col < columns_; ++col) {
        cout << ' ';
        if (col + 1 < 10) cout << ' ';
        cout << col + 1 << "   ";
    }
    cout << "\n";
    for (int row = 0; row < rows_; ++row) {
        cout << "    ";
        for (int col = 0; col < columns_; ++col) {
            char value = grid_[row][col];
            if (row == previewRow && col == previewColumn) value = previewSymbol;
            if (value == '.') cout << "|  .  ";
            else cout << "|=[" << value << "]= "; // ASCII-Kurzhantel
        }
        cout << "|\n";
    }
    cout << "    " << string(columns_ * 6 + 1, '-') << "\n";
}
