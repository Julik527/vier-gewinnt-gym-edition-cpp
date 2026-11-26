#!/usr/bin/env python3
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/vscode_config_test.py
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Prüft Debug- und Build-Konfigurationen von Visual Studio Code.
# ------------------------------------------------------------

"""Prüft, ob F5, CMake-Presets und Build-Ziele zusammenpassen."""

import json
from pathlib import Path
import re
import sys


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def main():
    root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()

    def read_json(name):
        return json.loads((root / name).read_text(encoding="utf-8"))

    modes = {
        "console": ("vier-gewinnt", "OFF", "OFF", "OFF"),
        "gui2d": ("vier-gewinnt-gui", "ON", "OFF", "OFF"),
        "gui3d": ("vier-gewinnt-3d", "ON", "ON", "OFF"),
        "ai": ("vier-gewinnt-ki", "OFF", "OFF", "ON"),
    }
    presets = read_json("CMakePresets.json")
    configs = {p["name"]: p for p in presets["configurePresets"]}
    common = configs["base"]
    check(common["binaryDir"] == "${sourceDir}/build-preset-${presetName}", "Build-Pfad")
    check(common["generator"] == "Ninja", "Generator")
    check(common["cacheVariables"]["CMAKE_BUILD_TYPE"] == "Debug", "Debug-Build")
    check(set(configs) == set(modes) | {"base"}, "Konfigurations-Presets")
    for kind in ("buildPresets", "testPresets"):
        check(len(presets[kind]) == len(modes), f"Doppelte {kind}")
        check({(p["name"], p["configurePreset"]) for p in presets[kind]}
              == {(mode, mode) for mode in modes}, kind)

    raw_tasks = read_json(".vscode/tasks.json")["tasks"]
    tasks = {t["label"]: t for t in raw_tasks}
    launches = read_json(".vscode/launch.json")["configurations"]
    check(len(raw_tasks) == len(tasks) == 8, "Build-Aufgaben fehlen oder sind doppelt")
    check(len(launches) == 4, "F5-Profile fehlen oder sind doppelt")
    cmake = (root / "CMakeLists.txt").read_text(encoding="utf-8")
    flags = ("GYM_BUILD_GUI", "GYM_BUILD_3D", "GYM_BUILD_AI")
    used = set()

    for mode, (target, *values) in modes.items():
        cfg = configs[mode]
        check(cfg["inherits"] == "base", f"{mode}: Basis-Preset")
        check(tuple(cfg["cacheVariables"][flag] for flag in flags) == tuple(values),
              f"{mode}: falsche Optionen")
        source = re.search(r"add_executable\(" + re.escape(target) + r"\s+([^)]*)\)", cmake)
        check(source is not None, f"{target}: CMake-Ziel fehlt")
        for name in source.group(1).split():
            check((root / name).is_file(), f"Quelldatei fehlt: {name}")

        exe = f"${{workspaceFolder}}/build-preset-{mode}/{target}.exe"
        matching = [p for p in launches if p["program"] == exe]
        check(len(matching) == 1, f"{mode}: falsche EXE")
        profile = matching[0]
        check(profile["cwd"] == "${workspaceFolder}", f"{mode}: Arbeitsordner")
        check(profile["miDebuggerPath"].endswith("/ucrt64/bin/gdb.exe"), f"{mode}: GDB")
        check(any(e["name"] == "PATH" and "ucrt64/bin" in e["value"]
                  for e in profile["environment"]), f"{mode}: DLL-Pfad")
        build = tasks[profile["preLaunchTask"]]
        check(build["args"] == ["--build", "--preset", mode, "--target", target,
                                "--parallel", "2"], f"{mode}: Build-Befehl")
        check(len(build["dependsOn"]) == 1 and build["dependsOrder"] == "sequence",
              f"{mode}: Build-Reihenfolge")
        configure = tasks[build["dependsOn"][0]]
        check(configure["args"] == ["--preset", mode,
              "-DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe",
              "-DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe"],
              f"{mode}: Konfiguration")
        for task in (configure, build):
            check(task["type"] == "process" and task["command"].endswith("/ucrt64/bin/cmake.exe"),
                  f"{mode}: CMake")
            check(task["options"]["cwd"] == "${workspaceFolder}", f"{mode}: Build-Ordner")
            check("ucrt64/bin" in task["options"]["env"]["PATH"], f"{mode}: Werkzeugpfad")
            used.add(task["label"])

    check(used == set(tasks), "Nicht verwendete Aufgaben")
    defaults = [t for t in tasks.values() if isinstance(t.get("group"), dict)
                and t["group"].get("isDefault")]
    check(len(defaults) == 1 and defaults[0]["args"][2] == "console", "Standard-Build")
    settings = read_json(".vscode/settings.json")
    check(settings["terminal.integrated.cwd"] == "${workspaceFolder}", "Terminal-Ordner")
    check(settings["terminal.integrated.defaultProfile.windows"] == "PowerShell", "Terminal")
    check(settings["git.autofetch"] is True, "Git-Fetch")
    print("Vier F5-Profile, Presets und CMake-Ziele stimmen überein.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
