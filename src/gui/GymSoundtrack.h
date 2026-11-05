// JK AXIS | Gym Edition: original instrumental training theme.
// Pure C++17 synthesizer: no external recordings, downloads, music royalties,
// runtime files, or audio devices are needed to render or test the composition.
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gym::audio {
constexpr unsigned sampleRate = 44100;
constexpr unsigned channels = 2;
constexpr float bpm = 116.f;
constexpr int bars = 8;

inline std::vector<std::int16_t> renderTheme() {
    constexpr float pi = 3.14159265358979323846f;
    constexpr std::array<std::array<float, 3>, 4> chords = {{
        {{293.665f, 349.228f, 440.000f}}, // D minor
        {{233.082f, 293.665f, 349.228f}}, // B-flat major
        {{261.626f, 349.228f, 440.000f}}, // F major (inversion)
        {{261.626f, 329.628f, 391.995f}}  // C major
    }};
    constexpr std::array<float, 4> bassRoots = {
        73.416f, 58.270f, 87.307f, 65.406f
    };
    constexpr std::array<int, 8> arpeggio = {{0, 1, 2, 1, 0, 2, 1, 2}};
    const float secondsPerBeat = 60.f / bpm;
    const float secondsPerBar = secondsPerBeat * 4.f;
    const float duration = secondsPerBar * static_cast<float>(bars);
    const auto frames = static_cast<std::size_t>(duration * sampleRate);
    std::vector<std::int16_t> pcm(frames * channels);
    std::uint32_t rng = 0xA51E2D17u;

    for (std::size_t i = 0; i < frames; ++i) {
        const float t = static_cast<float>(i) / sampleRate;
        const int absoluteBeat = static_cast<int>(t / secondsPerBeat);
        const int bar = std::min(bars - 1, absoluteBeat / 4);
        const int harmony = bar % 4;
        const int previousHarmony = (harmony + 3) % 4;
        const int beat = absoluteBeat % 4;
        const float beatTime = t - absoluteBeat * secondsPerBeat;
        const float barTime = t - bar * secondsPerBar;
        const int eighth = std::min(7, static_cast<int>(barTime / (secondsPerBeat * .5f)));
        const float eighthTime = std::fmod(t, secondsPerBeat * .5f);
        const float chordCrossfade = std::clamp(barTime / .12f, 0.f, 1.f);

        rng = rng * 1664525u + 1013904223u;
        const float noise = static_cast<float>((rng >> 8) & 65535u) / 32767.5f - 1.f;

        // Rhythm: gently pitched kick, two-and-four snare and articulated hats.
        const float kickEnvelope = std::exp(-beatTime * 21.f);
        const float kickPhase = 2.f * pi * (53.f * beatTime +
            43.f * (1.f - std::exp(-beatTime * 25.f)) / 25.f);
        const float kick = .30f * kickEnvelope * std::sin(kickPhase);
        const float snare = (beat == 1 || beat == 3)
            ? .14f * std::exp(-beatTime * 30.f) * noise : 0.f;
        const float hat = (.033f + (eighth % 2 ? .016f : 0.f))
            * std::exp(-eighthTime * 77.f) * noise;

        // Round bass with a soft decay at every beat; not sub-bass-only.
        const float bassGate = std::min(1.f, beatTime * 45.f)
            * std::exp(-beatTime * 2.3f);
        const float bassAngle = 2.f * pi * bassRoots[harmony] * beatTime;
        const float bass = .25f * bassGate *
            (std::sin(bassAngle) + .25f * std::sin(2.f * bassAngle));

        // The pad gradually crossfades between the four original harmonies.
        float padLeft = 0.f, padRight = 0.f;
        for (int n = 0; n < 3; ++n) {
            const float oldHz = chords[previousHarmony][n] * .5f;
            const float hz = chords[harmony][n] * .5f;
            const float oldL = std::sin(2.f * pi * oldHz * t);
            const float oldR = std::sin(2.f * pi * (oldHz + .11f) * t);
            const float newL = std::sin(2.f * pi * hz * t);
            const float newR = std::sin(2.f * pi * (hz + .11f) * t);
            padLeft += oldL * (1.f - chordCrossfade) + newL * chordCrossfade;
            padRight += oldR * (1.f - chordCrossfade) + newR * chordCrossfade;
        }
        padLeft *= .043f;
        padRight *= .043f;

        // Syncopated, harmonically matching 8th-note motif.
        const float arpHz = chords[harmony][arpeggio[eighth]];
        const float arpGate = std::min(1.f, eighthTime * 40.f)
            * std::exp(-eighthTime * 7.f);
        const float arpAngle = 2.f * pi * arpHz * eighthTime;
        const float arp = .12f * arpGate *
            (std::sin(arpAngle) + .17f * std::sin(2.f * arpAngle));

        // Every second four-bar phrase adds a quiet response melody.
        float response = 0.f;
        if (bar >= 4 && eighth % 2 == 1) {
            const float melodyHz = chords[harmony][(eighth / 2 + 1) % 3] * 2.f;
            const float envelope = std::min(1.f, eighthTime * 55.f)
                * std::exp(-eighthTime * 10.f);
            response = .057f * envelope * std::sin(2.f * pi * melodyHz * eighthTime);
        }

        // Stereophonic balance is deliberately subtle on laptop speakers.
        const float rhythm = kick + snare + bass;
        float left = rhythm + hat * .82f + padLeft + arp * .92f + response * .72f;
        float right = rhythm + hat * 1.18f + padRight + arp * 1.08f + response;
        const float edge = std::clamp(std::min(t, duration - t) * 32.f, 0.f, 1.f);
        left = std::tanh(left * .80f) * edge;
        right = std::tanh(right * .80f) * edge;
        pcm[2 * i] = static_cast<std::int16_t>(
            std::clamp(left, -.99f, .99f) * 28000.f);
        pcm[2 * i + 1] = static_cast<std::int16_t>(
            std::clamp(right, -.99f, .99f) * 28000.f);
    }
    return pcm;
}
} // namespace gym::audio
