// Reject malformed, truncated or duplicated highscore files.
#include "ScoreFile.h"
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;
namespace {
void check(bool ok, const char* why) { if (!ok) throw runtime_error(why); }
bool valid(const string& name) { return !name.empty() && name.size() <= 24 && name.find_first_not_of(' ') != string::npos; }
void reject(const string& content, const char* why) {
    istringstream stream(content);
    map<string, int> scores{{"Existing", 3}};
    check(!gym::parseScores(stream, scores, valid), why);
    check(scores.size() == 1 && scores.at("Existing") == 3, "transactional scores");
}
}
int main() {
    try {
        istringstream good("\"Ada\" 2\n\"Ben\" 0\n\n");
        map<string, int> scores;
        check(gym::parseScores(good, scores, valid), "valid file");
        check(scores.size() == 2 && scores.at("Ada") == 2, "correct scores");
        istringstream maximumScore(
            string(R"("Ada" )") +
            to_string(numeric_limits<int>::max()) + "\n");
        check(gym::parseScores(maximumScore, scores, valid), "INT_MAX highscore is valid");
        check(scores.at("Ada") == numeric_limits<int>::max(), "maximum parsed");
        reject("\"Ada\" 2\n\"Ben\"", "truncated score");
        reject("\"Ada\" 2\n\"Ada\" 8\n", "duplicate");
        reject("\"Ada\" -1\n", "negative");
        reject("\"Ada\" 1 extra\n", "trailing token");
        reject("\"Ada\" 999999999999999999999\n", "overflow");
        reject("Ada 2\n", "unquoted");
        reject("\"\" 1\n", "empty name");
        cout << "Highscores: strict loading and failure preservation OK.\n";
        return 0;
    } catch (const exception& e) {
        cerr << "ScoreFile test: " << e.what() << '\n';
        return 1;
    }
}
