/**
 * @file src/AiConsoleUI.cpp
 * @brief Rendert das KI-Menue im Erscheinungsbild der Gym-Edition-Konsole.
 * @details Nutzt die bestehende ANSI-Farbsteuerung und gepruefte 69-Spalten-Rahmen.
 * @par Abhängigkeiten
 * AiConsoleUI.h, ConsoleUI.h, Board und C++17.
 * @par Wichtige Randbedingungen
 * Ansichten sind rein; keine Schreibzugriffe auf Spielstaende oder KI-Entscheidungen.
 */
#include "AiConsoleUI.h"
#include "ConsoleUI.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

namespace {
constexpr int kWidth = 69;
constexpr int kContent = kWidth - 4;

std::string color(const std::string& value, const char* escape) {
    return console_ui::accent(value, escape);
}
void rule(std::ostringstream& out) {
    out << "  +" << std::string(kWidth - 2, '-') << "+\n";
}
void line(std::ostringstream& out, const std::string& value,
          const char* accent = nullptr) {
    // Limit lengths and pad before coloring, preserving 69-column layout.
    const auto visible = value.substr(0, kContent);
    const auto padded = visible + std::string(kContent - visible.size(), ' ');
    out << "  | " << (accent ? color(padded, accent) : padded) << " |\n";
}
void header(std::ostringstream& out, const std::string& section) {
    rule(out);
    line(out, "JK AXIS  /  VIER GEWINNT  /  GYM EDITION", "1;94");
    line(out, "KI-ARENA  |  MENSCH GEGEN COMPUTER  |  C++17", "1;97");
    line(out, "BERND-BLINDOW-SCHULEN  /  RAVENSBURG");
    rule(out);
    line(out, section, "1;96");
    rule(out);
}
void footer(std::ostringstream& out) { rule(out); }
std::string bar(const Board& board) {
    constexpr int width = 24;
    const int used = board.tokenCount();
    const int capacity = board.rows() * board.columns();
    const int filled = (used * width + capacity / 2) / capacity;
    return "[" + std::string(filled, '#') + std::string(width - filled, '.')
           + "] " + std::to_string(used) + "/" + std::to_string(capacity);
}
bool winnerCell(const std::vector<std::pair<int, int>>& cells,
                int row, int col) {
    return std::find(cells.begin(), cells.end(), std::make_pair(row, col))
           != cells.end();
}
void boardArea(std::ostringstream& out, const Board& board, int preview,
               const std::vector<std::pair<int, int>>& winning) {
    out << "\n     ";
    for (int col = 1; col <= board.columns(); ++col)
        out << std::setw(6) << col;
    out << "\n    +";
    for (int col = 0; col < board.columns(); ++col) out << "-----+";
    out << '\n';
    const int previewRow = board.nextFreeRow(preview);
    for (int r = 0; r < board.rows(); ++r) {
        out << "    |";
        for (int c = 0; c < board.columns(); ++c) {
            const char mark = board.cells()[r][c];
            if (r == previewRow && c == preview)
                out << color(" (R) ", "1;96") << '|';
            else if (mark == '.') out << "  .  |";
            else if (winnerCell(winning, r, c))
                out << color(" {" + std::string(1, mark) + "} ", "1;92") << '|';
            else
                out << color(" [" + std::string(1, mark) + "] ",
                             mark == 'R' ? "1;94" : "1;93") << '|';
        }
        out << '\n';
    }
    out << "    +";
    for (int col = 0; col < board.columns(); ++col) out << "-----+";
    out << '\n';
}
void scoreLine(std::ostringstream& out, const gym::ai_ui::SessionStats& stats) {
    line(out, "TRAININGSBILANZ  |  DU " + std::to_string(stats.humanWins)
         + "  /  KI " + std::to_string(stats.aiWins)
         + "  /  REMIS " + std::to_string(stats.draws));
}
} // namespace

