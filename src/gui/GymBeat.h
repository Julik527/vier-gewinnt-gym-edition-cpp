// JK AXIS | Gym Edition: original, procedurally synthesized 120 BPM theme.
// All tones are generated from simple waveforms. No external music recording.
#pragma once
#include <SFML/Audio.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

class GymBeat {
    sf::SoundBuffer buffer_;
    std::unique_ptr<sf::Sound> sound_;

public:
    GymBeat() {
        constexpr unsigned rate = 22050;
        constexpr float pi = 3.14159265358979323846f;
        constexpr float duration = 8.f; // 4 bars, 120 beats per minute
        std::vector<std::int16_t> samples(rate * 8u);
        const float bassNotes[] = {110.f, 110.f, 87.31f, 87.31f,
                                   130.81f, 130.81f, 98.00f, 98.00f};
        // Simple original rhythm: kick / snare / hats / warm bass.
        std::uint32_t random = 0xBEEFBEEFu;
        for (std::size_t i = 0; i < samples.size(); ++i) {
            const float t = static_cast<float>(i) / rate;
            const float beatPhase = std::fmod(t, .5f);
            const int beatIndex = static_cast<int>(t / .5f);
            const float eighthPhase = std::fmod(t, .25f);
            const float kickEnv = std::exp(-beatPhase * 24.f);
            const float kick = (beatIndex % 4 == 0 || beatIndex % 4 == 2)
                ? .34f * kickEnv * std::sin(2 * pi * (54.f * t + 27.f * beatPhase))
                : 0.f;
            random = random * 1664525u + 1013904223u;
            const float noise = static_cast<float>((random >> 16) & 65535u) / 32767.5f - 1.f;
            const float snareEnv = std::exp(-beatPhase * 30.f);
            const float snare = beatIndex % 2 == 1 ? .11f * snareEnv * noise : 0.f;
            const float hat = .055f * std::exp(-eighthPhase * 95.f) * noise;
            const int note = std::min(7, static_cast<int>(t));
            const float hz = bassNotes[note];
            const float gate = std::min(1.f, std::fmod(t, 1.f) * 16.f)
                * std::min(1.f, (1.f - std::fmod(t, 1.f)) * 12.f);
            const float bass = .19f * gate * (std::sin(2 * pi * hz * t)
                          + .19f * std::sin(2 * pi * 2.f * hz * t));
            const float edge = std::min(1.f, std::min(t, duration - t) * 24.f);
            const float value = std::clamp((kick + snare + hat + bass) * edge, -1.f, 1.f);
            samples[i] = static_cast<std::int16_t>(value * 18000.f);
        }
        if (!buffer_.loadFromSamples(samples.data(), samples.size(), 1, rate,
                                     {sf::SoundChannel::Mono}))
            throw std::runtime_error("Gym-Beat konnte nicht initialisiert werden.");
        // Erst NACH dem Laden an die SoundBuffer-Instanz binden.
        sound_ = std::make_unique<sf::Sound>(buffer_);
        sound_->setLooping(true);
        sound_->setVolume(65.f);
    }
    bool play() {
        sound_->play();
        return sound_->getStatus() == sf::SoundSource::Status::Playing;
    }
    void stop() { if (sound_) sound_->stop(); }
    bool playing() const {
        return sound_ && sound_->getStatus() == sf::SoundSource::Status::Playing;
    }
};
