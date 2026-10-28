<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/brand/jk-axis-precision-symbol-inverse.svg">
    <source media="(prefers-color-scheme: light)" srcset="assets/brand/jk-axis-precision-symbol.svg">
    <img src="assets/brand/jk-axis-precision-symbol.svg" alt="JK AXIS Präzisionslogo" width="96" height="100">
  </picture>
</p>

<h1 align="center">Vier Gewinnt | Gym Edition</h1>

<p align="center"><strong>C++17 · Objektorientierte Programmierung · Schulprojekt</strong></p>

<p align="center">
  <a href="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/cpp-quality.yml"><img alt="C++ CI: Linux und Windows" src="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/cpp-quality.yml/badge.svg"></a>
  <a href="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/docs-quality.yml"><img alt="Dokumentenprüfung" src="https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/workflows/docs-quality.yml/badge.svg"></a>
</p>

**C++17-Schulprojekt (ITA / Programmiertechnik, 2026/27)**  
**GitHub:** https://github.com/Julik527/vier-gewinnt-gym-edition-cpp · öffentlich.  
**Status: Entwicklungsstand vom 28.10.2026.** Das Repository stellt **keine bereits benotete oder vollständig abgenommene Endfassung** dar.

## Projektziel
Objektorientiertes Vier-Gewinnt-Konsolenspiel für **2 bis 4 Personen** mit variablen Spielfeldgrößen (7×6, 9×8, 11×10), Spielerverwaltung, vier Gewinnrichtungen, Remis, Eingabekontrolle und lokalem Speichern/Laden sowie Highscore. Die ASCII-Kurzhanteln sind die Gym-Edition-Darstellung. Eine fotorealistische oder klickbare 3D-GUI ist **nicht implementiert**; entsprechende Bilder sind Mockups.

## Schnellstart (Windows mit MSYS2 UCRT64 / g++)

Im Projektstammordner ein Terminal öffnen:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

Auf Linux/macOS entsprechend ohne `.exe` kompilieren. Die Dateipfade `data/` sind relativ zum Projektordner.

## Reproduzierbarer CMake-Build

Die kompakte `g++`-Variante oben bleibt für den Unterricht nutzbar. Für Debug, Release und automatische Tests:

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Auf Linux mit GCC/Clang kannst du beim Konfigurieren zusätzlich `-DCMAKE_BUILD_TYPE=Release` angeben. CTest führt die Board-Unit-Tests und, wenn Python 3 vorhanden ist, die Integrationstests aus. In GitHub Actions sind Linux/Windows und Debug/Release als Prüfmatrix eingerichtet.

## Spielmenü und Spielstart

Das Hauptmenü ist bewusst kompakt. **Neues Spiel** führt in einem Ablauf durch die Vorbereitung: zuerst **2–4 Spieler wählen**, dann **alle Namen nacheinander eingeben** und unmittelbar danach **die Runde starten**. Die früheren separaten Menüpunkte für Spielerzahl und Namen sind entfallen.

| Taste | Hauptmenü-Funktion |
|:--|:--|
| `1` | Neues Spiel einschließlich Spieler-Einrichtung |
| `2` | Aktuelle Runde fortsetzen, falls vorhanden |
| `3` | Spielstand aus `data/savegame.txt` laden |
| `4` | Highscore anzeigen |
| `5` | Anleitung und Steuerung |
| `0` | Programm beenden |

**Einrichtung:** Bei der Spielerzahl und Namenseingabe führt `0` zurück zum Hauptmenü. Ungültige Eingaben werden erneut abgefragt. Bereits laufende Partien werden nicht ohne Bestätigung verworfen; ein Abbruch der Einrichtung lässt sie bestehen.

**Während des Spiels:** Eine Spaltennummer legt eine Hantel ab. `V 4` zeigt eine Vorschau, `U` nimmt den letzten Zug zurück, `S` speichert, `H` öffnet die Anleitung, `I` zeigt Spielinformationen, `A` schaltet die Animation um und `M` kehrt zum Hauptmenü zurück. Danach kann die Runde mit **2** fortgesetzt werden. Das Hauptmenü zeigt bei einer aktiven Partie die Spielernamen, den nächsten Spieler und die Anzahl gelegter Hanteln.

**Nach einem Sieg oder Remis** erscheint ein eigener Ergebnisbildschirm. Dort gibt es **1 Revanche** (dieselben Spieler, neues leeres Feld), **2 Neues Spiel** (mit neuer Spieler-Einrichtung) oder **0 Hauptmenü**. Bei der Revanche bleiben Namen und Symbole erhalten. Der Ergebnisbildschirm zeigt zusätzlich die bisherigen Gesamtsiege aller beteiligten Spieler. Der Highscore steigt nur einmal pro tatsächlich gewonnener Partie.

**Ruhige Terminaldarstellung:** Das Spielfeld wird nicht bei jeder Hilfe- oder Fehleingabe komplett neu ausgegeben, sondern vor allem nach echten Spielzügen, Undo oder einer Vorschau. Die optionale Animation ist standardmäßig ausgeschaltet und verwendet keine ANSI-Bildschirmlöschung. Mit `A` lässt sie sich im Spiel aktivieren.

Die komplette Menüführung, Beispiele und Abnahmeszenarien stehen in [`docs/MENU_DESIGN.md`](docs/MENU_DESIGN.md).

## Kommentare und Dateiköpfe

Die C++-Quelldateien und Header besitzen jetzt einheitliche **JK AXIS | GOLD MASTER**-Dateiköpfe. Die Kommentare sind bewusst einfach geschrieben: Sie erklären vor allem Spielregeln, wichtige Entscheidungen und schwierigere Stellen, ohne jede Codezeile zu wiederholen. Auch die Test- und Hilfsskripte sind beschrieben.

