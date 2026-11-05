# Qualitätsreview | Vier Gewinnt – Gym Edition

**Datum:** 05.11.2026  
**Basis:** Aufgabenblatt 51, C++17, GitHub-Repository `Julik527/vier-gewinnt-gym-edition-cpp`  
**Prüfumfang:** Spielkern, Datenhaltung, automatische Tests, Dokumentation und GitHub-Workflow.

## 1. Technischer Bestand

| Bereich | Umsetzung und überprüfbarer Nachweis |
|---|---|
| Objektorientierung | `Game`, `Board`, `Player`, `ConsoleUI` mit getrennten Verantwortlichkeiten |
| 2 bis 4 Spieler | Dynamische Felder 7×6, 9×8 und 11×10 |
| Regeln | Schwerkraft, vier Gewinnrichtungen, Remis, Zugwechsel |
| Bedienung | Namenseingabe, Eingabeprüfung, Vorschau, Undo, Revanche und Konsolenmenüs |
| Datenhaltung | `GYM1`-Spielstände, dauerhaft gespeicherte Highscores, kontrolliertes Laden |
| Codequalität | C++17, CMake-Build, Unit-, Integrations- und VS-Code-Konfigurationstests |
| Dokumentation | README, Projektauftrag, Backlog, KI-Prompts, Testplan und Präsentationsdateien |

## 2. Verbesserungen am 05.11.2026

**Speichervorgänge:** Das Spiel schreibt nicht mehr direkt in die vorhandene Spielstands- oder Highscore-Datei. Zunächst wird eine temporäre Datei im gleichen Ordner vollständig geschrieben und geschlossen. Erst danach ersetzt `std::filesystem::rename` den alten Stand. Misslingt ein Schritt, wird die temporäre Datei möglichst entfernt und die vorhandene Datei nicht vorher gelöscht.

**Zusätzliche automatische Regressionen:** wiederholtes Überschreiben eines Spielstands, fehlgeschlagener Schreibvorgang, Bereinigung von `.tmp`-Dateien, Laden einer Vier-Spieler-Partie mit 11×10-Feld und Undo nach Neustart sowie Fortsetzen einer aktiven Partie nach einem abgewiesenen beschädigten Spielstand.

**Nachweis:** Änderungen und Testlauf liegen in [Pull Request #7](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/7). Das tatsächliche CI-Ergebnis ist dort im Reiter *Checks* einzusehen; diese Dokumentation allein ist noch kein Beleg für einen erfolgreichen Lauf.

## 3. Bereits geprüfte Entwicklungsschritte

[Pull Request #6](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/6) integrierte zusätzliche Eingabe-, Remis- und Highscore-Prüfungen. Die vier C++-Testkombinationen (Linux/Windows, Debug/Release) sowie die Dokumentenprüfung wurden beim Merge als erfolgreich kontrolliert.

Historische Dokumentationsabschnitte aus Oktober 2026 bleiben als damaliger Kenntnisstand erhalten. Der neue Review ersetzt keine fehlenden manuellen Abnahmen.

## 4. Grenzen und noch zu prüfende Aspekte

- **Kein garantierter Schutz vor Stromausfall:** Das Umbenennen reduziert das Risiko einer halb geschriebenen Zieldatei, garantiert aber ohne zusätzliche Synchronisationsmechanismen keine vollständige Datenträger-Dauerhaftigkeit.
- **Keine Mehrprozess-Koordination:** Die gleiche Speicherdatei sollte nicht gleichzeitig von mehreren Spielinstanzen beschrieben werden.
- **Schul-PC:** Kompilieren, F5-Debugging, Terminaldarstellung, Eingaben und Dateizugriff auf dem tatsächlich verwendeten Windows-Rechner prüfen.
- **Vorführung:** Ein echtes Spiel mit 2 und 4 Spielern, eine demonstrierte Fehlermeldung, ein Sieg, ein Remis, Speichern/Laden sowie die Highscore-Sortierung vorführen.
- **Fachliches Verständnis:** Konstruktoren, Kapselung, Board-Dimensionierung, Gewinnprüfung, `std::vector`, Datei-I/O, Unit- und Integrationstests selbst erklären können.
- **Projektabgabe:** Handout und Folien visuell kontrollieren, Generalprobe durchführen sowie KI-Nutzung und eigene Arbeitsschritte nachvollziehbar dokumentieren.

## 5. Reproduzierbare lokale Prüfung

Im Repository-Stammverzeichnis:

```bash
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

**Bewertungshinweis:** Die CI-Tests können technische Funktionen belegen, aber keine Schulnote oder fehlerfreie Software garantieren. Nach Aufgabenblatt 51 werden Programm (60 %), Präsentation (20 %) und Handout (20 %) bewertet. Für eine starke Abgabe müssen alle drei Bestandteile verständlich und vollständig sein.
