// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   include/Game.h
// Aufgabe: Beschreibt Spielablauf, Eingaben und den gespeicherten Spielstand.
// ------------------------------------------------------------

#pragma once
#include "Board.h"
#include "Player.h"
#include <map>
#include <string>
#include <vector>

// Ein Move merkt sich, wo ein Stein liegt und welcher Spieler ihn gesetzt hat.
struct Move {
    int row;
    int column;
    int player;
};

// Game steuert das Menü. Board kümmert sich nur um das Spielfeld.
class Game {
private:
    // Einstellung für die nächste Runde.
    int chosenPlayers_ = 2;
    std::vector<std::string> chosenNames_;
    std::vector<Player> players_;
    Board board_;
    // Diese Liste brauchen wir für den Undo-Befehl.
    std::vector<Move> history_;
    // Index des Spielers, der als Nächstes dran ist.
    int current_ = 0;
    // Zeigt an, ob gerade eine Runde läuft.
    bool active_ = false;
    bool animation_ = true;
    std::map<std::string, int> highscores_;

    // Der Startassistent fragt zuerst die Spielerzahl und danach die Namen ab.
    // Beide Schritte lassen sich mit 0 abbrechen, ohne eine Runde zu verändern.
    bool configurePlayers(int& playerCount);
    bool configureNames(int playerCount, std::vector<std::string>& names);
    // Startet eine frische Runde mit leerem Feld.
    void newGame();
    // Hier finden Spielerwechsel und Züge statt.
    void play();
    void showHelp() const;
    void showInfo() const;
    void showHighscores() const;
    // Speichert die aktive Runde in einer Textdatei.
    bool saveGame(const std::string& path = "data/savegame.txt") const;
    // Lädt nur einen vollständig geprüften Spielstand.
    bool loadGame(const std::string& path = "data/savegame.txt");
    // Liest die Siegerliste beim Programmstart ein.
    void loadHighscores();
    bool saveHighscores() const;
    void addWin(const std::string& name);
public:
    Game();
    // Startet das Hauptmenü und reagiert auf die Auswahl.
    void run();
};