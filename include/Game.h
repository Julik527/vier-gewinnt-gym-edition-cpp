#pragma once
#include "Board.h"
#include "Player.h"
#include <map>
#include <string>
#include <vector>

struct Move {
    int row;
    int column;
    int player;
};

class Game {
private:
    int chosenPlayers_ = 2;
    std::vector<std::string> chosenNames_;
    std::vector<Player> players_;
    Board board_;
    std::vector<Move> history_;
    int current_ = 0;
    bool active_ = false;
    bool animation_ = true;
    std::map<std::string, int> highscores_;

    void configurePlayers();
    void configureNames();
    void newGame();
    void play();
    void showHelp() const;
    void showInfo() const;
    void showHighscores() const;
    bool saveGame(const std::string& path = "data/savegame.txt") const;
    bool loadGame(const std::string& path = "data/savegame.txt");
    void loadHighscores();
    bool saveHighscores() const;
    void addWin(const std::string& name);
public:
    Game();
    void run();
};