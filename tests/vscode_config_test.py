#!/usr/bin/env python3
# JK AXIS | GOLD MASTER
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   tests/vscode_config_test.py
# Aufgabe: Prueft, dass F5 auch nach neuen C++-Dateien funktioniert.
# ------------------------------------------------------------

"""Vergleicht die VS-Code-Builddateien mit den echten C++-Quellen."""

import json
from pathlib import Path
import re
import sys


def check(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


def main() -> int:
    root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
    launch = json.loads((root / ".vscode" / "launch.json").read_text(encoding="utf-8"))
    tasks = json.loads((root / ".vscode" / "tasks.json").read_text(encoding="utf-8"))
    cmake = (root / "CMakeLists.txt").read_text(encoding="utf-8")

    # F5 muss die richtige Build-Aufgabe aufrufen.
    configs = [c for c in launch["configurations"]
               if c.get("name") == "Vier Gewinnt | Gym Edition"]
    check(len(configs) == 1, "Debug-Konfiguration fehlt oder doppelt")
    debug = configs[0]
    task_list = [t for t in tasks["tasks"]
                 if t.get("label") == debug.get("preLaunchTask")]
    check(len(task_list) == 1, "preLaunchTask zeigt auf keine eindeutige Aufgabe")
    task = task_list[0]

    check(debug.get("program") == "${workspaceFolder}/vier-gewinnt.exe",
          "F5 startet nicht die gebaute EXE")
    check(debug.get("cwd") == "${workspaceFolder}", "Arbeitsordner fuer Daten fehlt")
    check(task.get("options", {}).get("cwd") == "${workspaceFolder}",
          "Kompilieren muss im Projektordner stattfinden")
    check("-g" in task["args"] and "-O0" in task["args"],
          "Debug-Build benoetigt Symbole und ausgeschaltete Optimierung")

    # Debug- und Buildkonfiguration duerfen keine unbekannten Tasks aufrufen.
    labels = [t.get("label") for t in tasks["tasks"]]
    check(len(labels) == len(set(labels)), "Mehrfach vergebene Task-Namen")
    for configuration in launch["configurations"]:
        check(configuration.get("preLaunchTask") in labels,
              f"Unbekannte Build-Aufgabe: {configuration.get('preLaunchTask')}")
    check(task.get("type") == "process",
          "Compiler direkt starten, ohne Shell-Interpretation der Pfade")
    args = task["args"]
    check("-o" in args and args[args.index("-o") + 1] == "vier-gewinnt.exe",
          "Ausgabedatei muss relativ sein und zur Debug-Konfiguration passen")
    check(task.get("command") == "C:/msys64/ucrt64/bin/g++.exe",
          "UCRT64-Compilerpfad stimmt nicht")
    check(debug.get("miDebuggerPath") == "C:/msys64/ucrt64/bin/gdb.exe",
          "UCRT64-Debuggerpfad stimmt nicht")
    check(any(value.get("name") == "PATH" and "C:/msys64/ucrt64/bin" in value.get("value", "")
              for value in debug.get("environment", [])),
          "UCRT64-Laufzeitbibliotheken muessen auffindbar sein")
    check(all("2D" not in configuration.get("name", "") and
              "3D" not in configuration.get("name", "")
              for configuration in launch["configurations"]),
          "Grafikprogramme ohne Quellcode und Build-Ziel nicht als lauffaehig anbieten")

    # Beide CMake-Ziele muessen im manuellen g++-Build enthalten sein.
    game = re.search(r"add_executable\(vier-gewinnt\s+([^)]*)\)", cmake)
    board = re.search(r"add_library\(gym_board\s+STATIC\s+([^)]*)\)", cmake)
    check(game is not None and board is not None, "CMake-Quelldateien fehlen")
    expected = set(game.group(1).split() + board.group(1).split())
    provided = {item for item in task["args"] if item.endswith(".cpp")}
    check(provided == expected,
          f"VS-Code-Build stimmt nicht mit CMake ueberein: fehlend={expected-provided}, "
          f"zusaetzlich={provided-expected}")
    for path in expected:
        check((root / path).is_file(), f"Quelldatei fehlt: {path}")

    print("VS Code: launch.json, tasks.json und CMake-Dateiliste stimmen ueberein.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
