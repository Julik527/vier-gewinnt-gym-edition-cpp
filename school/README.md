# Vier Gewinnt | Gym Edition – Schulabgabe (nur Konsole)

**C++17 · Objektorientierung · Aufgabenblatt 51 · Programmiertechnik 2026/27**

Dieses Archiv ist die **eigenständig baubare Konsolenfassung**. Es enthält keine SFML- oder OpenGL-Quellen, kein grafisches Programm, keine Audio-Dateien und keine persönlichen Spielstände. Im übergeordneten Entwicklungsrepository dürfen freiwillige Erweiterungen weiterhin bestehen; sie sind **nicht Bestandteil dieser Schulabgabe**.

## Spielumfang

- **2 bis 4 Personen** mit frei wählbaren, eindeutigen Namen.
- Spielfeldgrößen: 2 Personen **7 Spalten × 6 Reihen**, 3 Personen **9 × 8**, 4 Personen **11 × 10**.
- Spaltenauswahl, Schwerkraft, horizontale/vertikale/diagonale Viererreihe, volle Spalte und Remis.
- Hauptmenü, Fortsetzen, Speichern/Laden (`GYM1`), Highscore, Anleitung und Beenden.
- Zusätzliche reine Konsolenfunktionen: Zugvorschau, Undo, Revanche und abschaltbare Textanimation.

## Schnellstart: C++17 mit CMake

Voraussetzung: CMake 3.16 oder neuer, C++17-Compiler. Unter Windows mit MSYS2 UCRT64 zusätzlich Ninja verwenden oder einen korrekt konfigurierten Visual-Studio-Generator auswählen.

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Start nach erfolgreichem Build: Unter MSYS2/Ninja `./build/vier-gewinnt.exe`, unter Visual Studio häufig `./build/Debug/vier-gewinnt.exe`. Auf Linux `./build/vier-gewinnt`. Der genaue Pfad hängt vom CMake-Generator ab.

**Einfacher Windows-UCRT64-Compileraufruf** aus dem Archiv-Stammordner:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

Für die Python-basierten Konsolen-Integrationstests muss Python 3 installiert sein. Fehlt Python, werden diese Tests nicht registriert; dies wird bei der CMake-Konfiguration angezeigt.

## Projektstruktur

```text
src/main.cpp       Startpunkt und Ausnahmebehandlung
src/Board.cpp      Spielfeld, Schwerkraft, Gewinnerkennung
src/Game.cpp       Menü, Runden, Spielstand und Highscores
src/ConsoleUI.cpp  Textausgabe und Konsolenanimation
include/           Klassen und wiederverwendbare Datei-Funktionen
tests/             Unit-, Property- und Konsolen-Integrationstests
data/              Lokale Save-/Highscore-Dateien (werden erst beim Spielen erstellt)
HANDOUT.md         Kompaktes Handout für die Schulabgabe
FACHGESPRAECH.md   Erklärhilfen für die eigenständige mündliche Präsentation
ABGABE_CHECKLISTE.md  Schulische Nachweise und offene Prüfpunkte
TESTPROTOKOLL.md   Leere Vorlage für echte manuelle Testergebnisse
KI_NUTZUNG.md      Transparente Dokumentation der KI-Unterstützung
MANIFEST.json      SHA-256-Dateiliste des Abgabe-Archivs
```

## Grenzen und Prüfnachweise

Die automatische Build- und Testprüfung ist von der realen Windows-Vorführung getrennt. **Ein erfolgreiches CTest-Ergebnis beweist nicht**, dass der Schul-PC, die Eingabeaufforderung und der Beamer bereits persönlich geprüft wurden. Die Vorlage `TESTPROTOKOLL.md` wird erst nach tatsächlicher Durchführung ausgefüllt. Eine Note oder Lehrkraftfreigabe wird hier nicht vorweggenommen.

Sichere Speicherung reduziert das Risiko beschädigter Dateien. Sie garantiert keine vollständige Stromausfallsicherheit und koordiniert keine gleichzeitigen Programmstarts.

Schulische Bewertungsgrundlage gemäß vorliegender Anforderungsmatrix: Programm 60 %, Präsentation 20 %, Handout 20 %. Den verbindlichen Wortlaut von Aufgabenblatt 51 und etwaige Vorgaben der Lehrkraft vor der Abgabe abgleichen.
