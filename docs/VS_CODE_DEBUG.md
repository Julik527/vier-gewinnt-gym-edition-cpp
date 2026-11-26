# VS Code: Spiel starten und debuggen

## Voraussetzungen

- VS Code mit Microsoft C/C++-Erweiterung
- MSYS2 UCRT64 unter `C:\msys64`, mit GCC, GDB, CMake und Ninja
- CMake ab 3.21 für die Konsole und KI, ab 3.28 für die Grafikversionen

Die [Windows-Anleitung](WINDOWS_QUICKSTART.md) enthält Installation und Fehlerdiagnose.

## F5

Öffne den Repository-Stammordner in VS Code. Wähle unter **Ausführen und Debuggen** das gewünschte Profil und drücke F5. CMake konfiguriert und baut automatisch das zugehörige Ziel; Ninja kompiliert nur geänderte Dateien.

| Profil | Preset | Programm |
|---|---|---|
| Konsole | `console` | `build-preset-console/vier-gewinnt.exe` |
| 2D | `gui2d` | `build-preset-gui2d/vier-gewinnt-gui.exe` |
| 3D | `gui3d` | `build-preset-gui3d/vier-gewinnt-3d.exe` |
| KI-Agent | `ai` | `build-preset-ai/vier-gewinnt-ki.exe` |

Die Presets stehen in [CMakePresets.json](../CMakePresets.json). Alle Profile verwenden dieselbe Compiler- und Debuggerumgebung, aber getrennte Build-Ordner.

## Fehler eingrenzen

- Bei einem Build-Fehler zuerst die erste konkrete Fehlermeldung im Terminal prüfen.
- Bei einem alten lokalen Git-Merge Konflikte auflösen oder nach Sicherung eigener Änderungen den Merge abbrechen.
- Bei Unicode- oder zu langen SFML-Pfaden eine separate Kopie unter einem kurzen Pfad wie `C:\Dev\GymEdition` verwenden. Bestehende Dateien nicht löschen.
- Für den ersten Grafik-Build werden SFML 3.1 und seine Abhängigkeiten benötigt. Audio und interaktive Darstellung separat testen.

`python tests/vscode_config_test.py` prüft die Zuordnung von Profilen, Presets und CMake-Zielen. Ein erfolgreicher automatisierter Build ersetzt keinen F5-Test auf dem Schulrechner.
