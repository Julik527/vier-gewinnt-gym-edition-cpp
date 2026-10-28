// JK AXIS | GOLD MASTER
// Projekt: Vier Gewinnt | Gym Edition (C++17)
// Datei:   src/Game.cpp
// Aufgabe: Steuert Menü, Spielerwechsel, Spielstand und Highscore.
// ------------------------------------------------------------

#include "Game.h"
#include "ConsoleUI.h"
#include "AtomicFile.h"
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
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {
const char SYMBOLS[] = {'R', 'B', 'G', 'Y'};

// Entfernt Leerzeichen am Anfang und Ende einer Eingabe.
std::string trim(const std::string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

// Eine Eingabe wie "3abc" soll keine gültige Zahl sein.
bool wholeInt(const std::string& raw, int& result) {
    std::istringstream in(trim(raw));
    char extra;
    return (in >> result) && !(in >> extra);
}

// Holt eine ganze Zeile vom Benutzer und bereinigt sie.
std::string ask(const std::string& message) {
    std::cout << message;
    std::string line;
    if (!std::getline(std::cin, line)) throw std::runtime_error("Eingabe wurde beendet.");
    return trim(line);
}

// Namen dürfen nicht leer oder zu lang sein.
bool validName(const std::string& name) {
    if (name.empty() || name.size() > 24) return false;
    for (unsigned char ch : name) {
        // Tab, ESC und andere Steuerzeichen duerfen das Terminal nicht veraendern.
        if (ch < 32 || ch == 127) return false;
    }
    return true;
}

// Fuer Sicherheitsabfragen zaehlen nur J oder JA als Zustimmung.
bool confirmed(const std::string& answer) {
    std::string lower = trim(answer);
    for (char& letter : lower) {
        letter = static_cast<char>(std::tolower(static_cast<unsigned char>(letter)));
    }
    return lower == "j" || lower == "ja";
}
}

// Standardmäßig startet das Projekt mit zwei Spielern.
Game::Game() : board_(2) {
    chosenNames_.resize(2);
    std::error_code error;
    std::filesystem::create_directories("data", error);
    if (error) std::cerr << "Warnung: Datenordner nicht verfuegbar: " << error.message() << "\n";
    loadHighscores();
}

// Schritt 1: Spielerzahl auswaehlen. Mit 0 kann man zum Menue zurueck.
bool Game::configurePlayers(int& playerCount) {
    std::cout << "\n  NEUES SPIEL  |  SCHRITT 1 VON 2\n"
              << "  -----------------------------\n"
              << "  Wie viele Spieler machen mit? (2, 3 oder 4)\n"
              << "  0 = Abbrechen\n\n";
    for (;;) {
        int number = 0;
        if (wholeInt(ask("  Spieleranzahl [2-4]: "), number)) {
            if (number == 0) return false;
            if (number >= 2 && number <= 4) {
                playerCount = number;
                return true;
            }
        }
        std::cout << "  Bitte 2, 3 oder 4 eingeben. Mit 0 abbrechen.\n";
    }
}

// Schritt 2: Namen nacheinander eingeben. Die alte Runde bleibt bis
// zum fertigen Spielstart unveraendert.
bool Game::configureNames(int playerCount, std::vector<std::string>& names) {
    std::cout << "\n  NEUES SPIEL  |  SCHRITT 2 VON 2\n"
              << "  -----------------------------\n"
              << "  Bitte fuer jeden Spieler einen Namen eingeben.\n"
              << "  0 = Abbrechen\n\n";
    names.clear();
    names.reserve(static_cast<std::size_t>(playerCount));
    for (int i = 0; i < playerCount; ++i) {
        for (;;) {
            const std::string name = ask("  Spieler " + std::to_string(i + 1)
                + " [" + SYMBOLS[i] + "]: ");
            if (name == "0") return false;
            if (!validName(name)) {
                std::cout << "  Bitte einen Namen mit 1 bis 24 Zeichen eingeben.\n";
                continue;
            }
            if (std::find(names.begin(), names.end(), name) != names.end()) {
                std::cout << "  Dieser Name wird schon verwendet.\n";
                continue;
            }
            names.push_back(name);
            break;
        }
    }
    return true;
}

// Neues Spiel ist ein zusammenhaengender Ablauf, kein weiteres Untermenue.
void Game::newGame() {
    // Eine unterbrochene Runde nicht einfach ohne Rueckfrage verwerfen.
    if (active_ && !confirmed(ask(
        "\n  Eine Runde laeuft noch. Neue Runde starten? [j/N]: "))) {
        std::cout << "  Die bisherige Runde bleibt erhalten.\n";
        console_ui::pauseForReading();
        return;
    }

    int nextCount = 2;
    std::vector<std::string> nextNames;
    if (!configurePlayers(nextCount) || !configureNames(nextCount, nextNames)) {
        std::cout << "\n  Spielstart abgebrochen. Zurueck zum Hauptmenue.\n";
        console_ui::pauseForReading();
        return;
    }

    // Erst nach beiden Schritten wird die bisherige Partie ersetzt.
    chosenPlayers_ = nextCount;
    chosenNames_ = nextNames;
    players_.clear();
    for (int i = 0; i < chosenPlayers_; ++i) {
        players_.emplace_back(chosenNames_[i], SYMBOLS[i]);
    }
    board_ = Board(chosenPlayers_);
    history_.clear();
    current_ = 0;
    active_ = true;
    resultPending_ = false;
    lastWinner_.clear();
    lastWinningCells_.clear();
    round_ = 1;

    std::cout << "\n  SPIEL BEREIT\n"
              << "  " << chosenPlayers_ << " Spieler | "
              << board_.columns() << " Spalten x " << board_.rows()
              << " Reihen\n";
    for (const auto& player : players_) {
        std::cout << "  [" << player.getSymbol() << "] " << player.getName() << "\n";
    }
    std::cout << "\n  Los geht's!\n";
    play();
}

// Die Hilfe funktioniert im Hauptmenue und waehrend einer Spielrunde.
void Game::showHelp() const {
    std::cout << "\n  +-----------------------------------------------------+\n"
              << "  | JK AXIS  /  GYM EDITION  /  ANLEITUNG            |\n"
              << "  +-----------------------------------------------------+\n"
              << "  ZIEL: Vier eigene Hanteln in einer Reihe platzieren.\n"
              << "  Das geht waagerecht, senkrecht oder diagonal.\n\n"
              << "  1-7 / 1-9 / 1-11   Hantel in die gewaehlte Spalte setzen\n"
              << "  V 4                Vorschau fuer die vierte Spalte\n"
              << "  U                  Letzten Zug rueckgaengig machen\n"
              << "  S                  Spielstand speichern\n"
              << "  I                  Infos zur laufenden Runde\n"
              << "  A                  Kleine Animation ein/aus\n"
              << "  H                  Diese Anleitung anzeigen\n"
              << "  M                  Zum Hauptmenue (Runde bleibt erhalten)\n\n"
              << "  [R] [B] [G] [Y] = Hanteln deiner Spieler\n"
              << "  (R) = Vorschau  |  {R} = Hantel der Gewinnreihe\n"
              << "  +-----------------------------------------------------+\n";
}

// Hier sieht man auf einen Blick, wer spielt und wie weit die Runde ist.
void Game::showInfo() const {
    std::cout << "\n  JK AXIS  /  TRAININGS-INFORMATIONEN\n"
              << "  Runde " << round_ << "  |  "
              << board_.columns() << " Spalten x " << board_.rows()
              << " Reihen  |  " << board_.tokenCount()
              << " von " << board_.rows() * board_.columns() << " Feldern belegt\n";
    for (std::size_t i = 0; i < players_.size(); ++i) {
        const auto& player = players_[i];
        const auto found = highscores_.find(player.getName());
        const int wins = found == highscores_.end() ? 0 : found->second;
        std::cout << (static_cast<int>(i) == current_ ? "  >> " : "     ")
                  << "[" << player.getSymbol() << "] " << player.getName()
                  << "  |  " << wins << (wins == 1 ? " Sieg" : " Siege")
                  << (static_cast<int>(i) == current_ ? " (am Zug)" : "") << '\n';
    }
}

// Nach jeder abgeschlossenen Runde stehen Revanche, Neues Spiel und Hauptmenue bereit.
// Die Auswahl wird im Hauptprogramm behandelt, damit kein Spiel ins naechste verschachtelt wird.
void Game::rematch() {
    board_ = Board(static_cast<int>(players_.size()));
    history_.clear();
    current_ = 0;
    active_ = true;
    resultPending_ = false;
    lastWinner_.clear();
    lastWinningCells_.clear();
    ++round_;
    std::cout << "\n  REVANCHE  |  Gleiche Spieler, neues leeres Feld.\n";
    showInfo();
}

// Die Ergebnisse erscheinen nur nach Sieg oder Remis, nicht beim Verlassen mit M.
int Game::showResultMenu() const {
    console_ui::present(console_ui::result(
        board_, players_, highscores_, lastWinner_, round_, lastWinningCells_));
    for (;;) {
        int choice = -1;
        if (wholeInt(ask("  Deine Auswahl: "), choice)
            && (choice == 0 || choice == 1 || choice == 2)) return choice;
        std::cout << "  Bitte 0, 1 oder 2 eingeben.\n";
    }
}

// Das Spielfeld wird nur nach einer Aenderung neu gezeichnet.
// Hilfe oder Fehlermeldungen brauchen keinen kompletten Neudruck.
void Game::play() {
    std::cout << "\n  SPIELFELD  |  " << players_.size()
              << " Spieler  |  H = Hilfe  |  M = Hauptmenue\n";
    bool redraw = true;
    while (active_) {
        if (redraw) {
            console_ui::present(console_ui::arena(
                board_, players_, current_, round_, highscores_));
            redraw = false;
        }
        std::string command = ask("Spalte oder Befehl (H=Hilfe): ");
        std::string upper = command;
        for (char& ch : upper) {
            ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        }
        if (upper == "H") { showHelp(); continue; }
        if (upper == "I") { showInfo(); continue; }
        if (upper == "M") { std::cout << "Zurueck zum Hauptmenue.\n"; return; }
        if (upper == "A") {
            animation_ = !animation_;
            std::cout << "Animation " << (animation_ ? "an" : "aus") << ".\n";
            continue;
        }
        if (upper == "S") {
            std::cout << (saveGame() ? "Spiel gespeichert.\n" : "Speichern fehlgeschlagen.\n");
            continue;
        }
        // Undo ist nur fuer den obersten Stein moeglich.
        if (upper == "U") {
            if (history_.empty()) { std::cout << "Kein Zug vorhanden.\n"; continue; }
            Move last = history_.back();
            if (board_.undo(last.row, last.column)) {
                current_ = last.player;
                history_.pop_back();
                std::cout << "Letzter Zug entfernt.\n";
                redraw = true;
            } else {
                std::cout << "Zug konnte nicht entfernt werden.\n";
            }
            continue;
        }
        // Vorschau veraendert den Spielstand nicht; danach erscheint das echte Feld.
        if (upper.size() >= 2 && upper[0] == 'V' && upper[1] == ' ') {
            int col;
            if (!wholeInt(upper.substr(2), col) || col < 1 || col > board_.columns()
                || board_.nextFreeRow(col - 1) == -1) {
                std::cout << "Vorschau nicht moeglich. Beispiel: V 4\n";
            } else {
                console_ui::present(console_ui::arena(
                    board_, players_, current_, round_, highscores_,
                    col - 1, players_[current_].getSymbol()));
                std::cout << "Nur Vorschau: Feld wurde NICHT veraendert.\n";
                // Die Vorschau bleibt bis zur naechsten Eingabe sichtbar.
                redraw = false;
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
        if (landing == -1) {
            std::cout << "Spalte ist voll. Erneut waehlen.\n";
            continue;
        }
        char symbol = players_[current_].getSymbol();
        if (animation_) console_ui::animateDrop(board_, players_, current_, round_,
                                                highscores_, zeroCol, landing, symbol);
        int row;
        if (!board_.drop(zeroCol, symbol, row)) continue;
        history_.push_back({row, zeroCol, current_});
        if (board_.hasWon(row, zeroCol, symbol)) {
            // Die vier verbundenen Hanteln werden im Ergebnis gruen markiert.
            lastWinningCells_ = board_.winningCells(row, zeroCol, symbol);
            addWin(players_[current_].getName());
            lastWinner_ = players_[current_].getName();
            active_ = false;
            resultPending_ = true;
            return;
        }
        if (board_.isFull()) {
            lastWinner_.clear();
            lastWinningCells_.clear();
            active_ = false;
            resultPending_ = true;
            return;
        }
        current_ = (current_ + 1) % static_cast<int>(players_.size());
        redraw = true;
    }
}

// Speichert den Spielstand als Textdatei, damit man später weiterspielen kann.
bool Game::saveGame(const std::string& path) const {
    if (!active_) return false;
    return gym::writeSafely(path, [this](std::ostream& out) {
        // GYM1 ist die bisherige Kennung und bleibt voll kompatibel.
        out << "GYM1\n";
        out << players_.size() << ' ' << current_ << ' ' << board_.rows() << ' '
            << board_.columns() << ' ' << history_.size() << '\n';
        for (const auto& player : players_) {
            out << std::quoted(player.getName()) << ' ' << player.getSymbol() << '\n';
        }
        for (const auto& move : history_) {
            out << move.row << ' ' << move.column << ' ' << move.player << '\n';
        }
        for (const auto& row : board_.cells()) {
            for (char value : row) out << value;
            out << '\n';
        }
    });
}

// Erst alles in einem neuen Spielfeld prüfen, dann wirklich laden.
bool Game::loadGame(const std::string& path) {
    std::ifstream in(path);
    std::string signature;
    if (!(in >> signature) || signature != "GYM1") return false;
    int n, current, rows, cols, moves;
    if (!(in >> n >> current >> rows >> cols >> moves)) return false;
    if (n < 2 || n > 4 || current < 0 || current >= n) return false;
    // Das Test-Spielfeld verhindert, dass eine kaputte Datei die Runde verändert.
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
    // Die gespeicherten Züge werden einzeln nachgespielt und kontrolliert.
    for (int i = 0; i < moves; ++i) {
        int savedRow, column, player;
        if (!(in >> savedRow >> column >> player) || player != i % n) return false;
        int actualRow;
        if (!candidate.drop(column, loadedPlayers[player].getSymbol(), actualRow)
            || actualRow != savedRow) return false;
        // Gespeicherte Partien müssen fortsetzbar sein.
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
    // Kein weiteres Token nach der letzten Rasterzeile akzeptieren.
    // So werden auch ansonsten plausible, aber manipulierte Spielstaende verworfen.
    std::string trailingToken;
    if (in >> trailingToken) return false;

    // Ausschließlich nach vollständiger Prüfung den echten Spielstand ersetzen.
    // Bis hierhin ist die Datei gültig. Jetzt wird der Spielstand übernommen.
    board_ = std::move(candidate);
    players_ = std::move(loadedPlayers);
    history_ = std::move(loadedMoves);
    current_ = current;
    chosenPlayers_ = n;
    chosenNames_.clear();
    for (const auto& player : players_) chosenNames_.push_back(player.getName());
    active_ = true;
    resultPending_ = false;
    lastWinner_.clear();
    lastWinningCells_.clear();
    round_ = 1; // Das bisherige Speicherformat enthaelt keine Rundennummer.
    return true;
}

// Liest die bisher gespeicherten Siege aus der Highscore-Datei.
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

// Schreibt alle aktuellen Siege in die Highscore-Datei.
bool Game::saveHighscores() const {
    return gym::writeSafely("data/highscore.txt", [this](std::ostream& out) {
        for (const auto& item : highscores_) {
            out << std::quoted(item.first) << ' ' << item.second << '\n';
        }
    });
}

// Nach einem Sieg bekommt der Gewinner genau einen Punkt dazu.
void Game::addWin(const std::string& name) {
    ++highscores_[name];
    if (!saveHighscores()) std::cerr << "Warnung: Highscore konnte nicht gespeichert werden.\n";
}

// Die meisten Siege sollen in der Liste ganz oben stehen.
void Game::showHighscores() const {
    std::vector<std::pair<std::string, int>> order(highscores_.begin(), highscores_.end());
    std::sort(order.begin(), order.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });

    std::ostringstream screen;
    screen << "\n  +-----------------------------------------------------+\n"
           << "  | JK AXIS  /  GYM EDITION  /  BESTENLISTE          |\n"
           << "  +-----------------------------------------------------+\n";
    if (order.empty()) {
        screen << "  Noch keine Siege gespeichert.\n"
               << "  Spiele eine Runde und hol dir Platz 1!\n";
    }
    int rank = 1;
    for (const auto& item : order) {
        screen << "  " << rank++ << ". " << item.first << ": " << item.second
               << (item.second == 1 ? " Sieg\n" : " Siege\n");
    }
    screen << "  -----------------------------------------------------\n";
    console_ui::present(screen.str());
}

// Das Hauptmenue bleibt bewusst einfach: ein Spiel starten, fortsetzen,
// laden oder die Ergebnisse ansehen. Die Vorbereitung passiert bei Neues Spiel.
void Game::run() {
    while (true) {
        // Ergebnis erst nach Rueckkehr aus play() behandeln: keine Rekursion.
        if (resultPending_) {
            const int action = showResultMenu();
            resultPending_ = false;
            if (action == 1) {
                rematch();
                play();
                continue;
            }
            if (action == 2) {
                newGame();
                continue;
            }
        }
        console_ui::present(console_ui::mainMenu(
            players_, board_, active_, current_, round_));
        int option = -1;
        if (!wholeInt(ask("  Deine Auswahl: "), option)) option = -1;
        switch (option) {
        case 1:
            newGame();
            break;
        case 2:
            if (active_) {
                std::cout << "\n  Deine Runde wird fortgesetzt.\n";
                play();
            } else {
                std::cout << "\n  Keine Runde aktiv. Starte mit 1 ein neues Spiel.\n";
                console_ui::pauseForReading();
            }
            break;
        case 3:
            if (active_ && !confirmed(ask(
                "\n  Laufende Runde durch Speicherstand ersetzen? [j/N]: "))) {
                std::cout << "  Die bisherige Runde bleibt erhalten.\n";
                console_ui::pauseForReading();
                break;
            }
            if (loadGame()) {
                std::cout << "\n  Spielstand geladen. Viel Erfolg!\n";
                play();
            } else {
                std::cout << "\n  Keine gueltige Speicherdatei gefunden.\n"
                          << "  Eine laufende Runde wurde nicht veraendert.\n";
                console_ui::pauseForReading();
            }
            break;
        case 4:
            showHighscores();
            console_ui::pauseForReading();
            break;
        case 5:
            showHelp();
            console_ui::pauseForReading();
            break;
        case 0:
            if (active_ && !confirmed(ask(
                "\n  Die laufende Runde verlassen? Ungespeicherte Zuege gehen verloren. [j/N]: "))) {
                std::cout << "  Zurueck zum Hauptmenue.\n";
                break;
            }
            std::cout << "\n  Programm beendet. Bis zum naechsten Spiel!\n";
            return;
        default:
            std::cout << "\n  Bitte 0 bis 5 eingeben.\n";
            console_ui::pauseForReading();
        }
    }
}
