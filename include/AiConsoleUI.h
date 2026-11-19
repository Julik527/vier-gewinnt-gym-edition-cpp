/**
 * @file include/AiConsoleUI.h
 * @brief Deklariert Menues, Arena, Ergebnis und Replay-Darstellung des KI-Modus.
 * @details Reine Ansichts-Funktionen im JK-AXIS-Konsolenstil ohne Zustandsaenderung.
 * @par Abhängigkeiten
 * Board.h, GymAi.h und C++17-Standardbibliothek.
 * @par Wichtige Randbedingungen
 * Kein Zugriff auf Highscores oder GYM1; Vorschau veraendert Board nicht.
 */
#pragma once

#include "Board.h"
#include "GymAi.h"
#include <string>
#include <utility>
#include <vector>

namespace gym::ai_ui {
struct SessionStats {
    int played = 0;
    int humanWins = 0;
    int aiWins = 0;
    int draws = 0;
};

const char* levelName(AiLevel level);
std::string mainMenu(const std::string& player, AiLevel level,
                     const SessionStats& stats, bool active,
                     bool replayAvailable, const std::string& notice = "");
std::string difficultyMenu(AiLevel level);
std::string nameMenu(const std::string& player);
std::string instructions();
std::string statistics(const SessionStats& stats, const std::string& player,
                       AiLevel level);
std::string arena(const Board& board, const std::string& player, AiLevel level,
                  const SessionStats& stats, bool humanTurn,
                  const std::string& notice = "", int preview = -1,
                  bool replay = false, std::size_t step = 0,
                  std::size_t total = 0,
                  const std::vector<std::pair<int, int>>& winning = {});
std::string result(const Board& board, const std::string& player,
                   AiLevel level, const SessionStats& stats,
                   const std::string& winner,
                   const std::vector<std::pair<int, int>>& winning);
} // namespace gym::ai_ui
