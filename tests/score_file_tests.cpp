// Reject malformed, truncated or duplicated highscore files.
#include "ScoreFile.h"
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
namespace {
void check(bool ok, const char* why) { if (!ok) throw std::runtime_error(why); }
bool valid(const std::string& name) { return !name.empty() && name.size() <= 24 && name.find_first_not_of(' ') != std::string::npos; }
void reject(const std::string& content, const char* why) {
    std::istringstream stream(content);
    std::map<std::string, int> scores{{"Existing", 3}};
    check(!gym::parseScores(stream, scores, valid), why);
    check(scores.size() == 1 && scores.at("Existing") == 3, "transactional scores");
}
}
int main() {
    try {
        std::istringstream good("\"Ada\" 2\n\"Ben\" 0\n\n");
        std::map<std::string, int> scores;
        check(gym::parseScores(good, scores, valid), "valid file");
        check(scores.size() == 2 && scores.at("Ada") == 2, "correct scores");
        std::istringstream maximumScore("\\"Ada\\" " +
            std::to_string(std::numeric_limits<int>::max()) + "\\n");
        check(gym::parseScores(maximumScore, scores, valid), "INT_MAX highscore is valid");
        check(scores.at("Ada") == std::numeric_limits<int>::max(), "maximum parsed");
        reject("\"Ada\" 2\n\"Ben\"", "truncated score");
        reject("\"Ada\" 2\n\"Ada\" 8\n", "duplicate");
        reject("\"Ada\" -1\n", "negative");
        reject("\"Ada\" 1 extra\n", "trailing token");
        reject("\"Ada\" 999999999999999999999\n", "overflow");
        reject("Ada 2\n", "unquoted");
        reject("\"\" 1\n", "empty name");
        std::cout << "Highscores: strict loading and failure preservation OK.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ScoreFile test: " << e.what() << '\n';
        return 1;
    }
}
