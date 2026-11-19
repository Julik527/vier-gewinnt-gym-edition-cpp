/**
 * @file include/GymAi.h
 * @brief Schnittstelle fuer den optionalen, deterministischen KI-Gegner.
 * @details Drei Schwierigkeitsstufen fuer einen separaten Zweispielermodus.
 * @par Abhängigkeiten
 * Board.h und C++17; keine Grafik- oder Dateizugriffe.
 * @par Wichtige Randbedingungen
 * Nur 7x6-Bretter mit R/B; der Pflicht-Spielmodus bleibt unveraendert.
 */
#pragma once
#include "Board.h"
#include <cstddef>

namespace gym {
enum class AiLevel { Easy, Normal, Hard };
struct AiChoice {
    int column = -1;
    int score = 0;
    std::size_t nodes = 0;
};
AiChoice chooseAiMove(const Board& board, char aiSymbol, AiLevel level);
} // namespace gym
