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

    # Die optionale SFML-3.1-GUI braucht ihren eigenen F5-Eintrag.
    gui = [c for c in launch["configurations"]
           if c.get("name") == "Vier Gewinnt | Gym Edition 2D"]
    check(len(gui) == 1, "GUI-F5-Konfiguration fehlt")
    gui_task = [t for t in tasks["tasks"]
                if t.get("label") == gui[0].get("preLaunchTask")]
    check(len(gui_task) == 1, "GUI Build-Aufgabe fehlt")
    check("vier-gewinnt-gui.exe" in gui[0]["program"], "GUI startet falsche EXE")
    check("src/gui/Main.cpp" in cmake and "GYM_BUILD_GUI" in cmake,
          "Eigenstaendiges GUI-CMake-Ziel fehlt")
    check("Gym Edition 2D konfigurieren" in gui_task[0].get("dependsOn", []),
          "GUI-Build muss SFML zuerst konfigurieren")
    check(any("GYM_BUILD_GUI=ON" in str(arg) for t in tasks["tasks"]
              if t.get("label") == "Gym Edition 2D konfigurieren"
              for arg in t.get("args", [])), "GUI-Konfiguration nicht aktiviert")

    # Die optionale OpenGL-3D-Arena hat einen eigenen F5-Eintrag.
    three_d = [c for c in launch['configurations']
               if c.get('name') == 'Vier Gewinnt | Gym Edition 3D']
    check(len(three_d) == 1, '3D-F5-Konfiguration fehlt oder ist doppelt')
    three_d_tasks = [t for t in tasks['tasks']
                     if t.get('label') == three_d[0].get('preLaunchTask')]
    check(len(three_d_tasks) == 1, '3D-Build-Aufgabe nicht eindeutig')
    check('vier-gewinnt-3d.exe' in three_d[0]['program'], '3D-Profil startet falsche EXE')
    check(three_d[0]['cwd'] == '${workspaceFolder}', '3D-Arbeitsordner fehlt')
    check('Gym Edition 3D konfigurieren' in three_d_tasks[0].get('dependsOn', []),
          '3D-OpenGL-Konfiguration fehlt im Build-Ablauf')
    check('src/gui/Main3D.cpp' in cmake and 'GYM_BUILD_3D' in cmake,
          'Eigenstaendiges OpenGL-CMake-Ziel fehlt')
    check(any('GYM_BUILD_3D=ON' in str(arg) for t in tasks['tasks']
              if t.get('label') == 'Gym Edition 3D konfigurieren'
              for arg in t.get('args', [])), '3D-Konfiguration nicht aktiviert')
    check((root / 'src/gui/Gym3DMath.h').is_file(), '3D-Mathematik-Header fehlt')
    print("VS Code: Konsole, SFML 2D und OpenGL 3D stimmen mit CMake ueberein.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
