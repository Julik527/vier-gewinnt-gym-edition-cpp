# Handout | Vier Gewinnt – Gym Edition (Konsolenfassung)

**Fach:** Programmiertechnik (C++17) · **Ausbildung:** Informationstechnischer Assistent · **Aufgabe:** Vier Gewinnt nach Aufgabenblatt 51

## 1. Projektziel

Entwickelt wurde ein objektorientiertes C++-Konsolenspiel für zwei bis vier Personen. Die Spieler wählen Namen und setzen nacheinander Hantelsymbole in ein Spielfeld. Vier gleiche Steine in einer waagerechten, senkrechten oder diagonalen Linie führen zum Sieg. Sind alle Felder belegt, ohne dass jemand gewonnen hat, endet das Spiel unentschieden.

Die **Schulabgabe enthält ausschließlich das Konsolenprogramm**. Die über das Entwicklungsrepository möglichen grafischen Experimente gehören nicht zum Abgabeumfang.

## 2. Spielablauf

1. Im Hauptmenü `Neues Spiel` wählen.
2. Spielerzahl (2–4) und eindeutige Namen eingeben.
3. Die gewünschte Spalte wählen. Der Stein fällt auf das tiefste freie Feld.
4. Das Programm prüft nach jedem gültigen Zug Sieg, Remis und Spielerwechsel.
5. Bei Bedarf Zug zurücknehmen, Partie speichern oder später fortsetzen.
6. Nach einem Sieg stehen Ergebnis, Bestenliste und Revanche zur Verfügung.

| Spielerzahl | Feldgröße | Maximale Feldbelegung |
|---|---:|---:|
| 2 | 7 × 6 | 42 Steine |
| 3 | 9 × 8 | 72 Steine |
| 4 | 11 × 10 | 110 Steine |

## 3. Objektorientierte Struktur

| Datei/Klasse | Verantwortung |
|---|---|
| `main.cpp` | Programmstart, Hauptausnahmebehandlung |
| `Board` | Raster, Einwurf, Undo, Schwerkraft und Gewinnerkennung |
| `Player` | Spielername und zugehöriges Symbol |
| `Game` | Menü, Eingabeprüfung, Zugreihenfolge, Speichern und Highscores |
| `ConsoleUI` | Konsolenausgabe, Anleitung, Vorschau und Textanimation |
| `AtomicFile` / `ScoreFile` | Kontrolliertes Schreiben und Lesen von Daten |

**Prinzip:** Das Spielfeld kennt keine Spielernamen und keine Menüführung. `Game` nutzt `Board`, während die Ausgabe separat über `console_ui` erfolgt.

## 4. Wichtige Algorithmen

**Schwerkraft:** `Board::nextFreeRow()` sucht in der gewählten Spalte von unten nach oben das erste freie Feld. `Board::drop()` setzt dort das Symbol. Eine volle oder ungültige Spalte verändert das Feld nicht.

**Gewinnerkennung:** `Board::hasWon()` prüft vom zuletzt gesetzten Stein aus vier Achsen: waagerecht, senkrecht und beide Diagonalen. In jede Achse wird in **beide Richtungen** gezählt. Ab vier zusammenhängenden Steinen liegt ein Gewinn vor.

**Undo:** Der zuletzt gesetzte, nicht überdeckte Stein wird entfernt und der passende Spieler erhält erneut den Zug.

## 5. Speichern, Laden und Sicherheit

Die Partie wird als Textdatei im Format `GYM1` gespeichert. Vor dem Laden werden Spielerdaten, Spielfeldmaße, Zugfolge und Raster überprüft. Ungültige Dateien sollen den laufenden Spielzustand nicht ersetzen. Die Bestenliste verwendet einen streng geprüften Parser.

Beim Schreiben wird zuerst eine temporäre Datei erstellt und anschließend die Zieldatei ersetzt. Das ist kein vollständiger Schutz vor Stromausfall oder parallelen Programmstarts.

## 6. Nachweise und Grenzen

Automatische Tests prüfen Spielfeldgrößen, Gewinnrichtungen, Schwerkraft, ungültige Eingaben, Undo, Speicherfehler und Highscores. Die C++-Property-Tests vergleichen **360 deterministisch erzeugte Testpartien** mit einem Referenzmodell.

**Noch persönlich zu prüfen:** Start auf dem vorgesehenen Schul-PC, vollständige Testpartien, Präsentation/Fachgespräch sowie das echte Handout-Layout.

**Technologie:** Standard-C++17, `std::vector`, `std::map`, `std::filesystem`, CMake/CTest. Keine GUI-Abhängigkeit im Abgabepaket.
