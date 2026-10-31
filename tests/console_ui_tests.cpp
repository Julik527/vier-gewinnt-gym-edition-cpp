// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   tests/console_ui_tests.cpp
// Aufgabe: Prueft die sichtbaren Texte der neuen Trainings-Arena.
// ------------------------------------------------------------

#include "Board.h"
#include "ConsoleUI.h"
#include "Player.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

int count(const std::string& text, const std::string& token) {
    int result = 0;
    std::size_t position = 0;
    while ((position = text.find(token, position)) != std::string::npos) {
        ++result;
        position += token.size();
    }
    return result;
}

void put(Board& board, int col, char symbol) {
    int row = -1;
    check(board.drop(col, symbol, row), "Hantel konnte nicht gesetzt werden");
}
}

int main() {
    try {
        Board board(2);
        const std::vector<Player> players = {
            Player("Ada", 'R'), Player("Ben", 'B')
        };
        const std::map<std::string, int> scores = {
            {"Ada", 3}, {"Ben", 1}
        };

        // Der Startbildschirm soll klare Aktionen und den aktuellen Status haben.
        const std::string main = console_ui::mainMenu(players, board, false, 0, 1);
        check(main.find("JK AXIS") != std::string::npos, "Marke fehlt");
        check(main.find("GYM EDITION") != std::string::npos, "Gym Edition fehlt");
        check(main.find("keine Runde") != std::string::npos, "Fortsetzen-Zustand fehlt");

        const std::string initial = console_ui::arena(board, players, 0, 1, scores);
        check(initial.find("TRAININGS-ARENA") != std::string::npos, "Arena-Titel fehlt");
        check(initial.find("Ada") != std::string::npos, "Spieler 1 fehlt");
        check(initial.find("Ben") != std::string::npos, "Spieler 2 fehlt");
        check(initial.find("3 Siege") != std::string::npos, "Punktestand fehlt");
        check(count(initial, "|  .  ") == 42, "Leeres Feld nicht vollstaendig");
        check(initial.find("AM ZUG") != std::string::npos, "Aktueller Spieler fehlt");
        check(initial.find("Rueckgaengig") != std::string::npos, "Bedienhilfe fehlt");

        const std::string preview = console_ui::arena(board, players, 0, 1, scores, 2, 'R');
        check(preview.find(" (R) ") != std::string::npos, "Vorschau fehlt");
        const std::string inAir = console_ui::arena(board, players, 0, 1, scores,
                                                    2, 'R', {}, -1);
        const std::string falling = console_ui::arena(board, players, 0, 1, scores,
                                                      2, 'R', {}, 2);
        check(inAir.find(" (R) ") != std::string::npos, "Start ueber Feld fehlt");
        check(falling.find(" (R) ") != std::string::npos, "Bewegter Stein fehlt");
        check(inAir != falling, "Fallanimation hat keine verschiedenen Frames");
        check(board.tokenCount() == 0, "Animation aendert Spielregeln");
        check(board.tokenCount() == 0, "Vorschau hat das Feld veraendert");

        // Ein echter Sieg muss im Endbildschirm sichtbar markiert sein.
        for (int col = 0; col < 4; ++col) put(board, col, 'R');
        const auto winner = board.winningCells(5, 3, 'R');
        check(winner.size() == 4, "Gewinnlinie falsch berechnet");
        const std::string result = console_ui::result(board, players, scores, "Ada", 1, winner);
        check(result.find("GEWONNEN: Ada!") != std::string::npos, "Gewinnmeldung fehlt");
        check(count(result, " {R} ") == 4, "Gewinn-Hanteln nicht markiert");
        check(result.find("Revanche") != std::string::npos, "Revanche fehlt");
        check(result.find("3 Siege") != std::string::npos, "Gesamtsiege fehlen");

        // Bei 4 Spielern muss das grosse Feld ohne abgeschnittene Spalten passen.
        Board large(4);
        const std::vector<Player> four = {
            Player("A", 'R'), Player("B", 'B'),
            Player("C", 'G'), Player("D", 'Y')
        };
        const std::string biggest = console_ui::arena(large, four, 3, 2, {});
        check(biggest.find("11") != std::string::npos, "Spalte 11 fehlt");
        check(count(biggest, "|  .  ") == 110, "11x10 Feld fehlerhaft");
        check(biggest.find("D") != std::string::npos, "Spieler 4 fehlt");

        std::cout << "ConsoleUI: Menue, 2/4 Spieler, Vorschau, "
                  << "Gewinnmarkierung, Bestenliste und Bedienhinweise OK.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "ConsoleUI-Test fehlgeschlagen: " << error.what() << "\n";
        return 1;
    }
}
