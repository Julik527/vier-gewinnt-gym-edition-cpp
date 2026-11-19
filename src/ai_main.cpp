/**
 * @file src/ai_main.cpp
 * @brief Eigenstaendige KI-Konsole mit Gym-Edition-Menue und Replay.
 * @details Verwendet denselben Regelkern und die Terminalausgabe wie die
 *          Pflichtkonsole, schreibt aber keine Schulspielstaende oder Scores.
 */
#include "AiConsoleUI.h"
#include "ConsoleUI.h"
#include "GymAi.h"
#include "GymReplay.h"
#include <algorithm>
#include <cctype>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
using gym::AiLevel;
using gym::ai_ui::SessionStats;

enum class Screen { Menu, Match, Result, Exit };

std::string trim(const std::string& input) {
    const auto first = std::find_if_not(input.begin(), input.end(),
        [](unsigned char ch) { return std::isspace(ch) != 0; });
    if (first == input.end()) return "";
    const auto last = std::find_if_not(input.rbegin(), input.rend(),
        [](unsigned char ch) { return std::isspace(ch) != 0; }).base();
    return std::string(first, last);
}

std::string lower(std::string text) {
    for (auto& ch : text) ch = static_cast<char>(
        std::tolower(static_cast<unsigned char>(ch)));
    return text;
}

bool read(const char* prompt, std::string& value) {
    std::cout << "  " << prompt << std::flush;
    if (!std::getline(std::cin, value)) return false;
    value = trim(value);
    return true;
}

bool isChoice(const std::string& entered, int choice) {
    return entered == std::to_string(choice)
        || (choice < 10 && entered == "0" + std::to_string(choice));
}

bool validName(const std::string& name) {
    if (name.empty() || name.size() > 20) return false;
    for (unsigned char ch : name)
        if (ch < 32 || ch > 126) return false;
    return true;
}

int previewColumn(const std::string& input) {
    // Akzeptiert "v4" und "v 4", veraendert das Spielfeld aber nie.
    const auto text = lower(input);
    if (text.size() >= 2 && text.front() == 'v') {
        const auto number = trim(text.substr(1));
        if (number.size() == 1 && number[0] >= '1' && number[0] <= '7')
            return number[0] - '1';
    }
    return -1;
}

class AiConsoleGame {
    std::string player_ = "SPIELER 1";
    AiLevel level_ = AiLevel::Normal;
    AiLevel roundLevel_ = AiLevel::Normal;
    AiLevel lastLevel_ = AiLevel::Normal;
    std::string lastPlayer_ = "SPIELER 1";
    SessionStats stats_;
    Board board_{2};
    std::vector<int> moves_;
    std::vector<int> lastMoves_;
    std::vector<std::pair<int, int>> winning_;
    std::string winner_;
    std::string notice_;
    bool active_ = false;

    void newMatch() {
        board_ = Board(2);
        moves_.clear();
        winning_.clear();
        winner_.clear();
        notice_.clear();
        roundLevel_ = level_;
        active_ = true;
    }

    bool confirmAbandon() {
        std::string answer;
        if (!read("Laufende Partie verwerfen? (j/n) > ", answer))
            return false;
        return lower(answer) == "j" || lower(answer) == "ja";
    }

    bool chooseLevel() {
        for (;;) {
            console_ui::present(gym::ai_ui::difficultyMenu(level_));
            std::string choice;
            if (!read("STUFE > ", choice)) return false;
            if (isChoice(choice, 0)) return true;
            if (isChoice(choice, 1)) level_ = AiLevel::Easy;
            else if (isChoice(choice, 2)) level_ = AiLevel::Normal;
            else if (isChoice(choice, 3)) level_ = AiLevel::Hard;
            else {
                std::cout << "  Bitte 1, 2, 3 oder 0 eingeben.\n";
                continue;
            }
            notice_ = active_ ? "Neue Stufe gilt erst ab dem naechsten Duell."
                              : "Schwierigkeit auf " + std::string(gym::ai_ui::levelName(level_)) + " gesetzt.";
            return true;
        }
    }

    bool chooseName() {
        for (;;) {
            console_ui::present(gym::ai_ui::nameMenu(player_));
            std::string name;
            if (!read("NAME > ", name)) return false;
            if (name == "00") return true;
            if (!validName(name)) {
                std::cout << "  Bitte 1-20 sichtbare Zeichen eingeben.\n";
                continue;
            }
            player_ = name;
            notice_ = "Spielername aktualisiert.";
            return true;
        }
    }

    bool showInfo(const std::string& screen) {
        console_ui::present(screen);
        std::string ignored;
        return read("ENTER FUER ZURUECK > ", ignored);
    }

    void finish(const std::string& winner, int row, int column, char symbol) {
        active_ = false;
        winner_ = winner;
        if (!winner.empty()) winning_ = board_.winningCells(row, column, symbol);
        else winning_.clear();
        lastMoves_ = moves_;
        lastLevel_ = roundLevel_;
        lastPlayer_ = player_;
        ++stats_.played;
        if (winner == "R") ++stats_.humanWins;
        else if (winner == "B") ++stats_.aiWins;
        else ++stats_.draws;
    }

    bool put(int column, char symbol) {
        int row = -1;
        if (!board_.drop(column, symbol, row)) return false;
        moves_.push_back(column);
        if (board_.hasWon(row, column, symbol)) finish(std::string(1, symbol), row, column, symbol);
        else if (board_.isFull()) finish("", row, column, symbol);
        return true;
    }

