/**
 * @file tests/ai_replay_tests.cpp
 * @brief Testet KI-Siege, Blockzuege, Replay und Eingabegrenzen.
 * @details Deterministische Pruefungen werfen auch im Release-Build bei Fehlern.
 * @par Abhängigkeiten
 * GymAi, GymReplay, Board und C++17.
 * @par Wichtige Randbedingungen
 * Keine Dateien oder nutzerspezifischen Highscores werden angefasst.
 */
#include "GymAi.h"
#include "GymReplay.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool ok, const std::string& description) {
    if (!ok) throw std::runtime_error("KI/REPLAY FAILED: " + description);
}
template <typename Function>
void requireInvalid(Function fn, const std::string& description) {
    bool thrown = false;
    try { fn(); } catch (const std::invalid_argument&) { thrown = true; }
    require(thrown, description);
}
void put(Board& board, int column, char symbol) {
    int row = -1;
    require(board.drop(column, symbol, row), "Test-Vorbereitung: Board::drop");
}
} // namespace

int main() {
    try {
        Board empty(2);
        for (auto level : {gym::AiLevel::Easy, gym::AiLevel::Normal, gym::AiLevel::Hard}) {
            const auto move = gym::chooseAiMove(empty, 'B', level);
            require(move.column >= 0 && move.column < 7, "Alle Stufen finden einen legalen Zug");
            require(move.nodes > 0, "Jede Stufe meldet Suchpositionen");
        }
        require(gym::chooseAiMove(empty, 'B', gym::AiLevel::Hard).column
                    == gym::chooseAiMove(empty, 'B', gym::AiLevel::Hard).column,
                "Gleiche Position ergibt gleiche Entscheidung");
        requireInvalid([&] { (void)gym::chooseAiMove(Board(3), 'B', gym::AiLevel::Hard); },
                       "KI verweigert Mehrspielerbrett");
        requireInvalid([&] { (void)gym::chooseAiMove(empty, 'G', gym::AiLevel::Normal); },
                       "KI verweigert unbekanntes Symbol");
        requireInvalid([&] {
            (void)gym::chooseAiMove(empty, 'B', static_cast<gym::AiLevel>(123));
        }, "KI verweigert unbekannte Schwierigkeit");

        Board win(2);
        put(win, 0, 'B'); put(win, 1, 'B'); put(win, 2, 'B');
        const auto before = win.cells();
        require(gym::chooseAiMove(win, 'B', gym::AiLevel::Easy).column == 3,
                "Einfach nutzt Sofortsieg");
        require(gym::chooseAiMove(win, 'B', gym::AiLevel::Normal).column == 3,
                "Normal nimmt unmittelbaren Sieg");
        require(gym::chooseAiMove(win, 'B', gym::AiLevel::Hard).column == 3,
                "Schwer nimmt unmittelbaren Sieg");
        require(win.cells() == before, "KI darf Ausgangsboard nicht veraendern");

        Board block(2);
        put(block, 0, 'R'); put(block, 1, 'R'); put(block, 2, 'R');
        require(gym::chooseAiMove(block, 'B', gym::AiLevel::Normal).column == 3,
                "Normal blockiert unmittelbare Niederlage");
        require(gym::chooseAiMove(block, 'B', gym::AiLevel::Hard).column == 3,
                "Schwer blockiert unmittelbare Niederlage");
        put(block, 3, 'R');
        require(gym::chooseAiMove(block, 'B', gym::AiLevel::Hard).column == -1,
                "Kein weiterer Zug auf gewonnenem Board");

        // Vertikale Bedrohung und volle Mittelspalte.
        Board vertical(2);
        for (int i = 0; i < 3; ++i) put(vertical, 5, 'R');
        const auto verticalBefore = vertical.cells();
        for (auto level : {gym::AiLevel::Normal, gym::AiLevel::Hard})
            require(gym::chooseAiMove(vertical, 'B', level).column == 5,
                    "Vertikale Bedrohung blockiert");
        require(vertical.cells() == verticalBefore, "Suchbaum laesst Board unveraendert");
        Board fullColumn(2);
        for (int i = 0; i < 6; ++i) put(fullColumn, 3, i % 2 ? 'B' : 'R');
        const auto fullBefore = fullColumn.cells();
        for (auto level : {gym::AiLevel::Easy, gym::AiLevel::Normal, gym::AiLevel::Hard}) {
            const auto choice = gym::chooseAiMove(fullColumn, 'B', level);
            require(choice.column >= 0 && choice.column != 3, "Keine volle Spalte");
            require(fullColumn.cells() == fullBefore, "Keine Mutation durch Suchbaum");
        }
        // Zweifache unmittelbare Bedrohung: kein scheinbar neutraler Score.
        Board fork(2);
        put(fork, 1, 'R'); put(fork, 2, 'R'); put(fork, 3, 'R');
        require(gym::chooseAiMove(fork, 'B', gym::AiLevel::Hard).score < -900000,
                "Erzwungene Niederlage bei Doppelbedrohung erkannt");

        const std::vector<int> game{3, 2, 3, 2, 3, 2, 3};
        gym::MatchReplay replay(game);
        require(replay.position() == 0 && replay.totalMoves() == 7, "Replay startet leer");
        require(replay.board().tokenCount() == 0, "Replay-Anfang ohne Stein");
        for (int i = 1; i <= 7; ++i) {
            require(replay.next(), "Replay-Vorwaertsschritt");
            require(replay.board().tokenCount() == i, "Ein Stein pro Schritt");
        }
        require(replay.board().hasWon(2, 3, 'R'), "Replay bildet Gewinn ab");
        require(!replay.next(), "Replay nach letzter Position stoppen");
        require(replay.previous() && replay.position() == 6, "Replay zurueck");
        require(replay.board().tokenCount() == 6, "Zurueck ohne Zerstörung");
        require(replay.next() && replay.board().hasWon(2, 3, 'R'),
                "Vorwaerts reproduziert gleiche Gewinnposition");

        gym::MatchReplay noMoves({});
        require(!noMoves.next() && !noMoves.previous(), "Leere Zugfolge erlaubt");
        gym::MatchReplay four({0, 1, 2, 3}, 4);
        for (int i = 0; i < 4; ++i) require(four.next(), "Vier-Spieler-Replay");
        require(four.board().tokenCount() == 4, "Vier Spieler korrekt wiedergegeben");
        requireInvalid([&] { gym::MatchReplay invalid({-1}); }, "Negative Spalte");
        requireInvalid([&] { gym::MatchReplay invalid({7}); }, "Spalte ueber Grenze");
        requireInvalid([&] { gym::MatchReplay invalid({3,2,3,2,3,2,3,4}); },
                       "Replay verweigert Zuege nach Sieg");
        requireInvalid([&] { gym::MatchReplay invalid({}, 1); },
                       "Replay verweigert Spielerzahl 1");

        std::cout << "KI und Replay: alle Taktik-, Grenz- und Zustandspruefungen bestanden.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
