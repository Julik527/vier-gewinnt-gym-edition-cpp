/**
 * @file include/ScoreFile.h
 * @brief Deklariert den gemeinsam genutzten Highscore-Parser.
 * @details parseScores liest zitierte Spielernamen und nichtnegative Siege in eine temporäre map ein.
 * @par Abhängigkeiten
 * C++17 iostream, std::quoted und ein übergebener Namensvalidator; keine GUI-Abhängigkeit.
 * @par Wichtige Randbedingungen
 * Ungültige, doppelte oder über 512 Zeichen lange Zeilen verhindern die komplette Übernahme.
 */

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
        // Bound every record, including whitespace-only lines.
        if (line.size() > 512) return false;
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
        const auto first = line.find_first_not_of(" \t");
        if (first == std::string::npos || line[first] != '"') return false;

        // std::quoted liest Spielernamen mit Leerzeichen als ein Feld.
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
    // Commit erst nach erfolgreicher vollständiger Prüfung: keine Teilergebnisse.
    destination = std::move(parsed);
    return true;
}
} // namespace gym
