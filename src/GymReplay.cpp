/**
 * @file src/GymReplay.cpp
 * @brief Rekonstruiert gueltige, nicht destruktive Board-Snapshots.
 * @details Replay ist unabhaengig von GUI, Savegames und Highscores.
 * @par Abhängigkeiten
 * GymReplay.h und C++17-Standardbibliothek.
 * @par Wichtige Randbedingungen
 * Ungueltige Zuege und Zuege nach Spielende werden abgewiesen.
 */
#include "GymReplay.h"
#include <stdexcept>

namespace gym {
MatchReplay::MatchReplay(const std::vector<int>& moves, int players) {
    if (players < 2 || players > 4)
        throw std::invalid_argument("Replay: Spielerzahl muss 2 bis 4 sein.");
    const char symbols[] = {'R', 'B', 'G', 'Y'};
    Board current(players);
    snapshots_.push_back(current);
    bool finished = false;
    for (std::size_t index = 0; index < moves.size(); ++index) {
        if (finished) throw std::invalid_argument("Replay: Zug nach Spielende.");
        const char symbol = symbols[index % static_cast<std::size_t>(players)];
        int row = -1;
        if (!current.drop(moves[index], symbol, row))
            throw std::invalid_argument("Replay: ungueltige oder volle Spalte.");
        snapshots_.push_back(current);
        finished = current.hasWon(row, moves[index], symbol) || current.isFull();
    }
}
const Board& MatchReplay::board() const { return snapshots_.at(cursor_); }
bool MatchReplay::next() {
    if (cursor_ + 1 >= snapshots_.size()) return false;
    ++cursor_;
    return true;
}
bool MatchReplay::previous() {
    if (cursor_ == 0) return false;
    --cursor_;
    return true;
}
} // namespace gym
