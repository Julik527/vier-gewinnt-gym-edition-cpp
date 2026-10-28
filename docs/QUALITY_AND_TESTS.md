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
