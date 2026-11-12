// Offline asset-path tests: no SFML, speakers or Windows desktop needed.
#include "GymAudioAssets.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

void check(bool ok, const char* reason) {
    if (!ok) throw std::runtime_error(reason);
}

int main() {
    namespace fs = std::filesystem;
    try {
        const auto id = std::chrono::steady_clock::now().time_since_epoch().count();
        const auto root = fs::temp_directory_path() /
            ("jk-audio-check-" + std::to_string(id));
        const auto music = root / "assets/audio/sci-fi";
        fs::create_directories(music);
        const auto fakeExe = root / "vier-gewinnt-3d.exe";
        try {
            auto report = gymaudio::inspectAssets(fakeExe);
            check(!report.complete() && report.found == 0, "missing pack");
            for (auto* name : gymaudio::requiredOgg) {
                std::ofstream f(music / name, std::ios::binary);
                f << "OggS";
                for (int n = 0; n < 260; ++n) f.put('x');
            }
            report = gymaudio::inspectAssets(fakeExe);
            check(report.complete(), "complete portable pack");
            check(!gymaudio::isGymPhonkV4(report.folder), "original profile");
            {
                std::ofstream profile(music / "PROFILE.txt");
                profile << "JK_AXIS_GYM_PHONK_V4\n";
            }
            check(gymaudio::isGymPhonkV4(music), "V4 profile marker");
            {
                std::ofstream profile(music / "PROFILE.txt", std::ios::trunc);
                profile << "not-a-valid-version\n";
            }
            check(!gymaudio::isGymPhonkV4(music), "reject unknown version marker");
            check(report.folder == music, "portable location");
            std::ofstream(music / "pulse.ogg", std::ios::trunc) << "bad";
            report = gymaudio::inspectAssets(fakeExe);
            check(!report.complete() && report.found == 7, "bad OGG header");
        } catch (...) {
            fs::remove_all(root);
            throw;
        }
        fs::remove_all(root);
        std::cout << "Audio assets: portable location, 8 files and OGG header OK\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Audio asset check: " << error.what() << '\n';
        return 1;
    }
}
