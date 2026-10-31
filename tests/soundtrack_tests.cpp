// Original soundtrack verification without SFML, speakers or a GUI.
#include "GymSoundtrack.h"
#include <algorithm>
#include <stdexcept>

static void require(bool passed, const char* message) {
    if (!passed) throw std::runtime_error(message);
}
#include <cmath>
#include <cstdint>
#include <iostream>

int main() {
    using namespace gym::audio;
    const auto audio = renderTheme();
    const std::size_t frames = static_cast<std::size_t>(
        (static_cast<float>(bars) * 4.f * 60.f / bpm) * sampleRate);
    require(audio.size() == frames * channels);
    require(!audio.empty());
    require(audio.front() == 0);
    std::int64_t totalAbsolute = 0;
    std::int16_t peak = 0;
    bool stereo = false;
    for (std::size_t i = 0; i < audio.size(); i += channels) {
        const auto left = static_cast<int>(audio[i]);
        const auto right = static_cast<int>(audio[i + 1]);
        totalAbsolute += std::abs(left) + std::abs(right);
        peak = std::max(peak, static_cast<std::int16_t>(std::max(std::abs(left), std::abs(right))));
        stereo |= left != right;
    }
    const double avg = static_cast<double>(totalAbsolute) / audio.size();
    require(avg > 200.0 && avg < 15000.0);
    require(peak < 30000 && peak > 2000);
    require(stereo);
    // Avoid an audible click at the loop boundary.
    require(std::abs(static_cast<int>(audio[audio.size() - 2])) < 450);
    require(std::abs(static_cast<int>(audio.back())) < 450);
    // Synthesis must be deterministic: byte-for-byte same result.
    require(audio == renderTheme());
    std::cout << "Original stereo soundtrack: " << bars << " bars at "
              << bpm << " BPM, peak " << peak << ", mean " << avg
              << ", loop boundary OK.\n";
}
