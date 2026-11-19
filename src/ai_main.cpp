/**
 * @file src/ai_main.cpp
 * @brief Eigenstaendiger C++-Konsolenmodus Mensch gegen KI mit Replay.
 * @details Nutzt den vorhandenen Board-Regelkern; Session-Statistik im RAM.
 * @par Abhängigkeiten
 * GymAi.h, GymReplay.h, Board.h und C++17.
 * @par Wichtige Randbedingungen
 * Keine Schreibzugriffe auf bestehende Schulspielstaende oder Highscores.
 */
#include "GymAi.h"
#include "GymReplay.h"
#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace {
bool read(std::string& value) {
    return static_cast<bool>(std::getline(std::cin, value));
}
bool yes(const std::string& value) { return value == "j" || value == "J"; }

void showReplay(const std::vector<int>& moves) {
    gym::MatchReplay replay(moves);
    std::cout << "\nReplay: n=naechster Zug, p=vorheriger Zug, q=zurueck.\n";
    std::string command;
    for (;;) {
        std::cout << "\nSchritt " << replay.position()
                  << "/" << replay.totalMoves() << "\n";
        replay.board().print();
        std::cout << "Replay > ";
        if (!read(command) || command == "q" || command == "Q") return;
        if (command == "n" || command == "N") replay.next();
        else if (command == "p" || command == "P") replay.previous();
        else std::cout << "Bitte n, p oder q eingeben.\n";
    }
}
} // namespace

int main() {
    try {
        std::cout << "Vier Gewinnt | Gym Edition - KI-Labor (Zusatzprogramm)\n"
                  << "Du bist R, die KI ist B. Stufe 1=Einfach, 2=Normal, 3=Schwer.\n"
                  << "Stufe > ";
        std::string input;
        if (!read(input)) return 0;
        gym::AiLevel level;
        if (input == "1") level = gym::AiLevel::Easy;
        else if (input == "2") level = gym::AiLevel::Normal;
        else if (input == "3") level = gym::AiLevel::Hard;
        else {
            std::cerr << "Unbekannte Stufe. Bitte 1, 2 oder 3 verwenden.\n";
            return 1;
        }

        int playerWins = 0, aiWins = 0, draws = 0;
        for (;;) {
            Board board(2);
            std::vector<int> moves;
            bool humanTurn = true;
            bool finished = false;
            while (!finished) {
                std::cout << '\n';
                board.print();
                int column = -1;
                if (humanTurn) {
                    std::cout << "Spalte 1-7 (q beendet) > ";
                    if (!read(input) || input == "q" || input == "Q") return 0;
                    if (input.size() != 1 || input[0] < '1' || input[0] > '7') {
                        std::cout << "Bitte eine Zahl von 1 bis 7 eingeben.\n";
                        continue;
                    }
                    column = input[0] - '1';
                } else {
                    const auto choice = gym::chooseAiMove(board, 'B', level);
                    column = choice.column;
                    if (column < 0) {
                        std::cerr << "Keine legale KI-Aktion vorhanden.\n";
                        return 2;
                    }
                    std::cout << "KI waehlt Spalte " << (column + 1)
                              << " (" << choice.nodes << " Suchpositionen).\n";
                }
                const char symbol = humanTurn ? 'R' : 'B';
                int row = -1;
                if (!board.drop(column, symbol, row)) {
                    std::cout << "Die Spalte ist voll. Waehle eine andere.\n";
                    continue;
                }
                moves.push_back(column);
                if (board.hasWon(row, column, symbol)) {
                    std::cout << (humanTurn ? "Du gewinnst!\n" : "Die KI gewinnt!\n");
                    if (humanTurn) ++playerWins;
                    else ++aiWins;
                    finished = true;
                } else if (board.isFull()) {
                    std::cout << "Unentschieden.\n";
                    ++draws;
                    finished = true;
                }
                humanTurn = !humanTurn;
            }
            board.print();
            std::cout << "Bilanz (nur diese Sitzung): Du " << playerWins
                      << " | KI " << aiWins << " | Remis " << draws << '\n';
            std::cout << "Replay anschauen? (j/n) > ";
            if (!read(input)) return 0;
            if (yes(input)) showReplay(moves);
            std::cout << "Neue Runde? (j/n) > ";
            if (!read(input) || !yes(input)) break;
        }
    } catch (const std::exception& ex) {
        std::cerr << "KI-Labor beendet: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
