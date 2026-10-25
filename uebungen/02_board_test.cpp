#include "Board.h"
#include <iostream>

int main() {
    Board board(2);        // 7 Spalten, 6 Reihen
    int row = -1;
    board.drop(3, 'R', row); // vierte Spalte (Index 3)
    std::cout << "Stein landet in Reihe (Index): " << row << '\n';
    board.print();
}