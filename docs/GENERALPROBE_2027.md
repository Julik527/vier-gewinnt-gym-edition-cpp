# Generalprobe, Live-Demo und Fachgespräch - Aufgabenblatt 51

**Abschlussvorführung:** Donnerstag, 18.02.2027 · **Generalprobe:** Donnerstag, 11.02.2027.  
**Zeit:** 30 Minuten, davon möglichst 27 Minuten Vortrag/Demo/Code und 3 Minuten Fragen bzw. Reserve.

## 30-Minuten-Ablauf

| Zeit | Inhalt | Nachweis |
|---|---|---|
| 00:00–02:00 | Projekt und Ziel | Pflicht-Konsole von freiwilliger 2D-GUI unterscheiden |
| 02:00–04:00 | Anforderungen | 2/3/4 Personen, Größen 7×6 / 9×8 / 11×10 |
| 04:00–07:00 | OOP-Klassen | `Player`, `Board`, `Game`, `ConsoleUI` |
| 07:00–15:00 | Konsolen-Live-Demo | Neues Spiel, Namen, ungültiger Zug, Vorführdatei, vorbereiteter Sieg, Highscore, 4 Spieler |
| 15:00–20:00 | C++-Code: Raster/Zug | `Board::Board`, `nextFreeRow`, `drop` |
| 20:00–23:00 | C++-Code: Sieg | `countDirection`, `hasWon`, alle 4 Achsen |
| 23:00–25:30 | Datei und Fehler | `Game::loadGame`, `saveGame`, `ScoreFile` |
| 25:30–27:00 | Qualität/GitHub/KI | CI, Commit-Historie, Promptregister und Grenzen |
| 27:00–30:00 | Rückfragen | Nur bei genügend Zeit freiwillige GUI |

## Vorbereitete Testdaten

Unter `tests/fixtures/` liegen bewusst **keine persönlichen Spielstände**:

- `school-near-win-2p.txt`: Ada und Ben, 7×6, sechs Züge. Ada kann mit **Spalte 4** (gezählt 1–7) waagerecht gewinnen.
- `school-four-players.txt`: Ada, Ben, Cem und Dana; 11×10 und acht Züge.
- `school-highscore-demo.txt`: reine Testliste 8, 6, 4, 2 Siege.

Die beiden GYM1-Daten werden automatisch mit `school_demo_fixture_tests` auf Lesbarkeit/Dimensionen und möglichen Sieg geprüft.

**Datensicherheit bei der Demo:** Führe das Programm mit separatem Arbeitsverzeichnis aus, z. B. `C:\Dev\GymDemo`. Kopiere nur für die Vorführung das gewünschte Fixture dort nach `data/savegame.txt`; kopiere die Highscore-Probe nach `data/highscore.txt`. Die persönlichen `data/`-Dateien im normalen Projektordner **nicht** ersetzen. Konsole und GUI nicht gleichzeitig auf dasselbe Verzeichnis schreiben lassen.

## Technische Generalprobe

1. Konsolen-EXE und ggf. GUI **vor dem Unterricht** fertig bauen. Nicht den ersten SFML-Download live durchführen.
2. Wenn der Benutzerpfad ein `ß` enthält, den Build in einem kurzen ASCII-Pfad (`C:\Dev\GymEdition`) anlegen. In PowerShell heißt Laufwerkswechsel `Set-Location V:`, nicht `cd /v`.
3. Zwei Spieler starten, einen ungültigen Wert und einen gültigen Zug zeigen.
4. Mit vorbereiteter Datei einen Sieg und anschließend den Highscore demonstrieren.
5. Vier-Spieler-Datei laden und das vergrößerte Feld erklären.
6. Speichern, Programm schließen, neu öffnen und Laden überprüfen.
7. Auf dem Schul-PC und Beamer mit Stoppuhr prüfen; das Fenster und die Schriftgröße müssen lesbar sein.
8. Alternativ zur Live-GUI bereitgestellte Konsolen-Screenshots und einen GitHub-Testlink vorbereiten.

## Mündliche Kernfragen

**Warum OOP?** `Player` kapselt Daten, `Board` kapselt Regeln, `Game` steuert den Ablauf. So ist die Logik von der Darstellung getrennt.

**Wie wächst das Feld?** Die Zeilenzahl ist `6 + 2 * (Personen - 2)`, die Spaltenzahl ist um eins größer.

**Wie fällt ein Stein?** `nextFreeRow` sucht ab der unteren Zeile. `drop` setzt nur in eine gültige freie Position.

**Wie erkennt das Programm einen Sieg?** Vom letzten gesetzten Stein aus vier Achsen in beide Richtungen auf gleiche Symbole prüfen.

**Warum validiert Laden vor Übernahme?** Fehlerhafte Dateien sollen den aktuellen Spielzustand nicht beschädigen.

**Was ist die Rolle von KI?** Technische Unterstützung, Vorschläge und Dokumentation müssen transparent sein; die tatsächliche fachliche Erklärung und Testbeobachtung sind selbst zu erbringen.

## Checkliste - nur nach realer Durchführung abhaken

- [ ] Konsole ohne Neu-Build gestartet und alle benötigten Funktionen gezeigt
- [ ] 2 und 4 Spieler, Eingabefehler, Gewinn und Highscore demonstriert
- [ ] Save/Load auch nach Neustart erfolgreich
- [ ] Board, Game, Player und Gewinnalgorithmus selbst erläutert
- [ ] GitHub und KI-Promptregister geöffnet
- [ ] Handout als PDF und Ausdruck geprüft
- [ ] 30-Minuten-Generalprobe gemessen: ______ Minuten
- [ ] Beamer/Schul-PC und Fallback überprüft
- [ ] Rückmeldung der Lehrkraft: _______________________

**Aktueller Status:** Ablauf und Vorführdaten vorbereitet, reale Generalprobe **noch offen**.
