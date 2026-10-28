# Vortragsleitfaden: Vier Gewinnt Gym Edition

**Stand:** 28.10.2026 · **Ziel:** 15 Minuten Projektvorstellung und 15 Minuten Code mit Demo
Die Sprechtexte sind Übungsformulierungen mit KI-Unterstützung. Sie belegen keine bereits selbstständig gehaltene Präsentation.

## 1. Einstieg in etwa 40 Sekunden

„Mein Projekt ist ein Vier-Gewinnt-Spiel in C++17 für zwei bis vier Personen. Ich habe es als Gym Edition mit Hantel-Spielsteinen gestaltet. Es gibt eine Terminalversion und zusätzlich eine grafische 2D-Anwendung mit SFML. Die Regeln liegen in Board. Die Konsole nutzt Game, die grafische Version eine MatchSession. Dazu kommen Speichern und Laden, eine Bestenliste, Revanche und ein schaltbarer synthetischer Gym-Beat. Bei der Entwicklung habe ich KI-Unterstützung genutzt. Die wichtigsten Funktionen erkläre ich direkt am Code.“

## 2. Zeitplan für den Projektvortrag

| Zeit | Folien | Inhalt |
|:--|:--|:--|
| 00:00–01:30 | 1–2 | Projekt, Ziel und Ablauf vorstellen |
| 01:30–04:00 | 3–4 | Konsole und 2D-Version, Hanteln und Feldgrößen |
| 04:00–06:30 | 5–6 | Menüablauf, Board, Game und MatchSession |
| 06:30–09:00 | 7–8 | Dateien, C++-Schnittstellen und vier Gewinnachsen |
| 09:00–11:30 | 9–10 | GYM1, sicherer Dateiersatz, Revanche, Musik und Logo |
| 11:30–14:00 | 11–12 | Verifizierte CI-Ergebnisse und offene Zielgeräteprüfung |
| 14:00–15:00 | 13–14 | Kernaussage und Wechsel zu VS Code |

Die drei Reservefolien 15–17 dienen Rückfragen und Quellen. Zeitangaben sind ein Plan, keine gemessene Probe.

## 3. Code und beide Programme in 15 Minuten

| Zeit | Datei oder Aktion | Was ich erkläre |
|:--|:--|:--|
| 00–02 | `src/main.cpp`, `src/gui/Main.cpp` | Zwei Einstiegspunkte und zwei Programme |
| 02–05 | `include/Board.h`, `src/Board.cpp` | Raster, Feldgröße, nextFreeRow und drop |
| 05–08 | `Board::hasWon`, `winningCells` | Vier Achsen, beide Richtungen, markierte Gewinnlinie |
| 08–11 | `MatchSession`, `Game`, `AtomicFile.h` | Regeln gegenüber Partiezustand, GYM1 und sicherer Ersatz |
| 11–13 | Grafisches Programm | Zwei Namen, Hantel setzen, F9 kurz zeigen, Fenster schließen |
| 13–15 | Terminalversion | Spiel starten, Vorschau, Undo, Speichern und Menü zeigen |

Die Anwendung vor dem Unterricht fertig bauen. Nicht den ersten SFML-Download während der Vorführung abwarten. Für eine längere Demo entsprechend Zeit im Vortrag einsparen.

## 4. Start und kurze Terminalfolge

Im aktuellen Entwicklungsbranch und Projektstamm:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\demo.ps1 -Mode Both
```

Mit `-NoBuild` nur starten, wenn die EXE-Dateien bereits über dieses Skript gebaut wurden. Die GUI liegt dann unter `build-gui-ucrt64`. Das F5-Profil baut dagegen nach `build-gui`.

Für eine reine Konsolenvorführung:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

Nacheinander eingeben: `1`, `2`, `Julian`, `Daniel`, `V 4`, `4`, `U`, `4`, `S`, `M`, `3`, `j`. Damit zeige ich Einrichtung, Vorschau, Zug, Undo, Speichern und Laden. `A` schaltet in der Konsole die anfänglich ausgeschaltete Animation ein oder aus. In der GUI ist die Fallanimation zunächst an.

## 5. Häufige Lehrerfragen

**Was unterscheidet Board und MatchSession?** Board kennt das Raster und die Regeln. MatchSession merkt sich zusätzlich Spieler, Zugfolge, Ergebnis und Dateien für die GUI.

**Warum gibt es außerdem Game?** Game ist der bestehende Controller der Konsole. Die GUI kam als separates Programm dazu. Beide verwenden Board und den Datei-Helfer. Der gesamte Spielablauf ist noch nicht in einer einzigen gemeinsamen Klasse zusammengeführt.

**Wie funktioniert die Schwerkraft?** nextFreeRow sucht in der gewählten Spalte von unten nach einem freien Feld. drop setzt die Hantel nur bei einem gültigen Ziel.

**Wie erkennt das Programm einen Sieg?** Es zählt vom letzten Stein in vier Achsen und jeweils in beide Richtungen. Ab vier gleichen zusammenhängenden Symbolen ist die Partie gewonnen.

**Warum eine temporäre Datei beim Speichern?** Die bisherige Datei soll bei einem getesteten Schreibfehler erhalten bleiben. Erst nach erfolgreichem Schreiben ersetzt der Helfer das Ziel. Zwei gleichzeitige Spielprozesse werden dadurch nicht koordiniert.

**Ist die Musik ein bekannter Song?** Nein. GymBeat erzeugt Töne und Rhythmus im Programm. F9 schaltet den Loop. Standardmäßig ist die Musik aus.

**Woher stammt das Schullogo?** Von der Bernd Blindow Gruppe. Quelle, CC-BY-SA-4.0-Angabe und die verwendete skalierte Kopie sind in SCHOOL_LOGO.md dokumentiert.

**Was ist getestet?** Automatische Builds und Tests unter Windows und Ubuntu, jeweils Debug und Release für den Kern. Dazu GUI-Release-Builds, MatchSession-Prüfungen und unter Linux ein kurzer virtueller Fensterstart. Der lokale F5- und Hörtest bleibt gesondert nötig.

**Welche Unterstützung hast du genutzt?** Die tatsächlich genutzte KI-Unterstützung angeben und an einer Funktion zeigen, was ich davon verstanden und selbst geprüft habe. Keine auswendig gelernte Behauptung über Eigenleistung verwenden.

## 6. Check vor dem Unterricht

CMake und Ninja verfügbar machen, beide F5-Profile starten, Schullogo und F9 prüfen, Windows-Ausgabegerät kontrollieren, Präsentation in PowerPoint öffnen und Schriftgröße auf dem Beamer testen. Der letzte CMake-Fehler ist erst nach einem erfolgreichen lokalen Wiederholungsversuch als behoben bestätigt.
