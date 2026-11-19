/**
 * @file tests/ai_console_ui_tests.cpp
 * @brief Prueft KI-Menue, Arena, Vorschau, Replay und Ergebnisansichten.
 * @details Kontrolliert UI-Texte, Rahmenbreite und unveraenderten Spielfeldzustand.
 * @par Abhängigkeiten
 * AiConsoleUI, Board und C++17.
 * @par Wichtige Randbedingungen
 * Keine Dateien, Highscores oder Benutzerprofile werden geschrieben.
 */
#include "AiConsoleUI.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

namespace {
void require(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error("KI-UI: " + message);
}
void contains(const std::string& screen, const std::string& expected) {
    require(screen.find(expected) != std::string::npos, "Fehlt: " + expected);
}
void checkLines(const std::string& screen) {
    // Jeder gerahmte Menue-Eintrag muss dieselbe ASCII-Breite besitzen.
    const std::string stripped = std::regex_replace(screen, std::regex("\x1b\\[[0-9;]*m"), "");
    std::size_t start = 0;
    while (start < stripped.size()) {
        const auto end = stripped.find('\n', start);
        const auto line = stripped.substr(start, end == std::string::npos ? end : end - start);
        if (line.rfind("  |", 0) == 0 || line.rfind("  +", 0) == 0)
            require(line.size() == 71, "Rahmenbreite ist nicht einheitlich");
        if (end == std::string::npos) break;
        start = end + 1;
    }
}
}
int main() {
    try {
        gym::ai_ui::SessionStats stats{3, 2, 1, 0};
        const auto menu = gym::ai_ui::mainMenu("JULIAN", gym::AiLevel::Normal,
                                                 stats, true, true);
        contains(menu, "JK AXIS");
        contains(menu, "BERND-BLINDOW-SCHULEN");
        contains(menu, "Laufendes Duell fortsetzen");
        contains(menu, "Letzte Partie wiedergeben");
        contains(menu, "JULIAN");
        checkLines(menu);
        const auto empty = gym::ai_ui::mainMenu("SPIELER 1", gym::AiLevel::Easy,
                                                  {}, false, false);
        contains(empty, "keine Partie");
        contains(empty, "noch keine Partie");
        checkLines(empty);
        const auto difficulty = gym::ai_ui::difficultyMenu(gym::AiLevel::Hard);
        contains(difficulty, "Suchtiefe 3");
        contains(difficulty, "Suchtiefe 6");
        checkLines(difficulty);
        const auto help = gym::ai_ui::instructions();
        contains(help, "V4");
        contains(help, "PAUSIEREN");
        checkLines(help);
        Board board(2);
        const auto before = board.cells();
        const auto preview = gym::ai_ui::arena(board, "JULIAN",
                            gym::AiLevel::Normal, stats, true, "", 3);
        contains(preview, "(R)");
        contains(preview, "0/42");
        require(board.cells() == before, "Vorschau muss Board unveraendert lassen");
        const auto replay = gym::ai_ui::arena(board, "JULIAN",
                             gym::AiLevel::Normal, stats, false, "", -1,
                             true, 2, 7);
        contains(replay, "REPLAY");
        contains(replay, "SCHRITT 2 VON 7");
        contains(replay, "N Naechster");
        const auto score = gym::ai_ui::statistics(stats, "JULIAN", gym::AiLevel::Hard);
        contains(score, "GESP. PARTIEN   3");
        contains(score, "DEINE SIEGE     2");
        checkLines(score);
        int row = -1;
        for(int c=0;c<4;++c) require(board.drop(c, 'R', row), "Test Board::drop");
        const auto winning = board.winningCells(row, 3, 'R');
        const auto end = gym::ai_ui::result(board, "JULIAN", gym::AiLevel::Hard,
                                             stats, "R", winning);
        contains(end, "DU GEWINNST!");
        contains(end, "{R}");
        contains(end, "Revanche");
        std::cout << "KI-Konsolenoberflaeche: Menue, Arena, Vorschau, Replay, Hilfe und Ergebnis OK.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
