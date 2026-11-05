# Vier Gewinnt | Gym Edition
## Handout zum Aufgabenblatt 51 - Programmiertechnik 2026/2027

**Lehrkraft:** Dr. Michael Litera · **Technologie:** C++17  
**Projekt:** Vier Gewinnt | Gym Edition · **Stand:** 31.10.2026  
**GitHub:** https://github.com/Julik527/vier-gewinnt-gym-edition-cpp

**Abgabestatus:** Die Konsolenpflichtfunktionen sind implementiert und automatisiert getestet. Die praktische Vorführung, ein persönlicher Code-Nachweis und die schulische Endabnahme stehen noch aus. Die SFML-2D-Version ist eine freiwillige Erweiterung.

### 1. Ziel und Anforderungen

Ein objektorientiertes Konsolenspiel für **zwei bis vier menschliche Spieler**. Vor dem Spiel werden die Personenzahl und die Namen eingegeben. Hantelsymbole werden spaltenweise auf das unterste freie Feld gesetzt. **Vier gleiche Symbole** waagerecht, senkrecht oder diagonal gewinnen. Bei einem vollen Feld ohne Gewinner endet die Runde unentschieden.

| Spieler | Spalten | Reihen |
|:--:|:--:|:--:|
| 2 | 7 | 6 |
| 3 | 9 | 8 |
| 4 | 11 | 10 |

Die Spielfeldgröße wird einmal zentral berechnet: `Reihen = 6 + 2 * (Spieler - 2)`, `Spalten = Reihen + 1`.

### 2. Menü und Bedienung

Das Konsolen-Hauptmenü zeigt: `1` Neues Spiel, `2` Fortsetzen, `3` Spielstand laden, `4` Bestenliste, `5` Anleitung, `0` Beenden. Beim neuen Spiel folgen Spielerzahl und Namenseingabe. In der Partie wählt man die Spalte. Zusätzliche Funktionen sind Zugvorschau, Undo, Animation, Speichern und Revanche.

Ungültige Zahlen, volle Spalten und unzulässige Namen werden abgewiesen. Nur ein gültiger Zug verändert das Spielfeld und die Zugfolge.

### 3. Objektorientierte Programmstruktur

| Baustein | Aufgabe |
|---|---|
| `Player` | Name und eindeutiges Spielsymbol kapseln |
| `Board` | Spielfeldgröße, Zellen, Setzen, Schwerkraft, Gewinn und Remis |
| `Game` | Konsolenmenü, Eingaben, Spielerwechsel, Runden und Speicherung |
| `ConsoleUI` | Darstellung von Menü, Spielbrett und Meldungen |
| `MatchSession` | Partieverwaltung der freiwilligen 2D-Oberfläche |
| `AtomicFile` / `ScoreFile` | Datei sicherer ersetzen beziehungsweise Highscores prüfen |

**Zusammenspiel:** `Game` erhält die Eingabe und ruft die Funktionen von `Board` auf. `Board` setzt den Stein. Danach prüft `Game` Sieg oder Remis und wechselt gegebenenfalls den aktuellen Spieler. So sind Regeln, Ablauf und Darstellung getrennt.

### 4. Gewinnprüfung

`Board::hasWon` untersucht vom zuletzt gesetzten Stein aus die Achsen **waagerecht (0,1), senkrecht (1,0) sowie diagonal (1,1) und (1,-1)**. In jeder Achse werden gleiche Steine nach vorne und hinten gezählt. Der gesetzte Stein wird einmal addiert. Bei einer Gesamtlänge von mindestens vier liegt ein Sieg vor. Der Algorithmus funktioniert unabhängig von der Spielerzahl.

### 5. Spielstand und Highscore

Das Format `GYM1` speichert in `data/savegame.txt` die Formatkennung, Spielerzahl, Namen, Symbole, aktuellen Spieler, Feldgröße, Zughistorie und Zellinhalt. Beim Laden werden die Daten geprüft; beschädigte Daten dürfen die laufende Runde nicht ersetzen.

In `data/highscore.txt` stehen die dauerhaft gespeicherten Siege pro Spielername. Die Liste wird nach Siegen **absteigend**, bei Gleichstand alphabetisch ausgegeben. Der Schreibhelfer speichert zuerst temporär und ersetzt dann die Zieldatei. Gleichzeitig schreibende Spielprozesse werden dadurch nicht automatisch koordiniert.

### 6. GitHub, Tests und agiles Vorgehen

Die Repository-Ordner heißen `src/` (Quellcode), `include/` (Klassen-Schnittstellen), `tests/` (automatisierte Tests), `docs/` (Projektunterlagen), `praesentation/` (Vortragsdateien). Wichtige Änderungen sind über Commits und einen Entwicklungsbranch nachvollziehbar.

Geprüft werden unter anderem Feldgrößen, gültige/ungültige Züge, alle Gewinnrichtungen, Remis, Speichern/Laden, beschädigte Dateien, Highscores und Menüs. [Automatisierte C++-Prüfung am Commit 466f5530: 9 von 9 Jobs erfolgreich](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/41202025895). Echte Windows- und Schulvorführungen sind getrennte Aufgaben.

Das Product Backlog wird in kleinen Iterationen umgesetzt. Zu den schulischen Donnerstags-Terminen gehören die tatsächliche Funktionsdemo, der Kanban-Status, ein Review und eine Retrospektive. Offene Termine dürfen nicht vorab als abgeschlossen angegeben werden.

### 7. Ergebnis, Erweiterungen und Grenzen

Die verpflichtende **Konsolenfassung** ist der Kern des Projekts. Die JK-AXIS-Gym-Edition erweitert sie freiwillig um Hantelspielsteine, Vorschau, Undo, Revanche, eine **SFML-2D-Oberfläche** und eigene elektronische Stereomusik. Es gibt keinen KI-Gegner und keinen Online-Modus; Konsolen- und GUI-Steuerung sind noch separate Module.

### 8. KI-Unterstützung und Abschluss

Die KI-Unterstützung wird im [Promptregister](KI_PROMPT_DOKUMENTATION.md) transparent dokumentiert. Sie ersetzt nicht das eigene Verstehen und Erklären der Funktionen. Verbindlich sind eine persönlich getestete Anwendung, der nachvollziehbare Quellcode und die geplante Generalprobe am **11.02.2027** vor der Abschlussvorführung am **18.02.2027**.

**Fachliche Referenzen:** `include/Board.h`, `src/Board.cpp`, `src/Game.cpp`, `include/Player.h`, `docs/ARCHITECTURE.md`, `docs/TESTPLAN.md`. Maßgeblich: **Aufgabenblatt 51, Seiten 1–13**.
