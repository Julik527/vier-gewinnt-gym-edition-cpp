// JK AXIS | Vier Gewinnt: strict, shared highscore parser (C++17).
// A malformed or partially written file never replaces an existing scoreboard.
#pragma once

#include <iomanip>
#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

namespace gym {
template <typename NameValidator>
bool parseScores(std::istream& input,
                 std::map<std::string, int>& destination,
                 NameValidator validName) {
    std::map<std::string, int> parsed;
    std::string line;
    while (std::getline(input, line)) {
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
        if (line.size() > 512) return false;
        const auto first = line.find_first_not_of(" \t");
        if (first == std::string::npos || line[first] != '"') return false;

        std::istringstream row(line);
        std::string name;
        int victories = -1;
        std::string extra;
        if (!(row >> std::quoted(name) >> victories)
            || !validName(name) || victories < 0 || (row >> extra)
            || !parsed.emplace(name, victories).second)
            return false;
    }
    if (input.bad()) return false;
    destination = std::move(parsed);
    return true;
}
} // namespace gym