Der [Code- und Kommentarleitfaden](docs/CODE_STYLE_JK_AXIS.md) zeigt den Aufbau und erklärt die Aufgaben jeder Datei anhand eines Beispiels. Die Ergänzungen vom 28.10.2026 betreffen Kommentare und Dateibeschreibungen; die Programmlogik wurde dabei nicht verändert.

## Architektur

- `Game` (`include/Game.h`, `src/Game.cpp`) steuert Menü, Spielerwechsel und Persistenz.
- `Board` (`include/Board.h`, `src/Board.cpp`) hält das Raster und prüft Spielzüge/Gewinn.
- `Player` (`include/Player.h`) speichert Name und Symbol.
- `Move` (`include/Game.h`) speichert Zugdaten der Undo-Historie.
- `src/main.cpp` erzeugt das `Game`-Objekt und ruft `run()` auf.

## Tests und Qualitätsnachweise

```bash
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Die Tests umfassen Spielfeldgrößen, Schwerkraft, Überlauf und Gewinnrichtungen sowie über `tests/integration_smoke.py` den durchgängigen Spielstart, Abbruch/Fortsetzen, Namen, Vorschau, Undo, Laden/Speichern, beschädigte Dateien, Gewinnen, Revanche mit unveränderten Namen, Ergebnisnavigation, ruhige Darstellung und dauerhafte Highscores.

**Nachweisstand:** Die Board-Tests und der Konsolen-Smoke-Test vom 25.10.2026 wurden historisch auf Linux ausgeführt. Der erweiterte Testkatalog und die GitHub-Actions-Matrix wurden am 28.10.2026 ergänzt. Die erfolgreichen CI-Läufe und der Windows-Schulrechner müssen anhand ihrer tatsächlichen Ergebnisse bestätigt werden. Keine Schul- oder Produktfreigabe wird daraus abgeleitet. Siehe [Qualität und Tests](docs/QUALITY_AND_TESTS.md).

## Design und Dateien

Die Projektpräsentation, Anforderungsanalyse, UML-Dokumentation, Meilensteinbericht, KI-Prompt-Dokumentation, Gesamtdokumentation und der Vortragsleitfaden wurden am 28.10.2026 technisch auf **JK AXIS | GOLD MASTER** aktualisiert. In der PowerPoint wurden 17 Legacy-Logos und 17 ältere Designkennzeichnungen ersetzt. Weitere acht identifizierte Logobilder wurden in den PDFs ausgetauscht; die Präsentations-PDF wurde aus der neuen PowerPoint erzeugt.

Die technische Migration ist durch den [Branding- und QA-Bericht](docs/BRAND_QA_REPORT.md) nachgewiesen. Die [technische Dokumentprüfung](scripts/document_preflight.py) prüft PDF-Seiten, PowerPoint-Struktur, SVG-Grafiken und lokale README-Links. Für eine endgültige Freigabe bleibt die visuelle Einzelkontrolle und ein PowerPoint-Test am Zielgerät notwendig.

## Dokumentation und Sprintstart

- [`docs/PROJECT_CHARTER.md`](docs/PROJECT_CHARTER.md) – Projektauftrag, Ziele, Grenzen
- [`docs/PRODUCT_BACKLOG.md`](docs/PRODUCT_BACKLOG.md) – priorisierte Anforderungen und Abnahmebedingungen
- [`docs/MILESTONE_2026-10-29.md`](docs/MILESTONE_2026-10-29.md) – Meilensteinstatus
- [`docs/MENU_DESIGN.md`](docs/MENU_DESIGN.md) – Hauptmenü, neuer Spielstart und Ablaufdiagramm
- [`docs/CODE_STYLE_JK_AXIS.md`](docs/CODE_STYLE_JK_AXIS.md) – Dateikoepfe, Kommentare und Erklaerungen auf Schuelerniveau
- [`docs/QUALITY_AND_TESTS.md`](docs/QUALITY_AND_TESTS.md) – Testnachweise/noch offene Tests
- [`docs/REPOSITORY_GUIDE.md`](docs/REPOSITORY_GUIDE.md) – GitHub-Veröffentlichung und Weiterarbeit
- [`docs/KI_PROMPT_DOKUMENTATION.md`](docs/KI_PROMPT_DOKUMENTATION.md) – Promptregister (FR-16)
- [`docs/ASSET_UPLOAD_CHECKLIST.md`](docs/ASSET_UPLOAD_CHECKLIST.md) – bestätigter Upload der PDF- und Präsentationsdateien
- [`reference/`](reference/) – Anforderungsanalyse, UML, Meilensteinbericht, KI-Protokoll und Gesamtdokumentation als PDF
- [`praesentation/`](praesentation/) – Präsentation (PPTX und PDF) sowie Vortragsleitfaden

> **GitHub-Stand (25.10.2026):** C++-Quellcode, Tests und Markdown-Dokumentation sind öffentlich hochgeladen. Die acht PDF-/PPTX-Originaldateien wurden in Commit [`53612455`](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/commit/53612455ee21bf4363b7d5627fe8cd388a8c7e8b) unter `reference/` und `praesentation/` hochgeladen. Die alte lokale ZIP-Historie unterscheidet sich von den über die GitHub-Integration erstellten Commits. Für die weitere Arbeit das Online-Repository neu klonen.

**© 2026 Julian Krauß · JK AXIS | GOLD MASTER.** Aktualisiert am 28.10.2026.