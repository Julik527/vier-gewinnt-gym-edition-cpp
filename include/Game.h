/**
 * @file include/Game.h
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
 * @brief Deklariert die Steuerung des vollständigen Konsolenspiels.
 * @details Game verarbeitet Menüs, Spieleranlage, Zugreihenfolge, Revanche, Undo, GYM1-Spielstände und Highscores.
 * @par Abhängigkeiten
 * Board, Player und die getrennte ConsoleUI-Implementierung; keine SFML-Bibliothek.
 * @par Wichtige Randbedingungen
 * Der neue Zustand wird bei ungültigen Eingaben oder defekten Speicherdateien nicht teilweise übernommen.
 */

#pragma once
#include "Board.h"
#include "Player.h"
#include <map>
#include <string>
#include <vector>
#include <utility>

// Ein Move merkt sich, wo ein Stein liegt und welcher Spieler ihn gesetzt hat.
struct Move {
    int row;
    int column;
    int player;
};

// Die Konsolensteuerung verarbeitet Eingaben und Spielzustand; Board enthält
// nur die Regeln. ConsoleUI rendert den Zustand, ohne Züge selbst auszuführen.
class Game {
private:
    // Einstellung für die nächste Runde.
    int chosenPlayers_ = 2;
    std::vector<std::string> chosenNames_;
    std::vector<Player> players_;
    Board board_;
    // Jeder gültige Zug wird protokolliert: Undo und GYM1-Laden benutzen die Reihenfolge.
    std::vector<Move> history_;
    // Index des Spielers, der als Nächstes dran ist.
    int current_ = 0;
    // Zeigt an, ob gerade eine Runde läuft.
    bool active_ = false;
    // Nach Sieg oder Remis zeigt das Hauptprogramm ein Ergebnis-Menue.
    bool resultPending_ = false;
    std::string lastWinner_;
    std::vector<std::pair<int, int>> lastWinningCells_;
    int round_ = 1;
    bool animation_ = false; // Standard: ruhig, ohne Animation.
    std::map<std::string, int> highscores_;

    // Der Startassistent fragt zuerst die Spielerzahl und danach die Namen ab.
    // Beide Schritte lassen sich mit 0 abbrechen, ohne eine Runde zu verändern.
    bool configurePlayers(int& playerCount);
    bool configureNames(int playerCount, std::vector<std::string>& names);
    // Startet eine frische Runde mit leerem Feld.
    void newGame();
    // Hier finden Spielerwechsel und Züge statt.
    void play();
    void rematch();
    int showResultMenu() const;
    void showHelp() const;
    void showInfo() const;
    void showHighscores() const;
    // GYM1-Format: Spieler, Zugliste und Feldzustand; nur für aktive Runden.
    bool saveGame(const std::string& path = "data/savegame.txt") const;
    // Erst alle Daten in temporären Objekten prüfen, dann das laufende Match ersetzen.
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