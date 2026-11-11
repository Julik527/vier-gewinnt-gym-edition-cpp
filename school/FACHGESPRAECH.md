# Fachgespräch | C++17-Konsolenprogramm

**Ziel:** Den Code verstehen und am Bildschirm erklären. Die Musterantworten sind Lernhilfen, keine auswendig vorzutragenden Antworten.

## 1. Einstieg in 60 Sekunden

„Ich habe Vier Gewinnt als objektorientiertes C++17-Konsolenspiel umgesetzt. `Board` verwaltet ausschließlich das Raster und die Spielregeln. `Game` übernimmt Menü, Eingaben, Spielzüge und gespeicherte Daten. `Player` hält den Namen und das Spielsymbol, `console_ui` erzeugt die Textdarstellung. Zwei bis vier Personen können spielen. Nach jedem gültigen Einwurf wird auf vier zusammenhängende Spielsteine oder ein Remis geprüft.“

## 2. Codefragen, die ich selbst beantworten können muss

**Wo beginnt das Programm?** In `src/main.cpp`, Funktion `main()`. Es wird ein Objekt vom Typ `Game` erstellt und dessen `run()` aufgerufen. Unerwartete Ausnahmen werden abgefangen.

**Was ist ein Konstruktor?** Eine spezielle Klassenfunktion zur Initialisierung eines Objekts. `Board(int playerCount)` setzt z. B. die Rastergröße und füllt das Feld mit Leerzeichen-Platzhaltern `'.'`.

**Was bedeutet Kapselung?** Interne Daten wie `rows_`, `columns_` und `grid_` sind `private` und werden kontrolliert über Methoden verändert oder gelesen.

**Warum ein `std::vector<std::vector<char>>`?** Das äußere `vector` enthält Reihen, jedes innere `vector` eine Zeile aus Feldsymbolen. Damit sind verschiedene Feldgrößen möglich.

**Was macht `Board::nextFreeRow`?** Sie prüft eine Spalte von unten nach oben. Beim ersten `'.'` gibt sie die freie Zeile zurück, sonst `-1`.

**Was passiert beim Einwurf?** `Board::drop` lehnt das leere Symbol und ungültige/volle Spalten ab. Bei gültiger Spalte ersetzt es genau ein leeres Feld und liefert dessen Zeile zurück.

**Wie prüfst du einen Gewinn?** Vom letzten Stein aus in vier Achsen. Für jede Achse werden beide Richtungen gezählt; der Startstein wird einmal addiert. `>= 4` bedeutet Sieg.

**Wie verhinderst du einen ungültigen Undo?** `Board::undo` entfernt nur einen vorhandenen Stein, der in seiner Spalte nicht von weiteren Steinen überdeckt ist. `Game` benutzt dazu die gespeicherte Zuggeschichte.

**Wann ist das Spiel unentschieden?** Wenn nach einem Zug kein Sieg vorliegt und das ganze Feld belegt ist (`Board::isFull`).

**Warum `std::map<std::string, int>`?** Der Spielername dient als Schlüssel, die Zahl der Siege als Wert. Die Bestenliste wird bei der Ausgabe nach Siegen sortiert.

**Wie funktioniert Speichern?** `Game::saveGame` erzeugt den Text im `GYM1`-Format. `gym::writeSafely` schreibt zunächst in eine temporäre Datei und ersetzt bei Erfolg die Zieldatei.

**Was verhindert das Laden fehlerhafter Dateien?** `Game::loadGame` validiert zuerst die Daten und baut eine geprüfte Kandidatenpartie auf. Erst danach wird die laufende Partie ersetzt.

**Warum ist das Speichern nicht vollkommen absturzsicher?** Ein Austausch per `rename` oder Windows-Dateioperation ist noch keine vollständig synchronisierte Transaktion. Parallel laufende Prozesse werden nicht koordiniert.

**Wozu Unit- und Integrationstests?** Unit-Tests prüfen einzelne Teile wie `Board`. Integrationstests starten das echte Konsolenprogramm mit simulierten Eingaben. Property-Tests vergleichen viele deterministische Zustände mit einem Referenzmodell.

**Was bedeutet `const`?** Eine als `const` markierte Methode darf den beobachtbaren Objektzustand nicht verändern. `const T&` vermeidet eine Kopie und erlaubt nur lesenden Zugriff auf den übergebenen Wert.

**Warum CMake?** Es beschreibt plattformübergreifend, welche Quelldateien zusammengehören, wie das Programm kompiliert wird und wie CTest die Prüfungen ausführt.

## 3. Praktische Demonstration am Quellcode

1. `Board.h` öffnen und `private` sowie `public` erläutern.
2. `Board.cpp`: Größe für 2/3/4 Spieler, `nextFreeRow`, `drop` und vier Gewinnachsen zeigen.
3. `Game.cpp`: Hauptmenü, gültige Eingabe, Speichern/Laden und Highscore vorstellen.
4. `ConsoleUI.cpp`: Trennung von Darstellung und Spielzustand erläutern.
5. `tests/board_tests.cpp`: einen gezielten Grenzfall prüfen.
6. Konsolenprogramm starten, eine kurze Partie und Spielstand demonstrieren.

## 4. Kritische Fragen

**„Warum keine GUI?“** „Die Schulaufgabe fordert ein C++-Konsolenprogramm. Ich gebe nur die vollständig dokumentierte und prüfbare Konsolenfassung ab.“

**„Hast du den Code eigenständig verstanden?“** An einer unbekannten Eingabe oder einem konkreten Grenzfall die Wirkung der Funktionen **live** erläutern und notfalls eine kleine Änderung mit anschließendem Test vornehmen. KI-Unterstützung offen angeben.

**„Was würdest du verbessern?“** Weitere Vereinfachung der Benutzereingaben, robustere Dateiverarbeitung, klarere Trennung von Menü und Regeln und zusätzliche manuelle Endtests. Nicht behaupten, dass bereits alles perfekt ist.
