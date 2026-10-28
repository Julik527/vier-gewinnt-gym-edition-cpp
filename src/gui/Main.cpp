// JK AXIS | Gym Edition -- eigenstaendiges 2D-Spiel, SFML 3.1.
// Die Regeln und das kompatible GYM1-Format kommen aus MatchSession / Board.
#include "MatchSession.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr float W = 1280.f;
constexpr float H = 800.f;
const sf::Color navy(12, 24, 43);
const sf::Color navy2(20, 39, 65);
const sf::Color panel(27, 50, 79);
const sf::Color blue(68, 138, 242);
const sf::Color white(238, 246, 255);
const sf::Color muted(151, 177, 208);
const sf::Color green(71, 212, 153);

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
    App() : window_(sf::VideoMode({1280u, 800u}), "JK AXIS | Vier Gewinnt - Gym Edition"),
            font_(systemFont()) {
        window_.setFramerateLimit(60);
        resizeView();
    }

    void run() {
        sf::Clock frame;
        while (window_.isOpen()) {
            const float dt = std::min(frame.restart().asSeconds(), 0.08f);
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
            if (falling_) {
                fallRow_ += dt * (fallRow_ < targetRow_ - 1.f ? 13.f : 7.f);
                if (fallRow_ >= static_cast<float>(targetRow_)) {
                    falling_ = false;
                    match_.drop(fallColumn_);
                    if (match_.finished()) screen_ = Screen::Result;
                }
            }
            render();
        }
    }
