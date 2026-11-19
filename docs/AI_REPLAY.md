# Vier Gewinnt | Gym Edition - Optionales KI-Labor und Replay

**Stand:** KI-Konsolenoberflaeche 19.11.2026.  
**C++:** C++17, keine Abhaengigkeiten auf SFML oder OpenGL.  
**Schulabgabe:** Der bisherige Pflicht-Konsolenmodus wird nicht veraendert.

## Enthalten

| Baustein | Funktion |
| --- | --- |
| GymAi.h / GymAi.cpp | Drei Schwierigkeitsstufen mit Minimax und Alpha-Beta-Pruning |
| GymReplay.h / GymReplay.cpp | Legale Zugfolge zu einzelnen Spielfeld-Snapshots zusammensetzen |
| ai_main.cpp / AiConsoleUI.cpp | Separate, gestaltete KI-Konsole mit Menue, Spielstand, Hilfe, Schwierigkeit, Vorschau, Partie-Pause und Replay |
| ai_replay_tests.cpp | Deterministische Tests fuer Gewinne, Blockade, Zustandsgrenzen und Replay |

Das Programm heisst **vier-gewinnt-ki**. Es verwendet die bereits bestehende Klasse **Board** und vermeidet doppelte Spielfeld- oder Gewinnregeln.

## Unter Windows/Linux bauen

Voraussetzung: CMake 3.16 oder neuer sowie ein C++17-Compiler. Ein regulärer Build ohne den AI-Schalter bleibt bei den bisherigen Zielen.

~~~bash
cmake -S . -B build-ai -DGYM_BUILD_AI=ON -DGYM_BUILD_GUI=OFF
cmake --build build-ai --parallel
ctest --test-dir build-ai --output-on-failure -R ai_replay_unit_tests
~~~

Start unter Linux haeufig mit **./build-ai/vier-gewinnt-ki**. Bei Windows mit Ninja **.\build-ai\vier-gewinnt-ki.exe**, bei Visual Studio moeglicherweise **.\build-ai\Debug\vier-gewinnt-ki.exe**.

## Neues Hauptmenue im Stil der Schulkonsole

Die KI-Ausgabe verwendet dieselbe ANSI-Erkennung, die gleichen Farben
(Blau fuer R, Gold fuer B) und die 69-Spalten-Rahmengestaltung der
normalen Gym-Edition-Konsole. Ohne ANSI-Unterstuetzung oder mit
\`NO_COLOR\` bleibt die Bedienung lesbar. Das Schulspiel und seine
Menues wurden nicht umgebaut; die KI bleibt ein getrenntes Programm.

| Auswahl | Funktion |
| --- | --- |
| 01 | Neues KI-Duell (vor Verwerfen einer laufenden Partie wird gefragt) |
| 02 | Laufendes Duell fortsetzen |
| 03 | Schwierigkeit fuer das naechste Duell aendern |
| 04 | Spielername (druckbares ASCII, 1-20 Zeichen) |
| 05 | Regeln und Steuerung |
| 06 | Sitzungsbilanz |
| 07 | Letzte abgeschlossene Partie im Replay |
| 00 | Programm beenden |

**Während des Duells:** Mit \`1\` bis \`7\` einen Stein setzen,
mit \`V4\` eine unverbindliche Vorschau fuer Spalte 4 betrachten,
mit \`H\` die Hilfe aufrufen und mit \`M\` ins Hauptmenue wechseln,
ohne den laufenden Brettzustand zu verlieren. Die KI zieht danach
automatisch. Bei einem Sieg sind die Gewinnzellen markiert.

**Rundenende:** \`01\` Revanche, \`02\` Schwierigkeit waehlen,
\`03\` Replay, \`00\` Hauptmenue. Replay: \`N\` vorwaerts,
\`P\` zurueck, \`Q\` verlassen. Letzte Partie und Bilanz
bleiben nur im RAM; die bisherigen Highscores und GYM1-Dateien
werden weder gelesen noch ueberschrieben.

**VS Code:** Nach \`git pull\` im Arbeitsordner \`C:\\Dev\\GymEdition\`
unter **Ausfuehren und Debuggen** das vierte Profil
\`Vier Gewinnt | KI-Agent\` waehlen und **F5** druecken.
Es konfiguriert \`GYM_BUILD_AI=ON\` und baut nur die KI-Konsole.
Alternativ gelten die manuellen CMake-Befehle oben.


## Spielregeln und Stufen

- **Einfach (1):** Nimmt die erste freie Spalte in fester Mitte-zuerst-Reihenfolge; kein Minimax.
- **Normal (2):** Minimax mit Alpha-Beta-Pruning bis Tiefe 3; erkennt unmittelbare Siege und blockiert einen direkt bevorstehenden Sieg.
- **Schwer (3):** Dasselbe Verfahren mit Tiefe 6 und groesserem Suchraum.

Die Heuristik bewertet Viererfenster und die Mitte des Spielfelds. Der Computer verwendet **regelbasierte Suche**, kein maschinelles Lernen und keinen Trainingsdatensatz. Optimales Spiel wird fuer keine Stufe garantiert. Der KI-Modus unterstuetzt nur zwei Spieler auf dem 7x6-Feld mit den Symbolen R und B.

## Replay

Nach einer beendeten KI-Partie kann man das Replay im Ergebnisbildschirm oder ueber das Hauptmenue starten.

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
