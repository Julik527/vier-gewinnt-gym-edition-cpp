# Quellcode lesen und erklären

Dieser Leitfaden ergänzt die **Kommentare direkt in den C++-Dateien**.
Die standardisierten Doxygen-Dateiköpfe sind unter [Dateikopf-Standard](DATEIKOPF_STANDARD.md) erläutert. Die
Kommentare erklären wichtige Entscheidungen, Zustände, Ein-/Ausgaben und
Grenzfälle. Selbstverständliche Syntax wie `i++` wird bewusst nicht kommentiert.

## 1. Von `main()` zur Spielrunde

Die Schulkonsole beginnt in [`src/main.cpp`](../src/main.cpp). Sie erzeugt
ein `Game`-Objekt und startet `Game::run()`. Die Menüführung liegt in
[`src/Game.cpp`](../src/Game.cpp), die Ausgabe in
[`src/ConsoleUI.cpp`](../src/ConsoleUI.cpp). So bleibt die
Spielregelklasse unabhängig vom Terminal.

## 2. Warum `Board` eine eigene Klasse ist

[`Board`](../include/Board.h) speichert das Raster und prüft Einwurf,
Schwerkraft, Undo und vier Gewinnrichtungen. Ein Feld ist `grid_[zeile][spalte]`
mit **nullbasierten Indizes**. Die Konsole zeigt dagegen die Spalten **ab 1**;
vor `Board::drop()` wird daher `1` abgezogen.

### Gewinnerkennung in einem Satz

`Board::hasWon()` zählt ab dem zuletzt gelegten Stein auf **vier Achsen**
(horizontal, vertikal, zwei Diagonalen) jeweils beide Richtungen. Sind
einschließlich Startfeld mindestens vier gleiche Symbole verbunden, ist es
ein Sieg. `winningCells()` liefert die markierbaren Feldkoordinaten.

## 3. Was bedeutet Undo?

Die Liste `history_` enthält nur erfolgreich gesetzte Züge, jeweils mit
Zeile, Spalte und Spielerindex. Undo entfernt **den letzten gesetzten Stein**
und stellt die Zugreihenfolge wieder her. Steine darunter dürfen nicht
entfernt werden, da sonst ein schwebender Spielzustand entstehen würde.

## 4. Warum wird ein Speicherstand nachgespielt?

Das GYM1-Format enthält Spieler, Zugfolge und das resultierende Raster.
Beim Laden werden die Züge zunächst auf einem **temporären Board**
rekonstruiert und danach mit dem gespeicherten Raster verglichen. Das
erkennt ungültige Spalten, falsche Spielerfolgen, unzulässige Schwerkraft,
bereits beendete Partien und widersprüchliche Dateien. Erst nach vollständiger
Prüfung wird der alte Zustand ersetzt.

Für das Schreiben verwendet
[`AtomicFile.h`](../include/AtomicFile.h) eine temporäre Datei und
anschließendes Ersetzen. Die Implementierung reduziert das Risiko
unvollständiger Dateien, garantiert aber keine absolute Datensicherheit
bei Stromausfall.

## 5. Wo sind 2D, 3D und Sound?

[`src/gui/Main.cpp`](../src/gui/Main.cpp) behandelt SFML-2D-Eingaben,
Anzeige und Animation. [`Main3D.cpp`](../src/gui/Main3D.cpp) zeichnet
räumliche Geometrie mit OpenGL und das Menü mit SFML. Beide verwenden
[`MatchSession`](../src/MatchSession.cpp), statt die Regeln neu zu
implementieren. Für die 3D-Spaltenauswahl wird ein Pixelstrahl mit der
Spielbrettebene geschnitten.

[`GymAudio.h`](../src/gui/GymAudio.h) steuert OGG-Streams und
Überblendungen. `GymAudioAssets.h` prüft den Fundort und den OGG-Header;
die **vollständigen SHA-256-Werte** prüft erst der gesonderte
Audio-Installer. Der Audiozustand verändert keine Spielregeln.

## 6. Wie sind die Aussagen überprüfbar?

`tests/board_tests.cpp` enthält Grenzfälle für Spielfeld und Siege.
`tests/board_property_tests.cpp` testet viele reproduzierbare Zugfolgen
gegen ein unabhängiges Referenzmodell. `tests/match_session_tests.cpp`
prüft den Spielzustand, gespeicherte Daten und korrupte Dateien. Die
GitHub-Actions-Builds prüfen die Übersetzbarkeit und automatische Tests,
aber ersetzen keinen echten Hör-, Grafik- oder Vortragstest.

## Sinnvolle Fragen im Fachgespräch

- Warum gehören Gewinnerkennung und Schwerkraft in `Board`?
- Wo wird aus Spalte 1 für den Spieler der interne Index 0?
- Warum wird der neue GYM1-Spielstand erst **nach** der Prüfung übernommen?
- Welchen Vorteil bietet ein `vector<Move>` für Undo und Speichern?
- Was trennt `MatchSession` von den 2D-/3D-Oberflächen?
- Was prüfen die automatisierten Tests, was können sie nicht prüfen?

Die Antworten lassen sich im verlinkten Code finden. Ziel ist, die
Implementierung nachvollziehen und selbst erklären zu können.