private:
    sf::RenderWindow window_;
    sf::Font font_;
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
    void title(const std::string& subtitle) {
        box(0, 0, W, 112, navy2);
        box(0, 109, W, 3, blue);
        dumbbell(74, 51, 57, blue);
        label("JK AXIS  |  VIER GEWINNT", 122, 19, 29, white, true);
        label("GYM EDITION  /  HEBEN. SETZEN. SIEGEN.", 123, 60, 17, muted);
        label(subtitle, 800, 39, 21, sf::Color(154, 207, 255));
    }
    std::vector<Button> buttons() const {
        auto add = [](std::vector<Button>& b, const char* name, float x, float y,
                      float w, int action, bool enabled = true) {
            b.push_back({name, sf::FloatRect({x, y}, {w, 53.f}), action, enabled});
        };
        std::vector<Button> b;
        switch (screen_) {
            case Screen::Menu:
                add(b, "NEUES SPIEL", 420, 232, 440, 1);
                add(b, "SPIEL FORTSETZEN", 420, 298, 440, 2, match_.active());
                add(b, "SPIELSTAND LADEN", 420, 364, 440, 3);
                add(b, "BESTENLISTE", 420, 430, 440, 4);
                add(b, "ANLEITUNG", 420, 496, 440, 5);
                add(b, "BEENDEN", 420, 562, 440, 0);
                break;
            case Screen::Count:
                add(b, "2 SPIELER", 390, 275, 500, 2);
                add(b, "3 SPIELER", 390, 344, 500, 3);
                add(b, "4 SPIELER", 390, 413, 500, 4);
                add(b, "ZURUECK", 390, 530, 500, 0);
                break;
            case Screen::Names:
                add(b, "NAME UEBERNEHMEN (ENTER)", 390, 420, 500, 1, !input_.empty());
                add(b, "ABBRECHEN", 390, 510, 500, 0);
                break;
            case Screen::Confirm:
                add(b, "TRAINING STARTEN", 390, 480, 500, 1);
                add(b, "NAMEN KORRIGIEREN", 390, 555, 500, 2);
                add(b, "ABBRECHEN", 390, 630, 500, 0);
                break;
            case Screen::Arena:
                add(b, "LETZTEN ZUG ZURUECK", 965, 359, 269, 1);
                add(b, "SPIEL SPEICHERN", 965, 423, 269, 2);
                add(b, animation_ ? "ANIMATION: AN" : "ANIMATION: AUS", 965, 487, 269, 3);
                add(b, "ANLEITUNG", 965, 551, 269, 4);
                add(b, "HAUPTMENUE", 965, 615, 269, 0);
                break;
            case Screen::Result:
                add(b, "REVANCHE", 965, 455, 269, 1);
                add(b, "NEUE SPIELER", 965, 519, 269, 2);
                add(b, "HAUPTMENUE", 965, 583, 269, 0);
                break;
            case Screen::Scores: case Screen::Help:
                add(b, "ZURUECK", 420, 673, 440, 0);
                break;
            case Screen::ConfirmNew: case Screen::ConfirmLoad: case Screen::ConfirmExit:
                add(b, "JA, WEITER", 385, 436, 242, 1);
                add(b, "NEIN, ZURUECK", 654, 436, 242, 0);
                break;
        }
        return b;
    }
    void drawButtons() {
        const auto pixel = sf::Mouse::getPosition(window_);
        const sf::Vector2f mouse = window_.mapPixelToCoords(pixel);
        for (const auto& b : buttons()) {
            const bool hover = b.enabled && b.box.contains(mouse);
            box(b.box.position.x, b.box.position.y, b.box.size.x, b.box.size.y,
                !b.enabled ? sf::Color(36, 48, 67)
                    : hover ? sf::Color(66, 117, 187) : panel,
                !b.enabled ? sf::Color::Transparent : sf::Color(69, 104, 143));
            label(b.text, b.box.position.x + 18, b.box.position.y + 12, 18,
                  b.enabled ? white : muted, true);
        }
    }
    void newSetup() {
        names_.clear(); input_.clear(); number_ = 2;
        notice_.clear();
        screen_ = Screen::Count;
    }
    void act(int choice) {
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
             std::min(540.f / match_.board().rows(), 835.f / match_.board().columns()));
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
        box(g.x - 15, g.y - 15, g.cell * board.columns() + 30,
            g.cell * board.rows() + 30, sf::Color(38, 74, 123), blue);
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
                box(x + 2, y + 2, g.cell - 4, g.cell - 4, navy2);
                const char symbol = board.cells()[r][c];
                if (symbol != '.') {
                    int player = 0;
                    for (std::size_t i = 0; i < match_.players().size(); ++i)
                        if (match_.players()[i].getSymbol() == symbol) player = static_cast<int>(i);
                    const bool winner = std::find(match_.winLine().begin(), match_.winLine().end(),
                                                  std::make_pair(r, c)) != match_.winLine().end();
                    if (winner)
                        box(x + 3, y + 3, g.cell - 6, g.cell - 6, sf::Color(36, 102, 80), green);
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
        box(949, 160, 292, 531, panel);
        label(end ? "TRAINING BEENDET" : "TRAININGSSTATUS", 968, 178, 20, white, true);
        label("RUNDE " + std::to_string(match_.round()), 968, 217, 17, muted);
        label("ZUEGE  " + std::to_string(match_.board().tokenCount()), 968, 243, 17, muted);
        for (std::size_t i = 0; i < match_.players().size(); ++i) {
            const auto& player = match_.players()[i];
            const float y = 278 + static_cast<float>(i) * 54;
            if (static_cast<int>(i) == match_.current() && !end)
                box(960, y - 3, 273, 48, sf::Color(42, 80, 129));
            dumbbell(981, y + 20, 26, tokenColor(static_cast<int>(i)));
            label(player.getName(), 1008, y + 2, 17);
            label(std::to_string(match_.wins(player.getName())) + " Siege", 1008, y + 26, 14, muted);
        }
        if (end) {
            box(77, 710, 829, 58, sf::Color(34, 65, 96));
            label(match_.winner().empty() ? "REMIS - GUTES TRAINING!"
                     : "GEWONNEN: " + match_.winner(), 95, 719, 24, green, true);
            if (!match_.scoreSaved()) label("Warnung: Highscore konnte nicht gespeichert werden.", 96, 751, 12);
        } else {
            label(falling_ ? "HANTEL FAELLT ..." : "MAUS: SPALTE WAEHLEN  /  LEERTASTE: SETZEN",
                  89, 710, 17, muted);
        }
    }
    void render() {
        window_.clear(navy);
        title(screen_ == Screen::Arena ? "TRAININGS-ARENA"
              : screen_ == Screen::Result ? "ERGEBNIS"
              : screen_ == Screen::Menu ? "HAUPTMENUE" : "GYM EDITION");
        switch (screen_) {
            case Screen::Menu:
                label("WAeHLE DEIN TRAINING", 425, 165, 27, white, true);
                if (match_.active()) label("Eine Runde kann fortgesetzt werden.", 424, 194, 17, muted);
                break;
            case Screen::Count:
                label("WIE VIELE ATHLETEN SPIELEN?", 390, 190, 26, white, true);
                label("Je nach Spielerzahl waechst das Feld auf 7x6, 9x8 oder 11x10.", 390, 230, 17, muted);
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
                label("BESTENLISTE - HARTES TRAINING ZAeHLT", 335, 150, 25, white, true);
                std::vector<std::pair<std::string, int>> sorted(match_.scores().begin(), match_.scores().end());
                std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
                    return a.second != b.second ? a.second > b.second : a.first < b.first;
                });
                if (sorted.empty()) label("Noch keine Siege eingetragen.", 410, 283, 22, muted);
                for (std::size_t i = 0; i < sorted.size() && i < 11; ++i) {
                    label(std::to_string(i + 1) + ".  " + sorted[i].first, 365,
                          227 + 36 * i, 20, i == 0 ? green : white);
                    label(std::to_string(sorted[i].second) + " Siege", 760,
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
        if (!notice_.empty())
            label(notice_, 78, 758, 16, sf::Color(254, 213, 132));
        drawButtons();
        window_.display();
    }
};
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc > 1 && std::string(argv[1]) == "--self-test") {
            MatchSession test;
            if (!test.begin({"Ada", "Ben"})) return 1;
            if (!test.drop(0) || test.board().tokenCount() != 1) return 1;
            std::cout << "GUI match core: OK\n";
            return 0;
        }
        App app;
        app.run();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Gym Edition GUI: " << error.what() << '\n';
        return 1;
    }
}
