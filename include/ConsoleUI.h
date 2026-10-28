// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   include/ConsoleUI.h
// Aufgabe: Einheitliche Bildschirme fuer Menue, Spielfeld und Ergebnis.
// ------------------------------------------------------------

#pragma once

#include "Board.h"
#include "Player.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace console_ui {

// Ein Spiel bleibt ein Konsolenprogramm. Farben sind nur eine Ergaenzung.
void present(const std::string& screen);

std::string mainMenu(const std::vector<Player>& players, const Board& board,
                     bool active, int currentPlayer, int round);

std::string arena(const Board& board, const std::vector<Player>& players,
                  int currentPlayer, int round,
                  const std::map<std::string, int>& highscores,
                  int previewColumn = -1, char previewSymbol = '.',
                  const std::vector<std::pair<int, int>>& winningCells = {});

std::string result(const Board& board, const std::vector<Player>& players,
                   const std::map<std::string, int>& highscores,
                   const std::string& winner, int round,
                   const std::vector<std::pair<int, int>>& winningCells);
} // namespace console_ui
