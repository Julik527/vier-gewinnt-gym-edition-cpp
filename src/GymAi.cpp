/**
 * @file src/GymAi.cpp
 * @brief Minimax-Gegner mit Alpha-Beta-Pruning fuer den optionalen Zweispielermodus.
 * @details Endliche, reproduzierbare Suchtiefen und eine Fenster-Heuristik.
 * @par Abhängigkeiten
 * GymAi.h, Board und C++17; kein Zufallsgenerator.
 * @par Wichtige Randbedingungen
 * Original-Board bleibt unveraendert; ausserhalb 7x6/R/B wird abgelehnt.
 */
#include "GymAi.h"
#include <array>
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace {
constexpr int kWin = 1000000;
constexpr int kInfinity = 2000000;
constexpr std::array<int, 7> kCenterFirst{{3, 2, 4, 1, 5, 0, 6}};

char otherSymbol(char symbol) { return symbol == 'R' ? 'B' : 'R'; }

bool terminal(const Board& board) {
    if (board.isFull()) return true;
    for (int row = 0; row < board.rows(); ++row) {
        for (int col = 0; col < board.columns(); ++col) {
            const char token = board.cells()[row][col];
            if (token != '.' && board.hasWon(row, col, token)) return true;
        }
    }
    return false;
}

int windowScore(int own, int enemy, int empty, int playable) {
    if (own != 0 && enemy != 0) return 0;
    if (own == 4) return 100000;
    if (enemy == 4) return -100000;
    if (own == 3 && empty == 1) return playable ? 240 : 45;
    if (enemy == 3 && empty == 1) return playable ? -300 : -55;
    if (own == 2 && empty == 2) return 12;
    if (enemy == 2 && empty == 2) return -16;
    if (own == 1 && empty == 3) return 1;
    return 0;
}

int evaluate(const Board& board, char ai, char opponent) {
    int score = 0;
    const auto& cells = board.cells();
    std::array<int, 7> freeRows{};
    for (int col = 0; col < board.columns(); ++col)
        freeRows[static_cast<std::size_t>(col)] = board.nextFreeRow(col);
    const int center = board.columns() / 2;
    for (int row = 0; row < board.rows(); ++row) {
        if (cells[row][center] == ai) score += 5;
        if (cells[row][center] == opponent) score -= 5;
    }
    constexpr std::array<std::pair<int, int>, 4> axes{{
        {0, 1}, {1, 0}, {1, 1}, {1, -1}
    }};
    for (int row = 0; row < board.rows(); ++row) {
        for (int col = 0; col < board.columns(); ++col) {
            for (auto axis : axes) {
                const int endRow = row + 3 * axis.first;
                const int endCol = col + 3 * axis.second;
                if (endRow < 0 || endRow >= board.rows()
                    || endCol < 0 || endCol >= board.columns()) continue;
                int own = 0;
                int enemy = 0;
                int empty = 0;
                int playable = 0;
                for (int offset = 0; offset < 4; ++offset) {
                    const int r = row + offset * axis.first;
                    const int c = col + offset * axis.second;
                    const char token = cells[r][c];
                    if (token == ai) ++own;
                    else if (token == opponent) ++enemy;
                    else {
                        ++empty;
                        if (freeRows[static_cast<std::size_t>(c)] == r) ++playable;
                    }
                }
                score += windowScore(own, enemy, empty, playable);
            }
        }
    }
    return score;
}

int search(Board& board, int depth, int alpha, int beta,
           char nextPlayer, char ai, char opponent, int lastRow,
           int lastColumn, char lastSymbol, std::size_t& nodes) {
    ++nodes;
    if (lastRow >= 0 && board.hasWon(lastRow, lastColumn, lastSymbol)) {
        return lastSymbol == ai ? kWin + depth : -kWin - depth;
    }
    if (board.isFull()) return 0;
    if (depth == 0) return evaluate(board, ai, opponent);
    const bool maximizing = nextPlayer == ai;
    int best = maximizing ? -kInfinity : kInfinity;
    for (int column : kCenterFirst) {
        if (board.nextFreeRow(column) < 0) continue;
        int row = -1;
        if (!board.drop(column, nextPlayer, row)) continue;
        const int value = search(board, depth - 1, alpha, beta,
                                 otherSymbol(nextPlayer), ai, opponent,
                                 row, column, nextPlayer, nodes);
        if (!board.undo(row, column))
            throw std::logic_error("KI: Suchzug konnte nicht entfernt werden.");
        if (maximizing) {
            if (value > best) best = value;
            if (best > alpha) alpha = best;
        } else {
            if (value < best) best = value;
            if (best < beta) beta = best;
        }
        if (alpha >= beta) break;
    }
    return best;
}
} // namespace

