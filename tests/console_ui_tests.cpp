// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   tests/console_ui_tests.cpp
// Aufgabe: Prüft Textdarstellung und Bedienhinweise ohne interaktives Terminal.
// Regeln und Grafik sind getrennt, deshalb lassen sich Screens als Strings testen.
// ------------------------------------------------------------

#include "Board.h"
#include "ConsoleUI.h"
#include "Player.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw runtime_error(message);
}

int count(const string& text, const string& token) {
    int result = 0;
    size_t position = 0;
    while ((position = text.find(token, position)) != string::npos) {
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
        const vector<Player> players = {
            Player("Ada", 'R'), Player("Ben", 'B')
        };
        const map<string, int> scores = {
            {"Ada", 3}, {"Ben", 1}
        };

        // Die sichtbaren Texte dienen hier als überprüfbarer UI-Vertrag:
        // Start, Fortsetzen, Hilfe und Exit müssen erkennbar bleiben.
        const string main = console_ui::mainMenu(players, board, false, 0, 1);
        check(main.find("JK AXIS") != string::npos, "Marke fehlt");
        check(main.find("GYM EDITION") != string::npos, "Gym Edition fehlt");
        check(main.find("keine Runde") != string::npos, "Fortsetzen-Zustand fehlt");
        check(main.find("MATCH LOBBY") != string::npos, "Lobby-Kennzeichnung fehlt");
        check(main.find("Anleitung / Regeln und Steuerung") != string::npos,
              "Hilfe-Einstieg fehlt");
        check(main.find("00   Beenden") != string::npos, "Ausgang fehlt");
        const string resumed = console_ui::mainMenu(players, board, true, 1, 3);
        check(resumed.find("LIVE MATCH  /  RUNDE 3") != string::npos,
              "Live-Match-Status fehlt");
        check(resumed.find("[........................] 0/42") != string::npos,
              "Fortschritt der neuen Partie fehlt");

        const string initial = console_ui::arena(board, players, 0, 1, scores);
        check(initial.find("TRAININGS-ARENA") != string::npos, "Arena-Titel fehlt");
        check(initial.find("Ada") != string::npos, "Spieler 1 fehlt");
        check(initial.find("Ben") != string::npos, "Spieler 2 fehlt");
        check(initial.find("3 Siege") != string::npos, "Punktestand fehlt");
        check(count(initial, "|  .  ") == 42, "Leeres Feld nicht vollstaendig");
        check(initial.find("AM ZUG") != string::npos, "Aktueller Spieler fehlt");
        check(initial.find("Rueckgaengig") != string::npos, "Bedienhilfe fehlt");
        check(initial.find("MATCH-FORTSCHRITT") != string::npos,
              "Fortschrittsanzeige fehlt");
        check(initial.find("0/42") != string::npos, "Leerer Fortschritt fehlt");

        const string preview = console_ui::arena(board, players, 0, 1, scores, 2, 'R');
        check(preview.find(" (R) ") != string::npos, "Vorschau fehlt");
        const string inAir = console_ui::arena(board, players, 0, 1, scores,
                                                    2, 'R', {}, -1);
        const string falling = console_ui::arena(board, players, 0, 1, scores,
                                                      2, 'R', {}, 2);
        check(inAir.find(" (R) ") != string::npos, "Start ueber Feld fehlt");
        check(falling.find(" (R) ") != string::npos, "Bewegter Stein fehlt");
        check(inAir != falling, "Fallanimation hat keine verschiedenen Frames");
        check(board.tokenCount() == 0, "Animation aendert Spielregeln");
        check(board.tokenCount() == 0, "Vorschau hat das Feld veraendert");

        // Ein echter Sieg muss im Endbildschirm sichtbar markiert sein.
        for (int col = 0; col < 4; ++col) put(board, col, 'R');
        const auto winner = board.winningCells(5, 3, 'R');
        check(winner.size() == 4, "Gewinnlinie falsch berechnet");
        const string result = console_ui::result(board, players, scores, "Ada", 1, winner);
        check(result.find("GEWONNEN: Ada!") != string::npos, "Gewinnmeldung fehlt");
        check(count(result, " {R} ") == 4, "Gewinn-Hanteln nicht markiert");
        check(result.find("Revanche") != string::npos, "Revanche fehlt");
        check(result.find("3 Siege") != string::npos, "Gesamtsiege fehlen");
        check(result.find("4/42") != string::npos,
              "Ergebnis-Fortschritt fehlt");

        // Bei 4 Spielern muss das grosse Feld ohne abgeschnittene Spalten passen.
        Board large(4);
        const vector<Player> four = {
            Player("A", 'R'), Player("B", 'B'),
            Player("C", 'G'), Player("D", 'Y')
        };
        const string biggest = console_ui::arena(large, four, 3, 2, {});
        check(biggest.find("11") != string::npos, "Spalte 11 fehlt");
        check(count(biggest, "|  .  ") == 110, "11x10 Feld fehlerhaft");
        check(biggest.find("D") != string::npos, "Spieler 4 fehlt");

        cout << "ConsoleUI: Menue, 2/4 Spieler, Vorschau, "
                  << "Gewinnmarkierung, Bestenliste und Bedienhinweise OK.\n";
        return 0;
    } catch (const exception& error) {
        cerr << "ConsoleUI-Test fehlgeschlagen: " << error.what() << "\n";
        return 1;
    }
}
