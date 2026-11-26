<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/brand/jk-axis-precision-symbol-inverse.svg">
    <source media="(prefers-color-scheme: light)" srcset="assets/brand/jk-axis-precision-symbol.svg">
    <img src="assets/brand/jk-axis-precision-symbol.svg" alt="JK AXIS" width="88" height="92">
  </picture>
</p>

# Vier Gewinnt | Gym Edition

C++17-Schulprojekt für die Ausbildung zum Informationstechnischen Assistenten. Zwei bis vier Personen spielen am selben Rechner. Vier gleiche Spielsteine in einer waagerechten, senkrechten oder diagonalen Reihe gewinnen.

[![C++-Tests](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/cpp-quality.yml/badge.svg)](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/cpp-quality.yml)
[![Dokumente](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/docs-quality.yml/badge.svg)](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/docs-quality.yml)

Die Konsole ist die Schulabgabe. SFML-2D, OpenGL-3D und ein optionaler KI-Gegner sind Erweiterungen; sie werden getrennt gebaut.

| Spieler | Spielfeld |
|---|---|
| 2 | 7 × 6 |
| 3 | 9 × 8 |
| 4 | 11 × 10 |

## Schnellstart

Voraussetzung: C++17-Compiler und CMake ab 3.16. Für die Schulversion ist keine Grafikbibliothek nötig.

~~~sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
~~~

Das Programm liegt je nach Generator unter `build/vier-gewinnt` (Linux), `build/vier-gewinnt.exe` (Windows/Ninja) oder `build/Debug/` (Visual Studio).

Mit CMake ab 3.21 und Ninja sind auch diese Befehle möglich:

~~~sh
cmake --preset console
cmake --build --preset console
ctest --preset console
~~~

Weitere Presets: `gui2d`, `gui3d` und `ai`. Für die Grafikversionen gelten zusätzlich die [SFML-/OpenGL-Voraussetzungen](docs/WINDOWS_QUICKSTART.md). Unter Windows empfiehlt sich ein UCRT64-Terminal oder die vorhandene [VS-Code-Konfiguration mit vier F5-Profilen](docs/VS_CODE_DEBUG.md).

## Schulabgabe und Präsentation

~~~sh
python scripts/package-school-console.py
python scripts/package-school-console.py --verify
~~~

- [Schulabgabe: Anleitung und Checkliste](school/README.md)
- [Präsentationsfassungen und Freigabestatus](milestones/PRAESENTATION_VERSIONEN.md)
- [V7.1: jüngste Präsentationskandidaten](milestones/meilenstein-1-und-2/v7.1/README.md)
- [Bisherige 10-Minuten-Arbeitsfassung](praesentation/README.md)

Die tatsächlich verwendete Präsentationsfassung und die Abnahme durch die Lehrkraft müssen separat bestätigt werden.

## Quellcode

| Datei | Aufgabe |
|---|---|
| [Board.cpp](src/Board.cpp) | Spielfeld, Schwerkraft und Gewinnprüfung |
| [Game.cpp](src/Game.cpp) | Eingaben, Menüs und Konsolenspiel |
| [GameState.h](include/GameState.h) | Gemeinsame Zugverwaltung und GYM1-Spielstände |
| [MatchSession.cpp](src/MatchSession.cpp) | Match-Zustand für die Grafikversionen |
| [Player.h](include/Player.h) | Spielername und Symbol |
| [ConsoleUI.cpp](src/ConsoleUI.cpp) | Konsolendarstellung |

[Architektur](docs/ARCHITECTURE.md) · [Dokumentation](docs/README.md) · [Projektverlauf](milestones/README.md) · [Abgabedateien](deliverables/README.md)

## Tests und Nachweise

[Tests und Qualität](docs/QUALITY_AND_TESTS.md) · [Abnahmekriterien](docs/RELEASE_ABNAHME.md) · [Änderungen](CHANGELOG.md) · [KI-Nutzung](school/KI_NUTZUNG.md) · [Lizenzen](docs/LICENSING.md)

Automatisierte Tests ersetzen keine praktische Vorführung auf dem Schul-PC. Die Originalunterlagen und früheren Prüfnachweise bleiben erhalten.
