// Audio policy tests run without a window, speakers, or SFML.
#include "GymAudioScene.h"
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>

void verify(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

int main() {
    using gymaudio::Scene;
    using gymaudio::filename;
    using gymaudio::gain;
    using gymaudio::gameplayScene;
    try {
        verify(std::string(filename(Scene::Intro)) == "airy.ogg", "intro");
        verify(std::string(filename(Scene::Menu)) == "title.ogg", "menu");
        verify(std::string(filename(Scene::Setup)) == "sector.ogg", "setup");
        verify(std::string(filename(Scene::Match)) == "pulse.ogg", "gameplay");
        verify(std::string(filename(Scene::Tense)) == "urgent.ogg", "tension");
        verify(std::string(filename(Scene::Results)) == "transmission.ogg", "results");
        for (Scene scene : {Scene::Intro, Scene::Menu, Scene::Setup,
                            Scene::Match, Scene::Tense, Scene::Results}) {
            verify(gain(scene) > 0.f && gain(scene) <= 1.f, "safe track gain");
        }
        verify(gameplayScene(0, 42) == Scene::Match, "early match");
        verify(gameplayScene(31, 42) == Scene::Match, "before threshold");
        verify(gameplayScene(32, 42) == Scene::Tense, "after threshold");
        verify(gameplayScene(0, 0) == Scene::Match, "invalid empty capacity");
        verify(gameplayScene(-1, 42) == Scene::Match, "negative counter");
        std::cout << "Sci-Fi Audio: scenes, gain factors and progress threshold OK.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Sci-Fi Audio test: " << error.what() << '\n';
        return 1;
    }
}
