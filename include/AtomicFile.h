// Gemeinsamer, sicherer Datei-Ersatz fuer Konsole und GUI.
// Gleicher Ordner fuer Temp-Datei und Ziel; Original bleibt bei Fehler erhalten.
#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace gym {
template <typename Writer>
bool writeSafely(const std::string& name, Writer writer) {
    namespace fs = std::filesystem;
    const fs::path target = fs::u8path(name);
    fs::path temporary = target;
    temporary += ".tmp";
    {
        std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
        if (!output) return false;
        writer(output);
        output.flush();
        const bool valid = static_cast<bool>(output);
        output.close();
        if (!valid || !output) {
            std::error_code ignored;
            fs::remove(temporary, ignored);
            return false;
        }
    }
#ifdef _WIN32
    const bool success = MoveFileExW(temporary.c_str(), target.c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
    std::error_code error;
    fs::rename(temporary, target, error);
    const bool success = !error;
#endif
    if (!success) {
        std::error_code ignored;
        fs::remove(temporary, ignored);
    }
    return success;
}
} // namespace gym
