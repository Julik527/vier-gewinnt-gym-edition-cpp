// JK AXIS | Public-domain Sci-Fi audio scene and level policy.
// Pure C++17; tested independently of SFML and audio devices.
#pragma once

namespace gymaudio {

enum class Scene { Intro, Menu, Setup, Match, Tense, Results };

constexpr const char* filename(Scene scene) {
    switch (scene) {
        case Scene::Intro:   return "airy.ogg";
        case Scene::Menu:    return "title.ogg";
        case Scene::Setup:   return "sector.ogg";
        case Scene::Match:   return "pulse.ogg";
        case Scene::Tense:   return "urgent.ogg";
        case Scene::Results: return "transmission.ogg";
    }
    return "title.ogg";
}

// Volume compensation from measured integrated loudness.
// Controls are multipliers on the user's 0..100 volume setting.
constexpr float gain(Scene scene) {
    switch (scene) {
        case Scene::Intro:   return 0.48f;
        case Scene::Menu:    return 0.86f;
        case Scene::Setup:   return 0.35f;
        case Scene::Match:   return 0.78f;
        case Scene::Tense:   return 0.85f;
        case Scene::Results: return 0.45f;
    }
    return 0.75f;
}

// Switch to a more intense theme only when a match is well advanced.
constexpr Scene gameplayScene(int occupied, int capacity) {
    return capacity > 0 && occupied >= 0 && occupied * 4 >= capacity * 3
        ? Scene::Tense : Scene::Match;
}

} // namespace gymaudio
