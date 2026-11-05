# JK AXIS | Vortragsleitfaden

**Vier Gewinnt | Gym Edition** · Programmiertechnik · C++17 · ITA 2026/27  
**Revision:** 02.11.2026 · Entwicklungsbranch `quality/gameplay-persistence-2026-10-28`  
**Präsentationsziel:** Eine fachlich korrekte, verständliche Demonstration und Codeverteidigung. Die reale Zeitmessung und Schulabnahme stehen noch aus.

## 1. Direkter Einstieg

- [**Teil A: acht vollständig ausformulierte Sprechkarten**](SPRECHKARTEN_TEIL_A_PROJEKT.md)
- [**Teil B: acht vollständig ausformulierte Codekarten**](SPRECHKARTEN_TEIL_B_CODE.md)
- [**Druckfassung A5 quer und digitaler Einzelkartenmodus**](SPRECHKARTEN_DRUCKVERSION.html)
- [**31 typische Fachgesprächsfragen**](FACHGESPRAECH_KURZANTWORTEN.md)
- [**Prüfungs- und Generalprobencheckliste**](PRUEFUNG_CHECKLISTE.md)
- [**Übersicht und Druckanleitung**](README.md)

Die ausführlichen Redetexte stehen ausschließlich in den beiden Karten-Dokumenten. Dieser Leitfaden fasst den Ablauf und die technischen Grundregeln zusammen, ohne eine konkurrierende zweite Vorlesefassung zu erzeugen.

## 2. Zeitplan Teil A · Projektvorstellung (15:00)

| Karte | Zeitpunkt | Vorführen / erklären |
|---|---|---|
| A01 | 00:00–01:20 | Projektidee, Fitness-Design, Pflichtumfang |
| A02 | 01:20–03:05 | 2–4 Spieler, Feldgrößen, vier Gewinnachsen |
| A03 | 03:05–04:50 | Menüs, Vorschau, Undo und Revanche |
| A04 | 04:50–06:40 | `Player`, `Board`, `Game`, `ConsoleUI` |
| A05 | 06:40–08:30 | Konsole, SFML-2D, optionale OpenGL-3D-Erweiterung |
| A06 | 08:30–10:20 | `GYM1`, Dateiprüfung und Highscores |
| A07 | 10:20–12:20 | Teststrategie, GitHub, KI-Unterstützung |
| A08 | 12:20–15:00 | Konsolen-Kurzdemo und Übergang zum Code |

**Vorführprinzip:** Die Schulpflicht wird durch die Konsole demonstriert. Grafische Funktionen nur zeigen, wenn sie auf dem verwendeten Rechner zuvor wirklich geprüft wurden.

## 3. Zeitplan Teil B · C++-Codeerklärung (15:00)

| Karte | Zeitpunkt | Quelldatei / Schwerpunkt |
|---|---|---|
| B01 | 00:00–01:20 | `src/main.cpp`, `Game::run` |
| B02 | 01:20–03:00 | `include/Board.h`, `Board::Board` |
| B03 | 03:00–05:15 | `Board::nextFreeRow`, `Board::drop` |
| B04 | 05:15–08:00 | `Board::countDirection`, `hasWon`, `winningCells` |
| B05 | 08:00–09:50 | `Game::play`, `history_`, Spielerwechsel und Undo |
| B06 | 09:50–12:05 | `Game::saveGame`, `loadGame`, `AtomicFile.h` |
| B07 | 12:05–13:25 | `tests/board_tests.cpp`, CTest, GitHub Actions |
| B08 | 13:25–15:00 | Gesamtfluss, Grenzen und fachlicher Abschluss |

**Bei Fragen innerhalb der 30 Minuten:** Beide Teile auf jeweils 13:30 kürzen und 03:00 für das Fachgespräch reservieren. Der Zeitplan mit vollständig 15 + 15 Minuten **enthält keine** zusätzliche Fragerunde.

## 4. Fachliche Kernaussagen

