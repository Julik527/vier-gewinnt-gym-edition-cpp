# Vier Gewinnt | Gym Edition

**C++17-Schulprojekt (ITA / Programmiertechnik, 2026/27)**  
**GitHub:** https://github.com/Julik527/vier-gewinnt-gym-edition-cpp · öffentlich.  
**Status: Lern- und Entwicklungsstand vom 25.10.2026.** Das Repository stellt **keine bereits benotete oder vollständig abgenommene Endfassung** dar.

## Projektziel
Objektorientiertes Vier-Gewinnt-Konsolenspiel für **2 bis 4 Personen** mit variablen Spielfeldgrößen (7×6, 9×8, 11×10), Spielerverwaltung, vier Gewinnrichtungen, Remis, Eingabekontrolle und lokalem Speichern/Laden sowie Highscore. Die ASCII-Kurzhanteln sind die Gym-Edition-Darstellung. Eine fotorealistische oder klickbare 3D-GUI ist **nicht implementiert**; entsprechende Bilder sind Mockups.

## Schnellstart (Windows mit MSYS2 UCRT64 / g++)

Im Projektstammordner ein Terminal öffnen:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

Auf Linux/macOS entsprechend ohne `.exe` kompilieren. Die Dateipfade `data/` sind relativ zum Projektordner.

## Spielablauf / Menü

| Taste | Funktion |
|:--|:--|
| `1` | Neues Spiel |
| `2` | Spieleranzahl (2-4) |
| `3` | Namen eingeben |
| `4` | Spiel speichern |
| `5` | Spiel laden |
| `6` | Highscore anzeigen |
| `0` | Programm beenden |

Im Spiel: Spaltennummer eingeben; `V 4` für Vorschau, `U` Undo, `S` Speichern, `H` Hilfe, `I` Info, `A` Animation an/aus, `M` Menü.

## Architektur

- `Game` (`include/Game.h`, `src/Game.cpp`) steuert Menü, Spielerwechsel und Persistenz.
- `Board` (`include/Board.h`, `src/Board.cpp`) hält das Raster und prüft Spielzüge/Gewinn.
- `Player` (`include/Player.h`) speichert Name und Symbol.
- `Move` (`include/Game.h`) speichert Zugdaten der Undo-Historie.
- `src/main.cpp` erzeugt das `Game`-Objekt und ruft `run()` auf.

## Tests

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/board_tests.cpp src/Board.cpp -o board_tests
./board_tests
```

Am 25.10.2026 wurden Kompilierung, vorhandene Board-Assertions und ein Konsolenmenü-Smoke-Test erfolgreich ausgeführt (GCC 14.2.0 / Linux). **Offen:** vollständige interaktive Abnahmetests, Save/Load-Fehlerfälle, Windows-Reproduktion und Lehrerfreigabe. Siehe [`docs/QUALITY_AND_TESTS.md`](docs/QUALITY_AND_TESTS.md).

## Dokumentation und Sprintstart

- [`docs/PROJECT_CHARTER.md`](docs/PROJECT_CHARTER.md) – Projektauftrag, Ziele, Grenzen
- [`docs/PRODUCT_BACKLOG.md`](docs/PRODUCT_BACKLOG.md) – priorisierte Anforderungen und Abnahmebedingungen
- [`docs/MILESTONE_2026-10-29.md`](docs/MILESTONE_2026-10-29.md) – Meilensteinstatus
- [`docs/QUALITY_AND_TESTS.md`](docs/QUALITY_AND_TESTS.md) – Testnachweise/noch offene Tests
- [`docs/REPOSITORY_GUIDE.md`](docs/REPOSITORY_GUIDE.md) – GitHub-Veröffentlichung
- [`docs/reference/`](docs/reference/) – Anforderungsanalyse und UML-Klassendiagramm

**© 2026 Julian Krauß · Design-ID JK-CD-DE-2026-003.** Dokumentationsstand 25.10.2026.