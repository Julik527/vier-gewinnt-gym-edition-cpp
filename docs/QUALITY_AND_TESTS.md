# Qualitätssicherung und Testnachweise

**Projekt:** Vier Gewinnt | Gym Edition · C++17  
**Qualitätsstand:** 28.10.2026 · JK AXIS | GOLD MASTER

## 1. Historische, tatsächlich ausgeführte Prüfungen

Am **25.10.2026** wurden auf Linux mit g++ 14.2.0 folgende Prüfungen dokumentiert:

| ID | Prüfung | Historischer Nachweis | Einschränkung |
|:--|:--|:--|:--|
| Q-01 | Build mit `-std=c++17 -Wall -Wextra -pedantic` | Bestanden | Nicht auf Windows reproduziert |
| Q-02 | Board-Tests kompilieren und ausführen | Bestanden | Nur Spielfeld |
| Q-03 | Dimensionen, Grenzen, vier Gewinnrichtungen, Undo | Bestanden | Kein vollständiger E2E-Test |
| Q-04 | Menüfolge `6, 8, 0` | Bestanden | Nur Smoke-Test |

Diese Tabelle dokumentiert die alte Testausführung und belegt **nicht** den Erfolg der am 28.10.2026 neu ergänzten Tests.

## 2. Am 28.10.2026 implementierte, reproduzierbare Testfälle

| ID | Ebene | Umfang | Datei |
|:--|:--|:--|:--|
| Q-05 | Unit | 2/3/4 Personen, Dimensionen, negative und übergroße Spalten, Schwerkraft | `tests/board_tests.cpp` |
| Q-06 | Unit | horizontal, vertikal, beide diagonalen Gewinnrichtungen; Undo, volle Spalte, volles Feld | `tests/board_tests.cpp` |
| Q-07 | Integration | Spielernamen und doppelte Namen, ungültige Befehle, Vorschau und Undo | `tests/integration_smoke.py` |
| Q-08 | Integration | Speichern, Laden, beschädigte Spielstände, unerwartete Zusatzdaten | `tests/integration_smoke.py` |
| Q-09 | Integration | Vollständige Siegpartie, Highscore +1 und Neustart-Persistenz | `tests/integration_smoke.py` |
| Q-10 | CI-Matrix | Linux/Windows jeweils Debug und Release | `.github/workflows/cpp-quality.yml` |
| Q-11 | Dokumente | 17 PPTX-Folien, 151 PDF-Seiten, SVG-Validität und README-Dateilinks | `scripts/document_preflight.py` |

**Status der neuen Zeilen:** Testcode und Automatisierung sind eingecheckt. Ohne bestätigte CI-Läufe dürfen die Prüfungen nicht als bestanden ausgewiesen werden.

**Wichtige Korrektur:** Die Unit-Tests verwenden eigene Prüfbedingungen statt `assert()`, damit die Kontrolle auch in Release-Builds mit `NDEBUG` erhalten bleibt. Die Ladefunktion weist außerdem Spielstände mit zusätzlichen Tokens nach dem Raster zurück.

## 2a. Neuer Menüablauf vom 28.10.2026

Die Benutzerführung wurde auf ein einzelnes Hauptmenü und einen geführten Zwei-Schritt-Spielstart umgestellt. Neue Tests prüfen:

- **Neues Spiel:** Spielerzahl 2–4, nacheinander eingegebene Namen und direkter Spielstart.
- **Fehleingaben:** Ungültige Spielerzahl, leerer oder mehrfach verwendeter Name.
- **Abbruch:** `0` während des Setups verändert die laufende Runde nicht.
- **Fortsetzen:** `M` kehrt zum Hauptmenü zurück; Auswahl `2` setzt die Partie fort.
- **Sicherheitsabfragen:** Eine aktive Partie wird vor einem Neustart oder dem Laden einer anderen Datei nicht ungefragt überschrieben.
- **Regression:** Undo, Vorschau, Speichern/Laden, beschädigte Spielstände und Highscore.

Diese Szenarien liegen in `tests/integration_smoke.py`. **Implementierter Testcode ist noch kein bestätigtes Testergebnis**. Die erfolgreiche Ausführung auf dem GitHub-CI-System beziehungsweise einem Windows-Rechner muss separat geprüft werden.

## 2b. Ergebnisbildschirm und Revanche

Neu ab 28.10.2026: `Game::showResultMenu()` bietet nach einer abgeschlossenen Partie die Auswahl **Revanche / Neues Spiel / Hauptmenü**. Die Revanche verwendet die vorhandenen Spieler und Symbole, setzt aber Spielfeld, Zugliste und aktiven Spieler zurück. Die Entscheidung wird außerhalb der Spielschleife in `Game::run()` bearbeitet, um rekursive Spielstarts zu vermeiden.

Neue automatisierte Akzeptanzfälle in `tests/integration_smoke.py`:

- Zwei aufeinanderfolgende Siege über den Revanche-Befehl, ohne Namen erneut einzugeben.
- Highscore steigt für diese zwei Siege auf **2**, nicht häufiger.
- Ergebnisoption „Neues Spiel“ führt wieder durch Spielerzahl und Namen.
- Ungültige Ergebnisauswahl wird abgelehnt.
- Hilfe, Info und ungültige Befehle erzeugen keine zusätzlichen Spielfeldabdrucke.
- Hauptmenü zeigt bei laufender Partie die Namen und den nächsten Spieler.

**Nachweisstatus:** Die Tests sind als ausführbarer Code vorhanden. Ein erfolgreicher CTest-/Windows-Lauf muss separat belegt werden; hier wird keiner behauptet.