namespace gym::ai_ui {
const char* levelName(AiLevel level) {
    switch (level) {
        case AiLevel::Easy: return "EINFACH";
        case AiLevel::Normal: return "NORMAL";
        case AiLevel::Hard: return "SCHWER";
    }
    return "UNBEKANNT";
}

std::string mainMenu(const std::string& player, AiLevel level,
                     const SessionStats& stats, bool active,
                     bool replayAvailable, const std::string& notice) {
    std::ostringstream out;
    header(out, "MATCH LOBBY  /  " + std::string(active ? "PARTIE PAUSIERT" : "BEREIT"));
    line(out, "SPIELER  " + player + " [R]  |  KI [B]");
    line(out, "SCHWIERIGKEIT  " + std::string(levelName(level)));
    scoreLine(out, stats);
    rule(out);
    line(out, " 01   Neues KI-Duell", "1;97");
    line(out, active ? " 02   Laufendes Duell fortsetzen"
                     : " 02   Duell fortsetzen (keine Partie)");
    line(out, " 03   Schwierigkeit einstellen");
    line(out, " 04   Spielernamen aendern (nur ohne laufende Partie)");
    line(out, " 05   Anleitung / Steuerung");
    line(out, " 06   Trainingsbilanz");
    line(out, replayAvailable ? " 07   Letzte Partie wiedergeben"
                              : " 07   Replay (noch keine Partie)");
    line(out, " 00   Beenden");
    rule(out);
    line(out, notice.empty() ? "ZAHL EINGEBEN + ENTER  |  IM DUELL: M FUER MENUE"
                             : "INFO: " + notice, notice.empty() ? nullptr : "1;93");
    footer(out);
    return out.str();
}

std::string difficultyMenu(AiLevel current) {
    std::ostringstream out;
    header(out, "KI-TRAINING  /  SCHWIERIGKEIT EINSTELLEN");
    line(out, "AKTUELL  " + std::string(levelName(current)));
    rule(out);
    line(out, " 01   EINFACH  /  Sofortsieg, sonst Mitte-zuerst");
    line(out, " 02   NORMAL   /  Minimax, Suchtiefe 4");
    line(out, " 03   SCHWER   /  Minimax, Suchtiefe 7");
    line(out, " 00   Zurueck zum Hauptmenue");
    rule(out);
    line(out, "NEUE EINSTELLUNG GILT AB DEM NAECHSTEN DUELL");
    footer(out);
    return out.str();
}

std::string nameMenu(const std::string& player) {
    std::ostringstream out;
    header(out, "SPIELERPROFIL  /  NAME FESTLEGEN");
    line(out, "AKTUELLER NAME  " + player);
    line(out, "ERLAUBT  /  1 BIS 20 DRUCKBARE ASCII-ZEICHEN");
    line(out, " 00   Zurueck ohne Aenderung");
    footer(out);
    return out.str();
}

std::string instructions() {
    std::ostringstream out;
    header(out, "ANLEITUNG  /  REGELN UND STEUERUNG");
    line(out, "ZIEL: VIER GLEICHE HANTELN IN EINE REIHE SETZEN.");
    line(out, "GEWINN: WAAGRECHT, SENKRECHT ODER DIAGONAL.");
    line(out, "DU SPIELST [R]  /  DIE KI SPIELT [B].");
    rule(out);
    line(out, " 1 BIS 7    HANTEL IN DIE GEWAEHLTE SPALTE SETZEN");
    line(out, " V4         VORSCHAU FUER SPALTE 4 ANZEIGEN");
    line(out, " T          ZUGTIPP (KEIN STEIN WIRD GESETZT)");
    line(out, " U          LETZTES ZUGPAAR RUECKGAENGIG MACHEN");
    line(out, " H          HILFE / REGELN ANZEIGEN");
    line(out, " M          PARTIE PAUSIEREN UND ZUM MENUE");
    line(out, " N / P      REPLAY VOR / ZURUECK");
    line(out, " Q          REPLAY VERLASSEN");
    rule(out);
    line(out, "DIE KI SUCHT LEGALE ZUEGE; KEIN MASCHINELLES LERNEN.");
    line(out, "BILANZ UND REPLAY BLEIBEN NUR IN DIESER SITZUNG.");
    footer(out);
    return out.str();
}

std::string statistics(const SessionStats& stats, const std::string& player,
                       AiLevel level) {
    std::ostringstream out;
    header(out, "TRAININGSBILANZ  /  DIESE SITZUNG");
    line(out, "SPIELER: " + player + "  |  KI-STUFE: " + levelName(level));
    rule(out);
    line(out, "GESP. PARTIEN   " + std::to_string(stats.played));
    line(out, "DEINE SIEGE     " + std::to_string(stats.humanWins));
    line(out, "KI-SIEGE        " + std::to_string(stats.aiWins));
    line(out, "UNENTSCHIEDEN   " + std::to_string(stats.draws));
    rule(out);
    line(out, "NUR IM ARBEITSSPEICHER  /  KEIN HIGHSCORE-ZUGRIFF");
    line(out, "ENTER  /  ZURUECK ZUM HAUPTMENUE");
    footer(out);
    return out.str();
}

std::string arena(const Board& board, const std::string& player, AiLevel level,
                  const SessionStats& stats, bool humanTurn,
                  const std::string& notice, int preview, bool replay,
                  std::size_t step, std::size_t total,
                  const std::vector<std::pair<int, int>>& winning) {
    std::ostringstream out;
    header(out, replay ? "REPLAY  /  SCHRITT " + std::to_string(step)
                          + " VON " + std::to_string(total)
                       : "LIVE MATCH  /  KI-STUFE " + std::string(levelName(level)));
    line(out, "[R] " + player + (humanTurn ? "  < AM ZUG" : "")
         + "    [B] KI-AGENT" + (!humanTurn ? "  < AM ZUG" : ""));
    scoreLine(out, stats);
    boardArea(out, board, preview, winning);
    out << "  " << color("MATCH-FORTSCHRITT", "1;94") << "  " << bar(board) << '\n';
    out << "  " << color("SPIELZUG", "1;96") << "  |  "
        << board.tokenCount() << " Hanteln gesetzt  |  7 Spalten\n";
    if (preview >= 0) out << "  VORSCHAU: Spalte " << preview + 1 << " (noch kein Zug).\n";
    if (!notice.empty()) out << "  HINWEIS: " << notice.substr(0, 58) << '\n';
    if (replay) out << "  N Naechster  |  P Vorheriger  |  Q Zurueck\n";
    else out << "  1-7 Setzen  |  V4 Vorschau  |  T Tipp  |  U Undo\n  H Hilfe  |  M Menue\n";
    footer(out);
    return out.str();
}

std::string result(const Board& board, const std::string& player,
                   AiLevel level, const SessionStats& stats,
                   const std::string& winner,
                   const std::vector<std::pair<int, int>>& winning) {
    std::ostringstream out;
    header(out, "RUNDENENDE  /  KI-ARENA");
    const std::string status = winner == "R" ? "DU GEWINNST!  /  " + player
        : winner == "B" ? "DIE KI GEWINNT!" : "REMIS  /  SPIELFELD VOLL";
    line(out, status, winner == "R" ? "1;92" : "1;93");
    line(out, "STUFE  " + std::string(levelName(level))
         + "  |  RUNDE " + std::to_string(stats.played));
    boardArea(out, board, -1, winning);
    scoreLine(out, stats);
    rule(out);
    line(out, " 01   Revanche  /  gleiche Einstellungen", "1;97");
    line(out, " 02   Schwierigkeit waehlen");
    line(out, " 03   Match-Replay ansehen");
    line(out, " 00   Hauptmenue");
    footer(out);
    return out.str();
}
} // namespace gym::ai_ui
