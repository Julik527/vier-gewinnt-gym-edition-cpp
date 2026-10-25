#include "Game.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
const char SYMBOLS[] = {'R', 'B', 'G', 'Y'};
std::string trim(const std::string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

bool wholeInt(const std::string& raw, int& result) {
    std::istringstream in(trim(raw));
    char extra;
    return (in >> result) && !(in >> extra);
}

std::string ask(const std::string& message) {
    std::cout << message;
    std::string line;
    if (!std::getline(std::cin, line)) throw std::runtime_error("Eingabe wurde beendet.");
    return trim(line);
}

bool validName(const std::string& name) {
    return !name.empty() && name.size() <= 24
        && name.find('\n') == std::string::npos
        && name.find('\r') == std::string::npos;
}
}

Game::Game() : board_(2) {
    chosenNames_.resize(2);
    std::error_code error;
    std::filesystem::create_directories("data", error);
    if (error) std::cerr << "Warnung: Datenordner nicht verfuegbar: " << error.message() << "\n";
    loadHighscores();
}

void Game::configurePlayers() {
    int number;
    for (;;) {
        if (wholeInt(ask("Spieleranzahl [2-4]: "), number)
            && number >= 2 && number <= 4) break;
        std::cout << "Bitte nur 2, 3 oder 4 eingeben.\n";
    }
    chosenPlayers_ = number;
    chosenNames_.assign(number, "");
    std::cout << "Naechstes neues Spiel: " << number << " Spieler.\n";
}

void Game::configureNames() {
    chosenNames_.assign(chosenPlayers_, "");
    for (int i = 0; i < chosenPlayers_; ++i) {
        for (;;) {
            std::string name = ask("Name fuer Spieler " + std::to_string(i + 1) + ": ");
            bool duplicate = std::find(chosenNames_.begin(), chosenNames_.end(), name)
                             != chosenNames_.end();
            if (validName(name) && !duplicate) {
                chosenNames_[i] = name;
                break;
            }
            std::cout << "Name darf nicht leer/doppelt sein (maximal 24 Zeichen).\n";
        }
    }
}

void Game::newGame() {
    bool namesMissing = static_cast<int>(chosenNames_.size()) != chosenPlayers_;
    for (const auto& name : chosenNames_) if (!validName(name)) namesMissing = true;
    if (namesMissing) configureNames();
    players_.clear();
    for (int i = 0; i < chosenPlayers_; ++i) {
        players_.emplace_back(chosenNames_[i], SYMBOLS[i]);
    }
    board_ = Board(chosenPlayers_);
    history_.clear();
    current_ = 0;
    active_ = true;
    std::cout << "\nNeues Spiel: " << board_.columns() << " Spalten x "
              << board_.rows() << " Reihen.\n";
    play();
}

void Game::showHelp() const {
    std::cout << "\nHILFE | GYM EDITION\n"
              << "Zahl: Kurzhantel in Spalte fallen lassen (1-" << board_.columns() << ")\n"
              << "V 4: Vorschau fuer Spalte 4 (passt sich an)\n"
              << "U: letzten gueltigen Zug rueckgaengig machen\n"
              << "S: aktuellen Spielstand speichern | I: Information\n"
              << "H: diese Hilfe | A: Animation ein/aus | M: zum Hauptmenue\n"
              << "Vier eigene Kurzhanteln horizontal, vertikal oder diagonal gewinnen.\n";
}

void Game::showInfo() const {
    std::cout << "\nINFO: " << board_.columns() << " Spalten, " << board_.rows()
              << " Reihen; belegte Felder: " << board_.tokenCount()
              << "/" << board_.rows() * board_.columns() << "\n";
    for (const auto& p : players_) {
        std::cout << "  =[" << p.getSymbol() << "]=  " << p.getName() << "\n";
    }
}

