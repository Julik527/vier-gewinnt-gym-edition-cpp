// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   src/ConsoleUI.cpp
// Aufgabe: Zeichnet die Trainings-Arena und zeigt klare Spielinformationen.
// ------------------------------------------------------------

#include "ConsoleUI.h"
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <cstdio>
#include <unistd.h>
#endif

using namespace std;

namespace {
constexpr int WIDTH = 69; // Rahmen so breit wie das Feld mit 11 Spalten.

bool terminalSupportsAnsi() {
    if (std::getenv("NO_COLOR") != nullptr) return false;
#ifdef _WIN32
    HANDLE terminal = GetStdHandle(STD_OUTPUT_HANDLE);
    if (terminal == INVALID_HANDLE_VALUE || terminal == nullptr) return false;
    DWORD flags = 0;
    if (!GetConsoleMode(terminal, &flags)) return false;
    if ((flags & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0) return true;
    return SetConsoleMode(terminal, flags | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
#else
    const char* term = std::getenv("TERM");
    return ::isatty(fileno(stdout)) && term != nullptr && string(term) != "dumb";
#endif
}

bool ansiEnabled() {
    static const bool value = terminalSupportsAnsi();
    return value;
}

string paint(const string& text, const char* code) {
    if (!ansiEnabled()) return text;
    return string("\x1b[") + code + "m" + text + "\x1b[0m";
}

const char* playerColor(char symbol) {
    switch (symbol) {
        case 'R': return "1;94"; // Blau
        case 'B': return "1;93"; // Gold
        case 'G': return "1;92"; // Gruen
        case 'Y': return "1;95"; // Violett
        default: return "1;97";
    }
}

void separator(ostringstream& out) {
    out << "  +" << string(WIDTH - 2, '-') << "+\n";
}

void menuLine(ostringstream& out, const string& text,
              const char* accent = nullptr) {
    // Die Abstaende kommen VOR der Farbe. So bleibt der Rahmen gerade.
    ostringstream cell;
    cell << std::left << setw(WIDTH - 4) << text;
    out << "  | " << (accent == nullptr ? cell.str() : paint(cell.str(), accent))
        << " |\n";
}

// Compact progress readout works on terminals without ANSI colors.
string progressBar(const Board& board) {
    constexpr int segments = 24;
    const int capacity = board.rows() * board.columns();
    const int used = board.tokenCount();
    const int filled = capacity > 0 ? (used * segments + capacity / 2) / capacity : 0;
    return "[" + string(filled, '#') + string(segments - filled, '.') + "] "
        + to_string(used) + "/" + to_string(capacity);
}

int score(const map<string, int>& scores, const string& name) {
    const auto found = scores.find(name);
    return found == scores.end() ? 0 : found->second;
}

bool winningCell(const vector<pair<int, int>>& cells,
                 int row, int col) {
    for (const auto& cell : cells) {
        if (cell.first == row && cell.second == col) return true;
    }
    return false;
}

void boardArea(ostringstream& out, const Board& board,
               int previewColumn, char previewSymbol,
               const vector<pair<int, int>>& win, int animationRow) {
    out << "\n     ";
    for (int col = 1; col <= board.columns(); ++col) {
        out << std::right << setw(6) << col;
    }
    if (animationRow == -1 && previewColumn >= 0) {
        out << "\n     ";
        for (int col = 0; col < board.columns(); ++col)
            out << (col == previewColumn ? " (" + string(1, previewSymbol) + ")  " : "      ");
    }
    out << "\n    +";
    for (int col = 0; col < board.columns(); ++col) out << "-----+";
    out << "\n";

    const int previewRow = animationRow >= 0
        ? animationRow : (animationRow == -1 || previewColumn < 0
                          ? -1 : board.nextFreeRow(previewColumn));
    for (int row = 0; row < board.rows(); ++row) {
        out << "    |";
        for (int col = 0; col < board.columns(); ++col) {
            const char symbol = board.cells()[row][col];
            if (row == previewRow && col == previewColumn) {
                out << paint(" (" + string(1, previewSymbol) + ") ", "1;96") << '|';
            } else if (symbol == '.') {
                out << "  .  |";
            } else if (winningCell(win, row, col)) {
                out << paint(" {" + string(1, symbol) + "} ", "1;92") << '|';
            } else {
                out << paint(" [" + string(1, symbol) + "] ", playerColor(symbol)) << '|';
            }
        }
        out << "\n";
    }
    out << "    +";
    for (int col = 0; col < board.columns(); ++col) out << "-----+";
    out << "\n";
}
} // namespace

namespace console_ui {

void pauseForReading() {
    // Wenn die Eingabe aus einer Testdatei kommt, niemals auf Enter warten.
#ifdef _WIN32
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD flags = 0;
    const bool interactive = input != INVALID_HANDLE_VALUE && input != nullptr
                             && GetConsoleMode(input, &flags) != 0;
#else
    const bool interactive = ::isatty(fileno(stdin)) != 0
                             && ::isatty(fileno(stdout)) != 0;
#endif
    if (!interactive) return;
    cout << "\n  Enter druecken, um zum Hauptmenue zurueckzugehen...";
    cout.flush();
    string ignored;
    getline(cin, ignored);
}

void present(const string& screen) {
    if (!ansiEnabled()) {
        // Umgeleitete Ausgabe enthaelt keine Steuerzeichen.
        cout << '\n' << screen << flush;
        return;
    }

    // Nicht zuerst den ganzen Bildschirm leeren: das vermeidet Flackern.
    // Jede kuerzere Zeile wird separat bereinigt, ebenso der Rest unten.
    cout << "\x1b[H";
    istringstream lines(screen);
    string line;
    while (getline(lines, line)) {
        cout << line << "\x1b[K\n";
    }
    cout << "\x1b[J" << flush;
}

string mainMenu(const vector<Player>& players, const Board& board,
                     bool active, int currentPlayer, int round) {
    ostringstream out;
    separator(out);
    menuLine(out, "JK AXIS  /  VIER GEWINNT  /  GYM EDITION", "1;94");
    menuLine(out, "KONSOLE  |  HEBEN. SETZEN. SIEGEN.  |  C++17", "1;97");
    menuLine(out, "BERND-BLINDOW-SCHULEN  /  RAVENSBURG");
    separator(out);
    menuLine(out, active ? "LIVE MATCH  /  RUNDE " + to_string(round)
                         : "MATCH LOBBY  /  BEREIT FUER DEIN MATCH",
             active ? "1;92" : "1;96");
    menuLine(out, active ? "FORTSCHRITT  " + progressBar(board)
                         : "2-4 SPIELER  |  4 IN EINER REIHE");
    separator(out);
    menuLine(out, " 01   Neues Spiel / Spieler waehlen", "1;97");
    menuLine(out, active ? " 02   Spiel fortsetzen" : " 02   Spiel fortsetzen (keine Runde)");
    menuLine(out, " 03   Spielstand laden");
    menuLine(out, " 04   Bestenliste");
    menuLine(out, " 05   Anleitung / Regeln und Steuerung");
    menuLine(out, " 00   Beenden");
    separator(out);
    if (active && !players.empty() && currentPlayer >= 0 &&
        currentPlayer < static_cast<int>(players.size())) {
        menuLine(out, "TEAM  /  ZUGREIHENFOLGE", "1;94");
        for (size_t i = 0; i < players.size(); ++i) {
            const bool now = static_cast<int>(i) == currentPlayer;
            menuLine(out, string(now ? " > " : "   ") + "[" + players[i].getSymbol()
                     + "] " + players[i].getName() + (now ? "  < AM ZUG" : ""),
                     now ? "1;92" : nullptr);
        }
        separator(out);
    }
    menuLine(out, "ZAHL EINGEBEN + ENTER  |  IM SPIEL: H FUER HILFE");
    separator(out);
    return out.str();
}

string arena(const Board& board, const vector<Player>& players,
                  int currentPlayer, int round,
                  const map<string, int>& highscores,
                  int previewColumn, char previewSymbol,
                  const vector<pair<int, int>>& winningCells, int animationRow) {
    ostringstream out;
    separator(out);
    menuLine(out, "JK AXIS  /  VIER GEWINNT  /  GYM EDITION");
    menuLine(out, "TRAININGS-ARENA  |  RUNDE " + to_string(round), "1;97");
    menuLine(out, "BERND-BLINDOW-SCHULEN  /  RAVENSBURG");
    separator(out);

    for (size_t i = 0; i < players.size(); ++i) {
        const auto& player = players[i];
        const bool onTurn = static_cast<int>(i) == currentPlayer;
        const int wins = score(highscores, player.getName());
        out << (onTurn ? "  >> " : "     ")
            << paint("[" + string(1, player.getSymbol()) + "]",
                     playerColor(player.getSymbol()))
            << ' ' << player.getName() << "  |  "
            << wins << (wins == 1 ? " Sieg" : " Siege")
            << (onTurn ? "  < AM ZUG" : "") << '\n';
    }
    boardArea(out, board, previewColumn, previewSymbol, winningCells, animationRow);
    out << "  " << paint("TRAININGSSTAND / MATCH-FORTSCHRITT", "1;94") << "  "
        << progressBar(board) << "\n";
    out << "  " << paint("SPIELZUG", "1;96")
        << "  |  " << (winningCells.empty() ? "Naechster Zug: " : "Letzter Zug: ")
        << board.tokenCount() + (winningCells.empty() ? 1 : 0)
        << "  |  " << board.columns() << " Spalten\n";
    out << "  " << (previewColumn >= 0 ? "VORSCHAU: Noch kein Stein gesetzt.\n"
                                     : "EINGABE: Spalte 1-" + to_string(board.columns()) + "\n");
    out << "  U Rueckgaengig  |  V 4 Vorschau  |  S Speichern\n"
        << "  H Hilfe  |  I Info  |  A Animation  |  M Menue\n";
    separator(out);
    return out.str();
}

void animateDrop(const Board& board, const vector<Player>& players,
                 int currentPlayer, int round,
                 const map<string, int>& highscores,
                 int column, int landingRow, char symbol) {
    // Nur die Darstellung wird animiert; Board::drop() folgt genau einmal.
    for (int row = -1; row <= landingRow; ++row) {
        present(arena(board, players, currentPlayer, round, highscores,
                      column, symbol, {}, row));
        if (row < landingRow)
            std::this_thread::sleep_for(std::chrono::milliseconds(45));
    }
}

string result(const Board& board, const vector<Player>& players,
                   const map<string, int>& highscores,
                   const string& winner, int round,
                   const vector<pair<int, int>>& winningCells) {
    ostringstream out;
    separator(out);
    menuLine(out, "JK AXIS  /  GYM EDITION  /  RUNDENENDE", "1;94");
    menuLine(out, "TRAINING ABGESCHLOSSEN  |  RUNDE " + to_string(round));
    menuLine(out, "BERND-BLINDOW-SCHULEN  /  RAVENSBURG");
    separator(out);
    out << "\n  " << (winner.empty() ? "REMIS: Spielfeld voll."
                       : "GEWONNEN: " + winner + "!") << "\n";
    if (!winner.empty()) out << "  Vier Hanteln in einer Reihe!\n";
    out << "  SPIELSTAND  " << progressBar(board) << "\n";
    boardArea(out, board, -1, '.', winningCells, -2);
    out << "  GESAMTSIEGE\n";
    for (const auto& player : players) {
        const int wins = score(highscores, player.getName());
        out << "  [" << player.getSymbol() << "] " << player.getName() << ": "
            << wins << (wins == 1 ? " Sieg\n" : " Siege\n");
    }
    separator(out);
    menuLine(out, "1  Revanche (gleiche Spieler)");
    menuLine(out, "2  Neues Spiel (Spieler neu waehlen)");
    menuLine(out, "0  Hauptmenue");
    separator(out);
    return out.str();
}
} // namespace console_ui
