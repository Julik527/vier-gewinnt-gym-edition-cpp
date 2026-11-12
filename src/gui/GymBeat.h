/**
 * @file src/gui/GymBeat.h
 * @brief Kapselt das ältere, synthetisch erzeugte 116-BPM-Thema.
 * @details GymBeat erzeugt sf::SoundBuffer aus renderTheme() und erlaubt Start, Stop und Lautstärke.
 * @par Abhängigkeiten
 * GymSoundtrack.h sowie SFML 3.1 Audio; nicht Teil der aktuellen OGG-Audio-Engine.
 * @par Wichtige Randbedingungen
 * Legacy-Baustein: GymAudio nutzt heute OGG-Dateien ohne automatischen Rückfall auf dieses Thema.
 */

#pragma once
#include "GymSoundtrack.h"
#include <SFML/Audio.hpp>
#include <algorithm>
#include <memory>
#include <stdexcept>

class GymBeat {
    sf::SoundBuffer buffer_;
    std::unique_ptr<sf::Sound> sound_;
    float volume_ = 40.f;

public:
    GymBeat() {
        const auto pcm = gym::audio::renderTheme();
        if (!buffer_.loadFromSamples(
                pcm.data(), pcm.size(), gym::audio::channels,
                gym::audio::sampleRate,
                {sf::SoundChannel::FrontLeft, sf::SoundChannel::FrontRight})) {
            throw std::runtime_error("Das originale Musikthema konnte nicht initialisiert werden.");
        }
        sound_ = std::make_unique<sf::Sound>(buffer_);
        sound_->setLooping(true);
        sound_->setVolume(volume_);
    }

    bool play() {
        sound_->play();
        return sound_->getStatus() == sf::SoundSource::Status::Playing;
    }
    void stop() { if (sound_) sound_->stop(); }
    void setVolume(float volume) {
        volume_ = std::clamp(volume, 0.f, 100.f);
        if (sound_) sound_->setVolume(volume_);
    }
    float volume() const { return volume_; }
    bool playing() const {
        return sound_ && sound_->getStatus() == sf::SoundSource::Status::Playing;
    }
};
