# Lehrer-Abnahmetest | Vier Gewinnt – Gym Edition

**Stand:** 05.11.2026 · **Fach:** Programmiertechnik · **Sprache:** C++17  
**Status:** Reproduzierbare technische Prüfungen und noch offene manuelle Abnahme.

> Der Bericht ist ein mit KI-Unterstützung vorbereitetes Testprotokoll.
> Er ersetzt weder die eigene Prüfung noch eine Vorführung auf dem Schul-PC.
> Technische Aussagen sind über GitHub Actions und CTest nachzuweisen.

## 1. Automatische Prüfung und ihre Quellen

| ID | Testbereich | Nachweis |
|---|---|---|
| A-01 | Spielfeldgrößen für 2/3/4 Spieler und Grenzen | tests/board_tests.cpp |
| A-02 | Schwerkraft, ungültige/volle Spalten, Undo | tests/board_tests.cpp |
| A-03 | Horizontaler, vertikaler und diagonaler Sieg | tests/board_tests.cpp |
| A-04 | 360 vollständig gefüllte Zufalls-Spielfelder mit Referenzmodell | tests/board_property_tests.cpp |
| A-05 | Unabhängiger Vergleich der Siegerkennung und Gewinnmarkierung | tests/board_property_tests.cpp |
| A-06 | Menüs, Vorschau, Revanche, Spielerwechsel, Remis | tests/integration_smoke.py |
| A-07 | Große Zahlen, leere Eingaben, doppelter/zu langer Name | tests/integration_adversarial.py |
| A-08 | Volle Spalte darf keinen Zug hinzufügen | tests/integration_adversarial.py |
| A-09 | 13 manipulierte Speicherdateien werden abgewiesen | tests/integration_adversarial.py |
| A-10 | Spielstand nach ungültigen Dateien wieder ladbar | tests/integration_adversarial.py |
| A-11 | Leerer Drei-Spieler-Spielstand kann gespeichert werden | tests/integration_adversarial.py |
| A-12 | Manipulierte Highscore-Datei als Ganzes abweisen | tests/integration_adversarial.py, include/ScoreFile.h |
| A-13 | Unerwartetes Eingabeende wird kontrolliert gemeldet | tests/integration_adversarial.py |
| A-14 | Textoberfläche, vier Spieler, Gewinnmarkierung | tests/console_ui_tests.cpp |
| A-15 | VS-Code-/CMake-Abgleich und F5-Build-Ziel | tests/vscode_config_test.py |
| A-16 | Windows und Linux, Debug und Release | .github/workflows/cpp-quality.yml |
| A-17 | UndefinedBehaviorSanitizer (UBSan) unter Linux | .github/workflows/cpp-quality.yml |
| A-18 | Der echte Windows-MSYS2-UCRT64-Build, CTest und direkter g++-Compilerbefehl | .github/workflows/cpp-quality.yml |
| A-19 | SFML-2D-/OpenGL-3D-Builds, Headless-Smoke-Tests und Paketierung | .github/workflows/cpp-quality.yml |
| A-20 | Technischer PDF-/PPTX-/Link-Preflight | .github/workflows/docs-quality.yml |

**Wichtig:** Die 360 Referenzprüfungen füllen das Brett auch nach einem ersten Gewinn weiter. Sie sind Belastungstests für die Board-Klasse und keine 360 regelkonform beendeten Spielpartien. Der feste Seed 0xC0FFEE42 ermöglicht die Wiederholung.

Die 13 negativen Dateitests umfassen falsche Kennungen, ungültige Header, unplausible Zugzahlen, doppelte Namen, falsche Spieler/Symbole, nicht mögliche Fallhöhen, veränderte Rasterinhalte, Anhänge und abgeschnittene Dateien.

## 2. So führt Herr Litera die Tests selbst aus

Voraussetzungen: C++17-Compiler, CMake 3.16 oder neuer, Python 3.

~~~powershell
git clone https://github.com/Julik527/vier-gewinnt-gym-edition-cpp.git
cd vier-gewinnt-gym-edition-cpp
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
~~~

CTest muss für jede Build-Art alle registrierten Tests als bestanden melden. Bei Fehlschlag zeigt es den genauen Testnamen und die Ausgabe. Die Tests legen eigene temporäre Datenordner an und greifen nicht auf persönliche Highscores zu.

Ein separater Windows-Job richtet MSYS2 UCRT64 ein und überprüft den CMake-Build, alle CTest-Prüfungen sowie den direkten g++-Befehl aus der VS-Code-Buildaufgabe.

