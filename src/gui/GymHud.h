/**
 * @file src/gui/GymHud.h
 * @brief Gemeinsame Zeichenelemente der 2D- und 3D-Oberfläche.
 * @details Zeichnet Text, Rechtecke und das JK-AXIS-Zeichen; berechnet den Viewport.
 * @par Abhängigkeiten
 * SFML Graphics und C++17.
 * @par Wichtige Randbedingungen
 * Designkoordinaten bleiben beim Ändern der Fenstergröße erhalten.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <string>

namespace gymui {

inline sf::FloatRect fitViewport(sf::Vector2u size, float width, float height) {
    sf::FloatRect viewport({0.f, 0.f}, {1.f, 1.f});
    if (!size.x || !size.y) return viewport;
    const float displayRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
    const float designRatio = width / height;
    if (displayRatio > designRatio) {
        viewport.size.x = designRatio / displayRatio;
        viewport.position.x = (1.f - viewport.size.x) * .5f;
    } else {
        viewport.size.y = displayRatio / designRatio;
        viewport.position.y = (1.f - viewport.size.y) * .5f;
    }
    return viewport;
}

inline void rectangle(sf::RenderTarget& target, float x, float y, float w, float h,
                      sf::Color fill, sf::Color outline) {
    sf::RectangleShape shape({w, h});
    shape.setPosition({x, y});
    shape.setFillColor(fill);
    shape.setOutlineColor(outline);
    shape.setOutlineThickness(outline == sf::Color::Transparent ? 0.f : 1.f);
    target.draw(shape);
}

inline void text(sf::RenderTarget& target, const sf::Font& font,
                 const std::string& value, float x, float y, unsigned size,
                 sf::Color tint, bool bold) {
    sf::Text label(font, sf::String::fromUtf8(value.begin(), value.end()), size);
    label.setFillColor(tint);
    if (bold) label.setStyle(sf::Text::Bold);
    label.setPosition({x, y});
    target.draw(label);
}

// Dieselben Dreiecke wie im SVG; die Aufrufer bestimmen nur die Farben.
inline void brandSymbol(sf::RenderTarget& target, float x, float y, float scale,
                        sf::Color first, sf::Color second) {
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
    const auto drawPart = [&](const auto& points, sf::Color tint) {
        sf::VertexArray triangles(sf::PrimitiveType::Triangles);
        for (const auto& point : points) {
            sf::Vertex vertex;
            vertex.position = {x + point.x * scale, y + point.y * scale};
            vertex.color = tint;
            triangles.append(vertex);
        }
        target.draw(triangles);
    };
    drawPart(j, first);
    drawPart(k, second);
}
} // namespace gymui
