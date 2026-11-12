/**
 * @file tests/match_session_tests.cpp
 * @author Julian Krauß
 * @date 2026-11-12 (Dokumentationsstand)
 * @par Projekt
 * Vier Gewinnt | Gym Edition (C++17)
 * @par Schule
 * Bernd-Blindow-Schule Ravensburg
 * @par Klasse
 * ITA25/9
 * @par Unterricht
 * Programmiertechnik (C++)
 * @par Fachlehrer
 * Dr. Litera
 * @brief Prüft den gemeinsamen Spielzustand der 2D- und 3D-Version.
 * @details Testet Spieleranlage, Zugfolge, Undo, Sieg, Revanche, Highscores, GYM1 und fehlerhafte Speicherdaten.
 * @par Abhängigkeiten
 * MatchSession.h und Standardbibliothek; unabhängig von SFML/OpenGL.
 * @par Wichtige Randbedingungen
 * Testdateien liegen im temporären Verzeichnis und sollen echte Spielstände nicht verändern.
 */

#include "MatchSession.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

namespace {
void verify(bool value, const char* what) {
    if (!value) throw runtime_error(what);
}
// Bekannte Zugfolge: Rot gewinnt horizontal, Blau besetzt Spalte 7.
// Damit können Sieg, Highscore und Revanche deterministisch getestet werden.
void win(MatchSession& match) {
    for (int column : {0, 6, 1, 6, 2, 6, 3})
        verify(match.drop(column), "valid winning move");
}
}