Ein weiterer CI-Lauf prüft unter Linux mit UndefinedBehaviorSanitizer, ob die ausgeführten C++-Pfade undefiniertes Verhalten verursachen. Ein erfolgreicher Sanitizer-Lauf ist kein mathematischer Beweis völliger Fehlerfreiheit.

## 3. Manuelle Abnahme auf dem echten Schul-PC

Diese Schritte sind **noch vor Ort zu prüfen**. Nach der Ausführung Datum, Rechner, Compiler-Version, Ergebnis und gegebenenfalls Screenshots notieren.

| ID | Aktion | Erwartetes Resultat |
|---|---|---|
| M-01 | Projektstammordner in VS Code öffnen, Strg+Shift+B, F5 | Konsolenversion baut und startet |
| M-02 | Zwei Spieler: Ada, Ben | Spielfeld 7 Spalten × 6 Reihen |
| M-03 | Zugfolge: 1, 7, 2, 7, 3, 7, 4 | Horizontaler Sieg Ada, Gewinnsymbole markiert |
| M-04 | Revanche auswählen | Gleiche Spieler, neues leeres Brett |
| M-05 | Drei und vier Spieler testen | 9×8 beziehungsweise 11×10 |
| M-06 | In eine volle Spalte einwerfen | Fehlermeldung, unveränderter Spielstand |
| M-07 | Vorschau und Undo testen | Vorschau verändert nichts, Undo setzt zurück |
| M-08 | Spiel speichern, Programm beenden, erneut laden | Spielstand und Spielerwechsel bleiben erhalten |
| M-09 | abc, -1 und übergroße Zahl eingeben | Kein unkontrollierter Absturz |
| M-10 | Bestenliste nach Siegen prüfen | Korrekte Anzahl und Sortierung |
| M-11 | Repository frisch auf einem zweiten Rechner klonen | Build ohne lokale Zusatzdateien möglich |

## 4. Fachgespräch: mögliche Lehrerfragen

**Wofür sind die Klassen da?** Board verwaltet das Raster und die Regeln. Player hält Name und Symbol. Game steuert Eingaben, Runden und Dateien. ConsoleUI ist die getrennte Einheit für die Textdarstellung. Das ermöglicht kleinere, isolierbare Tests.

**Wie funktioniert die Gewinnerkennung?** Vom zuletzt gesetzten Feld aus werden vier Richtungen untersucht. Gleiche Symbole werden vorwärts und rückwärts gezählt. Mit dem gesetzten Stein müssen mindestens vier hintereinanderliegen.

**Warum ist das Speichern zweistufig?** Eine temporäre Datei wird zuerst vollständig geschrieben und geschlossen. Erst dann wird versucht, die bisherige Datei zu ersetzen. Das reduziert das Risiko einer teilweise überschriebenen Datei, garantiert aber keine Stromausfallsicherheit und keine konfliktfreie Mehrprozessnutzung.

**Wozu verschiedene Testebenen?** Unit-Tests überprüfen Board und ConsoleUI gezielt. Integrationstests starten das fertige Spiel über echte Standardeingabe und kontrollieren Konsolentext und gespeicherte Dateien.

**Warum reichen grüne Tests nicht für eine garantierte 1,0?** Auch Verständlichkeit, Aufgabenbezug, eigenständige Erklärung, Präsentation und Handout sowie unvorhergesehene Testfälle können bewertet werden.

## 5. Offen und nicht als abgeschlossen ausgeben

- Die 2D-/3D-Quellcodes und ihre SFML-/OpenGL-CMake-Ziele sind im integrierten Entwicklungsstand enthalten. Ein erfolgreicher CI-Headless-/Fenster-Smoke-Test ersetzt weiterhin nicht die manuelle Grafik- und Audioabnahme auf dem Schul-PC.
- Das F5-Debugging und das Terminal auf dem **tatsächlichen Schul-PC** sind separat zu testen.
- Technische Dokumententests ersetzen keine vollständige visuelle Prüfung aller Folien und keine Generalprobe.
- Die Software ist ein lokales Schulprojekt, nicht für parallele Schreibzugriffe oder beliebig große feindliche Dateien entwickelt.
- Das Testprotokoll ist keine Notengarantie und behauptet keine bereits durchgeführte schulische Abnahme.

**Prüfquellen:** [GitHub Actions](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions) · [Pull Request #9](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/9). Maßgeblich ist der erfolgreiche CI-Lauf des tatsächlich eingereichten Commit-Stands.