**Objektorientierung:** `Player` kapselt Name und Symbol, `Board` die Rasterdaten und Regeln, `Game` die Konsolensteuerung. `ConsoleUI` liefert die Darstellung. Die grafischen Varianten nutzen `MatchSession` als weiteren Controller und teilen sich die Regeln über `Board`.

**Schwerkraft:** `nextFreeRow` sucht von der untersten Reihe nach oben. Die sichtbare Spaltennummer wird vor dem Zugriff auf einen nullbasierten Index umgerechnet. `drop` setzt nur an einer gültigen freien Position.

**Gewinnprüfung:** `hasWon` ermittelt die zusammenhängende Länge in vier Achsen und jeweils zwei Richtungen. Die Länge ist `1 + links + rechts` beziehungsweise das entsprechende Richtungs-Paar. Ab vier gleichen Symbolen liegt ein Sieg vor.

**Datensicherheit:** `GYM1` speichert Spieler-, Feld- und Zugdaten. Der Loader prüft einen Kandidaten und übernimmt erst danach den tatsächlichen Zustand. `writeSafely` schreibt zuerst temporär und ersetzt anschließend die Zieldatei; konkurrierende Prozesse werden dadurch nicht serialisiert.

**Nachweisgrenzen:** Grüne automatisierte Prüfungen belegen deren konkrete Testfälle, nicht automatisch Grafik, Musik, Bedienung oder die mündliche Prüfung am Schul-PC.

## 5. Kurze, vorhersagbare Demo

Im vorab getesteten, separaten Demo-Verzeichnis:

```text
1      Neues Spiel
2      Zwei Spieler
Ada    Name Spieler 1
Ben    Name Spieler 2
V 4    Vorschau Spalte 4
4      Spielstein setzen
U      Letzten Zug zurücknehmen
S      Spielstand speichern
M      Zurück zum Hauptmenü
```

Für eine Gewinnvorführung die Datei `tests/fixtures/school-near-win-2p.txt` in einem **isolierten** Demo-Datenordner als `data/savegame.txt` verwenden. Den tatsächlich geladenen Stand und seinen nächsten Zug vorher prüfen. Persönliche Spielstände niemals dafür ersetzen.

## 6. Technik und Fehlersuche

In einem bereits eingerichteten C++17/MSYS2-UCRT64-Terminal im Repository kann die reine Konsole manuell gebaut werden:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

Für reproduzierbare Qualitätsprüfungen gilt entsprechend dem CMake-Projekt:

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Vortragsgerät und Projektverzeichnis vorher prüfen. Bei fehleranfälligen Windows-Pfaden mit `ß` ist ein kurzer Pfad wie `C:\Dev\GymEdition` dokumentiert. Eine GUI mit Abhängigkeiten nicht erstmalig vor Publikum herunterladen und bauen. Zuverlässige Konsole, Präsentationsdatei und Code lokal bereithalten.

## 7. Nachweis und Üben

Die [Schulanforderungen](../docs/SCHULANFORDERUNGEN_51_STATUS.md) priorisieren das Programm (60 %), die Präsentation (20 %) und das Handout (20 %). Die Projektunterlagen dokumentieren einen erfolgreichen automatisierten C++-Workflow vom 31.10.2026: [GitHub Actions, neun erfolgreiche Jobs](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/41221586099). Diese Aussage bezieht sich auf den genannten Lauf, nicht auf einen späteren Commit.

Am Ende jeder Übung in der [Checkliste](PRUEFUNG_CHECKLISTE.md) notieren: Datum, Commit, Redezeiten, manuell tatsächlich geprüfte Funktionen und offene Probleme. Im [Fachgespräch](FACHGESPRAECH_KURZANTWORTEN.md) anschließend mindestens die Fragen zu `Board`, Schleifen, Referenzen, `const`, Richtungsvektoren, Dateiprüfung und Testgrenzen frei beantworten.

**Freigabestatus:** Die Unterlagen sind vorbereitet, aber eine vollständige Vor-Ort-Generalprobe oder Lehrkraftfreigabe ist noch nicht belegt.
