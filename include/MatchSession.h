// Spielzustand für 2D und 3D. Beide GUIs rufen dieselben Methoden auf;
// Regeln und Zugprüfung bleiben in Board, nicht im Grafikcode.
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
    // Startet nur mit 2 bis 4 gültigen, unterschiedlichen Namen.
    // Bei ungültiger Eingabe bleibt die bisherige Session unverändert.
    bool begin(const std::vector<std::string>& names);
    // Ein Zug ist nur während eines aktiven Spiels erlaubt.
    // Sieg/Remis beenden die Runde, sonst wechselt der aktuelle Spieler.
    bool drop(int zeroBasedColumn);
    // Setzt nur den letzten Zug einer noch laufenden Runde zurück.
    bool undo();
    bool rematch();
    // Schreibvorgänge verwenden eine temporäre Datei; GYM1 bleibt kompatibel.
    bool save(const std::string& path = "data/savegame.txt") const;
    // Prüft die komplette Zugfolge durch Nachspielen; keine Teilübernahme bei Fehlern.
    bool load(const std::string& path = "data/savegame.txt");
    bool loadScores(const std::string& path = "data/highscore.txt");
    bool saveScores(const std::string& path = "data/highscore.txt") const;
    // Wiederholt einen fehlgeschlagenen Dateischreibvorgang, ohne den
    // bereits im Speicher vergebenen Sieg nochmals hochzuzählen.
    bool retryPendingScoreSave();

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
    // false: Gewinn im Speicher gezählt, aber noch nicht auf Platte gesichert.
    bool scoreSaved_ = true;
};
