# GitHub-Projektverwaltung

**Repository:** https://github.com/Julik527/vier-gewinnt-gym-edition-cpp  
**Standardbranch:** `main`  
**Stand der Anleitung:** 11.11.2026

## Welcher Stand ist aktuell?

Der Standardbranch `main` enthält den Pflichtumfang **C++17-Konsole** sowie die freiwilligen Erweiterungen **SFML-2D** und **OpenGL-3D**. Ein Wechsel auf alte Entwicklungsbranches ist dafür nicht erforderlich. Historische Entwicklungszweige, darunter `quality/gameplay-persistence-2026-10-28`, dokumentieren frühere Arbeiten, sind aber keine allgemeine Startanweisung.

Die schulische Abgabe kann zusätzlich als **isoliertes Konsolen-ZIP** erstellt werden. Es enthält keine Grafik- oder Audioquellen. Details: [Schulabgabe](../school/README.md).

## Repository sicher beziehen oder aktualisieren

Bei einer neuen Arbeitskopie:

```powershell
git clone https://github.com/Julik527/vier-gewinnt-gym-edition-cpp.git
Set-Location .\vier-gewinnt-gym-edition-cpp
git branch --show-current
git status --short
```

Bei einer vorhandenen Arbeitskopie zuerst ungesicherte Änderungen sichern. Nur bei sauberem Arbeitsverzeichnis:

```powershell
git switch main
git pull --ff-only
```

`git pull --ff-only` bricht bei auseinanderlaufenden lokalen Commits ab, statt diese unbemerkt zusammenzuführen. In diesem Fall die Situation gezielt mit `git status`, `git log` und gegebenenfalls `git fetch` prüfen. Kein `reset --hard` und kein Force-Push ohne gesicherte Daten.

## Projekt bauen

**Pflichtkonsole** mit C++17, im Projektstamm:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

Alternativ mit CMake und CTest:

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Die drei VS-Code-Startprofile decken Konsole, 2D und 3D ab. Die grafischen Ziele benötigen eine geeignete CMake-/SFML-Toolchain und Grafiktreiber. Anleitung und Fehleranalyse: [Windows-Schnellstart](WINDOWS_QUICKSTART.md), [VS-Code-Debugging](VS_CODE_DEBUG.md).

## Präsentation und Nachweise

- [Aktuelle Einzelpräsentation: acht Folien, ca. zehn Minuten](../praesentation/README.md)
- [Prüfungslernzettel Programmiertechnik](PT_PRUEFUNG_19_11_2026.md)
- [Architektur mit den drei Frontends](ARCHITECTURE.md)
- [Testnachweise](QUALITY_AND_TESTS.md)
- [Release- und Abnahmekriterien](RELEASE_ABNAHME.md)
- [Hinweise zu den historischen PDF-/PPTX-Dateien](PRUEFUNG_DOKUMENTDATEIEN_2026-11-11.md)

Die Referenz-PDFs und die ältere 17-Folien-Präsentation sind Projektmaterialien aus dem Verlauf. Die aktuelle zehnminütige Fassung ist gesondert verlinkt. Spielstände, persönliche Highscores, Build-Verzeichnisse sowie nicht veröffentlichte Schul- und Gerätekontrollen gehören nicht unkritisch ins Repository.

GitHub Actions belegt nur die tatsächlich ausgeführten Builds und Tests für eine bestimmte Commit-SHA. Der lokale Windows-Start, das Bildschirmbild, die hörbare Audioausgabe und die mündliche Erklärung müssen am vorgesehenen Rechner persönlich geprüft werden.
