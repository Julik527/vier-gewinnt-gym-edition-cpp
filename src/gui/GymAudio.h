// JK AXIS | Sci-Fi Audio Director for the 2D and 3D SFML 3.1 games.
// Streams CC0 OGG music, crossfades scenes and loop boundaries, and keeps
// the original procedural soundtrack as the offline fallback.
#pragma once

#include "GymAudioScene.h"
#include "GymBeat.h"
#include <SFML/Audio.hpp>
#include <algorithm>
#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <system_error>
#include <utility>

namespace gymaudio {

class Director {
    std::filesystem::path folder_;
    std::unique_ptr<sf::Music> current_;
    std::unique_ptr<sf::Music> outgoing_;
    std::unique_ptr<GymBeat> fallback_;
    sf::SoundBuffer hoverBuffer_;
    sf::SoundBuffer victoryBuffer_;
    // Sound must be destroyed before the buffers referenced by it.
    std::unique_ptr<sf::Sound> hover_;
    std::unique_ptr<sf::Sound> victory_;
    Scene requested_ = Scene::Menu;
    Scene playingScene_ = Scene::Menu;
    float volume_ = 35.f;
    float currentGain_ = 1.f;
    float outgoingGain_ = 1.f;
    float fade_ = 1.f;
    bool enabled_ = false;
    bool effectsLoaded_ = false;

    static std::filesystem::path discover(const std::filesystem::path& executable) {
        namespace fs = std::filesystem;
        std::error_code ec;
        const auto dir = executable.parent_path();
        const std::array<fs::path, 5> locations = {{
            dir / "assets/audio/sci-fi",
            fs::current_path(ec) / "assets/audio/sci-fi",
            dir.parent_path() / "assets/audio/sci-fi",
            dir.parent_path().parent_path() / "assets/audio/sci-fi",
            dir.parent_path().parent_path().parent_path() / "assets/audio/sci-fi"
        }};
        for (const auto& location : locations) {
            if (fs::is_regular_file(location / "title.ogg", ec))
                return location;
            ec.clear();
        }
        return {};
    }

    std::unique_ptr<sf::Music> open(Scene scene) const {
        if (folder_.empty()) return {};
        std::error_code ec;
        const auto file = folder_ / filename(scene);
        if (!std::filesystem::is_regular_file(file, ec)) return {};
        auto stream = std::make_unique<sf::Music>();
        if (!stream->openFromFile(file)) return {};
        stream->setLooping(false);
        stream->setVolume(0.f);
        stream->play();
        if (stream->getStatus() != sf::SoundSource::Status::Playing)
            return {};
        return stream;
    }

    bool switchTo(Scene next) {
        std::unique_ptr<sf::Music> fresh;
        try {
            fresh = open(next);
        } catch (...) {
            return false; // GUI remains playable if audio is unavailable.
        }
        if (!fresh) return false;
        if (fallback_) fallback_->stop();
        outgoing_ = std::move(current_);
        outgoingGain_ = currentGain_;
        current_ = std::move(fresh);
        currentGain_ = gain(next);
        playingScene_ = next;
        fade_ = 0.f;
        return true;
    }

    void prepareEffects() {
        if (effectsLoaded_ || folder_.empty()) return;
        effectsLoaded_ = true; // Never retry missing assets on every click.
        try {
            if (hoverBuffer_.loadFromFile(folder_ / "hover.ogg"))
                hover_ = std::make_unique<sf::Sound>(hoverBuffer_);
            if (victoryBuffer_.loadFromFile(folder_ / "victory.ogg"))
                victory_ = std::make_unique<sf::Sound>(victoryBuffer_);
            applyVolume();
        } catch (...) {
            hover_.reset();
            victory_.reset();
        }
    }

    void applyVolume() {
        if (current_)
            current_->setVolume(volume_ * currentGain_ * fade_);
        if (outgoing_)
            outgoing_->setVolume(volume_ * outgoingGain_ * (1.f - fade_));
        if (fallback_) fallback_->setVolume(volume_);
        if (hover_) hover_->setVolume(volume_ * .34f);
        if (victory_) victory_->setVolume(volume_ * .46f);
    }

public:
    explicit Director(const std::filesystem::path& executable)
        : folder_(discover(executable)) {}

    // Music is opt-in, preserving the original F9 behavior and quiet startup.
    bool play() {
        if (enabled_) return true;
        try {
            if (sf::PlaybackDevice::getAvailableDevices().empty()) return false;
            enabled_ = true;
            prepareEffects();
            if (switchTo(requested_)) return true;
            fallback_ = std::make_unique<GymBeat>();
            fallback_->setVolume(volume_);
            if (fallback_->play()) return true;
        } catch (...) {
            // No valid audio output must never interrupt a match.
        }
        stop();
        return false;
    }

    void stop() {
        enabled_ = false;
        if (hover_) hover_->stop();
        if (victory_) victory_->stop();
        outgoing_.reset();
        current_.reset();
        if (fallback_) fallback_->stop();
        fade_ = 1.f;
    }

    void setScene(Scene next) {
        if (next == requested_) return;
        requested_ = next;
        if (enabled_ && current_ && !switchTo(next)) {
            // Keep the last healthy stream when an optional track is missing.
            playingScene_ = next;
        }
    }

    void update(float deltaSeconds) {
        if (!enabled_) return;
        if (current_ && !outgoing_) {
            // Crossfade the final 420 ms into a new stream of the same track,
            // avoiding hard end-to-start seeks at OGG loop boundaries.
            const float remaining = current_->getDuration().asSeconds()
                - current_->getPlayingOffset().asSeconds();
            if (remaining <= .42f) switchTo(playingScene_);
        }
        if (current_ && fade_ < 1.f) {
            fade_ = std::min(1.f, fade_ +
                std::clamp(deltaSeconds, 0.f, .10f) / .42f);
            applyVolume();
            if (fade_ >= 1.f) outgoing_.reset();
        }
    }

    void setVolume(float amount) {
        volume_ = std::clamp(amount, 0.f, 100.f);
        applyVolume();
    }

    void button() {
        if (!enabled_) return;
        prepareEffects();
        if (hover_) hover_->play();
    }

    void win() {
        if (!enabled_) return;
        prepareEffects();
        if (victory_) victory_->play();
    }

    bool enabled() const { return enabled_; }
    bool usingSciFiPack() const { return current_ != nullptr; }
    const char* sourceLabel() const {
        return usingSciFiPack() ? "DARK SCI-FI / SRG774"
                                : "ORIGINAL 116 BPM";
    }
};

} // namespace gymaudio
