<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/brand/jk-axis-precision-symbol-inverse.svg">
    <source media="(prefers-color-scheme: light)" srcset="assets/brand/jk-axis-precision-symbol.svg">
    <img src="assets/brand/jk-axis-precision-symbol.svg" alt="JK AXIS" width="88" height="92">
  </picture>
</p>

<h1 align="center">Vier Gewinnt | Gym Edition</h1>

<p align="center"><strong>Ein C++17-Spiel mit Training-Arena, eigener 2D-Grafik und nachvollziehbarer Softwaretechnik.</strong></p>

<p align="center">
  <a href="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/cpp-quality.yml"><img src="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/cpp-quality.yml/badge.svg" alt="C++-Qualitätsprüfung"></a>
  <a href="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/docs-quality.yml"><img src="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/docs-quality.yml/badge.svg" alt="Dokumentationsprüfung"></a>
</p>

**ITA-Programmierprojekt 2026/27 · JK AXIS · C++17 · SFML 3.1.0 · CMake / CTest**

> **Versionsstatus:** Die weiterentwickelte 2D-Version befindet sich noch im [Draft-PR #1](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/1). Der Branch `main` enthält bisher die stabile Konsolenbasis. Ein erfolgreicher automatischer Testlauf ist keine vollständige Freigabe des interaktiven Windows-Spiels.

## Schulabgabe: Aufgabenblatt 51

Die fachlichen Pflichtpunkte richten sich nach **Dr. Michael Literas Aufgabenblatt 51 (Programmiertechnik 2026/2027)**. Die Konsolenversion hat Priorität vor zusätzlichen 2D-Effekten.

- [Handout für die Präsentation](docs/HANDOUT_AUFGABENBLATT_51.md)
- [Muss-Anforderungen mit Umsetzungs- und Abnahmestatus](docs/SCHULANFORDERUNGEN_51_STATUS.md)
- [Kanban, Review und Retrospektive vom 31.10.2026](docs/SPRINT_REVIEW_2026-10-31.md)
- [Generalprobe und 30-Minuten-Vorführung](docs/GENERALPROBE_2027.md)
- [Geprüfte, isolierte Vorführspielstände](tests/fixtures/)
- [Aktuelles KI-Promptregister](docs/KI_PROMPT_DOKUMENTATION.md)

**Achtung:** Eine erfolgreiche GitHub-CI ist kein Nachweis, dass die neueste GUI auf dem Schulrechner startet oder dass ein mündliches Fachgespräch bereits bestanden wurde. Manuelle Testfälle und Lehrkraftbewertungen bleiben offen.

## Das Spiel

Mit **2 bis 4 Personen** setzt du Gym-Hanteln in ein Raster. Vier zusammenhängende Symbole gewinnen, horizontal, vertikal oder diagonal. Die Spielbreite passt sich der Spielerzahl an.

| Spieler | Feld | Besonderheiten |
|:--:|:--:|:--|
| 2 | 7 × 6 | Klassische Vier-Gewinnt-Partie |
| 3 | 9 × 8 | Größeres gemeinsames Feld |
| 4 | 11 × 10 | Vier Farben und Zugreihenfolge |

**Bereits implementiert:** Konsolenmodus und optionale SFML-2D-Version, Maus-/Tastatursteuerung, Fallanimation, Gewinnmarkierung, Undo, Vorschau (Konsole), Revanche, Highscores, Speichern/Laden, ein eigens komponierter 116-BPM-Stereo-Soundtrack (standardmäßig aus, F9/F10/F11).

**Wichtige Grenzen:** Kein KI-Gegner, kein Netzwerkspiel, keine 3D-Engine. Die GUI und die Konsole verwenden noch separate Ablauf-Controller; gemeinsam sind die Spielregeln im `Board` und die sichere Dateiersatzfunktion.

## Schnellstart

**Windows 11 und Visual Studio Code:** Die 2D-Version startet nur auf dem Entwicklungsbranch. Wenn `git branch --show-current` noch `main` meldet oder `src/gui/Main.cpp` fehlt, lies zuerst die [Windows-2D-Schnellstartanleitung](docs/WINDOWS_QUICKSTART.md). Dort sind der sichere Git-Branchwechsel, die getestete MSYS2-UCRT64-Toolchain, der Windows-Pfad mit `ß`, GDB-Fehler `-environment-cd` und die Befehle zur Fehlersuche dokumentiert. Im PowerShell-Projektordner bietet `./scripts/check-2d.ps1` eine Diagnose ohne Änderungen.

**Für Spieler:** Sobald ein geprüfter Release bereitsteht, [Releases öffnen](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/releases) und das Windows-ZIP entpacken. Vor einem veröffentlichten Release lassen sich automatisch erzeugte CI-Testpakete unter [Actions](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions) als temporäre Artifacts abrufen. Diese sind keine offizielle Endfassung.

**Für Entwickler, Konsolenspiel (Windows mit MSYS2 UCRT64):**

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

**CMake, Debug und automatische Tests (Linux/Windows):**

```bash
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

**Optional: SFML-Grafikversion (CMake mindestens 3.28):**

```bash
cmake -S . -B build-gui -DGYM_BUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-gui --config Release --target vier-gewinnt-gui
ctest --test-dir build-gui -C Release --output-on-failure
```

SFML 3.1.0 wird bei Bedarf über CMake FetchContent eingebunden. Auf Windows müssen ein geeigneter Compiler und die entsprechenden Laufzeitbibliotheken verfügbar sein.

**VS Code:** Zwei F5-Profile für Konsole und GUI liegen in `.vscode/`. Für den MSYS2-UCRT64-GUI-Build sind CMake, Ninja und die UCRT64-Toolchain erforderlich. [Details und Fehlerbehebung](docs/REPOSITORY_GUIDE.md).

**Schul-Demo in PowerShell:** `.\scripts\demo.ps1 -Mode Both` baut und startet zunächst die Grafik- und danach die Konsolenversion. Die Programme werden absichtlich **nicht parallel** ausgeführt, da sie dieselben lokalen Dateien verwenden.

## Architektur und Codequalität

```text
include/              Board, Player, Game, MatchSession, AtomicFile, ScoreFile
src/Board.cpp         Schwerkraft, Züge, Siegerkennung
src/Game.cpp          Ablauf der Konsole
src/ConsoleUI.cpp     Konsolenoberfläche
src/MatchSession.cpp  Testbare Spiellogik der grafischen Variante
src/gui/Main.cpp      SFML-Fenster, Eingaben, Zeichnen
src/gui/GymBeat.h     Originaler, synthetischer Gym-Beat
tests/                Unit-, Integrations- und Konfigurationsprüfungen
.github/workflows/    Linux-/Windows-/UCRT64-CI und Dokumentenchecks
docs/                 Architektur, Testpläne und Projektverlauf
praesentation/        Präsentation und Vortragsleitfaden
reference/            Schulische Referenzdokumente
```

- **Regeln:** `Board` kennt die Spielpositionen, aber keine Fenster oder Namen.
- **Zustand:** `Game` (Konsole) und `MatchSession` (GUI) koordinieren ihre jeweilige Partie.
- **Persistenz:** `GYM1`-Spielstände werden vor dem Übernehmen geprüft. Highscores werden strikt eingelesen; unvollständige Dateien dürfen keine gültigen Ergebnisse überschreiben.
- **Tests:** CTest umfasst Spielfeld, Spielzustand, Textoberfläche und Integration. Die GitHub-Actions-Matrix prüft unterschiedliche Compiler- und Betriebssystemkonfigurationen.

[Architektur im Detail](docs/ARCHITECTURE.md) · [Tests und Nachweise](docs/QUALITY_AND_TESTS.md) · [Windows-GUI-Testplan](docs/GUI_TESTPLAN.md)

## Präsentation und Projektnachweise

Das Projekt ist **Schulprojekt und technische Arbeitsprobe**, keine bereits benotete oder freigegebene Veröffentlichung.

[Anforderungsanalyse und Projektauftrag](docs/PROJECT_CHARTER.md) · [Product Backlog](docs/PRODUCT_BACKLOG.md) · [Entwicklungsverlauf](docs/DEVELOPMENT_LOG_2026-10-28.md) · [Änderungen](CHANGELOG.md)

[Vortragsleitfaden](praesentation/VORTRAGSLEITFADEN.md) · [Präsentationsdateien](praesentation/) · [Test- und Release-Abnahme](docs/RELEASE_CHECKLIST.md)

**Echte Spielfotos und Gameplay-Video:** noch nicht im Repository veröffentlicht. Als Nachweise sollen ausschließlich direkt aus der laufenden Anwendung aufgenommene Bilder und Videos ergänzt werden.

## Rechte und Herkunft

Eigener Anwendungscode und JK-AXIS-Markenbestandteile sowie das gekennzeichnete Logo der Bernd-Blindow-Schulen haben **unterschiedliche Rechtsgrundlagen**. Ohne explizite Quellcode-Lizenz gilt keine allgemeine Open-Source-Nutzungserlaubnis. Weitere Hinweise: [Schullogo und Attribution](docs/SCHOOL_LOGO.md) sowie [Lizenzhinweise](docs/LICENSING.md). Eine öffentliche Repository-Sichtbarkeit ist keine Freigabe aller Inhalte.

© 2026 Julian Krauß · JK AXIS. Entwicklungsstand, keine Produktzertifizierung.