int main() {
    namespace fs = std::filesystem;
    // Testdaten in ein separates temporäres Verzeichnis schreiben,
    // damit reale lokale Spielstände nicht überschrieben werden.
    const auto original = fs::current_path();
    const auto temp = fs::temp_directory_path() /
        ("gym_match_" + to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
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
        ifstream file("data/savegame.txt");
        string signature;
        file >> signature;
        file.close();
        verify(signature == "GYM1", "format compatibility");

        MatchSession loaded;
        verify(loaded.load(), "load saved game");
        verify(loaded.current() == 1 && loaded.board().tokenCount() == 1, "load move/turn");
        verify(loaded.board().cells()[5][2] == 'R', "loaded board");
        // Regression gegen Windows-Pfadprobleme: Ü und ß müssen beim
        // Speichern und Laden über std::filesystem::u8path funktionieren.
        const string unicodeSave = "data/unicode-\xC3\x9C-\xC3\x9F.txt";
        verify(loaded.save(unicodeSave), "save to UTF-8 path");
        MatchSession unicodeLoaded;
        verify(unicodeLoaded.load(unicodeSave), "load from UTF-8 path");
        verify(unicodeLoaded.current() == 1
               && unicodeLoaded.board().cells()[5][2] == 'R',
               "UTF-8 saved state preserved");
        verify(!loaded.begin({"Ada", "Ada"}), "invalid new game leaves state intact");
        verify(loaded.board().tokenCount() == 1, "state preserved after bad setup");
        {
            ofstream broken("data/corrupt.txt");
            broken << "GYM1" << endl << "2 0 BAD";
        }
        verify(!loaded.load("data/corrupt.txt"), "reject corrupted save");
        verify(loaded.board().tokenCount() == 1, "load failure leaves state intact");
        // Reject untrusted unbounded files before quoted() can allocate
        // arbitrarily long strings. Existing matches stay intact.
        {
            ofstream oversized("data/oversized.txt", ios::binary);
            oversized << "GYM1\n" << string(65536, ' ');
        }
        verify(!loaded.load("data/oversized.txt"), "oversized save rejected");
        verify(loaded.board().tokenCount() == 1,
               "oversized file does not change the active match");

        // An existing file survives a failed replacement.
        {
            ofstream sentinel("data/original.txt");
            sentinel << "KEEP";
        }
        verify(!loaded.save("data"), "replacing an existing directory rejected");
        {
            ifstream old("data/original.txt");
            string content;
            old >> content;
            verify(content == "KEEP", "original safe on write failure");
        }
        verify(loaded.save("data/original.txt"), "valid existing file can be replaced");

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
        const string unicodeScore = "data/highscore-\xC3\x9F.txt";
        verify(loaded.saveScores(unicodeScore), "save scores to UTF-8 path");
        MatchSession unicodeScores;
        verify(unicodeScores.loadScores(unicodeScore), "load scores from UTF-8 path");
        verify(unicodeScores.wins("Ada") == 2, "UTF-8 score round-trip");

        // A stale .tmp directory from an old application cannot block new writes.
        fs::create_directory("data/highscore.txt.tmp");
        verify(loaded.saveScores(), "unique temporary file avoids stale .tmp");
        fs::remove("data/highscore.txt.tmp");

        // Failed target replacement must preserve the previous highscore file.
        {
            ifstream before("data/highscore.txt", std::ios::binary);
            const string previous((istreambuf_iterator<char>(before)),
                                       istreambuf_iterator<char>());
            verify(!loaded.saveScores("data"), "cannot replace an existing directory");
            ifstream after("data/highscore.txt", std::ios::binary);
            const string current((istreambuf_iterator<char>(after)),
                                      istreambuf_iterator<char>());
            verify(current == previous, "previous score file unchanged after failure");
        }

        // Reject a truncated score without modifying the in-memory ranking.
        {
            ofstream broken("data/truncated-scores.txt");
            broken << R"("Ada" 999)" << endl << R"("Incomplete")";
            broken.close();
            verify(!loaded.loadScores("data/truncated-scores.txt"),
                   "truncated highscore rejected");
            verify(loaded.wins("Ada") == 2, "invalid scores keep previous results");
        }


        MatchSession fresh;
        verify(fresh.wins("Ada") == 2, "score survives process restart");
        verify(fresh.begin({"A", "B", "C", "D"}), "four people");
        verify(fresh.board().columns() == 11 && fresh.board().rows() == 10,
               "large board");
        // INT_MAX is valid legacy data and must never wrap on a new win.
        {
            ofstream maximum("data/maximum-scores.txt");
            maximum << R"("Ada" )" << numeric_limits<int>::max() << '\n';
        }
        MatchSession maximumMatch;
        verify(maximumMatch.loadScores("data/maximum-scores.txt"), "load maximum highscore");
        verify(maximumMatch.begin({"Ada", "Ben"}), "start at highscore limit");
        win(maximumMatch);
        verify(maximumMatch.wins("Ada") == numeric_limits<int>::max(),
               "victory at INT_MAX does not overflow");

        // The outcome is credited exactly once even if disk access fails,
        // then the same score can be persisted after fixing the directory.
        {
            fs::remove("data/highscore.txt");
            fs::create_directory("data/highscore.txt");
            MatchSession pending;
            verify(pending.begin({"Ada", "Ben"}), "begin score retry game");
            win(pending);
            verify(!pending.scoreSaved(), "failed automatic score save is visible");
            verify(pending.wins("Ada") == 1, "win credited once in memory");
            verify(!pending.retryPendingScoreSave(), "retry fails while blocked");
            fs::remove("data/highscore.txt");
            verify(pending.retryPendingScoreSave(), "retry succeeds after unblocking");
            verify(pending.scoreSaved(), "score save status now true");
            verify(pending.wins("Ada") == 1, "retry must not award twice");
            MatchSession reloaded;
            verify(reloaded.wins("Ada") == 1, "retry survives process restart");
        }

        fs::current_path(original);
        fs::remove_all(temp);
        cout << "MatchSession: GYM1, invalid files, atomic save, 2/4 players, "
                     "undo, rematch and highscore passed.\n";
        return 0;
    } catch (const exception& ex) {
        fs::current_path(original);
        fs::remove_all(temp);
        cerr << "MatchSession test: " << ex.what() << '\n';
        return 1;
    }
}
