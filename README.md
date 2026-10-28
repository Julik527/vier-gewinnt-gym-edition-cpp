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
**Status: Entwicklungsstand vom 28.10.2026.** Konsole plus optionale SFML-2D-GUI. Der aktuelle Zusatzumfang liegt im [Draft-Pull-Request #1](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/1) auf `quality/gameplay-persistence-2026-10-28` und ist noch nicht in `main` übernommen. Die schulische Endabnahme bleibt offen.

## Projektziel
Objektorientiertes Vier-Gewinnt-Spiel für **2 bis 4 Personen** mit variablen Spielfeldgrößen (7×6, 9×8, 11×10), vier Gewinnrichtungen, Remis, Eingabekontrolle, Speichern/Laden und Highscore. Es gibt zwei getrennte Programme: die C++17-Konsole mit Hantelsymbolen und die zusätzliche SFML-2D-Anwendung mit Maus/Tastatur, fallenden Hanteln, schaltbarem Gym-Beat und Schullogo. Die älteren fotorealistischen 3D-Bilder bleiben Design-Mockups. [Architektur](docs/ARCHITECTURE.md) und [heutiges Entwicklungsprotokoll](docs/DEVELOPMENT_LOG_2026-10-28.md) erklären den tatsächlichen Stand.

## Schnellstart (Windows mit MSYS2 UCRT64 / g++)

Im Projektstammordner ein Terminal öffnen:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o vier-gewinnt.exe
.\vier-gewinnt.exe
```

Auf Linux/macOS entsprechend ohne `.exe` kompilieren. Die Dateipfade `data/` sind relativ zum Projektordner.

## VS Code: Mit F5 starten und debuggen (Windows / MSYS2 UCRT64)

Öffne in VS Code den **Projektstammordner**, also den Ordner, in dem `CMakeLists.txt`, `src/`, `include/` und `.vscode/` liegen. Drücke danach **F5** und wähle **„Vier Gewinnt | Gym Edition“**.

Die mitgelieferten Einstellungen benutzen:

- `.vscode/tasks.json`: Kompiliert **`src/main.cpp`, `src/Board.cpp`, `src/Game.cpp` und `src/ConsoleUI.cpp`** mit `g++.exe`, Debugsymbolen (`-g`) und ohne Optimierung (`-O0`).
- `.vscode/launch.json`: Startet `vier-gewinnt.exe` nach der Build-Aufgabe im integrierten Terminal und verwendet `gdb.exe` aus MSYS2 UCRT64.

**Wichtig nach dem Arena-Umbau:** Ältere lokale Kopien der Build-Aufgabe kennen `ConsoleUI.cpp` noch nicht. Bei Linkerfehlern zu `console_ui::arena`, `console_ui::present` oder `console_ui::result` zuerst die aktuelle `.vscode/tasks.json` aus GitHub übernehmen. Außerdem kontrollieren, dass `C:/msys64/ucrt64/bin/g++.exe` und `C:/msys64/ucrt64/bin/gdb.exe` wirklich installiert sind.

Falls **F5** nicht startet: Über **Terminal → Buildaufgabe ausführen** zunächst die Aufgabe **„Vier Gewinnt bauen“** starten und die erste rote Fehlermeldung im Terminal ansehen. Ohne einen erfolgreichen Build lässt sich der Debugger nicht korrekt starten.

## Schulvorführung: Logo, Musik und zwei Versionen

**Schullogo:** Das Schulkennzeichen der **Bernd-Blindow-Schulen Ravensburg** liegt bereits als dokumentierte skalierte JPEG-Kopie unter `assets/school/bernd-blindow-schulen.jpg` im Entwicklungsbranch. Die GUI lädt es aus dem Projekt- oder EXE-Umfeld, CMake kopiert es neben die EXE. Falls es lokal fehlt, unterstützt `scripts/install-school-logo.ps1` eine alternative Bildquelle, Wikimedia als weiteren Versuch und `-LocalFile`. Damit wird die gemeldete HTTP-429-Sperre berücksichtigt. Die Nutzung und das CC-BY-SA-4.0-Attribut sind in [docs/SCHOOL_LOGO.md](docs/SCHOOL_LOGO.md) dokumentiert. Bei fehlender Grafik wird der Schulname als Text angezeigt, nicht als erfundenes Logo. In der PowerShell-Version steht der Schulname im Menü und über der Arena.

**Musik:** Die 2D-Version enthält einen selbst programmierten instrumentalen **120-BPM-Gym-Beat** ohne fremde Aufnahme. Standardmäßig ist sie **aus**: Oben rechts `MUSIK AN (F9)` anklicken oder **F9** drücken. Die Lautstärke des Spielsignals beträgt jetzt 65 %; zusätzlich wirkt der Windows-Lautstärkemixer. Die Anzeige meldet den Abspielstatus oder ein fehlendes Audiogerät. Überprüfe bei Stille die Windows-Ausgabe (Lautsprecher/Kopfhörer, App-Lautstärkemixer). Mit `vier-gewinnt-gui.exe --audio-devices` werden erkannte Geräte im Terminal ausgegeben.

**Beide Versionen nacheinander demonstrieren:** Starte im Projektstamm:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\demo.ps1 -Mode Both
```

Zuerst läuft das grafische Fenster. Wenn du es schließt, beginnt der Terminaldurchlauf in PowerShell. Alternativ funktionieren `-Mode GUI` und `-Mode Terminal`. Das Skript baut die Programme mit CMake/Ninja beziehungsweise MSYS2 g++; mit `-NoBuild` verwendet es bereits vorhandene EXE-Dateien. Die zwei Durchläufe erfolgen nacheinander, um konkurrierende Schreibvorgänge an `data/savegame.txt` und `data/highscore.txt` zu vermeiden.

**Bekannte kommerzielle Songs** sind nicht in GitHub eingecheckt. Für eine separate Präsentationsplaylist gelten die Lizenz- und Aufführungsregeln des jeweiligen Musikdienstes.

## Grafische Gym Edition (SFML 3.1)

Neben der bisherigen Terminal-Anwendung gibt es jetzt ein **eigenes grafisches Ziel** `vier-gewinnt-gui` mit Mausbedienung, über mehrere Frames fallenden Hanteln, Spielerübersicht, Bestenliste, Anleitung, Revanche und den Spielständen im bisherigen `GYM1`-Format. Die gemeinsame `Board`-Klasse übernimmt Gewinnregeln und Schwerkraft. `MatchSession` kapselt Spielfluss und Dateioperationen ohne SFML-Abhängigkeit.

**Abhängigkeit:** SFML **3.1.0**, im CMake-Projekt festgelegt. Benötigt CMake **3.28 oder neuer**; bei aktivierter Grafik wird SFML entweder aus einer passenden Installation verwendet oder reproduzierbar über `FetchContent` bezogen. Die Konsole benötigt SFML ausdrücklich nicht. Offizielle Projektquelle: https://github.com/SFML/SFML

```bash
cmake -S . -B build-gui -DGYM_BUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-gui --config Release --target vier-gewinnt-gui
ctest --test-dir build-gui -C Release --output-on-failure -R 'match_session_unit_tests|gui_core_smoke|gui_window_smoke'
```

Unter Windows entsteht die EXE je nach Generator unter `build-gui/Release/vier-gewinnt-gui.exe` (Visual Studio) oder `build-gui/vier-gewinnt-gui.exe` (Ninja/MinGW). Für MSYS2 UCRT64 zuerst den UCRT64-Compiler und CMake/Ninja verfügbar machen; die GUI hat eine eigene Abhängigkeitskette und wird nicht von der alten `g++`-Task kompiliert.

**VS Code F5:** In `.vscode/launch.json` gibt es zwei Profile: `Vier Gewinnt | Gym Edition` (Konsole) und `Vier Gewinnt | Gym Edition 2D` (Grafik). Das neue GUI-Profil ruft über `.vscode/tasks.json` die Ninja/CMake-Konfiguration auf. Benötigt CMake 3.28+, Ninja sowie die MSYS2-UCRT64-Compiler in `C:/msys64/ucrt64/bin`.

**Falls PowerShell „cmake wird nicht erkannt“ meldet:** Öffne über das Windows-Startmenü **MSYS2 UCRT64**. Aktualisiere MSYS2 mit `pacman -Syu`. Falls dabei das Terminal geschlossen werden muss, öffne MSYS2 UCRT64 erneut und wiederhole `pacman -Syu`. Installiere anschließend dort die passenden Werkzeuge:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb
```

Die beiden GUI-Build-Aufgaben ergänzen `C:/msys64/ucrt64/bin` selbst für ihren Prozess. So finden bereits Konfiguration und Build CMake, Ninja und die Compiler-DLLs; die `PATH`-Angabe in `launch.json` allein würde erst beim späteren Spielstart wirken. Der übrige Suchpfad bleibt erhalten. Nach dem Aktualisieren der Projektdateien den fehlgeschlagenen Task beenden und **F5 → Vier Gewinnt | Gym Edition 2D** erneut starten. Es ist keine systemweite `PATH`-Änderung nötig.

Für manuelle Build-Befehle in einem PowerShell-Terminal gilt dessen eigener Suchpfad. Ergänze ihn bei Bedarf für die aktuelle Sitzung mit `$env:Path = "C:\msys64\ucrt64\bin;$env:Path"` und prüfe mit `cmake --version` sowie `ninja --version`, ob beide Programme gefunden werden. Diese Terminal-Einstellung wird nicht automatisch an separat gestartete VS-Code-Tasks weitergegeben.

**Bedienung:** Hauptmenü anklicken, Spielerzahl 2–4 wählen, Namen nacheinander eingeben und starten. Während des Spiels eine Spalte anklicken oder mit **Links/Rechts + Leertaste** auswählen. `U` nimmt zurück, `S` speichert, `A` schaltet die Fallanimation um, `Esc` wechselt ins Menü. Nach Sieg/Remis stehen Revanche, neue Spieler und Hauptmenü bereit.

**Systemschrift:** Für Text benutzt die GUI vorhandene Schriftdateien (Segoe UI/Arial unter Windows, DejaVu/Liberation unter Linux). Schriftdateien werden nicht im Repository kopiert.

**Automatisierte Prüfung:** Vor dem obigen CTest-Aufruf auch `cmake --build build-gui --config Release --target match_session_tests` ausführen. Ohne eingeschränkten `-R`-Filter müssen zuerst alle Testziele gebaut werden. `match_session_tests` kontrolliert Spielzustand, Sicherung, Laden, Gewinn, Revanche und bestehende Highscores ohne grafischen Bildschirm. `vier-gewinnt-gui --self-test` prüft den minimalen GUI-Kern ohne Fenster. Ein tatsächlicher interaktiver Windows-/Linux-Sichttest mit Maus und Tastatur ist weiterhin gesondert zu protokollieren; siehe [GUI-Testplan](docs/GUI_TESTPLAN.md).

## Reproduzierbarer CMake-Build

Die kompakte `g++`-Variante oben bleibt für den Unterricht nutzbar. Für Debug, Release und automatische Tests:

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Auf Linux mit GCC/Clang kannst du beim Konfigurieren zusätzlich `-DCMAKE_BUILD_TYPE=Release` angeben. CTest führt die Board-Unit-Tests und, wenn Python 3 vorhanden ist, die Integrationstests und die neuen ConsoleUI-Tests aus. In GitHub Actions sind Linux/Windows und Debug/Release als Prüfmatrix eingerichtet.

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

**JK-AXIS-Trainings-Arena:** Neues Terminal-Layout mit Titel, Gym-Edition-Motto, klar nummerierten Spalten, durchgehenden Feldlinien, farblich hervorgehobenen Hantelsymbolen `[R]`, `[B]`, `[G]`, `[Y]`, Spielerübersicht, Gesamtsiegen, Zugstatus und festen Tastenhinweisen. Beim Sieg werden die zusammenhängenden Gewinnfelder mit `{R}` beziehungsweise dem passenden Symbol markiert. Der Ergebnisbildschirm zeigt die letzte Runde und die Revanche-Auswahl.

**Datensicheres Speichern:** Spielstand (`GYM1`) und Bestenliste werden zuerst in eine temporäre Datei geschrieben. Nur nach erfolgreichem Schreiben ersetzt das Programm die bisherige Datei. Bei einem Schreibfehler bleibt die vorherige Datei erhalten; die Spielstandsdatei behält ihr bisheriges Format. Die zusätzlichen Fehlerfall-Tests verwenden ausschließlich temporäre Testordner.

**Vorschau und Spielernamen:** Eine Vorschau bleibt bis zur nächsten Eingabe sichtbar. Mehrere lange Spielernamen stehen im Hauptmenü untereinander. Steuerzeichen im Spielernamen sind nicht zulässig; das Byte-Limit von 24 bleibt bestehen.

**Lesbare Infoseiten:** Anleitung, Bestenliste und wichtige Meldungen bleiben auf einem echten Terminal sichtbar, bis du Enter drückst. Bei automatisierten Tests mit umgeleiteter Eingabe erfolgt kein zusätzlicher Stopp.

**Ruhige Terminaldarstellung:** Das Spielfeld wird nur bei wichtigen Änderungen neu aufgebaut. In kompatiblen interaktiven Terminals aktualisiert es sich an derselben Bildschirmposition. Bei Umleitung, fehlender ANSI-Unterstützung oder `NO_COLOR` bleiben Rahmen, Buchstaben und Texte ohne Farbcodes lesbar. Die optionale Animation zeigt echte Positionsänderungen der Hantel innerhalb des Feldes über mehrere Frames; sie ist standardmäßig ausgeschaltet und kann mit `A` aktiviert werden. Diese Beschreibung gilt für die Terminalausgabe. Die zusätzliche SFML-Version besitzt ein eigenes Fenster und Maussteuerung.

Die neue Arena mit Spielerfarben, Hantelsymbolen, Gewinnmarkierung und Terminal-Kompatibilität ist im [JK-AXIS-Arena-Leitfaden](docs/ARENA_DESIGN.md) dokumentiert. Die komplette Menüführung, Beispiele und Abnahmeszenarien stehen in [`docs/MENU_DESIGN.md`](docs/MENU_DESIGN.md).

## Kommentare und Dateiköpfe

Die C++-Quelldateien und Header besitzen jetzt einheitliche **JK AXIS | GOLD MASTER**-Dateiköpfe. Die Kommentare sind bewusst einfach geschrieben: Sie erklären vor allem Spielregeln, wichtige Entscheidungen und schwierigere Stellen, ohne jede Codezeile zu wiederholen. Auch die Test- und Hilfsskripte sind beschrieben.

Der [Code- und Kommentarleitfaden](docs/CODE_STYLE_JK_AXIS.md) zeigt den Aufbau und erklärt die Aufgaben jeder Datei anhand eines Beispiels. Die Ergänzungen vom 28.10.2026 betreffen Kommentare und Dateibeschreibungen; die Programmlogik wurde dabei nicht verändert.

## Architektur

- `Game` (`include/Game.h`, `src/Game.cpp`) steuert Menü, Spielerwechsel und Persistenz.
- `ConsoleUI` (`include/ConsoleUI.h`, `src/ConsoleUI.cpp`) zeichnet das Gym-Edition-Layout für Hauptmenü, Arena und Ergebnis.
- `Board` (`include/Board.h`, `src/Board.cpp`) hält das Raster und prüft Spielzüge/Gewinn.
- `Player` (`include/Player.h`) speichert Name und Symbol.
- `Move` (`include/Game.h`) speichert Zugdaten der Undo-Historie.
- `src/main.cpp` erzeugt das `Game`-Objekt und ruft `run()` auf.
- `MatchSession` kapselt den Partiezustand der GUI ohne SFML. Die Konsole nutzt weiterhin `Game`.
- `App` in `src/gui/Main.cpp` verarbeitet Fensterereignisse und zeichnet die GUI.
- `GymBeat` in `src/gui/GymBeat.h` erzeugt den synthetischen Soundtrack.
- `gym::writeSafely` in `include/AtomicFile.h` ersetzt Dateien nach erfolgreichem Schreiben.

Der genaue Zusammenhang steht in [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

## Tests und Qualitätsnachweise

```bash
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Die Tests umfassen Spielfeldgrößen, Schwerkraft, Gewinnlinien samt Markierung, ConsoleUI-Darstellung für 2/4 Spieler sowie über `tests/integration_smoke.py` den durchgängigen Spielstart, Abbruch/Fortsetzen, Namen, Vorschau, Undo, Laden/Speichern, beschädigte Dateien, Gewinnen, Revanche mit unveränderten Namen, Ergebnisnavigation, ruhige Darstellung und dauerhafte Highscores.

**Nachweisstand:** Am Ausgangscommit `02eb79f8` sind [alle sechs C++-/GUI-Jobs](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/40779797878) sowie der [Dokumentenjob](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/40779798112) erfolgreich. Der [CI-Snapshot](docs/evidence/CI_SNAPSHOT_2026-10-28.json) enthält die abgefragten Jobstatus. Das belegt automatisierte Runner-Prüfungen. Der zuletzt gemeldete lokale CMake-/F5-Fehler, echte Audioausgabe, vollständige Bedienrunde und PowerPoint-/Beamerprobe müssen am Zielgerät erneut geprüft werden. Siehe [Qualität und Tests](docs/QUALITY_AND_TESTS.md).

## Design und Dateien

Die Projektpräsentation, Anforderungsanalyse, UML-Dokumentation, Meilensteinbericht, KI-Prompt-Dokumentation, Gesamtdokumentation und der Vortragsleitfaden wurden am 28.10.2026 technisch auf **JK AXIS | GOLD MASTER** aktualisiert. In der PowerPoint wurden 17 Legacy-Logos und 17 ältere Designkennzeichnungen ersetzt. Weitere acht identifizierte Logobilder wurden in den PDFs ausgetauscht; die Präsentations-PDF wurde aus der neuen PowerPoint erzeugt.

Die zusätzliche Dokumentationsrunde aktualisiert alle 17 Folien und ergänzt die fünf Referenz-PDFs um aktuelle Nachträge. Die historischen Originalseiten bleiben gekennzeichnet erhalten. Der Vortragsleitfaden ist neu auf drei Seiten gesetzt. Der aktuelle Bestand umfasst 183 PDF-Seiten. Promptregister, Architektur, Entwicklungsprotokoll und Dateiaudit sind aufeinander abgestimmt.

Die technische Migration ist durch den [Branding- und QA-Bericht](docs/BRAND_QA_REPORT.md) nachgewiesen. Die [technische Dokumentprüfung](scripts/document_preflight.py) prüft PDF-Seiten, PowerPoint-Struktur, SVG-Grafiken und lokale Verweise aller Markdown-Dateien. Für die endgültige Vorführung bleiben PowerPoint- und Beamerprüfung am Zielgerät notwendig.

## Dokumentation und Sprintstart

- [Entwicklungsprotokoll 28.10.2026](docs/DEVELOPMENT_LOG_2026-10-28.md): Markenintegration, Menü, Stabilisierung, 2D-GUI, Musik, Logo und CMake-Fehler.
- [Architektur](docs/ARCHITECTURE.md): gemeinsame Regeln und getrennte Frontends.
- [Dokumentenaudit](docs/DOCUMENTATION_AUDIT_2026-10-28.md): alle geprüften Dokumente, Korrekturen und Archivteile.
- [Vortragsleitfaden als Text](praesentation/VORTRAGSLEITFADEN.md): 15 Minuten Projekt plus 15 Minuten Code und beide Programme.
- [Änderungsprotokoll](CHANGELOG.md): die wesentlichen Entwicklungsschritte.

- [`docs/PROJECT_CHARTER.md`](docs/PROJECT_CHARTER.md) – Projektauftrag, Ziele, Grenzen
- [`docs/PRODUCT_BACKLOG.md`](docs/PRODUCT_BACKLOG.md) – priorisierte Anforderungen und Abnahmebedingungen
- [`docs/MILESTONE_2026-10-29.md`](docs/MILESTONE_2026-10-29.md) – Meilensteinstatus
- [`docs/MENU_DESIGN.md`](docs/MENU_DESIGN.md) – Hauptmenü, neuer Spielstart und Ablaufdiagramm
- [`docs/ARENA_DESIGN.md`](docs/ARENA_DESIGN.md) – Gym-Edition-Trainings-Arena, Farben, Spielsteine, Gewinnmarkierung und Terminalanforderungen
- [`docs/CODE_STYLE_JK_AXIS.md`](docs/CODE_STYLE_JK_AXIS.md) – Dateikoepfe, Kommentare und Erklaerungen auf Schuelerniveau
- [`docs/QUALITY_AND_TESTS.md`](docs/QUALITY_AND_TESTS.md) – Testnachweise/noch offene Tests
- [`docs/REPOSITORY_GUIDE.md`](docs/REPOSITORY_GUIDE.md) – GitHub-Veröffentlichung und Weiterarbeit
- [`docs/KI_PROMPT_DOKUMENTATION.md`](docs/KI_PROMPT_DOKUMENTATION.md) – Promptregister (FR-16)
- [`docs/ASSET_UPLOAD_CHECKLIST.md`](docs/ASSET_UPLOAD_CHECKLIST.md) – bestätigter Upload der PDF- und Präsentationsdateien
- [`reference/`](reference/) – Anforderungsanalyse, UML, Meilensteinbericht, KI-Protokoll und Gesamtdokumentation als PDF
- [`praesentation/`](praesentation/) – Präsentation (PPTX und PDF) sowie Vortragsleitfaden

> **GitHub-Stand (25.10.2026):** C++-Quellcode, Tests und Markdown-Dokumentation sind öffentlich hochgeladen. Die acht PDF-/PPTX-Originaldateien wurden in Commit [`53612455`](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/commit/53612455ee21bf4363b7d5627fe8cd388a8c7e8b) unter `reference/` und `praesentation/` hochgeladen. Die alte lokale ZIP-Historie unterscheidet sich von den über die GitHub-Integration erstellten Commits. Für die weitere Arbeit das Online-Repository neu klonen.

**© 2026 Julian Krauß · JK AXIS | GOLD MASTER.** Aktualisiert am 28.10.2026.
