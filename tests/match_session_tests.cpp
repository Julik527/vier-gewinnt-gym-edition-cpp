// Tests of the shared match state; no SFML or graphical display is required.
#include "MatchSession.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

namespace {
void verify(bool value, const char* what) {
    if (!value) throw std::runtime_error(what);
}
void win(MatchSession& match) {
    for (int column : {0, 6, 1, 6, 2, 6, 3})
        verify(match.drop(column), "valid winning move");
}
}

int main() {
    namespace fs = std::filesystem;
    const auto original = fs::current_path();
    const auto temp = fs::temp_directory_path() /
        ("gym_match_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    try {
        fs::create_directories(temp);
        fs::current_path(temp);
        MatchSession session;
        verify(!session.begin({"Duplicate", "Duplicate"}), "duplicate names rejected");
        verify(!session.begin({"   ", "Ben"}), "blank names rejected");
        verify(!session.begin({"Hi\nESC", "Other"}), "control char rejected");
        verify(session.begin({"Ada", "Ben"}), "new game");
        verify(session.board().columns() == 7 && session.board().rows() == 6, "dimensions");
        verify(session.board().nextFreeRow(0) == 5, "preview does not place a stone");
        verify(!session.drop(-1), "negative column rejected");
        verify(session.drop(0), "first move");
        verify(session.current() == 1, "player switches");
        verify(session.undo(), "undo works");
        verify(session.current() == 0 && session.board().tokenCount() == 0, "undo restores state");
        verify(session.drop(2), "drop before save");
        verify(session.save(), "save GYM1");
        std::ifstream file("data/savegame.txt");
        std::string signature;
        file >> signature;
        file.close();
        verify(signature == "GYM1", "format compatibility");

        MatchSession loaded;
        verify(loaded.load(), "load saved game");
        verify(loaded.current() == 1 && loaded.board().tokenCount() == 1, "load move/turn");
        verify(loaded.board().cells()[5][2] == 'R', "loaded board");
        verify(!loaded.begin({"Ada", "Ada"}), "invalid new game leaves state intact");
        verify(loaded.board().tokenCount() == 1, "state preserved after bad setup");
        {
            std::ofstream broken("data/corrupt.txt");
            broken << "GYM1\n2 0 BAD";
        }
        verify(!loaded.load("data/corrupt.txt"), "reject corrupted save");
        verify(loaded.board().tokenCount() == 1, "load failure leaves state intact");

        // An existing file survives a failed replacement.
        {
            std::ofstream sentinel("data/original.txt");
            sentinel << "KEEP";
        }
        fs::create_directory("data/original.txt.tmp");
        verify(!loaded.save("data/original.txt"), "failed atomic save rejected");
        {
            std::ifstream old("data/original.txt");
            std::string content;
            old >> content;
            verify(content == "KEEP", "original safe on write failure");
        }
        fs::remove("data/original.txt.tmp");

        verify(loaded.begin({"Ada", "Ben"}), "new round");
        win(loaded);
        verify(loaded.finished() && !loaded.active(), "win ends game");
        verify(loaded.winner() == "Ada" && loaded.winLine().size() == 4, "winner cells");
        verify(loaded.wins("Ada") == 1, "one victory credit");
        verify(!loaded.drop(4), "no extra moves after victory");
        verify(loaded.wins("Ada") == 1, "no duplicate credit");
        verify(loaded.rematch(), "rematch");
        verify(loaded.active() && loaded.current() == 0 && loaded.board().tokenCount() == 0,
               "rematch reset");
        verify(loaded.round() == 2 && loaded.players()[0].getName() == "Ada",
               "names and round preserved");
        win(loaded);
        verify(loaded.wins("Ada") == 2, "two games two credits");

        // Ein blockierter Highscore-Schreibvorgang darf alte Daten nicht zerstoeren.
        {
            std::ifstream before("data/highscore.txt", std::ios::binary);
            const std::string previous((std::istreambuf_iterator<char>(before)),
                                       std::istreambuf_iterator<char>());
            fs::create_directory("data/highscore.txt.tmp");
            verify(loaded.rematch(), "third round");
            win(loaded);
            verify(!loaded.scoreSaved(), "failed highscore write reported");
            std::ifstream after("data/highscore.txt", std::ios::binary);
            const std::string current((std::istreambuf_iterator<char>(after)),
                                      std::istreambuf_iterator<char>());
            verify(current == previous, "previous score file unchanged after failure");
            fs::remove("data/highscore.txt.tmp");
        }


        MatchSession fresh;
        verify(fresh.wins("Ada") == 2, "score survives process restart");
        verify(fresh.begin({"A", "B", "C", "D"}), "four people");
        verify(fresh.board().columns() == 11 && fresh.board().rows() == 10,
               "large board");
        fs::current_path(original);
        fs::remove_all(temp);
        std::cout << "MatchSession: GYM1, invalid files, atomic save, 2/4 players, "
                     "undo, rematch and highscore passed.\n";
        return 0;
    } catch (const std::exception& ex) {
        fs::current_path(original);
        fs::remove_all(temp);
        std::cerr << "MatchSession test: " << ex.what() << '\n';
        return 1;
    }
}
