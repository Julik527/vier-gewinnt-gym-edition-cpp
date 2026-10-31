#include "MatchSession.h"
#include "AtomicFile.h"
#include "ScoreFile.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <utility>

namespace {
constexpr char symbols[] = {'R', 'B', 'G', 'Y'};
}

MatchSession::MatchSession() {
    std::error_code error;
    std::filesystem::create_directories("data", error);
    loadScores();
}

bool MatchSession::validName(const std::string& name) {
    if (name.empty() || name.size() > 24
        || name.find_first_not_of(' ') == std::string::npos) return false;
    for (unsigned char c : name) if (c < 32 || c == 127) return false;
    return true;
}

bool MatchSession::begin(const std::vector<std::string>& names) {
    if (names.size() < 2 || names.size() > 4) return false;
    for (std::size_t i = 0; i < names.size(); ++i) {
        if (!validName(names[i])) return false;
        if (std::find(names.begin(), names.begin() + static_cast<std::ptrdiff_t>(i), names[i])
            != names.begin() + static_cast<std::ptrdiff_t>(i)) return false;
    }
    std::vector<Player> next;
    for (std::size_t i = 0; i < names.size(); ++i)
        next.emplace_back(names[i], symbols[i]);
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

bool MatchSession::drop(int column) {
    if (!active_ || finished_ || current_ < 0 || current_ >= static_cast<int>(players_.size()))
        return false;
    const int playing = current_;
    const char symbol = players_[playing].getSymbol();
    int row = -1;
    if (!board_.drop(column, symbol, row)) return false;
    moves_.push_back({row, column, playing});
    if (board_.hasWon(row, column, symbol)) {
        finished_ = true;
        active_ = false;
        winner_ = players_[playing].getName();
        winLine_ = board_.winningCells(row, column, symbol);
        ++scores_[winner_];
        scoreSaved_ = saveScores();
    } else if (board_.isFull()) {
        finished_ = true;
        active_ = false;
        winner_.clear();
        winLine_.clear();
    } else {
        current_ = (current_ + 1) % static_cast<int>(players_.size());
    }
    return true;
}

bool MatchSession::undo() {
    if (!active_ || moves_.empty()) return false;
    const GymMove last = moves_.back();
    if (!board_.undo(last.row, last.column)) return false;
    moves_.pop_back();
    current_ = last.player;
    return true;
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

int MatchSession::wins(const std::string& name) const {
    const auto found = scores_.find(name);
    return found == scores_.end() ? 0 : found->second;
}

bool MatchSession::save(const std::string& path) const {
    if (!active_) return false;
    return gym::writeSafely(path, [this](std::ostream& out) {
        out << "GYM1\n";
        out << players_.size() << ' ' << current_ << ' ' << board_.rows() << ' '
            << board_.columns() << ' ' << moves_.size() << '\n';
        for (const auto& player : players_)
            out << std::quoted(player.getName()) << ' ' << player.getSymbol() << '\n';
        for (const auto& move : moves_)
            out << move.row << ' ' << move.column << ' ' << move.player << '\n';
        for (const auto& row : board_.cells()) {
            for (char value : row) out << value;
            out << '\n';
        }
    });
}

bool MatchSession::load(const std::string& path) {
    std::ifstream in(path);
    std::string header;
    if (!(in >> header) || header != "GYM1") return false;
    int count, turn, rows, columns, moveCount;
    if (!(in >> count >> turn >> rows >> columns >> moveCount)) return false;
    if (count < 2 || count > 4 || turn < 0 || turn >= count) return false;
    Board candidate(count);
    if (rows != candidate.rows() || columns != candidate.columns()
        || moveCount < 0 || moveCount > rows * columns || turn != moveCount % count)
        return false;
    std::vector<Player> people;
    for (int i = 0; i < count; ++i) {
        std::string name;
        char symbol;
        if (!(in >> std::quoted(name) >> symbol) || !validName(name)
            || symbol != symbols[i]) return false;
        for (const auto& person : people) if (person.getName() == name) return false;
        people.emplace_back(name, symbol);
    }
    std::vector<GymMove> history;
    for (int i = 0; i < moveCount; ++i) {
        int savedRow, col, player;
        if (!(in >> savedRow >> col >> player) || player != i % count) return false;
        int row;
        if (!candidate.drop(col, people[player].getSymbol(), row) || row != savedRow
            || candidate.hasWon(row, col, people[player].getSymbol())) return false;
        history.push_back({row, col, player});
    }
    if (candidate.isFull()) return false;
    for (int row = 0; row < rows; ++row) {
        std::string line;
        if (!(in >> line) || static_cast<int>(line.size()) != columns) return false;
        for (int col = 0; col < columns; ++col)
            if (line[col] != candidate.cells()[row][col]) return false;
    }
    std::string trailing;
    if (in >> trailing) return false;
    board_ = std::move(candidate);
    players_ = std::move(people);
    moves_ = std::move(history);
    current_ = turn;
    round_ = 1;
    active_ = true;
    finished_ = false;
    winner_.clear();
    winLine_.clear();
    scoreSaved_ = true;
    return true;
}

bool MatchSession::loadScores(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;
    std::map<std::string, int> next;
    if (!gym::parseScores(in, next, validName)) return false;
    scores_ = std::move(next);
    return true;
}

bool MatchSession::saveScores(const std::string& path) const {
    return gym::writeSafely(path, [this](std::ostream& out) {
        for (const auto& item : scores_)
            out << std::quoted(item.first) << ' ' << item.second << '\n';
    });
}
