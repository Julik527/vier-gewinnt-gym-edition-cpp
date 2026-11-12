/**
 * @file include/ConsoleUI.h
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
 * @brief Deklariert die Textansichten und die Terminal-Ausgabe der Schulkonsole.
 * @details mainMenu, arena und result erzeugen Strings; present gibt sie aus, animateDrop simuliert den sichtbaren Fall.
 * @par Abhängigkeiten
 * Board und Player; ANSI-Farben sind optional und für die Regeln nicht erforderlich.
 * @par Wichtige Randbedingungen
 * Vorschau und Animation verändern den Spielfeldzustand nicht.
 */

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
// Laesst Anleitungen und Bestenlisten auf einem echten Terminal sichtbar.
void pauseForReading();

std::string mainMenu(const std::vector<Player>& players, const Board& board,
                     bool active, int currentPlayer, int round);

std::string arena(const Board& board, const std::vector<Player>& players,
                  int currentPlayer, int round,
                  const std::map<std::string, int>& highscores,
                  int previewColumn = -1, char previewSymbol = '.',
                  const std::vector<std::pair<int, int>>& winningCells = {},
                  int animationRow = -2);

// Animiert eine Hantel in mehreren sichtbaren Spielfeld-Frames.
void animateDrop(const Board& board, const std::vector<Player>& players,
                 int currentPlayer, int round,
                 const std::map<std::string, int>& highscores,
                 int column, int landingRow, char symbol);

std::string result(const Board& board, const std::vector<Player>& players,
                   const std::map<std::string, int>& highscores,
                   const std::string& winner, int round,
                   const std::vector<std::pair<int, int>>& winningCells);
} // namespace console_ui
