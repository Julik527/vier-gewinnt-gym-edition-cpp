# Vier Gewinnt | Gym Edition - Optionales KI-Labor und Replay

**Stand:** Zusatzentwicklung 19.11.2026 auf Feature-Branch.  
**C++:** C++17, keine Abhaengigkeiten auf SFML oder OpenGL.  
**Schulabgabe:** Der bisherige Pflicht-Konsolenmodus wird nicht veraendert.

## Enthalten

| Baustein | Funktion |
| --- | --- |
| GymAi.h / GymAi.cpp | Drei Schwierigkeitsstufen mit Minimax und Alpha-Beta-Pruning |
| GymReplay.h / GymReplay.cpp | Legale Zugfolge zu einzelnen Spielfeld-Snapshots zusammensetzen |
| ai_main.cpp | Separates CLI: Mensch gegen KI, Schritt-fuer-Schritt-Replay |
| ai_replay_tests.cpp | Deterministische Tests fuer Gewinne, Blockade, Zustandsgrenzen und Replay |

Das neue Programm heisst **vier-gewinnt-ki**. Es verwendet die bereits bestehende Klasse **Board** und vermeidet doppelte Spielfeld- oder Gewinnregeln.

## Unter Windows/Linux bauen

Voraussetzung: CMake 3.16 oder neuer sowie ein C++17-Compiler. Ein regulärer Build ohne den AI-Schalter bleibt bei den bisherigen Zielen.

~~~bash
cmake -S . -B build-ai -DGYM_BUILD_AI=ON -DGYM_BUILD_GUI=OFF
cmake --build build-ai --parallel
ctest --test-dir build-ai --output-on-failure -R ai_replay_unit_tests
~~~

Start unter Linux haeufig mit **./build-ai/vier-gewinnt-ki**. Bei Windows mit Ninja **.\build-ai\vier-gewinnt-ki.exe**, bei Visual Studio moeglicherweise **.\build-ai\Debug\vier-gewinnt-ki.exe**.

## Spielregeln und Stufen

- **Einfach (1):** Nimmt die erste freie Spalte in fester Mitte-zuerst-Reihenfolge; kein Minimax.
- **Normal (2):** Minimax mit Alpha-Beta-Pruning bis Tiefe 3; erkennt unmittelbare Siege und blockiert einen direkt bevorstehenden Sieg.
- **Schwer (3):** Dasselbe Verfahren mit Tiefe 6 und groesserem Suchraum.

Die Heuristik bewertet Viererfenster und die Mitte des Spielfelds. Der Computer verwendet **regelbasierte Suche**, kein maschinelles Lernen und keinen Trainingsdatensatz. Optimales Spiel wird fuer keine Stufe garantiert. Der KI-Modus unterstuetzt nur zwei Spieler auf dem 7x6-Feld mit den Symbolen R und B.

## Replay

Nach einer beendeten KI-Partie fragt das Programm nach der Wiedergabe.

- **n:** einen Zug vorwaerts
- **p:** einen Zug zurueck
- **q:** Replay beenden

Replay rekonstruiert die Partie im Arbeitsspeicher; jede Position ist ein separates Board-Snapshot. Fehlerhafte Spalten, volle Spalten und Zuege nach einem Sieg/Remis werden abgelehnt. Die Replay-Bibliothek kann auch gueltige Zugfolgen mit drei oder vier Spielern darstellen. Bestehende GYM1-Dateien werden nicht automatisch eingelesen.

## Datensicherheit und Grenzen

- **Kein Zugriff auf Spielstaende/Highscores:** Keine Spielstand-, Statistik- oder Konfigurationsdatei wird durch den KI-Zusatz geschrieben.
- **Sitzungsbilanz:** Siege, Niederlagen und Remis gelten nur fuer den laufenden Prozess und sind nach Neustart weg.
- **Nicht integriert:** Der neue Gegner ist noch nicht direkt in SFML 2D, OpenGL 3D oder in das originale Schulmenue eingebaut.
- **Kein Netzwerkmodus:** Auch kein Online-Multiplayer.
- **Keine Windows-Freigabe allein durch CI:** Grafik, Audio, Tastatur und Schulbeamer erfordern eigene Praxistests.

## Qualitaets- und Abnahmevorgaben

- [ ] Neue C++-Tests im Pull Request fuer den exakten Commit gruen (Linux/Windows Debug/Release, MSYS2-UCRT64).
- [ ] Mindestens drei vollstaendige Partien je Schwierigkeitsstufe manuell spielen.
- [ ] KI-Sofortsieg, Sofortblockade, volle Spalten, fehlerhafte Eingabe und Programmabbruch testen.
- [ ] Replay vor- und zuruecklaufen lassen, echten Schrittzustand pruefen.
- [ ] Separates Schulabgabe-ZIP aus dem bestehenden Packaging-Skript nach SHA-256 pruefen.
- [ ] Vor erstem offiziellen Release echte Windows-EXE, ZIP-Inhalt, Lizenzierung und reale Screenshots nachweisen.

Die Schulkonsole und ihre Pruefungen haben Vorrang vor freiwilligen Zusatzfunktionen.