    bool showReplay() {
        if (lastMoves_.empty()) return true;
        gym::MatchReplay replay(lastMoves_);
        std::string note = "N / P blaettern, Q schliesst das Replay.";
        for (;;) {
            const bool humanTurn = replay.position() % 2 == 0;
            console_ui::present(gym::ai_ui::arena(
                replay.board(), lastPlayer_, lastLevel_, stats_, humanTurn,
                note, -1, true, replay.position(), replay.totalMoves()));
            std::string command;
            if (!read("REPLAY > ", command)) return false;
            command = lower(command);
            if (command == "q" || command == "0") return true;
            if (command == "n") {
                note = replay.next() ? "Naechster Zug." : "Ende der Partie erreicht.";
            } else if (command == "p") {
                note = replay.previous() ? "Vorheriger Zug." : "Replay bereits am Anfang.";
            } else note = "Bitte N, P oder Q eingeben.";
        }
    }

    Screen menu() {
        for (;;) {
            console_ui::present(gym::ai_ui::mainMenu(player_, level_, stats_,
                                active_, !lastMoves_.empty(), notice_));
            notice_.clear();
            std::string choice;
            if (!read("AUSWAHL > ", choice)) return Screen::Exit;
            if (isChoice(choice, 0)) return Screen::Exit;
            if (isChoice(choice, 1)) {
                if (active_) {
                    if (!confirmAbandon()) {
                        if (std::cin.eof()) return Screen::Exit;
                        notice_ = "Laufende Partie bleibt erhalten.";
                        continue;
                    }
                }
                newMatch();
                return Screen::Match;
            }
            if (isChoice(choice, 2)) {
                if (active_) return Screen::Match;
                notice_ = "Keine laufende Partie. Waehle 01 fuer ein Duell.";
            } else if (isChoice(choice, 3)) {
                if (!chooseLevel()) return Screen::Exit;
            } else if (isChoice(choice, 4)) {
                if (!chooseName()) return Screen::Exit;
            } else if (isChoice(choice, 5)) {
                if (!showInfo(gym::ai_ui::instructions())) return Screen::Exit;
            } else if (isChoice(choice, 6)) {
                if (!showInfo(gym::ai_ui::statistics(stats_, player_, level_)))
                    return Screen::Exit;
            } else if (isChoice(choice, 7)) {
                if (lastMoves_.empty()) notice_ = "Noch keine abgeschlossene Partie.";
                else if (!showReplay()) return Screen::Exit;
            } else notice_ = "Bitte eine der angezeigten Menuezahlen eingeben.";
        }
    }

    Screen match() {
        for (;;) {
            console_ui::present(gym::ai_ui::arena(board_, player_, roundLevel_,
                                                   stats_, true, notice_));
            notice_.clear();
            std::string command;
            if (!read("SPALTE 1-7 / V4 / H / M > ", command)) return Screen::Exit;
            const auto action = lower(command);
            if (action == "m") {
                notice_ = "Partie pausiert. Mit 02 fortsetzen.";
                return Screen::Menu;
            }
            if (action == "h") {
                if (!showInfo(gym::ai_ui::instructions())) return Screen::Exit;
                continue;
            }
            if (const int preview = previewColumn(action); preview >= 0) {
                if (board_.nextFreeRow(preview) < 0) notice_ = "Diese Spalte ist voll.";
                else {
                    console_ui::present(gym::ai_ui::arena(
                        board_, player_, roundLevel_, stats_, true,
                        "Vorschau, noch kein Stein gesetzt.", preview));
                    if (!read("ENTER FUER ZUG > ", command)) return Screen::Exit;
                }
                continue;
            }
            if (action.size() != 1 || action[0] < '1' || action[0] > '7') {
                notice_ = "Ungueltige Eingabe. Bitte Spalte 1-7 waehlen.";
                continue;
            }
            const int column = action[0] - '1';
            if (!put(column, 'R')) {
                notice_ = "Spalte voll. Bitte andere Spalte waehlen.";
                continue;
            }
            if (!active_) return Screen::Result;
            const auto choice = gym::chooseAiMove(board_, 'B', roundLevel_);
            if (choice.column < 0 || !put(choice.column, 'B'))
                throw std::runtime_error("KI konnte keinen legalen Zug setzen.");
            notice_ = "KI setzt Spalte " + std::to_string(choice.column + 1)
                    + " (" + std::to_string(choice.nodes) + " Suchpositionen).";
            if (!active_) return Screen::Result;
        }
    }

    Screen result() {
        for (;;) {
            console_ui::present(gym::ai_ui::result(board_, player_, roundLevel_,
                                                    stats_, winner_, winning_));
            std::string choice;
            if (!read("AUSWAHL > ", choice)) return Screen::Exit;
            if (isChoice(choice, 0)) return Screen::Menu;
            if (isChoice(choice, 1)) {
                newMatch();
                return Screen::Match;
            }
            if (isChoice(choice, 2)) {
                if (!chooseLevel()) return Screen::Exit;
                return Screen::Menu;
            }
            if (isChoice(choice, 3)) {
                if (!showReplay()) return Screen::Exit;
            } else std::cout << "  Bitte 0, 1, 2 oder 3 waehlen.\n";
        }
    }

public:
    void run() {
        Screen screen = Screen::Menu;
        while (screen != Screen::Exit) {
            switch (screen) {
                case Screen::Menu: screen = menu(); break;
                case Screen::Match: screen = match(); break;
                case Screen::Result: screen = result(); break;
                case Screen::Exit: return;
            }
        }
    }
};
} // namespace

int main() {
    try {
        AiConsoleGame game;
        game.run();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "KI-Arena beendet: " << error.what() << '\n';
        return 1;
    }
}