namespace gym {
AiChoice chooseAiMove(const Board& board, char aiSymbol, AiLevel level) {
    if (board.rows() != 6 || board.columns() != 7
        || (aiSymbol != 'R' && aiSymbol != 'B')) {
        throw std::invalid_argument("KI: nur 2 Spieler (7x6, R/B).");
    }
    for (const auto& row : board.cells()) {
        for (char token : row) {
            if (token != '.' && token != 'R' && token != 'B') {
                throw std::invalid_argument("KI: falsches Spielersymbol.");
            }
        }
    }
    if (level != AiLevel::Easy && level != AiLevel::Normal && level != AiLevel::Hard)
        throw std::invalid_argument("KI: unbekannte Stufe.");
    if (terminal(board)) return {};
    const char opponent = otherSymbol(aiSymbol);
    Board position = board;
    std::size_t nodes = 0;
    // Sofortsiege haben bei jeder Stufe Vorrang.
    for (int column : kCenterFirst) {
        int row = -1;
        if (!position.drop(column, aiSymbol, row)) continue;
        ++nodes;
        const bool wins = position.hasWon(row, column, aiSymbol);
        if (!position.undo(row, column)) throw std::logic_error("KI: Undo fehlgeschlagen.");
        if (wins) return {column, kWin, nodes};
    }
    if (level == AiLevel::Easy) {
        // Einfach bleibt bewusst ohne vorausschauenden Suchbaum.
        for (int column : kCenterFirst)
            if (board.nextFreeRow(column) >= 0) return {column, 0, nodes};
        return {};
    }
    // Eine einzige unmittelbare Bedrohung muss blockiert werden.
    // Bei mehreren Bedrohungen bewertet die Suche die legalen Züge.
    int threatColumn = -1;
    int threats = 0;
    for (int column : kCenterFirst) {
        int row = -1;
        if (!position.drop(column, opponent, row)) continue;
        ++nodes;
        if (position.hasWon(row, column, opponent)) {
            threatColumn = column;
            ++threats;
        }
        if (!position.undo(row, column)) throw std::logic_error("KI: Undo fehlgeschlagen.");
    }
    const int depth = std::min(level == AiLevel::Normal ? 4 : 7,
                               42 - board.tokenCount());
    int alpha = -kInfinity;
    int bestScore = -kInfinity;
    int bestColumn = -1;
    for (int column : kCenterFirst) {
        if (threats == 1 && column != threatColumn) continue;
        int row = -1;
        if (!position.drop(column, aiSymbol, row)) continue;
        const int score = search(position, depth - 1, alpha, kInfinity,
                                 opponent, aiSymbol, opponent,
                                 row, column, aiSymbol, nodes);
        if (!position.undo(row, column)) throw std::logic_error("KI: Undo fehlgeschlagen.");
        if (bestColumn < 0 || score > bestScore) {
            bestColumn = column;
            bestScore = score;
        }
        if (score > alpha) alpha = score;
    }
    return {bestColumn, bestScore, nodes};
}
} // namespace gym