void Game::play() {
    while (active_) {
        board_.print();
        std::cout << "Am Zug: " << players_[current_].getName() << " =["
                  << players_[current_].getSymbol() << "]=\n";
        std::string command = ask("Spalte oder Befehl (H=Hilfe): ");
        std::string upper = command;
        for (char& ch : upper) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        if (upper == "H") { showHelp(); continue; }
        if (upper == "I") { showInfo(); continue; }
        if (upper == "M") { std::cout << "Zurueck zum Hauptmenue.\n"; return; }
        if (upper == "A") { animation_ = !animation_; std::cout << "Animation " << (animation_ ? "an" : "aus") << ".\n"; continue; }
        if (upper == "S") {
            std::cout << (saveGame() ? "Spiel gespeichert.\n" : "Speichern fehlgeschlagen.\n");
            continue;
        }
        if (upper == "U") {
            if (history_.empty()) { std::cout << "Kein Zug vorhanden.\n"; continue; }
            Move last = history_.back();
            if (board_.undo(last.row, last.column)) {
                current_ = last.player;
                history_.pop_back();
                std::cout << "Letzter Zug entfernt.\n";
            } else std::cout << "Zug konnte nicht entfernt werden.\n";
            continue;
        }
        if (upper.size() >= 2 && upper[0] == 'V' && upper[1] == ' ') {
            int col;
            if (!wholeInt(upper.substr(2), col) || col < 1 || col > board_.columns()
                || board_.nextFreeRow(col - 1) == -1) {
                std::cout << "Vorschau nicht moeglich. Beispiel: V 4\n";
            } else {
                board_.print(col - 1, players_[current_].getSymbol());
                std::cout << "Nur Vorschau: Feld wurde NICHT veraendert.\n";
            }
            continue;
        }
        int col;
        if (!wholeInt(command, col) || col < 1 || col > board_.columns()) {
            std::cout << "Ungueltige Eingabe. Bitte eine Spalte waehlen.\n";
            continue;
        }
        int zeroCol = col - 1;
        int landing = board_.nextFreeRow(zeroCol);
        if (landing == -1) { std::cout << "Spalte ist voll. Erneut waehlen.\n"; continue; }
        char symbol = players_[current_].getSymbol();
        if (animation_) board_.animateDrop(zeroCol, symbol, landing);
        int row;
        if (!board_.drop(zeroCol, symbol, row)) continue;
        history_.push_back({row, zeroCol, current_});
        if (board_.hasWon(row, zeroCol, symbol)) {
            board_.print();
            std::cout << "GEWONNEN: " << players_[current_].getName() << "!\n";
            addWin(players_[current_].getName());
            active_ = false;
            return;
        }
        if (board_.isFull()) {
            board_.print();
            std::cout << "REMIS: Spielfeld voll.\n";
            active_ = false;
            return;
        }
        current_ = (current_ + 1) % static_cast<int>(players_.size());
    }
}

bool Game::saveGame(const std::string& path) const {
    if (!active_) return false;
    std::ofstream out(path, std::ios::trunc);
    if (!out) return false;
    out << "GYM1\n";
    out << players_.size() << ' ' << current_ << ' ' << board_.rows() << ' '
        << board_.columns() << ' ' << history_.size() << '\n';
    for (const auto& p : players_) {
        out << std::quoted(p.getName()) << ' ' << p.getSymbol() << '\n';
    }
    for (const auto& m : history_) out << m.row << ' ' << m.column << ' ' << m.player << '\n';
    for (const auto& row : board_.cells()) {
        for (char value : row) out << value;
        out << '\n';
    }
    out.flush();
    return static_cast<bool>(out);
}

