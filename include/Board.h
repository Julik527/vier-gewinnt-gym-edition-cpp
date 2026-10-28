// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   include/Board.h
// Aufgabe: Beschreibt die Spielfeld-Klasse und ihre Funktionen.
// ------------------------------------------------------------

#pragma once
#include <vector>

// In Board liegt das ganze Spielfeld. Die Klasse kennt keine Spielernamen.
class Board {
private:
    int rows_;
    int columns_;
    // Jede Zeile enthält die Zeichen der einzelnen Spalten.
    std::vector<std::vector<char>> grid_;

    bool inside(int row, int column) const;
    int countDirection(int row, int column, int dRow, int dColumn, char symbol) const;
public:
    // Erstellt ein passendes Feld für 2, 3 oder 4 Spieler.
    explicit Board(int playerCount = 2);
    int rows() const { return rows_; }
    int columns() const { return columns_; }
    int tokenCount() const;
    // Liefert die nächste freie Zeile von unten oder -1 bei voller Spalte.
    int nextFreeRow(int column) const;
    // Setzt eine Hantel. Die tatsächliche Zeile kommt in rowPlaced.
    bool drop(int column, char symbol, int& rowPlaced);
    // Entfernt nur einen Stein, der nicht unter anderen Steinen liegt.
    bool undo(int row, int column);
    // Prüft von einem Feld aus alle vier Gewinnrichtungen.
    bool hasWon(int row, int column, char symbol) const;
    bool isFull() const;
    const std::vector<std::vector<char>>& cells() const { return grid_; }
    // Gibt das Spielfeld im Terminal aus. Eine Vorschau ist optional.
    void print(int previewColumn = -1, char previewSymbol = '.') const;
    // Zeigt auf Wunsch eine kurze, ruhige Zuganimation im Terminal.
    void animateDrop(int column, char symbol, int landingRow) const;
};