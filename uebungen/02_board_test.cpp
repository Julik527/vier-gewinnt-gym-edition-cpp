/**
 * @file uebungen/02_board_test.cpp
 * @brief Demonstriert einen einfachen Einwurf in die Klasse Board.
 * @details Erzeugt ein Zwei-Spieler-Feld, legt ein Symbol in Spalte 4 ab und gibt das Raster aus.
 * @par Abhängigkeiten
 * Board.h und <iostream>; die Übung benötigt die Board-Implementierung.
 * @par Wichtige Randbedingungen
 * Die Spalte wird intern nullbasiert als Index 3 übergeben; kein automatisierter Regressionstest.
 */

#include "Board.h"
#include <iostream>

using namespace std;

int main() {
    Board board(2);        // 7 Spalten, 6 Reihen
    int row = -1;
    board.drop(3, 'R', row); // vierte Spalte (Index 3)
    cout << "Stein landet in Reihe (Index): " << row << '\n';
    board.print();
}