bool Game::loadGame(const std::string& path) {
    std::ifstream in(path);
    std::string signature;
    if (!(in >> signature) || signature != "GYM1") return false;
    int n, current, rows, cols, moves;
    if (!(in >> n >> current >> rows >> cols >> moves)) return false;
    if (n < 2 || n > 4 || current < 0 || current >= n) return false;
    Board candidate(n);
    if (rows != candidate.rows() || cols != candidate.columns()
        || moves < 0 || moves > rows * cols) return false;
    if (current != moves % n) return false;
    std::vector<Player> loadedPlayers;
    for (int i = 0; i < n; ++i) {
        std::string name;
        char symbol;
        if (!(in >> std::quoted(name) >> symbol) || !validName(name)
            || symbol != SYMBOLS[i]) return false;
        for (const auto& existing : loadedPlayers) {
            if (existing.getName() == name) return false;
        }
        loadedPlayers.emplace_back(name, symbol);
    }
    std::vector<Move> loadedMoves;
    for (int i = 0; i < moves; ++i) {
        int savedRow, column, player;
        if (!(in >> savedRow >> column >> player) || player != i % n) return false;
        int actualRow;
        if (!candidate.drop(column, loadedPlayers[player].getSymbol(), actualRow)
            || actualRow != savedRow) return false;
        // Gespeicherte Partien muessen fortsetzbar sein.
        if (candidate.hasWon(actualRow, column, loadedPlayers[player].getSymbol())) return false;
        loadedMoves.push_back({actualRow, column, player});
    }
    if (candidate.isFull()) return false;
    for (int row = 0; row < rows; ++row) {
        std::string line;
        if (!(in >> line) || static_cast<int>(line.size()) != cols) return false;
        for (int col = 0; col < cols; ++col) {
            if (line[col] != candidate.cells()[row][col]) return false;
        }
    }
    // Ausschliesslich nach vollstaendiger Pruefung den echten Spielstand ersetzen.
    board_ = std::move(candidate);
    players_ = std::move(loadedPlayers);
    history_ = std::move(loadedMoves);
    current_ = current;
    chosenPlayers_ = n;
    chosenNames_.clear();
    for (const auto& player : players_) chosenNames_.push_back(player.getName());
    active_ = true;
    return true;
}

void Game::loadHighscores() {
    highscores_.clear();
    std::ifstream in("data/highscore.txt");
    if (!in) return;
    std::string name;
    int wins;
    while (in >> std::quoted(name) >> wins) {
        if (validName(name) && wins >= 0) highscores_[name] = wins;
    }
}

bool Game::saveHighscores() const {
    std::ofstream out("data/highscore.txt", std::ios::trunc);
    if (!out) return false;
    for (const auto& item : highscores_) {
        out << std::quoted(item.first) << ' ' << item.second << '\n';
    }
    out.flush();
    return static_cast<bool>(out);
}

void Game::addWin(const std::string& name) {
    ++highscores_[name];
    if (!saveHighscores()) std::cerr << "Warnung: Highscore konnte nicht gespeichert werden.\n";
}

void Game::showHighscores() const {
    std::vector<std::pair<std::string, int>> order(highscores_.begin(), highscores_.end());
    std::sort(order.begin(), order.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });
    std::cout << "\nHIGHSCORE (Siege absteigend)\n";
    if (order.empty()) std::cout << "Noch keine Siege gespeichert.\n";
    int rank = 1;
    for (const auto& item : order) std::cout << rank++ << ". " << item.first << ": " << item.second << '\n';
}

void Game::run() {
    while (true) {
        std::cout << "\n==============================================\n"
                  << "   VIER GEWINNT | GYM EDITION | C++17\n"
                  << "==============================================\n"
                  << "1  Neues Spiel\n2  Spieleranzahl festlegen\n3  Namen eingeben\n"
                  << "4  Spiel speichern\n5  Spiel laden\n6  Highscore\n0  Beenden\n";
        int option;
        if (!wholeInt(ask("Auswahl: "), option)) option = -1;
        switch (option) {
        case 1: newGame(); break;
        case 2: configurePlayers(); break;
        case 3: configureNames(); break;
        case 4: std::cout << (saveGame() ? "Gespeichert.\n" : "Kein aktives Spiel / Fehler.\n"); break;
        case 5:
            if (loadGame()) { std::cout << "Spielstand geladen.\n"; play(); }
            else std::cout << "Datei fehlt oder ist ungueltig. Laufender Spielstand bleibt erhalten.\n";
            break;
        case 6: showHighscores(); break;
        case 0: std::cout << "Programm beendet.\n"; return;
        default: std::cout << "Nur 0 bis 6 erlaubt.\n";
        }
    }
}