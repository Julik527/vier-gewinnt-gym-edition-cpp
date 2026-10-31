# Änderungsprotokoll

## 31.10.2026: Lokaler 2D-Start und UCRT64-GUI-Nachweis

- Windows-2D-Schnellstart für den tatsächlichen lokalen Fehlerfall: `main` statt GUI-Branch, fehlende Quellcode-Datei, CMake/Ninja und GDB-Sonderzeichenpfad.
- Read-only Diagnoseskript `scripts/check-2d.ps1` mit Abhängigkeits-, Branch- und Build-Pfadkontrolle.
- `scripts/demo.ps1` gibt bei fehlendem 2D-Quellcode und fehlenden MSYS2-Werkzeugen aussagekräftige Hinweise.
- GitHub Actions erhält einen eigenständigen SFML-3.1-GUI-Build mit MSYS2 UCRT64 / GCC. Dieser Job ist von einem erfolgreichen Windows-MSVC-GUI-Build zu unterscheiden.

## 31.10.2026: Datensicherheit und verifizierbare Windows-Auslieferung

- Gemeinsamer strikter `ScoreFile.h`-Parser für Highscores in Konsole und GUI. Beschädigte, doppelte oder abgeschnittene Records werden vor Zustandsänderung abgewiesen.
- `AtomicFile.h`: eindeutige temporäre Dateinamen statt eines festen `.tmp`-Namens. Keine Freigabe für konkurrierende, unkoordinierte Prozesse.
- Zusätzliche Unit- und Integrationstests für Datenintegrität und Temp-Kollisionen; überholte Fehlererwartungen an die neue Semantik angepasst.
- GitHub Actions ergänzt um MSYS2 UCRT64 Debug und Release sowie eine Windows-ZIP-Paketierung mit Self-Test und SHA-256.
- README verschlankt, reale Projektgrenzen hervorgehoben; [Abnahmecheckliste](docs/RELEASE_CHECKLIST.md) und [Lizenzhinweise](docs/LICENSING.md) ergänzt.
- Manuelle GUI-, Audio-, Zweitgerät- und Präsentationsabnahme weiterhin offen. Nur tatsächlich grüne CI-Jobs zählen als nachgewiesen.


## 28.10.2026: Entwicklungsstand Gym Edition 2.0

### Gestaltung und Projektdokumentation

- JK-AXIS-Präzisionssymbol in README und Projektunterlagen integriert. Historische Logo-Migration: `ea8bbef8`.
- Kommentarrichtlinien mit Dateiköpfen und verständlicher Schülersprache ergänzt.
- Heutigen Entwicklungsverlauf, Fehlerberichte, KI-Prompts, tatsächliche CI-Ergebnisse und offene Zielgeräteprüfung dokumentiert.
- Präsentation auf Konsole plus echte 2D-GUI aktualisiert. Alte Planungsunterlagen in den PDF-Nachträgen eindeutig als historisch eingeordnet.

### Bedienung und Stabilität

- Geführter Start, Fortsetzen, Ergebnisbildschirm und Revanche.
- Trainings-Arena, dauerhafte Vorschau, lesbare lange Namen und sichtbare Gewinnlinie.
- Sicherer Dateiersatz über `.tmp`; vorhandene Dateien bleiben bei den geprüften Fehlerfällen erhalten (`30ea62f`, danach gemeinsamer Helfer `34765ca`).
- Echte Fallbilder statt Punkt-Fortschritt (`4558ba3`, `3a13d28`).
- Beschädigten Markdown-Testplan repariert und Strukturprüfung ergänzt (`681b663`, `bb96456`).

### Zusätzliche 2D-Version

- Optionales SFML-3.1.0-Ziel mit Maus/Tastatur und testbarer MatchSession (`17d5aee`, `fa594f3`).
- Synthetischer 120-BPM-Beat, F9-Schalter, gut hörbarer Melodiebereich und Audio-Gerätediagnose (`939e5f2`, `ff83551`, `274d98a`). Hörtest bleibt offen.
- Schullogo mit Quellenhinweis, alternativer Bezugsquelle bei HTTP 429 und lokaler Installation. Skalierte JPEG-Kopie eingecheckt (`b7c9f44`, `7c50fef`, `702bdf7`).
- Beide Vorführwege nacheinander über `scripts/demo.ps1` (`6e42b08`).
- UCRT64-Suchpfad in beide GUI-Build-Aufgaben aufgenommen (`ab83df11`). Lokaler Wiederholungsversuch noch offen.

### Verifikation und Übergabe

Sechs C++-/GUI-Jobs und Dokumentenjob am Ausgangscommit `02eb79f8` erfolgreich. Beleg: [CI-Snapshot](docs/evidence/CI_SNAPSHOT_2026-10-28.json). Keine automatische Übernahme des [Draft-PR #1](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/1) nach `main`. Windows-F5, Hörtest, vollständige manuelle Spielrunde und PowerPoint-/Beamerprobe sind weiterhin offen.

## 25.10.2026: Abgabegrundlage

Anforderungsanalyse, ursprüngliches Klassenmodell, Backlog, KI-Protokoll, Präsentation und historische Linux-Tests. Die acht PDF-/PPTX-Dateien wurden im Upload-Commit `53612455` veröffentlicht. Die ursprünglichen Nachweise bleiben als Historie erhalten.
