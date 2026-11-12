// JK AXIS | Inspect original Sci-Fi assets without opening an audio device.
#pragma once
#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

namespace gymaudio {

inline constexpr std::array<const char*, 8> requiredOgg{{
    "airy.ogg", "hover.ogg", "pulse.ogg", "sector.ogg",
    "title.ogg", "transmission.ogg", "urgent.ogg", "victory.ogg"
}};

inline bool isGymPhonkV4(const std::filesystem::path& folder) {
    std::ifstream profile(folder / "PROFILE.txt");
    std::string marker;
    return static_cast<bool>(std::getline(profile, marker))
        && marker == "JK_AXIS_GYM_PHONK_V4";
}

struct AssetReport {
    std::filesystem::path folder;
    unsigned found = 0;
    bool complete() const { return found == requiredOgg.size(); }
};

inline bool isOgg(const std::filesystem::path& path) {
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec)
        || std::filesystem::file_size(path, ec) < 256 || ec)
        return false;
    std::ifstream input(path, std::ios::binary);
    char magic[4]{};
    return input.read(magic, sizeof(magic))
        && std::string(magic, sizeof(magic)) == "OggS";
}

inline AssetReport inspectFolder(std::filesystem::path folder) {
    AssetReport result{std::move(folder), 0};
    for (const auto* name : requiredOgg)
        if (isOgg(result.folder / name)) ++result.found;
    return result;
}

// Match portable ZIPs first; accept typical VS Code / CMake build paths.
// Never report a pack ready unless all eight source files are present.
inline AssetReport inspectAssets(const std::filesystem::path& executable) {
    namespace fs = std::filesystem;
    std::error_code ec;
    const auto dir = fs::absolute(executable, ec).parent_path();
    const auto suffix = fs::path("assets/audio/sci-fi");
    const std::array<fs::path, 5> candidates{{
        dir / suffix,
        fs::current_path(ec) / suffix,
        dir.parent_path() / suffix,
        dir.parent_path().parent_path() / suffix,
        dir.parent_path().parent_path().parent_path() / suffix
    }};
    AssetReport nearest{candidates[0], 0};
    for (const auto& path : candidates) {
        auto report = inspectFolder(path);
        if (report.complete()) return report;
        if (report.found > nearest.found) nearest = std::move(report);
    }
    return nearest;
}

} // namespace gymaudio
