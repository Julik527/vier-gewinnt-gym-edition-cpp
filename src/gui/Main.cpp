// JK AXIS | Gym Edition -- eigenstaendiges 2D-Spiel, SFML 3.1.
// Die Regeln und das kompatible GYM1-Format kommen aus MatchSession / Board.
#include "MatchSession.h"
#include "GymBeat.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {
constexpr float W = 1280.f;
constexpr float H = 800.f;
const sf::Color navy(7, 15, 29);
const sf::Color navy2(12, 27, 48);
const sf::Color panel(21, 40, 67);
const sf::Color blue(65, 139, 250);
const sf::Color white(242, 248, 255);
const sf::Color muted(151, 175, 204);
const sf::Color green(64, 225, 177);
const sf::Color sky(125, 196, 255);
const sf::Color border(48, 82, 122);

sf::Color tokenColor(int player) {
    const std::array<sf::Color, 4> colors = {
        sf::Color(83, 164, 255), sf::Color(244, 194, 92),
        sf::Color(94, 224, 166), sf::Color(185, 147, 255)
    };
    return colors[static_cast<std::size_t>(std::clamp(player, 0, 3))];
}

sf::Font systemFont() {
    sf::Font font;
    const std::array<const char*, 6> locations = {
        "C:/Windows/Fonts/segoeui.ttf", "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf"
    };
    for (const char* path : locations) if (font.openFromFile(path)) return font;
    throw std::runtime_error("Keine Systemschrift gefunden (Segoe UI, Arial oder DejaVu Sans).");
}

// Windows supplies argv[0] in a narrow process code page, not necessarily UTF-8.
// Constructing std::filesystem::path directly from it can throw when the user's
// directory contains characters such as 'ß'. Ask Windows for the actual module
// path in UTF-16, keeping the path wide all the way to std::filesystem.
std::filesystem::path executablePath(const char* argv0) {
#ifdef _WIN32
    std::wstring buffer(512, L'\0');
    while (buffer.size() <= 32768) {
        const DWORD length = GetModuleFileNameW(
            nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        if (length == 0)
            throw std::runtime_error("Windows konnte den Programmpfad nicht lesen.");
        if (length < buffer.size()) {
            buffer.resize(length);
            return std::filesystem::path(buffer);
        }
        buffer.resize(buffer.size() * 2);
    }
    throw std::runtime_error("Windows-Programmpfad ist zu lang.");
#else
    return std::filesystem::absolute(
        std::filesystem::u8path(argv0 ? argv0 : ""));
#endif
}

enum class Screen { Menu, Count, Names, Confirm, Arena, Result, Scores, Help,
                    ConfirmNew, ConfirmLoad, ConfirmExit };
struct Button {
    std::string text;
    sf::FloatRect box;
    int action;
    bool enabled = true;
};

class App {
public:
    explicit App(const std::filesystem::path& executable) :
            window_(sf::VideoMode({1280u, 800u}), "JK AXIS | Vier Gewinnt - Gym Edition"),
            font_(systemFont()) {
        window_.setFramerateLimit(60);
        resizeView();
        // Das originale Schul-Logo wird unveraendert aus einer lokalen Bilddatei geladen,
        // sofern es fuer das Schulprojekt zur Verfuegung gestellt wurde.
        // Bei F5 und beim Start direkt aus dem Build-Ordner das Asset finden.
        const auto executableDir = std::filesystem::absolute(executable).parent_path();
        const auto asset = std::filesystem::path("assets/school/bernd-blindow-schulen.jpg");
        const std::array<std::filesystem::path, 4> candidates = {{
            std::filesystem::current_path() / asset,
            executableDir / asset,
            executableDir.parent_path() / asset,
            executableDir.parent_path().parent_path() / asset
        }};
        for (const auto& location : candidates) {
            if (std::filesystem::is_regular_file(location) &&
                schoolLogo_.loadFromFile(location)) {
                schoolLogoReady_ = true;
                break;
            }
        }
    }

    void run(int maxFrames = -1) {
        int rendered = 0;
        sf::Clock frame;
        while (window_.isOpen()) {
            const float dt = std::min(frame.restart().asSeconds(), 0.08f);
            uiTime_ += dt;
            while (const auto event = window_.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    if (match_.active()) screen_ = Screen::ConfirmExit;
                    else window_.close();
                    continue;
                }
                if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                    if (resized->size.x && resized->size.y) resizeView();
                }
                if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left)
                        click(window_.mapPixelToCoords(mouse->position));
                }
                if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                    keyDown(key->code);
                if (const auto* typed = event->getIf<sf::Event::TextEntered>())
                    textInput(typed->unicode);
            }
            // Nach Fenster-X kein Rendern auf ein bereits geschlossenes Fenster.
            if (!window_.isOpen()) break;
            if (falling_ && screen_ == Screen::Arena) {
                fallRow_ += dt * (fallRow_ < targetRow_ - 1.f ? 13.f : 7.f);
                if (fallRow_ >= static_cast<float>(targetRow_)) {
                    falling_ = false;
                    match_.drop(fallColumn_);
                    if (match_.finished()) screen_ = Screen::Result;
                }
            }
            render();
            if (maxFrames > 0 && ++rendered >= maxFrames) window_.close();
        }
    }
