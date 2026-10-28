// Spielzustand fuer die grafische Ausgabe. Regeln bleiben ausschliesslich im Board.
#pragma once
#include "Board.h"
#include "Player.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

struct GymMove {
    int row;
    int column;
    int player;
};

class MatchSession {
public:
    MatchSession();
    bool begin(const std::vector<std::string>& names);
    bool drop(int zeroBasedColumn);
    bool undo();
    bool rematch();
    bool save(const std::string& path = "data/savegame.txt") const;
    bool load(const std::string& path = "data/savegame.txt");
    bool loadScores(const std::string& path = "data/highscore.txt");
    bool saveScores(const std::string& path = "data/highscore.txt") const;

    const Board& board() const { return board_; }
    const std::vector<Player>& players() const { return players_; }
    const std::vector<std::pair<int, int>>& winLine() const { return winLine_; }
    const std::string& winner() const { return winner_; }
    const std::map<std::string, int>& scores() const { return scores_; }
    int current() const { return current_; }
    int round() const { return round_; }
    bool active() const { return active_; }
    bool finished() const { return finished_; }
    bool scoreSaved() const { return scoreSaved_; }
    int wins(const std::string& name) const;
    static bool validName(const std::string& name);

private:
    Board board_{2};
    std::vector<Player> players_;
    std::vector<GymMove> moves_;
    std::map<std::string, int> scores_;
    std::vector<std::pair<int, int>> winLine_;
    std::string winner_;
    int current_ = 0;
    int round_ = 1;
    bool active_ = false;
    bool finished_ = false;
    bool scoreSaved_ = true;
};
