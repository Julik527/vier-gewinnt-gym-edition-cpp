#pragma once
#include <vector>

class Board {
private:
    int rows_;
    int columns_;
    std::vector<std::vector<char>> grid_;

    bool inside(int row, int column) const;
    int countDirection(int row, int column, int dRow, int dColumn, char symbol) const;
public:
    explicit Board(int playerCount = 2);
    int rows() const { return rows_; }
    int columns() const { return columns_; }
    int tokenCount() const;
    int nextFreeRow(int column) const;
    bool drop(int column, char symbol, int& rowPlaced);
    bool undo(int row, int column);
    bool hasWon(int row, int column, char symbol) const;
    bool isFull() const;
    const std::vector<std::vector<char>>& cells() const { return grid_; }
    void print(int previewColumn = -1, char previewSymbol = '.') const;
    void animateDrop(int column, char symbol, int landingRow) const;
};