/**
 * @file include/GameState.h
 * @brief Gemeinsame Zugverwaltung und GYM1-Spielstand.
 * @details Konsole und GUI verwenden dieselben Regeln zum Setzen, Undo und Laden.
 * @par Abhängigkeiten
 * Board, Player, AtomicFile und GameFileLimits.
 * @par Wichtige Randbedingungen
 * Ungültige Züge und Spielstände verändern die laufende Partie nicht.
 */

#pragma once
#include "Board.h"
#include "Player.h"
#include "AtomicFile.h"
#include "GameFileLimits.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>

namespace gym {
inline constexpr char playerSymbols[] = {'R', 'B', 'G', 'Y'};

struct Move {
    int row;
    int column;
    int player;
};

inline bool validPlayerName(const std::string& name) {
    if (name.empty() || name.size() > 24
        || name.find_first_not_of(' ') == std::string::npos) return false;
    for (unsigned char c : name) {
        if (c < 32 || c == 127) return false;
    }
    return true;
}

enum class TurnResult { Invalid, Continue, Win, Draw };

// Bei Sieg oder Remis bleibt der aktuelle Spieler unverändert.
inline TurnResult playMove(Board& board, const std::vector<Player>& players,
                           std::vector<Move>& history, int& current, int column) {
    if (current < 0 || current >= static_cast<int>(players.size()))
        return TurnResult::Invalid;
    const char symbol = players[current].getSymbol();
    int row;
    if (!board.drop(column, symbol, row)) return TurnResult::Invalid;
    history.push_back({row, column, current});
    if (board.hasWon(row, column, symbol)) return TurnResult::Win;
    if (board.isFull()) return TurnResult::Draw;
    current = (current + 1) % static_cast<int>(players.size());
    return TurnResult::Continue;
}

inline bool undoMove(Board& board, std::vector<Move>& history, int& current) {
    if (history.empty()) return false;
    const Move last = history.back();
    if (!board.undo(last.row, last.column)) return false;
    history.pop_back();
    current = last.player;
    return true;
}

struct SavedGame {
    Board board{2};
    std::vector<Player> players;
    std::vector<Move> history;
    int current = 0;
};

inline bool writeGame(const std::string& path, const Board& board,
                      const std::vector<Player>& players,
                      const std::vector<Move>& history, int current) {
    return writeSafely(path, [&](std::ostream& out) {
        out << "GYM1\n";
        out << players.size() << ' ' << current << ' ' << board.rows() << ' '
            << board.columns() << ' ' << history.size() << '\n';
        for (const auto& player : players)
            out << std::quoted(player.getName()) << ' ' << player.getSymbol() << '\n';
        for (const auto& move : history)
            out << move.row << ' ' << move.column << ' ' << move.player << '\n';
        for (const auto& row : board.cells()) {
            for (char cell : row) out << cell;
            out << '\n';
        }
    });
}

// Der gespeicherte Zustand wird durch Nachspielen aller Züge überprüft.
inline bool readGame(const std::string& path, SavedGame& destination) {
    if (!validSaveFileSize(path)) return false;
    std::ifstream in(std::filesystem::u8path(path));
    std::string signature;
    int count, turn, rows, columns, moveCount;
    if (!(in >> signature) || signature != "GYM1") return false;
    if (!(in >> count >> turn >> rows >> columns >> moveCount)) return false;
    if (count < 2 || count > 4 || turn < 0 || turn >= count) return false;

    SavedGame candidate;
    candidate.board = Board(count);
    if (rows != candidate.board.rows() || columns != candidate.board.columns()
        || moveCount < 0 || moveCount > rows * columns || turn != moveCount % count)
        return false;

    for (int i = 0; i < count; ++i) {
        std::string name;
        char symbol;
        if (!(in >> std::quoted(name) >> symbol) || !validPlayerName(name)
            || symbol != playerSymbols[i]) return false;
        for (const auto& player : candidate.players)
            if (player.getName() == name) return false;
        candidate.players.emplace_back(name, symbol);
    }
    for (int i = 0; i < moveCount; ++i) {
        int savedRow, column, player;
        if (!(in >> savedRow >> column >> player) || player != i % count) return false;
        int actualRow;
        const char symbol = candidate.players[player].getSymbol();
        if (!candidate.board.drop(column, symbol, actualRow) || actualRow != savedRow
            || candidate.board.hasWon(actualRow, column, symbol)) return false;
        candidate.history.push_back({actualRow, column, player});
    }
    if (candidate.board.isFull()) return false;
    for (int row = 0; row < rows; ++row) {
        std::string line;
        if (!(in >> line) || static_cast<int>(line.size()) != columns) return false;
        for (int col = 0; col < columns; ++col)
            if (line[col] != candidate.board.cells()[row][col]) return false;
    }
    std::string trailing;
    if (in >> trailing || in.bad()) return false;

    candidate.current = turn;
    destination = std::move(candidate);
    return true;
}
} // namespace gym