## 2c. Gym-Edition-Arena und Testabdeckung

Die Spielfelddarstellung wurde in `ConsoleUI.h` / `ConsoleUI.cpp` vom Spielezustand getrennt. `Board::winningCells()` berechnet die tatsächliche Gewinnreihe zur Anzeige.

| ID | Neu implementierter Test | Quelle | Nachweis |
|:--|:--|:--|:--|
| Q-12 | UI-Layout mit 2/4 Spielern, Terminal-Arena, Spielernamen, Gesamtsiegen, 11×10-Raster | `tests/console_ui_tests.cpp` | Ausführung noch prüfen |
| Q-13 | Vorschau ohne Spielstandsänderung, Gewinnanzeige und vier hervorgehobene Hanteln | `tests/console_ui_tests.cpp` | Ausführung noch prüfen |
| Q-14 | Zwei aufeinanderfolgende Siegrunden mit acht markierten Hanteln und korrektem Highscore | `tests/integration_smoke.py` | Ausführung noch prüfen |
| Q-15 | Keine zusätzliche Spielfeld-Ausgabe nach Hilfe, Info und ungültiger Eingabe | `tests/integration_smoke.py` | Ausführung noch prüfen |

Das Hauptmenü und die Spielfeldanzeige verwenden im interaktiven Terminal feste Bildschirminhalte mit optionaler ANSI-Farbe. Umgeleitete Ausgabe bleibt normales Textformat. Das ist eine bewusst begrenzte Windows-/Linux-kompatible Konsolentechnik, keine grafische Fensteroberfläche.

**Status:** Die neu erzeugten Dateien und Verweise wurden auf GitHub kontrolliert. Ein tatsächlich erfolgreicher Compilerlauf auf allen Plattformen ist hier noch nicht belegt und darf nicht als bestanden gelten.

## 2d. P0/P1-Stabilisierung auf Entwicklungsbranch

**Branch:** `quality/gameplay-persistence-2026-10-28` · **Pull Request:** [#1](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/1) · **Zielbranch `main` bleibt unangetastet.**

| Prioritaet | Nachgewiesener Ausgangszustand | Korrektur | Kontrollfall |
|:--|:--|:--|:--|
| P0 | `saveGame()` und `saveHighscores()` haben die vorhandenen Dateien direkt mit `std::ios::trunc` geoeffnet; ein Schreibfehler konnte den alten Stand zerstoeren | Zuerst `.tmp` im selben Ordner schreiben, Stream auf Fehler pruefen, danach unter Windows per `MoveFileExW` beziehungsweise unter Linux per `rename` ersetzen; bei Fehler Original erhalten | `tests/integration_smoke.py`: blockierte Temp-Datei bei bestehendem Spielstand und Highscore |
| P1 | Die Vorschau wurde durch `redraw = true` vor der naechsten Eingabe im ANSI-Terminal sofort wieder entfernt | Vorschau nicht sofort neu zeichnen | Integrationstest kontrolliert, dass zwischen Vorschau und naechster Eingabe keine neue Arena erscheint |
| P1 | Bei vier langen Spielernamen wurde die Statuszeile zu breit | Spieler einzeln untereinander zeigen | Integrationstest mit vier Namen zu je 24 Bytes |
| P1 | Spieler konnten ESC-/Steuerzeichen in ihren Namen verwenden | ASCII-Steuerzeichen bei der Namenseingabe und beim Laden ablehnen | Integrationstest mit ESC-Sequenz |
| P2 | Mehr Effekte, 2D-Mausoberflaeche | Bewusst nicht Teil des Korrektur-Patches; keine neue Abhaengigkeit | Separater Architekturentscheid erforderlich |

**Kompatibilitaet:** Das Speicherformat bleibt `GYM1`. Bereits vorhandene Spielstands- und Highscore-Dateien werden nicht migriert. Die temporären Dateien dienen ausschliesslich dazu, den bisherigen Stand vor fehlgeschlagenen Schreibvorgaengen zu schuetzen.

**Nachweisgrenze:** Eingebaute Tests allein belegen keine erfolgreiche Ausfuehrung. Den CI-Status im Pull Request pruefen; interaktive Windows-11-F5-Tests im manuellen Testplan dokumentieren.

## 3. Lokal reproduzieren

Voraussetzungen: CMake 3.16+, C++17-Compiler, Python 3 für Integrationstests.

```bash
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Zur PDF- und Markenprüfung zusätzlich Python-Pakete `PyMuPDF` und `python-pptx` installieren:

```bash
python -m pip install PyMuPDF python-pptx
python scripts/document_preflight.py
```

Die Tests arbeiten mit temporären Verzeichnissen. Persönliche Highscores und Spielstände bleiben außerhalb des Repositories.

## 4. Offene externe Abnahme

- Ergebnis aller GitHub-Actions-Jobs einschließlich Windows **verifizieren**, nicht nur Workflow-Dateien prüfen.
- Alle 151 PDF-Seiten visuell auf Lesbarkeit, alte Textkennzeichnungen, überlagerte Grafiken und Quellen prüfen.
- PowerPoint in Microsoft Office am Vorführgerät öffnen und Schriftumbruch, Animationen sowie Projektordateien kontrollieren.
- Interaktiven vollständigen Spieldurchlauf am Schulrechner bestätigen.
- Lehrkraftfreigabe und schulische Bewertung stehen außerhalb dieser technischen Prüfung.

**Freigaberegel:** Nur tatsächlich ausgeführte und dokumentierte Prüfungen dürfen als „bestanden“ markiert werden. Ein Gold-Standard-Design ist keine Abnahmezertifizierung.
