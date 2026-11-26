/**
 * @file src/MatchSession.cpp
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
 * @brief Implementiert den gemeinsamen Match-Zustand der beiden GUIs.
 * @details Validiert Spieler und Züge, erkennt Sieg/Remis, speichert GYM1 und verwaltet Highscores.
 * @par Abhängigkeiten
 * MatchSession.h, Board, AtomicFile, ScoreFile und GameFileLimits; ohne SFML.
 * @par Wichtige Randbedingungen
 * Eine beschädigte Datei wird nicht übernommen; ein nicht gespeicherter Sieg kann ohne Doppelzählung erneut geschrieben werden.
 */

#include "MatchSession.h"
#include "AtomicFile.h"
#include "ScoreFile.h"
#include "GameFileLimits.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <utility>

using namespace std;

MatchSession::MatchSession() {
    error_code error;
    std::filesystem::create_directories("data", error);
    loadScores();
}

bool MatchSession::validName(const string& name) {
    return gym::validPlayerName(name);
}

// Zuerst Spielerzahl, Namen und Duplikate prüfen. Erst danach eine neue
// Session anlegen, damit ein abgebrochener Start das Match nicht verwirft.
bool MatchSession::begin(const vector<string>& names) {
    if (names.size() < 2 || names.size() > 4) return false;
    for (size_t i = 0; i < names.size(); ++i) {
        if (!validName(names[i])) return false;
        if (std::find(names.begin(), names.begin() + static_cast<std::ptrdiff_t>(i), names[i])
            != names.begin() + static_cast<std::ptrdiff_t>(i)) return false;
    }
    vector<Player> next;
    for (size_t i = 0; i < names.size(); ++i)
        next.emplace_back(names[i], gym::playerSymbols[i]);
    players_ = std::move(next);
    board_ = Board(static_cast<int>(names.size()));
    moves_.clear();
    winLine_.clear();
    winner_.clear();
    current_ = 0;
    round_ = 1;
    active_ = true;
    finished_ = false;
    scoreSaved_ = true;
    return true;
}

// Erfolgreichen Zug in die Historie übernehmen und die drei möglichen
// Folgezustände behandeln: Sieg, Remis oder Spielerwechsel.
bool MatchSession::drop(int column) {
    if (!active_ || finished_) return false;
    const auto result = gym::playMove(board_, players_, moves_, current_, column);
    if (result == gym::TurnResult::Invalid) return false;
    if (result == gym::TurnResult::Win) {
        finished_ = true;
        active_ = false;
        const auto& last = moves_.back();
        winner_ = players_[current_].getName();
        winLine_ = board_.winningCells(last.row, last.column, players_[current_].getSymbol());
        auto& victories = scores_[winner_];
        if (victories < numeric_limits<int>::max()) ++victories;
        scoreSaved_ = saveScores();
    } else if (result == gym::TurnResult::Draw) {
        finished_ = true;
        active_ = false;
        winner_.clear();
        winLine_.clear();
    }
    return true;
}

bool MatchSession::undo() {
    return active_ && gym::undoMove(board_, moves_, current_);
}

bool MatchSession::rematch() {
    if (!finished_ || players_.empty()) return false;
    board_ = Board(static_cast<int>(players_.size()));
    moves_.clear();
    winLine_.clear();
    winner_.clear();
    current_ = 0;
    ++round_;
    active_ = true;
    finished_ = false;
    scoreSaved_ = true;
    return true;
}

// Wiederholt NUR das Schreiben. Der Sieg wurde bei drop() bereits gezählt.
bool MatchSession::retryPendingScoreSave() {
    if(scoreSaved_) return true;
    if(!finished_ || winner_.empty()) return false;
    scoreSaved_=saveScores();
    return scoreSaved_;
}

int MatchSession::wins(const string& name) const {
    const auto found = scores_.find(name);
    return found == scores_.end() ? 0 : found->second;
}

bool MatchSession::save(const string& path) const {
    return active_ && gym::writeGame(path, board_, players_, moves_, current_);
}

bool MatchSession::load(const string& path) {
    gym::SavedGame saved;
    if (!gym::readGame(path, saved)) return false;

    board_ = std::move(saved.board);
    players_ = std::move(saved.players);
    moves_ = std::move(saved.history);
    current_ = saved.current;
    round_ = 1;
    active_ = true;
    finished_ = false;
    winner_.clear();
    winLine_.clear();
    scoreSaved_ = true;
    return true;
}

bool MatchSession::loadScores(const string& path) {
    ifstream in(std::filesystem::u8path(path));
    if (!in) return false;
    map<string, int> next;
    if (!gym::parseScores(in, next, validName)) return false;
    scores_ = std::move(next);
    return true;
}

bool MatchSession::saveScores(const string& path) const {
    return gym::writeScores(path, scores_);
}
