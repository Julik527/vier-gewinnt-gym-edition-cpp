// JK AXIS | GOLD MASTER
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
    return ::isatty(fileno(stdout)) && term != nullptr && std::string(term) != "dumb";
#endif
}

bool ansiEnabled() {
    static const bool value = terminalSupportsAnsi();
    return value;
}

std::string paint(const std::string& text, const char* code) {
    if (!ansiEnabled()) return text;
    return std::string("\x1b[") + code + "m" + text + "\x1b[0m";
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

void separator(std::ostringstream& out) {
    out << "  +" << std::string(WIDTH - 2, '-') << "+\n";
}

void menuLine(std::ostringstream& out, const std::string& text,
              const char* accent = nullptr) {
    // Die Abstaende kommen VOR der Farbe. So bleibt der Rahmen gerade.
    std::ostringstream cell;
    cell << std::left << std::setw(WIDTH - 4) << text;
    out << "  | " << (accent == nullptr ? cell.str() : paint(cell.str(), accent))
        << " |\n";
}

int score(const std::map<std::string, int>& scores, const std::string& name) {
    const auto found = scores.find(name);
    return found == scores.end() ? 0 : found->second;
}

bool winningCell(const std::vector<std::pair<int, int>>& cells,
                 int row, int col) {
    for (const auto& cell : cells) {
        if (cell.first == row && cell.second == col) return true;
    }
    return false;
}

void boardArea(std::ostringstream& out, const Board& board,
               int previewColumn, char previewSymbol,
               const std::vector<std::pair<int, int>>& win, int animationRow) {
    out << "\n     ";
    for (int col = 1; col <= board.columns(); ++col) {
        out << std::right << std::setw(6) << col;
    }
    if (animationRow == -1 && previewColumn >= 0) {
        out << "\n     ";
        for (int col = 0; col < board.columns(); ++col)
            out << (col == previewColumn ? " (" + std::string(1, previewSymbol) + ")  " : "      ");
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
                out << paint(" (" + std::string(1, previewSymbol) + ") ", "1;96") << '|';
            } else if (symbol == '.') {
                out << "  .  |";
            } else if (winningCell(win, row, col)) {
                out << paint(" {" + std::string(1, symbol) + "} ", "1;92") << '|';
            } else {
                out << paint(" [" + std::string(1, symbol) + "] ", playerColor(symbol)) << '|';
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
    std::cout << "\n  Enter druecken, um zum Hauptmenue zurueckzugehen...";
    std::cout.flush();
    std::string ignored;
    std::getline(std::cin, ignored);
}

void present(const std::string& screen) {
    if (!ansiEnabled()) {
        // Umgeleitete Ausgabe enthaelt keine Steuerzeichen.
        std::cout << '\n' << screen << std::flush;
        return;
    }

    // Nicht zuerst den ganzen Bildschirm leeren: das vermeidet Flackern.
    // Jede kuerzere Zeile wird separat bereinigt, ebenso der Rest unten.
    std::cout << "\x1b[H";
    std::istringstream lines(screen);
    std::string line;
    while (std::getline(lines, line)) {
        std::cout << line << "\x1b[K\n";
    }
    std::cout << "\x1b[J" << std::flush;
}

std::string mainMenu(const std::vector<Player>& players, const Board& board,
                     bool active, int currentPlayer, int round) {
    std::ostringstream out;
    separator(out);
    menuLine(out, "JK AXIS  /  VIER GEWINNT  /  GYM EDITION", "1;94");
    menuLine(out, "HEBEN. SETZEN. SIEGEN.  |  C++17", "1;97");
    menuLine(out, "BERND-BLINDOW-SCHULEN  /  RAVENSBURG");
    separator(out);
    menuLine(out, active ? "STATUS: TRAININGSRUNDE LAEUFT" : "STATUS: BEREIT FUER DEIN MATCH");
    menuLine(out, "1  Neues Spiel");
    menuLine(out, active ? "2  Spiel fortsetzen" : "2  Spiel fortsetzen (keine Runde)");
    menuLine(out, "3  Spielstand laden");
    menuLine(out, "4  Bestenliste");
    menuLine(out, "5  Anleitung");
    menuLine(out, "0  Beenden");
    separator(out);
    if (active && !players.empty() && currentPlayer >= 0 &&
        currentPlayer < static_cast<int>(players.size())) {
        out << "  RUNDE " << round << "  |  " << board.tokenCount() << " Zuege\n"
            << "  SPIELER UND ZUGREIHENFOLGE:\n";
        // Jeder Name bekommt eine eigene Zeile; auch 4 lange Namen passen.
        for (std::size_t i = 0; i < players.size(); ++i) {
            out << (static_cast<int>(i) == currentPlayer ? "  >> " : "     ")
                << '[' << players[i].getSymbol() << "] " << players[i].getName()
                << (static_cast<int>(i) == currentPlayer ? "  < AM ZUG" : "")
                << "\n";
        }
    }
    out << "  Auswahl mit Zahl bestaetigen.\n";
    return out.str();
}

std::string arena(const Board& board, const std::vector<Player>& players,
                  int currentPlayer, int round,
                  const std::map<std::string, int>& highscores,
                  int previewColumn, char previewSymbol,
                  const std::vector<std::pair<int, int>>& winningCells, int animationRow) {
    std::ostringstream out;
    separator(out);
    menuLine(out, "JK AXIS  /  VIER GEWINNT  /  GYM EDITION");
    menuLine(out, "TRAININGS-ARENA  |  RUNDE " + std::to_string(round), "1;97");
    menuLine(out, "BERND-BLINDOW-SCHULEN  /  RAVENSBURG");
    separator(out);

    for (std::size_t i = 0; i < players.size(); ++i) {
        const auto& player = players[i];
        const bool onTurn = static_cast<int>(i) == currentPlayer;
        const int wins = score(highscores, player.getName());
        out << (onTurn ? "  >> " : "     ")
            << paint("[" + std::string(1, player.getSymbol()) + "]",
                     playerColor(player.getSymbol()))
            << ' ' << player.getName() << "  |  "
            << wins << (wins == 1 ? " Sieg" : " Siege")
            << (onTurn ? "  < AM ZUG" : "") << '\n';
    }
    boardArea(out, board, previewColumn, previewSymbol, winningCells, animationRow);
    out << "  " << paint("TRAININGSSTAND", "1;94")
        << "  |  Zug " << board.tokenCount() + (winningCells.empty() ? 1 : 0)
        << "  |  " << board.columns() << " Spalten\n";
    out << "  " << (previewColumn >= 0 ? "VORSCHAU: Noch kein Stein gesetzt.\n"
                                     : "EINGABE: Spalte 1-" + std::to_string(board.columns()) + "\n");
    out << "  U Rueckgaengig  |  V 4 Vorschau  |  S Speichern\n"
        << "  H Hilfe  |  I Info  |  A Animation  |  M Menue\n";
    separator(out);
    return out.str();
}

void animateDrop(const Board& board, const std::vector<Player>& players,
                 int currentPlayer, int round,
                 const std::map<std::string, int>& highscores,
                 int column, int landingRow, char symbol) {
    // Nur die Darstellung wird animiert; Board::drop() folgt genau einmal.
    for (int row = -1; row <= landingRow; ++row) {
        present(arena(board, players, currentPlayer, round, highscores,
                      column, symbol, {}, row));
        if (row < landingRow)
            std::this_thread::sleep_for(std::chrono::milliseconds(45));
    }
}

std::string result(const Board& board, const std::vector<Player>& players,
                   const std::map<std::string, int>& highscores,
                   const std::string& winner, int round,
                   const std::vector<std::pair<int, int>>& winningCells) {
    std::ostringstream out;
    separator(out);
    menuLine(out, "JK AXIS  /  GYM EDITION  /  RUNDENENDE", "1;94");
    menuLine(out, "TRAINING ABGESCHLOSSEN  |  RUNDE " + std::to_string(round));
    menuLine(out, "BERND-BLINDOW-SCHULEN  /  RAVENSBURG");
    separator(out);
    out << "\n  " << (winner.empty() ? "REMIS: Spielfeld voll."
                       : "GEWONNEN: " + winner + "!") << "\n";
    if (!winner.empty()) out << "  Vier Hanteln in einer Reihe!\n";
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
