/**
 * @file include/GymReplay.h
 * @brief Schnittstelle fuer eine isolierte, schrittweise Zugwiedergabe.
 * @details Legale Zuege werden auf Board-Kopien nachgespielt.
 * @par Abhängigkeiten
 * Board.h und C++17-Standardbibliothek; keine globalen Spielstaende.
 * @par Wichtige Randbedingungen
 * Keine Aenderung von Highscores, Speicherdaten oder vorhandenen Spielen.
 */
#pragma once
#include "Board.h"
#include <cstddef>
#include <vector>

namespace gym {
class MatchReplay {
public:
    explicit MatchReplay(const std::vector<int>& zeroBasedColumns, int players = 2);
    const Board& board() const;
    std::size_t position() const { return cursor_; }
    std::size_t totalMoves() const { return snapshots_.size() - 1; }
    bool next();
    bool previous();
private:
    std::vector<Board> snapshots_;
    std::size_t cursor_ = 0;
};
} // namespace gym
