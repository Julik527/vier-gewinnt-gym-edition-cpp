// Vier Gewinnt | Gym Edition
// Deterministische Regressionstests mit unabhaengigem Referenz-Spielfeld.
// Die Zufallsfolge wird durch einen festen Seed reproduzierbar.
#include "Board.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error("PROPERTY TEST FAILED: " + message);
}

bool within(const std::vector<std::string>& grid, int row, int col) {
    return row >= 0 && row < static_cast<int>(grid.size())
        && col >= 0 && col < static_cast<int>(grid[0].size());
}

bool referenceWin(const std::vector<std::string>& grid, int row, int col, char symbol) {
    if (!within(grid, row, col) || grid[row][col] != symbol) return false;
    const std::array<std::pair<int, int>, 4> directions = {
        std::make_pair(0, 1), std::make_pair(1, 0),
        std::make_pair(1, 1), std::make_pair(1, -1)
    };
    for (const auto& direction : directions) {
        int length = 1;
        for (int sign : {-1, 1}) {
            int r = row + sign * direction.first;
            int c = col + sign * direction.second;
            while (within(grid, r, c) && grid[r][c] == symbol) {
                ++length;
                r += sign * direction.first;
                c += sign * direction.second;
            }
        }
        if (length >= 4) return true;
    }
    return false;
}

void checkWinningLine(const Board& board,
                      const std::vector<std::pair<int, int>>& cells,
                      int row, int col, char symbol) {
    require(cells.size() >= 4, "Sieg muss mindestens vier Felder markieren");
    require(std::find(cells.begin(), cells.end(), std::make_pair(row, col)) != cells.end(),
            "Die Gewinnreihe muss den letzten Stein enthalten");
    const int deltaRow = cells[1].first - cells[0].first;
    const int deltaCol = cells[1].second - cells[0].second;
    require((deltaRow == 0 || deltaRow == 1 || deltaRow == -1)
                && (deltaCol == 0 || deltaCol == 1 || deltaCol == -1)
                && (deltaRow != 0 || deltaCol != 0),
            "Ungueltige Richtung der Gewinnreihe");
    for (std::size_t i = 0; i < cells.size(); ++i) {
        const int r = cells[i].first;
        const int c = cells[i].second;
        require(r >= 0 && r < board.rows() && c >= 0 && c < board.columns(),
                "Markiertes Feld ausserhalb des Spielfelds");
        require(board.cells()[r][c] == symbol, "Fremdes Symbol markiert");
        if (i != 0) {
            require(r - cells[i - 1].first == deltaRow
                        && c - cells[i - 1].second == deltaCol,
                    "Gewinnreihe nicht durchgehend");
        }
    }
}

void verifyBoard(const Board& board, const std::vector<std::string>& expected, int count) {
    require(board.tokenCount() == count, "Falscher Steinzaehler");
    require(board.isFull() == (count == board.rows() * board.columns()),
            "Falsche Voll-Erkennung");
    for (int row = 0; row < board.rows(); ++row) {
        for (int col = 0; col < board.columns(); ++col) {
            require(board.cells()[row][col] == expected[row][col],
                    "Spielfeld weicht vom Referenzmodell ab");
        }
    }
}

void exercise(int players, std::mt19937& random, int gameNumber) {
    Board board(players);
    std::vector<std::string> expected(
        board.rows(), std::string(static_cast<std::size_t>(board.columns()), '.'));
    std::uniform_int_distribution<int> columns(0, board.columns() - 1);
    std::uniform_int_distribution<int> symbols(0, players - 1);
    const std::array<char, 4> tokens = {'R', 'B', 'G', 'Y'};
    int inserted = 0;

    for (int invalid : {-1, board.columns(),
                        std::numeric_limits<int>::min(),
                        std::numeric_limits<int>::max()}) {
        int placed = 123;
        require(board.nextFreeRow(invalid) == -1, "Ungueltige Spalte nicht abgelehnt");
        require(!board.drop(invalid, 'R', placed) && placed == -1,
                "Drop ausserhalb des Spielfelds zugelassen");
        require(!board.undo(-1, invalid), "Undo ausserhalb des Spielfelds zugelassen");
    }

    while (inserted < board.rows() * board.columns()) {
        int col = columns(random);
        while (expected[0][col] != '.') col = columns(random);
        int row = board.rows() - 1;
        while (expected[row][col] != '.') --row;

        const char symbol = tokens[static_cast<std::size_t>(symbols(random))];
        int placed = -42;
        require(board.nextFreeRow(col) == row, "Schwerkraft-Referenzabweichung");
        require(board.drop(col, symbol, placed), "Gueltiger Zug fehlgeschlagen");
        require(placed == row, "Stein in falscher Zeile");
        expected[row][col] = symbol;
        ++inserted;
        verifyBoard(board, expected, inserted);

        const bool won = referenceWin(expected, row, col, symbol);
        require(board.hasWon(row, col, symbol) == won,
                "Gewinnvergleich fehlgeschlagen (Spiel " + std::to_string(gameNumber) + ")");
        const auto line = board.winningCells(row, col, symbol);
        if (won) checkWinningLine(board, line, row, col, symbol);
        else require(line.empty(), "Gewinnmarkierung ohne Sieg");

        // Ein direktes Undo muss genau den letzten Zug beseitigen.
        if (random() % 7 == 0) {
            require(board.undo(row, col), "Undo des obersten Steins fehlgeschlagen");
            expected[row][col] = '.';
            --inserted;
            verifyBoard(board, expected, inserted);
            require(!board.hasWon(row, col, symbol),
                    "Sieg nach Entfernen des ausloesenden Steins");
            require(board.drop(col, symbol, placed) && placed == row,
                    "Wiederherstellung des rueckgaengig gemachten Zugs");
            expected[row][col] = symbol;
            ++inserted;
            verifyBoard(board, expected, inserted);
        }
    }
    require(board.isFull(), "Vollstaendiges Spielbrett nicht als voll erkannt");
    for (int col = 0; col < board.columns(); ++col) {
        int placed = 27;
        require(board.nextFreeRow(col) == -1, "Gefuellte Spalte hat freien Platz");
        require(!board.drop(col, 'R', placed) && placed == -1,
                "Stein in volle Spalte gesetzt");
    }
}
} // namespace

int main() {
    try {
        constexpr std::uint32_t seed = 0xC0FFEE42u;
        std::mt19937 random(seed);
        constexpr int casesPerSize = 120;
        for (int players : {2, 3, 4}) {
            for (int trial = 0; trial < casesPerSize; ++trial) {
                exercise(players, random, trial);
            }
        }
        std::cout << "Property-Tests bestanden: 360 volle Spielfelder, "
                  << "2-4 Spieler, feste Startzahl 0xC0FFEE42, "
                  << "Gewinnerkennung, Schwerkraft, Undo und Grenzen.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
