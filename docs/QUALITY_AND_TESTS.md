# Qualitätssicherung und Testnachweise

**Stand:** Arbeitstag 28.10.2026 · **Quellcodebasis:** `02eb79f8c432ef4578cf650ca2b74efa4d448769`
**Branch:** `quality/gameplay-persistence-2026-10-28` · **Übergabe:** Draft-PR #1

## 1. Aktuell verifizierte GitHub-Ergebnisse

Die abgefragten Läufe am genannten Ausgangscommit sind erfolgreich abgeschlossen:

- [C++ Gold Standard QA, Lauf 40779797878](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/40779797878).
- [Document & Brand QA, Lauf 40779798112](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/40779798112).
- [Gesicherte Job- und Schrittstatus als JSON](evidence/CI_SNAPSHOT_2026-10-28.json).

| Job | Tatsächliches Ergebnis | Prüfumfang |
|:--|:--|:--|
| Ubuntu Debug | success | CMake-Build und registrierte Kern-/Konsolentests |
| Ubuntu Release | success | Gleiche Tests, Prüfbedingungen auch mit NDEBUG wirksam |
| Windows Debug | success | CMake-Build und CTest auf Windows-Runner |
| Windows Release | success | CMake-Build und CTest auf Windows-Runner |
| SFML 3.1 GUI / Ubuntu | success | Release-Build, MatchSession, GUI-Kern, kurzer Xvfb-Fensterstart und Logo-Prüfung |
| SFML 3.1 GUI / Windows | success | MSVC-Release-Build, MatchSession, GUI-Kern, Logo-Prüfung und Artefakt-Erstellung |
| Dokumente | success | Technischer PDF-/PPTX-/SVG-Preflight und 47 Markdown-Testfälle im damaligen Bestand |

Zeitstempel im JSON stehen in UTC. Die Läufe gehören zum Arbeitstag 28.10.2026 und endeten in Europe/Berlin nach Mitternacht am 29.10.2026. Das Protokoll dokumentiert einen festen Commit, keine unbegrenzt gültige Aussage über künftige Änderungen.

Die Windows-CI belegt keinen lokalen MSYS2-UCRT64-F5-Start. Die GUI-CI baut Release, keine eigene GUI-Debug-Matrix. Das virtuelle Linux-Fenster startet nur kurz und enthält keinen vollständigen manuellen Spielablauf oder Hörtest.

## 2. Was die Testdateien tatsächlich kontrollieren

| Datei | Abgedeckte Kontrollen | Grenze |
|:--|:--|:--|
| `tests/board_tests.cpp` | Größen, Grenzen, Schwerkraft, vier Gewinnachsen, Undo, volles Raster und Gewinnfelder | Kein interaktiver Bediennachweis |
| `tests/console_ui_tests.cpp` | Menü, Arena, 2/4 Spieler, Vorschau, Gewinnmarkierung und unterschiedliche Animationsbilder | Text- und Zustandsprüfung, kein Beamer-/Terminal-Sichttest |
| `tests/integration_smoke.py` | Komplette Konsolenabläufe mit Eingaben, Abbruch/Fortsetzen, Speichern/Laden, zwei Siege, Highscore-Neustart, lange Namen, Steuerzeichen und Schreibfehler | Umgeleitete Ein-/Ausgabe, kein echtes Tastaturgefühl |
| `tests/match_session_tests.cpp` | GUI-unabhängige Partie, ungültige Namen, 2/4 Spieler, Undo, GYM1, defekte Datei, Revanche und Score-/Schreibschutzfälle | Keine Mausereignisse und kein Rendering |
| `tests/vscode_config_test.py` | F5-Verknüpfungen und Quelldateien gegenüber CMake | Führt VS Code, GDB und Windows-PATH nicht aus |
| `tests/testplan_lint.py` | Überschrift, Tabellenstruktur, T01–T47, doppelte IDs und bekannte Beschädigungen | Prüft keinen tatsächlich durchgeführten manuellen Fall |
| `tests/verify_school_logo.py` | JPEG-Signatur, Größe, Abmessungen und bekannte Original-Prüfsumme | Alternative Kopie nur plausibilisiert, kein vollständiger Herkunftsbeweis |
| `scripts/document_preflight.py` | PDF-Seiten renderbar, PPTX-Struktur, Logos/SVG und lokale Dokumentverweise | Keine automatische fachliche Schulabnahme |

## 3. Relevante Fehler und Kontrollfälle

### Sicherer Dateiersatz

Früher öffnete das Spiel bestehende Dateien direkt mit `trunc`. Jetzt schreibt `gym::writeSafely` zuerst eine `.tmp`-Datei im Zielordner. Erst ein vollständig geschriebener Stream führt zum Ersatz über `MoveFileExW` oder `rename`. Die Tests blockieren unter anderem die Temp-Datei und kontrollieren den unveränderten Altbestand. Das Format bleibt GYM1.

Diese Tests belegen die konkret geprüften Schreibfehler. Sie sind keine Zusage gegen jeden Stromausfall und kein Schutz gegen zwei gleichzeitig gestartete Prozesse. Das Demo-Skript startet die Programme nacheinander.

### Menü, Darstellung und Animation

Neue Einrichtung und Abbruch erhalten bei Bedarf die bisherige Partie. Die Vorschau bleibt bis zur nächsten Eingabe sichtbar. Lange Namen stehen untereinander. Kontrollzeichen werden abgewiesen. Ergebnisbildschirm und Revanche erhöhen den Highscore genau einmal pro Sieg. Die Fallanimation verwendet wechselnde Board-Frames statt einer reinen Punktfolge. Die Konsole beginnt ohne Animation, die GUI mit Animation.