private:
    sf::RenderWindow window_;
    sf::Font font_;
    sf::Texture schoolLogo_;
    bool schoolLogoReady_ = false;
    std::unique_ptr<GymBeat> beat_;
    bool musicOn_ = false;
    int musicVolume_ = 40;
    float uiTime_ = 0.f;
    MatchSession match_;
    Screen screen_ = Screen::Menu;
    Screen helpReturn_ = Screen::Menu;
    int number_ = 2;
    std::vector<std::string> names_;
    std::string input_;
    std::string notice_;
    bool animation_ = true;
    bool falling_ = false;
    int fallColumn_ = -1;
    int targetRow_ = -1;
    float fallRow_ = -1.2f;
    int selectedColumn_ = 0;

    void resizeView() {
        // Letterboxing verhindert, dass Hanteln bei Fenster-Resize verzerrt wirken.
        const auto size = window_.getSize();
        if (size.x == 0 || size.y == 0) return;
        const float windowRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
        const float target = W / H;
        sf::FloatRect viewport({0.f, 0.f}, {1.f, 1.f});
        if (windowRatio > target) {
            viewport.size.x = target / windowRatio;
            viewport.position.x = (1.f - viewport.size.x) * .5f;
        } else {
            viewport.size.y = windowRatio / target;
            viewport.position.y = (1.f - viewport.size.y) * .5f;
        }
        sf::View view(sf::FloatRect({0.f, 0.f}, {W, H}));
        view.setViewport(viewport);
        window_.setView(view);
    }
    void box(float x, float y, float w, float h, sf::Color fill,
             sf::Color border = sf::Color::Transparent) {
        sf::RectangleShape rect({w, h});
        rect.setPosition({x, y});
        rect.setFillColor(fill);
        rect.setOutlineColor(border);
        rect.setOutlineThickness(border == sf::Color::Transparent ? 0.f : 1.f);
        window_.draw(rect);
    }
    void roundedBox(float x, float y, float w, float h, float radius,
                    sf::Color fill, sf::Color outline = sf::Color::Transparent) {
        sf::ConvexShape shape;
        shape.setPointCount(24);
        radius = std::min(radius, std::min(w, h) * .5f);
        constexpr float pi = 3.14159265358979323846f;
        const std::array<sf::Vector2f, 4> centres = {{
            {w - radius, radius}, {w - radius, h - radius},
            {radius, h - radius}, {radius, radius}
        }};
        for (int corner = 0; corner < 4; ++corner) {
            for (int point = 0; point < 6; ++point) {
                const float angle = (-.5f + corner + point / 5.f) * pi;
                shape.setPoint(static_cast<std::size_t>(corner * 6 + point),
                    {centres[corner].x + std::cos(angle) * radius,
                     centres[corner].y + std::sin(angle) * radius});
            }
        }
        shape.setPosition({x, y});
        shape.setFillColor(fill);
        if (outline != sf::Color::Transparent) {
            shape.setOutlineColor(outline);
            shape.setOutlineThickness(1.f);
        }
        window_.draw(shape);
    }
    void backdrop() {
        sf::VertexArray gradient(sf::PrimitiveType::TriangleStrip, 4);
        gradient[0].position = {0.f, 0.f};
        gradient[1].position = {W, 0.f};
        gradient[2].position = {0.f, H};
        gradient[3].position = {W, H};
        gradient[0].color = sf::Color(9, 22, 41);
        gradient[1].color = sf::Color(14, 32, 56);
        gradient[2].color = sf::Color(6, 13, 27);
        gradient[3].color = sf::Color(9, 22, 39);
        window_.draw(gradient);
        for (int x = 24; x < static_cast<int>(W); x += 64)
            box(static_cast<float>(x), 112, 1, H - 112, sf::Color(83, 139, 189, 12));
        for (int y = 112; y < static_cast<int>(H); y += 64)
            box(0, static_cast<float>(y), W, 1, sf::Color(83, 139, 189, 12));
        sf::CircleShape glow(260.f, 64);
        glow.setPosition({-190.f, 430.f});
        glow.setFillColor(sf::Color(33, 104, 197, 13));
        window_.draw(glow);
        glow.setRadius(200.f);
        glow.setPosition({1030.f, 115.f});
        glow.setFillColor(sf::Color(62, 164, 228, 10));
        window_.draw(glow);
    }
    void menuHero() {
        roundedBox(712, 171, 462, 552, 24, sf::Color(15, 35, 60, 235), border);
        box(742, 209, 46, 3, green);
        label("GYM EDITION  /  2D", 798, 196, 18, sky, true);
        label("DEINE ARENA.", 742, 244, 28, white, true);
        label("DEIN NAECHSTER ZUG ZAEHLT.", 742, 283, 17, muted);

        const float tile = 49.f, ox = 760.f, oy = 347.f;
        roundedBox(745, 332, 396, 279, 16, sf::Color(7, 20, 39), border);
        const std::array<std::array<int, 7>, 4> demo = {{
            {{-1, -1, -1, -1, -1, -1, -1}},
            {{-1, -1, -1, -1, -1, -1, -1}},
            {{-1, -1,  0,  1, -1, -1, -1}},
            {{-1,  1,  0,  0,  1, -1, -1}}
        }};
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 7; ++col) {
                const float x = ox + col * tile, y = oy + row * tile;
                roundedBox(x + 2, y + 2, tile - 5, tile - 5, 8,
                           sf::Color(20, 43, 72));
                const int player = demo[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
                if (player >= 0)
                    dumbbell(x + tile / 2.f, y + tile / 2.f, tile * .61f,
                             tokenColor(player));
            }
        }
        label("LOKAL  /  2-4 SPIELER", 757, 630, 16, green, true);
        label("TAKTIK   •   PRAEZISION   •   SPASS", 757, 665, 15, muted);
        label("WILLKOMMEN ZURUECK", 132, 157, 17, sky, true);
        label("HEBEN. SETZEN.", 132, 185, 33, white, true);
        label("SIEGEN.", 132, 223, 33, green, true);
    }
    void label(const std::string& value, float x, float y, unsigned size = 22,
               sf::Color color = white, bool bold = false) {
        sf::Text text(font_, sf::String::fromUtf8(value.begin(), value.end()), size);
        text.setFillColor(color);
        if (bold) text.setStyle(sf::Text::Bold);
        text.setPosition({x, y});
        window_.draw(text);
    }
    void dumbbell(float cx, float cy, float size, sf::Color color, float opacity = 1.f) {
        color.a = static_cast<std::uint8_t>(255 * opacity);
        const float barW = size * .73f, barH = size * .12f;
        box(cx - barW / 2, cy - barH / 2, barW, barH, color);
        const float plateW = size * .17f, plateH = size * .60f;
        box(cx - size * .42f, cy - plateH / 2, plateW, plateH, color);
        box(cx + size * .25f, cy - plateH / 2, plateW, plateH, color);
        box(cx - size * .48f, cy - plateH * .34f, size * .055f, plateH * .68f, color);
        box(cx + size * .425f, cy - plateH * .34f, size * .055f, plateH * .68f, color);
    }
    void brandSymbol(float x, float y, float scale) {
        // Exakte Dreiecksaufteilung der beiden offiziellen SVG-Polygone.
        // Keine neue Logovariante und keine Bitmap-Skalierung.
        const std::array<sf::Vector2f, 12> j = {{
            {17,80}, {17,18}, {50,75},
            {17,80}, {50,75}, {59,120},
            {59,120}, {50,75}, {77,102},
            {50,18}, {50,75}, {17,18}
        }};
        const std::array<sf::Vector2f, 12> k = {{
            {60,75}, {88,47}, {77,93},
            {77,93}, {88,47}, {114,57},
            {114,57}, {88,47}, {114,18},
            {114,18}, {88,47}, {88,18}
        }};
        const auto drawPart = [&](const auto& points, sf::Color color) {
            sf::VertexArray triangles(sf::PrimitiveType::Triangles);
            for (const auto point : points) {
                sf::Vertex vertex;
                vertex.position = {x + point.x * scale, y + point.y * scale};
                vertex.color = color;
                triangles.append(vertex);
            }
            window_.draw(triangles);
        };
        drawPart(j, white);
        drawPart(k, sf::Color(121, 165, 255));
    }
    void title(const std::string& subtitle) {
        box(0, 0, W, 112, sf::Color(9, 24, 45, 245));
        box(0, 110, W, 2, blue);
        box(20, 110, 125, 2, green);
        brandSymbol(20, 5, .75f);
        label("JK AXIS  |  VIER GEWINNT", 132, 20, 28, white, true);
        label("GYM EDITION  /  HEBEN. SETZEN. SIEGEN.", 133, 63, 16, muted);
        label(subtitle, 803, 29, 18, sky, true);
        if (schoolLogoReady_) {
            sf::Sprite school(schoolLogo_);
            const auto size = schoolLogo_.getSize();
            if (size.x != 0 && size.y != 0) {
                const float scale = std::min(221.f / size.x, 86.f / size.y);
                box(998, 8, 252, 96, white);
                school.setPosition({1010.f, 13.f});
                school.setScale({scale, scale});
                window_.draw(school);
            }
        } else {
            // Der Schulname wird nicht als gefaelschtes Logo ausgegeben.
            label("BERND BLINDOW SCHULEN | RAVENSBURG", 797, 72, 15, muted);
        }
    }
    std::vector<Button> buttons() const {
        auto add = [](std::vector<Button>& b, const char* name, float x, float y,
                      float w, int action, bool enabled = true) {
            b.push_back({name, sf::FloatRect({x, y}, {w, 53.f}), action, enabled});
        };
        const auto compact = [](std::vector<Button>& b, const char* name, float y,
                                int action) {
            b.push_back({name, sf::FloatRect({965.f, y}, {269.f, 42.f}), action, true});
        };
        std::vector<Button> b;
        switch (screen_) {
            case Screen::Menu:
                add(b, "NEUES SPIEL", 132, 289, 452, 1);
                add(b, "SPIEL FORTSETZEN", 132, 357, 452, 2, match_.active());
                add(b, "SPIELSTAND LADEN", 132, 425, 452, 3);
                add(b, "BESTENLISTE", 132, 493, 452, 4);
                add(b, "ANLEITUNG", 132, 561, 452, 5);
                add(b, "BEENDEN", 132, 629, 452, 0);
                break;
            case Screen::Count:
                add(b, "2 SPIELER", 390, 275, 500, 2);
                add(b, "3 SPIELER", 390, 344, 500, 3);
                add(b, "4 SPIELER", 390, 413, 500, 4);
                add(b, "ZURÜCK", 390, 530, 500, 0);
                break;
            case Screen::Names:
                add(b, "NAME ÜBERNEHMEN (ENTER)", 390, 420, 500, 1, !input_.empty());
                add(b, "ABBRECHEN", 390, 510, 500, 0);
                break;
            case Screen::Confirm:
                add(b, "TRAINING STARTEN", 390, 480, 500, 1);
                add(b, "NAMEN KORRIGIEREN", 390, 555, 500, 2);
                add(b, "ABBRECHEN", 390, 630, 500, 0);
                break;
            case Screen::Arena:
                // Erst unterhalb aller vier Spielernamen beginnen.
                compact(b, "LETZTEN ZUG ZURÜCK", 506, 1);
                compact(b, "SPIEL SPEICHERN", 554, 2);
                compact(b, animation_ ? "ANIMATION: AN" : "ANIMATION: AUS", 602, 3);
                compact(b, "ANLEITUNG", 650, 4);
                compact(b, "HAUPTMENUE", 698, 0);
                break;
            case Screen::Result:
                compact(b, "REVANCHE", 516, 1);
                compact(b, "NEUE SPIELER", 568, 2);
                compact(b, "HAUPTMENUE", 620, 0);
                break;
            case Screen::Scores: case Screen::Help:
                add(b, "ZURÜCK", 420, 673, 440, 0);
                break;
            case Screen::ConfirmNew: case Screen::ConfirmLoad: case Screen::ConfirmExit:
                add(b, "JA, WEITER", 385, 436, 242, 1);
                add(b, "NEIN, ZURÜCK", 654, 436, 242, 0);
                break;
        }
        // Auf allen Bildschirmen erreichbar, aber standardmaessig stumm.
        b.push_back({"-", sf::FloatRect({978.f, 117.f}, {36.f, 37.f}), 97, musicVolume_ > 0});
        b.push_back({musicOn_ ? "TON AN" : "TON AUS",
                     sf::FloatRect({1020.f, 117.f}, {143.f, 37.f}), 99, true});
        b.push_back({"+", sf::FloatRect({1169.f, 117.f}, {36.f, 37.f}), 98, musicVolume_ < 100});
        return b;
    }
    void drawButtons() {
        const auto pixel = sf::Mouse::getPosition(window_);
        const sf::Vector2f mouse = window_.mapPixelToCoords(pixel);
        for (const auto& b : buttons()) {
            const bool hover = b.enabled && b.box.contains(mouse);
            const bool audio = b.action >= 97 && b.action <= 99;
            const bool primary = (screen_ == Screen::Menu && b.action == 1)
                || (screen_ == Screen::Confirm && b.action == 1)
                || (screen_ == Screen::Result && b.action == 1);
            roundedBox(b.box.position.x, b.box.position.y + 3.f,
                       b.box.size.x, b.box.size.y, audio ? 10.f : 13.f,
                       sf::Color(0, 0, 0, audio ? 28 : 45));
            const sf::Color fill = !b.enabled ? sf::Color(29, 43, 62)
                : hover ? sf::Color(42, 105, 176)
                : primary ? sf::Color(34, 96, 170)
                : sf::Color(22, 48, 77);
            roundedBox(b.box.position.x, b.box.position.y,
                       b.box.size.x, b.box.size.y, audio ? 10.f : 13.f,
                       fill, b.enabled ? (hover || primary ? blue : border)
                                       : sf::Color(44, 61, 79));
            if (primary && b.enabled)
                roundedBox(b.box.position.x + 4.f, b.box.position.y + 8.f,
                           4.f, b.box.size.y - 16.f, 2.f, green);
            const float textX = b.box.position.x + (audio ? 12.f : 22.f);
            const float textY = b.box.position.y + (audio ? 8.f : 13.f);
            label(b.text, textX, textY, audio ? 15u : 17u,
                  b.enabled ? white : muted, true);
            if (screen_ == Screen::Menu && !audio && b.enabled)
                label(">", b.box.position.x + b.box.size.x - 35.f,
                      b.box.position.y + 13.f, 17, primary ? green : sky, true);
        }
    }
    void newSetup() {
        names_.clear(); input_.clear(); number_ = 2;
        notice_.clear();
        screen_ = Screen::Count;
    }
    void toggleMusic() {
        if (musicOn_) {
            if (beat_) beat_->stop();
            musicOn_ = false;
            notice_ = "Soundtrack pausiert. F9 = Fortsetzen.";
            return;
        }
        try {
            // Ohne echtes Ausgabegeraet wuerde SFML ggf. stumm spielen.
            if (sf::PlaybackDevice::getAvailableDevices().empty()) {
                musicOn_ = false;
                notice_ = "Kein Windows-Audiogeraet erkannt.";
                return;
            }
            if (!beat_) beat_ = std::make_unique<GymBeat>();
            beat_->setVolume(static_cast<float>(musicVolume_));
            musicOn_ = beat_->play();
            notice_ = musicOn_
                ? "Original-Soundtrack aktiv. F9: Ton / F10-F11: Lautstaerke."
                : "Audio konnte nicht gestartet werden. Windows-Ausgabe pruefen.";
        } catch (const std::exception&) {
            musicOn_ = false;
            notice_ = "Kein Audio-Ausgabegeraet verfuegbar.";
        }
    }
    void act(int choice) {
        if (choice == 97 || choice == 98) {
            musicVolume_ = std::clamp(musicVolume_ + (choice == 98 ? 10 : -10), 0, 100);
            if (beat_) beat_->setVolume(static_cast<float>(musicVolume_));
            notice_ = "Musiklautstaerke: " + std::to_string(musicVolume_) + "%";
            return;
        }
        if (choice == 99) {
            toggleMusic();
            return;
        }
        switch (screen_) {
            case Screen::Menu:
                if (choice == 1) {
                    if (match_.active()) screen_ = Screen::ConfirmNew;
                    else newSetup();
                } else if (choice == 2 && match_.active()) {
                    screen_ = Screen::Arena;
                } else if (choice == 3) {
                    if (match_.active()) screen_ = Screen::ConfirmLoad;
                    else load();
                } else if (choice == 4) screen_ = Screen::Scores;
                else if (choice == 5) { helpReturn_ = Screen::Menu; screen_ = Screen::Help; }
                else if (choice == 0) {
                    if (match_.active()) screen_ = Screen::ConfirmExit;
                    else window_.close();
                }
                break;
            case Screen::Count:
                if (choice == 0) screen_ = Screen::Menu;
                else {number_ = choice; names_.clear(); input_.clear(); screen_ = Screen::Names;}
                break;
            case Screen::Names:
                if (choice == 0) screen_ = Screen::Menu;
                else acceptName();
                break;
            case Screen::Confirm:
                if (choice == 0) screen_ = Screen::Menu;
                else if (choice == 2) {
                    names_.clear(); input_.clear(); screen_ = Screen::Names;
                } else if (match_.begin(names_)) {
                    notice_.clear(); selectedColumn_ = 0; screen_ = Screen::Arena;
                } else notice_ = "Namen bitte pruefen (1-24 Bytes, eindeutig).";
                break;
            case Screen::Arena:
                if (falling_) return;
                if (choice == 1) {
                    notice_ = match_.undo() ? "Letzter Zug entfernt." : "Kein Zug zum Zuruecknehmen.";
                } else if (choice == 2) {
                    notice_ = match_.save() ? "Spielstand sicher gespeichert." : "Speichern nicht moeglich.";
                } else if (choice == 3) animation_ = !animation_;
                else if (choice == 4) { helpReturn_ = Screen::Arena; screen_ = Screen::Help; }
                else if (choice == 0) screen_ = Screen::Menu;
                break;
            case Screen::Result:
                if (choice == 1 && match_.rematch()) {
                    selectedColumn_ = 0; screen_ = Screen::Arena; notice_.clear();
                } else if (choice == 2) newSetup();
                else if (choice == 0) screen_ = Screen::Menu;
                break;
            case Screen::Help:
                screen_ = helpReturn_;
                break;
            case Screen::Scores:
                screen_ = Screen::Menu;
                break;
            case Screen::ConfirmNew:
                if (choice == 1) newSetup(); else screen_ = Screen::Menu;
                break;
            case Screen::ConfirmLoad:
                if (choice == 1) load(); else screen_ = Screen::Menu;
                break;
            case Screen::ConfirmExit:
                if (choice == 1) window_.close(); else screen_ = Screen::Menu;
                break;
        }
    }
    void load() {
        if (match_.load()) {
            falling_ = false; notice_ = "Spielstand geladen."; selectedColumn_ = 0;
            screen_ = Screen::Arena;
        } else {
            notice_ = "Kein gueltiger GYM1-Spielstand gefunden.";
            screen_ = Screen::Menu;
        }
    }
    void acceptName() {
        if (!MatchSession::validName(input_)) {
            notice_ = "Bitte 1 bis 24 Bytes ohne Steuerzeichen.";
            return;
        }
        if (std::find(names_.begin(), names_.end(), input_) != names_.end()) {
            notice_ = "Jeder Spieler braucht einen eigenen Namen.";
            return;
        }
        names_.push_back(input_); input_.clear(); notice_.clear();
        if (static_cast<int>(names_.size()) == number_) screen_ = Screen::Confirm;
    }
    struct Grid {float x, y, cell;};
    Grid grid() const {
        const float cell = std::min(72.f,
             std::min(520.f / match_.board().rows(), 835.f / match_.board().columns()));
        return {100.f + (815.f - cell * match_.board().columns()) / 2.f,
                185.f, cell};
    }
    void move(int column) {
        if (falling_ || !match_.active()) return;
        const int goal = match_.board().nextFreeRow(column);
        if (goal < 0) {notice_ = "Diese Spalte ist bereits voll."; return;}
        selectedColumn_ = column;
        notice_.clear();
        if (!animation_) {
            match_.drop(column);
            if (match_.finished()) screen_ = Screen::Result;
            return;
        }
        falling_ = true;
        fallColumn_ = column;
        targetRow_ = goal;
        fallRow_ = -1.2f;
    }
    void click(sf::Vector2f point) {
        for (const auto& b : buttons()) {
            if (b.enabled && b.box.contains(point)) { act(b.action); return; }
        }
        if (screen_ == Screen::Arena && !falling_) {
            const auto g = grid();
            const int col = static_cast<int>(std::floor((point.x - g.x) / g.cell));
            if (point.x >= g.x && col >= 0 && col < match_.board().columns()
                && point.y >= g.y - 48.f
                && point.y <= g.y + g.cell * match_.board().rows())
                move(col);
        }
    }
    void textInput(char32_t unicode) {
        if (screen_ != Screen::Names) return;
        if (unicode < 32 || unicode == 127) return;
        const auto utf8 = sf::String(unicode).toUtf8();
        if (input_.size() + utf8.size() <= 24)
            for (const auto byte : utf8) input_.push_back(static_cast<char>(byte));
    }
    void keyDown(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::F10) { act(97); return; }
        if (key == sf::Keyboard::Key::F11) { act(98); return; }
        if (key == sf::Keyboard::Key::F9) {
            toggleMusic();
            return;
        }
        if (key == sf::Keyboard::Key::Escape) {
            if (screen_ == Screen::Arena && falling_) return;
            if (screen_ == Screen::Arena || screen_ == Screen::Result) screen_ = Screen::Menu;
            else if (screen_ == Screen::Names || screen_ == Screen::Confirm ||
                     screen_ == Screen::Count) screen_ = Screen::Menu;
            else if (screen_ == Screen::Help || screen_ == Screen::Scores) act(0);
            else if (screen_ == Screen::Menu) act(0);
            else screen_ = Screen::Menu;
            return;
        }
        if (screen_ == Screen::Names) {
            if (key == sf::Keyboard::Key::Backspace && !input_.empty()) {
                // UTF-8-Zeichen komplett entfernen, nicht nur ein Byte.
                do {
                    input_.pop_back();
                } while (!input_.empty()
                    && (static_cast<unsigned char>(input_.back()) & 0xC0) == 0x80);
            }
            if (key == sf::Keyboard::Key::Enter) acceptName();
            return;
        }
        if (screen_ == Screen::Arena && !falling_) {
            if (key == sf::Keyboard::Key::Left)
                selectedColumn_ = (selectedColumn_ + match_.board().columns() - 1)
                                % match_.board().columns();
            else if (key == sf::Keyboard::Key::Right)
                selectedColumn_ = (selectedColumn_ + 1) % match_.board().columns();
            else if (key == sf::Keyboard::Key::Space || key == sf::Keyboard::Key::Enter)
                move(selectedColumn_);
            else if (key == sf::Keyboard::Key::U) act(1);
            else if (key == sf::Keyboard::Key::S) act(2);
            else if (key == sf::Keyboard::Key::A) act(3);
            else if (key == sf::Keyboard::Key::H) act(4);
        } else if (screen_ == Screen::Menu) {
            if (key == sf::Keyboard::Key::Num1) act(1);
            else if (key == sf::Keyboard::Key::Num2) act(2);
            else if (key == sf::Keyboard::Key::Num3) act(3);
            else if (key == sf::Keyboard::Key::Num4) act(4);
            else if (key == sf::Keyboard::Key::Num5) act(5);
            else if (key == sf::Keyboard::Key::Num0) act(0);
        } else if (screen_ == Screen::Count) {
            if (key == sf::Keyboard::Key::Num2) act(2);
            else if (key == sf::Keyboard::Key::Num3) act(3);
            else if (key == sf::Keyboard::Key::Num4) act(4);
        }
    }
    void renderBoard(bool finished) {
        const auto g = grid();
        const Board& board = match_.board();
        roundedBox(g.x - 24, g.y - 26, g.cell * board.columns() + 48,
            g.cell * board.rows() + 55, 20, sf::Color(15, 34, 61), border);
        roundedBox(g.x - 15, g.y - 15, g.cell * board.columns() + 30,
            g.cell * board.rows() + 30, 14, sf::Color(31, 68, 112), blue);
        const auto pixel = sf::Mouse::getPosition(window_);
        const auto mouse = window_.mapPixelToCoords(pixel);
        int hover = static_cast<int>(std::floor((mouse.x - g.x) / g.cell));
        if (mouse.x < g.x || hover < 0 || hover >= board.columns()) hover = selectedColumn_;
        if (!finished && !falling_ && board.nextFreeRow(hover) >= 0)
            dumbbell(g.x + (hover + .5f) * g.cell, g.y - 29.f,
                     g.cell * .62f, tokenColor(match_.current()), .73f);
        for (int r = 0; r < board.rows(); ++r) {
            for (int c = 0; c < board.columns(); ++c) {
                const float x = g.x + c * g.cell, y = g.y + r * g.cell;
                const bool focused = !finished && c == hover;
                roundedBox(x + 2, y + 2, g.cell - 4, g.cell - 4, 8,
                           focused ? sf::Color(27, 56, 89) : navy2);
                const char symbol = board.cells()[r][c];
                if (symbol != '.') {
                    int player = 0;
                    for (std::size_t i = 0; i < match_.players().size(); ++i)
                        if (match_.players()[i].getSymbol() == symbol) player = static_cast<int>(i);
                    const bool winner = std::find(match_.winLine().begin(), match_.winLine().end(),
                                                  std::make_pair(r, c)) != match_.winLine().end();
                    if (winner)
                        roundedBox(x + 3, y + 3, g.cell - 6, g.cell - 6, 9,
                            sf::Color(29, 100, 81), green);
                    dumbbell(x + g.cell / 2, y + g.cell / 2, g.cell * .78f,
                             winner ? green : tokenColor(player));
                }
            }
        }
        if (falling_)
            dumbbell(g.x + (fallColumn_ + .5f) * g.cell,
                     g.y + (fallRow_ + .5f) * g.cell, g.cell * .78f,
                     tokenColor(match_.current()));
        for (int col = 0; col < board.columns(); ++col)
            label(std::to_string(col + 1), g.x + (col + .5f) * g.cell - 6, g.y - 58, 19, muted);
    }
    void arena(bool end) {
        renderBoard(end);
        roundedBox(947, 168, 300, 594, 18, sf::Color(16, 35, 60), border);
        box(967, 206, 64, 3, end ? green : blue);
        label(end ? "MATCH BEENDET" : "LIVE-MATCH", 967, 180, 20, white, true);
        label("RUNDE " + std::to_string(match_.round()) +
              "    /    " + std::to_string(match_.board().tokenCount()) + " ZUEGE",
              967, 224, 16, muted);
        label(end ? "ERGEBNIS" : "AKTIVER SPIELER", 967, 259, 14, sky, true);
        for (std::size_t i = 0; i < match_.players().size(); ++i) {
            const auto& player = match_.players()[i];
            const float y = 296 + static_cast<float>(i) * 48.f;
            if (static_cast<int>(i) == match_.current() && !end)
                roundedBox(960, y - 3, 276, 45, 10, sf::Color(31, 78, 122), blue);
            dumbbell(983, y + 17, 25, tokenColor(static_cast<int>(i)));
            label(player.getName(), 1007, y, 16);
            const int count = match_.wins(player.getName());
            label(std::to_string(count) + (count == 1 ? " SIEG" : " SIEGE"),
                  1007, y + 23, 13, muted);
        }
        if (end) {
            roundedBox(75, 708, 834, 59, 12, sf::Color(25, 74, 84), green);
            label(match_.winner().empty() ? "REMIS - GUTES TRAINING!"
                     : "GEWONNEN: " + match_.winner(), 95, 718, 23, green, true);
            if (!match_.scoreSaved()) label("Warnung: Highscore konnte nicht gespeichert werden.", 96, 751, 12);
        } else {
            label(falling_ ? "HANTEL FÄLLT ..." : "MAUS: SPALTE WAEHLEN  /  LEERTASTE: SETZEN",
                  89, 710, 17, muted);
        }
    }
    void render() {
        window_.clear(navy);
        backdrop();
        if (screen_ != Screen::Menu && screen_ != Screen::Arena &&
            screen_ != Screen::Result)
            roundedBox(260, 171, 760, 568, 22, sf::Color(12, 30, 52, 232), border);
        title(screen_ == Screen::Arena ? "TRAININGS-ARENA"
              : screen_ == Screen::Result ? "ERGEBNIS"
              : screen_ == Screen::Menu ? "HAUPTMENUE" : "GYM EDITION");
        switch (screen_) {
            case Screen::Menu:
                menuHero();
                if (match_.active()) label("Du kannst deine laufende Runde fortsetzen.", 132, 259, 15, muted);
                break;
            case Screen::Count:
                label("WIE VIELE ATHLETEN SPIELEN?", 390, 190, 26, white, true);
                label("Je nach Spielerzahl wächst das Feld auf 7x6, 9x8 oder 11x10.", 390, 230, 17, muted);
                break;
            case Screen::Names:
                label("SPIELER " + std::to_string(names_.size() + 1) + " VON " +
                      std::to_string(number_), 390, 237, 28, white, true);
                label("Name eingeben, dann Enter druecken.", 390, 295, 18, muted);
                box(390, 343, 500, 59, panel, blue);
                label(input_ + "_", 409, 358, 25, white, true);
                break;
            case Screen::Confirm:
                label("DEIN TEAM IST BEREIT", 390, 194, 28, white, true);
                for (std::size_t i = 0; i < names_.size(); ++i) {
                    dumbbell(408, 267 + 52 * i, 26, tokenColor(static_cast<int>(i)));
                    label(names_[i], 443, 252 + 52 * i, 23);
                }
                break;
            case Screen::Arena: arena(false); break;
            case Screen::Result: arena(true); break;
            case Screen::Scores: {
                label("BESTENLISTE - HARTES TRAINING ZÄHLT", 335, 150, 25, white, true);
                std::vector<std::pair<std::string, int>> sorted(match_.scores().begin(), match_.scores().end());
                std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
                    return a.second != b.second ? a.second > b.second : a.first < b.first;
                });
                if (sorted.empty()) label("Noch keine Siege eingetragen.", 410, 283, 22, muted);
                for (std::size_t i = 0; i < sorted.size() && i < 11; ++i) {
                    label(std::to_string(i + 1) + ".  " + sorted[i].first, 365,
                          227 + 36 * i, 20, i == 0 ? green : white);
                    label(std::to_string(sorted[i].second) + (sorted[i].second == 1 ? " Sieg" : " Siege"), 760,
                          227 + 36 * i, 20, muted);
                }
                break;
            }
            case Screen::Help:
                label("SO FUNKTIONIERT DIE GYM EDITION", 365, 176, 26, white, true);
                label("Vier Hanteln waagerecht, senkrecht oder diagonal gewinnen.", 304, 248, 20);
                label("Maus: Spalte anklicken oder Hantel ueber dem Feld waehlen.", 304, 297, 19, muted);
                label("Tastatur: Pfeile links/rechts, Leertaste oder Enter zum Setzen.", 304, 339, 19, muted);
                label("U = Rueckgaengig   S = Speichern   A = Animation an/aus.", 304, 381, 19, muted);
                label("Original-Musik: F9 an/aus, F10 leiser, F11 lauter.",
                      304, 509, 18, muted);
                label("Escape = Hauptmenue   H = Anleitung.", 304, 423, 19, muted);
                label("Nach einem Sieg: Revanche mit denselben Spielern moeglich.", 304, 465, 19, muted);
                break;
            case Screen::ConfirmNew: case Screen::ConfirmLoad: case Screen::ConfirmExit:
                label("DU HAST NOCH EINE LAUFENDE RUNDE.", 360, 249, 25, white, true);
                label(screen_ == Screen::ConfirmNew ? "Neue Runde starten und bisherige Runde verlassen?"
                      : screen_ == Screen::ConfirmLoad ? "Den laufenden Spielstand durch die Datei ersetzen?"
                      : "Spiel beenden und laufende Runde verlassen?", 330, 317, 20, muted);
                break;
        }
        label("MUSIK  " + std::to_string(musicVolume_) + "%", 826, 124, 14,
              musicOn_ ? green : muted, true);
        if (musicOn_)
            label("116 BPM  |  ORIGINAL SOUNDTRACK", 958, 768, 12, green, true);
        if (!notice_.empty())
            label(notice_, 78, 766, 14, sf::Color(254, 213, 132));
        drawButtons();
        window_.display();
    }
};
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc > 1 && std::string(argv[1]) == "--path-self-test") {
            const auto executable = executablePath(argc > 0 ? argv[0] : "");
            std::error_code error;
            if (!std::filesystem::is_regular_file(executable, error) || error)
                return 1;
            const auto directory = std::filesystem::current_path();
            if (directory.empty()) return 1;
            std::cout << "Unicode-safe executable path: OK\n";
            return 0;
        }
        if (argc > 1 && std::string(argv[1]) == "--audio-devices") {
            const auto devices = sf::PlaybackDevice::getAvailableDevices();
            std::cout << "Erkannte Ausgabegeraete: " << devices.size() << '\n';
            for (const auto& device : devices)
                std::cout << " - " << device << '\n';
            if (devices.empty()) {
                std::cerr << "Kein aktives Windows-Audiogeraet. Lautsprecher, "
                             "Mixer und Windows-Ausgabe pruefen.\n";
                return 2;
            }
            return 0;
        }
        if (argc > 1 && std::string(argv[1]) == "--self-test") {
            MatchSession test;
            if (!test.begin({"Ada", "Ben"})) return 1;
            if (!test.drop(0) || test.board().tokenCount() != 1) return 1;
            std::cout << "GUI match core: OK\n";
            return 0;
        }
        if (argc > 1 && std::string(argv[1]) == "--window-smoke") {
            App app(executablePath(argc > 0 ? argv[0] : ""));
            app.run(12);
            std::cout << "GUI graphical window: 12 frames rendered and closed.\n";
            return 0;
        }
        App app(executablePath(argc > 0 ? argv[0] : ""));
        app.run();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Gym Edition GUI: " << error.what() << '\n';
        return 1;
    }
}
