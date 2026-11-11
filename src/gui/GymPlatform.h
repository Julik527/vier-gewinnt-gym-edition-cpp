// Shared SFML platform integration (C++17).
#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <filesystem>
#include <stdexcept>
#include <string>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace gymui {
inline sf::Font systemFont() {
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
inline std::filesystem::path executablePath([[maybe_unused]] const char* argv0) {
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
} // namespace gymui
