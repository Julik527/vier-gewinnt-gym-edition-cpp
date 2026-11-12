/**
 * @file src/gui/GymAudio.h
 * @brief Steuert OGG-Musik und Effekte der grafischen Spielversionen.
 * @details gymaudio::Director lädt Szenentracks, mischt Lautstärken, blendet 420 ms über und spielt UI-/Siegsounds.
 * @par Abhängigkeiten
 * SFML 3.1 Audio; GymAudioAssets und GymAudioScene; getrennt von Board und GYM1.
 * @par Wichtige Randbedingungen
 * Bei fehlenden/defekten OGG-Dateien wird keine alte Synthesizer-Musik als Ersatz gestartet.
 */

#pragma once

#include "GymAudioScene.h"
#include "GymAudioAssets.h"
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
    std::filesystem::path executable_;
    AssetReport assets_;
    std::filesystem::path folder_;
    std::string lastError_;
    std::unique_ptr<sf::Music> current_;
    std::unique_ptr<sf::Music> outgoing_;
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

    // Jede Szene nutzt eine eigene OGG-Datei. Streaming hält lange Tracks
    // aus dem RAM; unlesbare Dateien werden als fehlender Stream gemeldet.
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

    // Neuen Track zunächst öffnen, erst danach den bisher laufenden Track
    // als ausblendenden Stream behalten. Bei Fehler bleibt er unberührt.
    bool switchTo(Scene next) {
        std::unique_ptr<sf::Music> fresh;
        try {
            fresh = open(next);
        } catch (...) {
            return false; // GUI remains playable if audio is unavailable.
        }
        if (!fresh) return false;
        outgoing_ = std::move(current_);
        outgoingGain_ = currentGain_;
        current_ = std::move(fresh);
        currentGain_ = gain(next, isGymPhonkV4(folder_));
        playingScene_ = next;
        fade_ = 0.f;
        return true;
    }

    // Kurze Klick-/Sieg-Sounds werden einmalig vollständig geladen.
    // Buffer leben länger als sf::Sound, das auf sie zugreift.
    void prepareEffects() {
        if (effectsLoaded_ || folder_.empty()) return;
        effectsLoaded_ = true; // Fehlende Dateien nicht bei jedem Klick neu laden.
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

    // Benutzerlautstärke, Track-Normalisierung und Überblendungsfaktor
    // werden multipliziert. Effekte sind absichtlich leiser als Musik.
    void applyVolume() {
        if (current_)
            current_->setVolume(volume_ * currentGain_ * fade_);
        if (outgoing_)
            outgoing_->setVolume(volume_ * outgoingGain_ * (1.f - fade_));
        if (hover_) hover_->setVolume(volume_ * .34f);
        if (victory_) victory_->setVolume(volume_ * .46f);
    }

public:
    explicit Director(const std::filesystem::path& executable)
        : executable_(executable), assets_(inspectAssets(executable)),
          folder_(assets_.complete() ? assets_.folder : std::filesystem::path{}) {}

    // The user's Sci-Fi pack is authoritative: no hidden 116-BPM fallback.
    // Musik nur starten, wenn acht OGG-Dateien und ein Ausgabegerät
    // gefunden wurden. Kein unbemerkter Wechsel zur alten Synthesizer-Musik.
    bool play() {
        if (enabled_) return true;
        assets_ = inspectAssets(executable_);
        folder_ = assets_.complete() ? assets_.folder : std::filesystem::path{};
        lastError_.clear();
        if (!assets_.complete()) {
            lastError_ = "SCI-FI OGG " + std::to_string(assets_.found) + "/8 FEHLEN";
            return false;
        }
        try {
            if (sf::PlaybackDevice::getAvailableDevices().empty()) {
                lastError_ = "KEIN AUDIOAUSGABEGERAET";
                return false;
            }
            enabled_ = true;
            if (switchTo(requested_)) {
                prepareEffects();
                return true;
            }
            lastError_ = "SCI-FI OGG NICHT LESBAR / DECODER";
        } catch (...) {
            lastError_ = "SCI-FI AUDIOINITIALISIERUNG FEHLGESCHLAGEN";
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
        fade_ = 1.f;
    }

    // Szenenwechsel aus der GUI; bei fehlender Datei Fehler sichtbar machen.
    void setScene(Scene next) {
        if (next == requested_) return;
        requested_ = next;
        if (enabled_ && current_ && !switchTo(next)) {
            // Never disguise a failed scene as the correct soundtrack.
            lastError_ = "SCI-FI SZENENWECHSEL FEHLGESCHLAGEN";
        } else if (enabled_ && current_) {
            lastError_.clear();
        }
    }

    // Pro Frame die 420-ms-Überblendung vorantreiben und kurz vor dem
    // Dateiende den nächsten Stream starten. Kein samplegenaues Beat-Matching.
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
    const std::string& lastError() const { return lastError_; }
    const AssetReport& assets() const { return assets_; }
    std::string statusLabel() const {
        if (!lastError_.empty()) return lastError_;
        if (enabled_ && usingSciFiPack())
            return isGymPhonkV4(folder_) ? "GYM PHONK V4 AKTIV"
                                        : "SCI-FI AKTIV / SRG774";
        if (assets_.complete())
            return isGymPhonkV4(folder_) ? "GYM PHONK V4 PAUSIERT / F9"
                                        : "SCI-FI PAUSIERT / F9 START";
        return "SCI-FI OGG " + std::to_string(assets_.found) + "/8 FEHLEN";
    }
    const char* sourceLabel() const {
        return usingSciFiPack()
            ? (isGymPhonkV4(folder_) ? "GYM PHONK V4" : "DARK SCI-FI / SRG774")
            : "AUDIO NICHT AKTIV";
    }
};

} // namespace gymaudio
