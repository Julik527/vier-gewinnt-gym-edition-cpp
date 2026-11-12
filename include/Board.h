/**
 * @file include/Board.h
 * @author Julian Krauß
 * @date 2026-11-12 (Dokumentationsstand)
 * @par Projekt
 * Vier Gewinnt | Gym Edition (C++17)
 * @par Schule
 * Bernd-Blindow-Schule Ravensburg
 * @par Klasse
 * ITA25/9
 * @par Unterricht
 * Programmiertechnik (C++)
 * @par Fachlehrer
 * Dr. Litera
 * @brief Deklariert die gemeinsamen Spielfeldregeln für zwei bis vier Spieler.
 * @details Board stellt Einwurf, Schwerkraft, Undo, vier Gewinnachsen, Gewinnzellen und eine einfache Textausgabe bereit.
 * @par Abhängigkeiten
 * C++17-Standardbibliothek; Game und MatchSession verwenden dieselbe Klasse.
 * @par Wichtige Randbedingungen
 * Interne Koordinaten beginnen bei 0; '.' ist leer; gültige Spielsymbole sind R, B, G und Y.
 */

#pragma once
#include <utility>
#include <vector>

// Spielfeld und Regeln sind unabhängig von Konsole, 2D und 3D.
// Koordinaten: Zeile 0 oben, Spalte 0 links; '.' kennzeichnet ein freies Feld.
class Board {
private:
    int rows_;
    int columns_;
    // grid_[zeile][spalte] speichert ausschließlich '.', 'R', 'B', 'G' oder 'Y'.
    std::vector<std::vector<char>> grid_;

    bool inside(int row, int column) const;
    int countDirection(int row, int column, int dRow, int dColumn, char symbol) const;
public:
    // 2 Spieler: 6x7, 3 Spieler: 8x9, 4 Spieler: 10x11.
    // Ungültige Spielerzahlen führen zu std::invalid_argument.
    explicit Board(int playerCount = 2);
    int rows() const { return rows_; }
    int columns() const { return columns_; }
    int tokenCount() const;
    // Liefert die nächste freie Zeile von unten oder -1 bei voller Spalte.
    int nextFreeRow(int column) const;
    // Spalte ist nullbasiert; gültige Symbole: R, B, G, Y.
    // false bei voller/ungültiger Spalte oder Symbol; rowPlaced wird dann -1.
    bool drop(int column, char symbol, int& rowPlaced);
    // Undo entfernt nur den obersten belegten Stein dieser Spalte.
    // Andere Steine müssen wegen der Schwerkraft unverändert bleiben.
    bool undo(int row, int column);
    // Prüft ausschließlich Linien durch den zuletzt gesetzten Stein.
    // Gezählt wird in beide Richtungen jeder der vier Achsen.
    bool hasWon(int row, int column, char symbol) const;
    // Liefert alle verbundenen Zellen der ersten gefundenen Gewinnlinie.
    // Bei fünf verbundenen Steinen können also mehr als vier Zellen zurückkommen.
    std::vector<std::pair<int, int>> winningCells(int row, int column, char symbol) const;
    bool isFull() const;
    const std::vector<std::vector<char>>& cells() const { return grid_; }
    // Gibt das Spielfeld im Terminal aus. Eine Vorschau ist optional.
    void print(int previewColumn = -1, char previewSymbol = '.') const;
};