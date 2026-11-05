# GitHub-Projektverwaltung

**Repository:** https://github.com/Julik527/vier-gewinnt-gym-edition-cpp
**Standardbranch:** `main` · **Aktuelle Erweiterungen:** `quality/gameplay-persistence-2026-10-28`
**Übergabe:** [Draft-Pull-Request #1](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/1) · **Sichtbarkeit:** öffentlich

## Welcher Stand enthält was?

`main` enthält den bisherigen Konsolenstand. Der Entwicklungsbranch enthält zusätzlich die 2D-GUI, MatchSession, Audio, Schullogo, die aktuellen Setup-Korrekturen und diese Dokumentation. Ein Blick auf `main` allein zeigt daher noch nicht den vollständigen aktuellen Arbeitsstand. Der Pull Request wird nicht automatisch gemergt.

## Aktuelles Projekt holen

```powershell
git clone https://github.com/Julik527/vier-gewinnt-gym-edition-cpp.git
cd vier-gewinnt-gym-edition-cpp
git switch quality/gameplay-persistence-2026-10-28
```

Eine vorhandene Arbeitskopie auf diesem Branch aktualisieren:

```powershell
git fetch origin
git switch quality/gameplay-persistence-2026-10-28
git pull --ff-only
```

Die frühere ZIP enthielt eine andere lokale Historie. Von dort keine fremde Historie über die Online-Entwicklung schreiben. Eigene uncommittete Änderungen erhalten und Git-Konflikte gezielt lösen.

Für lokale Windows-Probleme inklusive Sonderzeichen im Benutzerpfad, dem falschen `main`-Branch und dem F5-GUI-Build gibt es jetzt die [Windows-2D-Schnellstartanleitung](WINDOWS_QUICKSTART.md). Die read-only Diagnose lautet `./scripts/check-2d.ps1` im PowerShell-Projektverzeichnis.

## Konsole bauen

Im Projektstamm muss der UCRT64-Compiler verfügbar sein:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

`ConsoleUI.cpp` gehört ausdrücklich zum Build. Der frühere Befehl ohne diese Datei verursachte Linkerfehler.

## GUI und Vorführung

F5-Profil `Vier Gewinnt | Gym Edition 2D` verwendet CMake, Ninja und UCRT64. CMake 3.28 oder neuer ist erforderlich. Die Aufgaben ergänzen den UCRT64-Suchpfad. Falls `cmake` trotzdem nicht gefunden wird, die Paketinstallation aus der [README](../README.md) durchführen. Das Konsolenprofil ist davon unabhängig.

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\demo.ps1 -Mode Both
```

Das Skript baut seine GUI unter `build-gui-ucrt64`, wartet auf das Ende des Fensters und startet danach die Konsole. F5 verwendet dagegen `build-gui`. Die zwei Build-Ordner haben unterschiedliche Aufgaben und dürfen nicht als derselbe EXE-Pfad behandelt werden. `-NoBuild` setzt bereits vorhandene, passende EXE-Dateien voraus.

## Veröffentlichte Dokumente und Nachweise

Die fünf Referenz-PDFs stehen in `reference/`, PPTX und zwei Präsentations-PDFs in `praesentation/`. Der erste Binär-Upload ist in Commit `53612455` dokumentiert. Die Logo-Migration folgte in `ea8bbef8`. Die alten Dateinamen bleiben für bestehende Links erhalten. Historische Inhalte und neue Nachträge werden im [Dokumentenaudit](DOCUMENTATION_AUDIT_2026-10-28.md) unterschieden.

Der C++-Workflow prüft Linux/Windows in Debug/Release und die SFML-GUI. Der Dokumentenworkflow liest PDF, PPTX, SVG, Testplantabellen und Verweise. Der [CI-Snapshot](evidence/CI_SNAPSHOT_2026-10-28.json) belegt erfolgreiche Läufe am Ausgangsstand `02eb79f8`. Ein Windows-MSVC-Runner ersetzt keinen lokalen UCRT64-F5-Test.

Spielstände, persönliche Highscores und lokale Build-Ordner werden nicht eingecheckt. Das Workflow-Artefakt der GUI ist ein automatischer Windows-Build, keine bescheinigte Schulfreigabe.
