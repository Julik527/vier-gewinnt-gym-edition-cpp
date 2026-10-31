// JK AXIS | Vier Gewinnt: replace a completed file, keeping the previous copy
// if writing or replacing fails. Unique temporary names avoid a shared *.tmp path.
#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
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
    static std::atomic<unsigned long long> sequence{0};

    // Each attempt has a fresh timestamp, random nonce and in-process counter.
    // This avoids the deterministic .tmp collision between independent launches.
    fs::path temporary;
    bool available = false;
    for (int attempt = 0; attempt < 12; ++attempt) {
        temporary = target;
        temporary += "." + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count())
            + "." + std::to_string(std::random_device{}())
            + "." + std::to_string(sequence.fetch_add(1)) + ".tmp";
        std::error_code error;
        if (!fs::exists(temporary, error) && !error) {
            available = true;
            break;
        }
    }
    if (!available) return false;

    const auto cleanup = [&temporary]() {
        std::error_code ignored;
        fs::remove(temporary, ignored);
    };
    try {
        std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
        if (!output) return false;
        writer(output);
        output.flush();
        const bool good = static_cast<bool>(output);
        output.close();
        if (!good || !output) {
            cleanup();
            return false;
        }
    } catch (...) {
        cleanup();
        return false;
    }

#ifdef _WIN32
    const bool success = MoveFileExW(temporary.c_str(), target.c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
    std::error_code error;
    fs::rename(temporary, target, error);
    const bool success = !error;
#endif
    if (!success) cleanup();
    return success;
}
} // namespace gym