### Markdown und VS Code

Der manuelle Testplan enthielt beschädigte Tabellenfragmente und wörtliche Backslash-n-Sequenzen. Der ursprüngliche Preflight erfasste diese Fehler nicht. `testplan_lint.py` kontrolliert seitdem die 47 Fälle direkt.

Der ursprüngliche F5-Befehl ließ ConsoleUI.cpp aus. Ein späterer Fehler betraf die falsch maskierten regulären Ausdrücke im Konfigurationstest. Die aktuelle Fehlermeldung des Nutzers betrifft dagegen ein nicht gefundenes CMake. Beide GUI-Tasks ergänzen seit `ab83df11` den UCRT64-Pfad. Der statische Konfigurationstest kann die tatsächliche lokale Installation nicht bestätigen.

### Audio und Schullogo

Der Sound bindet erst nach dem Laden an seinen Buffer, verwendet 65 % Lautstärke und eine zusätzliche Melodie im mittleren Frequenzbereich. Musik ist zunächst aus. F9 oder die Schaltfläche aktiviert sie. `--audio-devices` nennt erkannte Geräte. Ein Playing-Status belegt keine hörbare Ausgabe.

Das Schullogo wird im Projekt-/EXE-Umfeld gesucht. Nach einer HTTP-429-Antwort von Wikimedia wurde eine alternative Quelle ergänzt. Ein gültiges JPEG liegt bereits im Entwicklungsbranch. Die CI prüft das Bild neben dem GUI-Binary. Der tatsächliche Eindruck am Windows-Bildschirm bleibt manuell zu kontrollieren.

## 4. Historischer Nachweis vom 25.10.2026

Die früheren Q-01 bis Q-04 belegen Linux-Build, Board-Testbau, Dimensionen/Gewinnachsen/Undo und einen einfachen Menü-Smoke-Test. Der Originalbeleg bleibt unter [BUILD_AND_TEST_LOG_2026-10-25.txt](evidence/BUILD_AND_TEST_LOG_2026-10-25.txt). Die damalige Menüfolge gehört zum alten Menü und ist kein aktuelles Demo-Skript.

Die früheren Kennungen Q-05 bis Q-15 bezogen sich auf die später ergänzten Board-, Integrations-, UI-, CI- und Dokumentenprüfungen. Ihre heutige Zuordnung steht in Abschnitt 2. Frühere Aussagen „Ausführung noch offen“ sind durch den konkreten CI-Nachweis in Abschnitt 1 für diese automatischen Prüfungen überholt. Die manuellen Testfelder bleiben unverändert offen.

## 5. Reproduzierbare Prüfung

Konsole und Kern, bei einem Generator mit einzelner Build-Konfiguration in getrennten Ordnern:

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug --config Debug
ctest --test-dir build-debug -C Debug --output-on-failure
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --config Release
ctest --test-dir build-release -C Release --output-on-failure
```

Für die zusätzliche GUI CMake 3.28+, einen passenden Compiler und die SFML-Buildvoraussetzungen bereitstellen. Unter Windows die UCRT64-Installation und den Suchpfad aus der README beachten. Nur die explizit gebauten GUI-Testziele ausführen:

```bash
cmake -S . -B build-gui -DGYM_BUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-gui --config Release --target vier-gewinnt-gui match_session_tests
ctest --test-dir build-gui -C Release --output-on-failure -R 'match_session_unit_tests|gui_core_smoke|gui_window_smoke'
```

Für die Dokumente:

```bash
python -m pip install PyMuPDF python-pptx
python tests/testplan_lint.py
python scripts/document_preflight.py
```

`python-pptx` dient hier ausschließlich dem vorhandenen lesenden Prüfskript. Die Präsentationsbearbeitung dieser Dokumentationsrunde erfolgt separat. Alle Spieltests verwenden temporäre Datenordner.

## 6. Dokumentationsrunde und offene Endabnahme

Das [Dokumentenaudit](DOCUMENTATION_AUDIT_2026-10-28.md) listet sämtliche geprüften Markdown-, PDF- und Präsentationsdateien. Aktuelle PDF-Nachträge stehen vor den ausdrücklich historischen Planungsseiten. Die Präsentation selbst beschreibt den aktuellen Stand.

Noch am echten Zielgerät durchzuführen:

- Beide F5-Profile nach dem gemeldeten CMake-Fehler erneut starten.
- GUI und Konsole jeweils bis zu Sieg beziehungsweise Remis, Revanche und Neustart testen.
- Spielstand zwischen den Frontends praktisch prüfen. Beide nutzen GYM1, der bisherige Unit-Test ersetzt keinen vollständigen wechselseitigen Bediennachweis.
- Schullogo sichtbar prüfen, F9 und Audioausgabegerät tatsächlich hören.
- PPTX in Microsoft PowerPoint sowie auf dem Schulbeamer öffnen.
- Board, Game, MatchSession, Audio und Dateiersatz in eigenen Worten erläutern.

Die Ist-Felder von [TESTPLAN.md](TESTPLAN.md) und [GUI_TESTPLAN.md](GUI_TESTPLAN.md) erst nach einem tatsächlich ausgeführten Versuch ausfüllen. Kein automatischer Merge nach main, keine vorweggenommene Note oder schulische Freigabe.
