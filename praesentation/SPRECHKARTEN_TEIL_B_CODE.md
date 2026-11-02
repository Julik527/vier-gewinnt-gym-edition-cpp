# Sprechkarten · Teil B: Codeerklärung

**Projekt:** Vier Gewinnt | Gym Edition · C++17 · ITA-Programmierprojekt 2026/27  
**Ausgabe:** JK AXIS | GOLD MASTER · Übungsfassung vom 02.11.2026  
**Quelle:** Entwicklungsbranch `quality/gameplay-persistence-2026-10-28` (Stand 31.10.2026)  
**Dauer:** ca. 15 Minuten für Teil B; exakte Sprechzeit muss mit Stoppuhr geprobt werden.

> **Benutzung:** Den Text unter **Sprechtext** darfst du zum Üben ablesen. Vor der Abgabe auf deinen echten Wortlaut, deine tatsächliche Eigenleistung und die endgültigen Folien anpassen. **Aktion** bedeutet: etwas im Programm, in VS Code oder auf einer Folie zeigen. Alles, was nicht am Zielrechner überprüft wurde, ausdrücklich als noch offen bezeichnen.
>
> **Zeitreserve:** Falls die gesamten 30 Minuten die Rückfragen einschließen, die letzten Karten kürzen und etwa 3 Minuten für das Fachgespräch freilassen. Die Zeitfenster sind Orientierung, keine bewiesenen Sprechzeiten.

---

## B01 · Einstiegspunkt und Programmfluss
**Geplantes Zeitfenster:** 00:00–01:30 · **Anzeigen:** src/main.cpp → Game::run()

### Sprechtext (Vorlesefassung)

> Ich beginne beim Einstiegspunkt der Konsolenanwendung, also bei „main.cpp“. In C++ startet die Ausführung in der Funktion „main“. Dort wird ein Objekt der Klasse „Game“ erzeugt und mit „game.run()“ die Hauptsteuerung aufgerufen. Das geschieht innerhalb eines „try“-Blocks. Falls eine unerwartete Exception auftritt, fängt der „catch“-Block sie ab, gibt eine Fehlermeldung aus und liefert einen Fehlerstatus zurück.
>
> > Die Methode „Game::run“ enthält die Schleife für das Hauptmenü. Anhand der Benutzereingabe wird entschieden, ob eine Partie neu beginnt, fortgesetzt, geladen oder das Programm beendet wird. Danach führt „Game::play“ die eigentliche Zugschleife aus. Das ist wichtig für die Architektur: „main“ bleibt klein und übersichtlich. Sie enthält nicht das gesamte Spiel. Bei der grafischen 2D- und 3D-Version gibt es eigene Einstiegspunkte, die stattdessen eine grafische Anwendung starten.

**Aktion / Blickkontakt:** Mit dem Mauszeiger die drei Stellen markieren: Game game; game.run(); catch.

**Merksatz:** main startet; Game organisiert.

---

## B02 · Board: Datenstruktur und Feldgröße
**Geplantes Zeitfenster:** 01:30–03:15 · **Anzeigen:** include/Board.h und Konstruktor in src/Board.cpp

### Sprechtext (Vorlesefassung)

> Hier sehen wir die Klasse „Board“. Die Spielfelddaten liegen in „grid_“. Das ist ein „vector“ von „vector<char>“, also ein zweidimensionaler, dynamisch großer Container. Jede Zelle enthält entweder einen Punkt für leer oder ein Zeichen für das Symbol eines Spielers. Die Member „rows_“ und „columns_“ speichern die Abmessungen. Sie sind privat, damit nicht jeder Programmteil den Zustand beliebig ändern kann.
>
> > Im Konstruktor wird die Größe anhand der Spieleranzahl festgelegt. Die Zeilenzahl ist sechs plus zweimal die Zahl der zusätzlichen Spieler über zwei hinaus. Die Spaltenzahl ist um eins größer. So entstehen für zwei Spieler sechs Reihen und sieben Spalten, für drei Spieler acht Reihen und neun Spalten und für vier Spieler zehn Reihen und elf Spalten. Die Werte sind intern nullbasiert indiziert. Dadurch ist bei einem Sechs-Reihen-Feld die unterste Reihe als Index fünf gespeichert.

**Aktion / Blickkontakt:** Auf rows_, columns_, grid_ und Board::Board zeigen.

**Merksatz:** Zustand kapseln statt globale Variablen verwenden.

---

## B03 · Schwerkraft und gültiger Zug
**Geplantes Zeitfenster:** 03:15–05:30 · **Anzeigen:** Board::nextFreeRow und Board::drop; optional Breakpoint

### Sprechtext (Vorlesefassung)

> Wenn eine Person beispielsweise Spalte vier auswählt, wird die sichtbare Spaltennummer zunächst auf einen internen Index umgerechnet. „Board::nextFreeRow“ erhält diesen Index und prüft zuerst die Grenzen. Ist er kleiner als null oder mindestens so groß wie die Spaltenanzahl, liefert die Methode minus eins zurück.
>
> > Danach beginnt eine „for“-Schleife bei „rows_ - 1“. Sie sucht also von unten nach oben. Sobald sie eine Zelle mit einem Punkt findet, gibt sie den Index dieser Reihe zurück. Das simuliert die Schwerkraft, ohne dass die Spiellogik eine Animation benötigt. Die Methode „drop“ ruft diese Suche auf und legt nur dann das Spielsymbol in das Raster, wenn wirklich ein freies Feld existiert. Die gesetzte Zeile wird über den Referenzparameter „rowPlaced“ zurückgegeben. Bei einer vollen oder ungültigen Spalte wird der Zug abgewiesen und der Spieler nicht gewechselt.

**Aktion / Blickkontakt:** Im Debugger die Werte column, rowPlaced und grid_[row][column] ansehen.

**Merksatz:** Erst die freie Zeile finden, dann das Symbol eintragen.

---

## B04 · Gewinnalgorithmus in vier Achsen
**Geplantes Zeitfenster:** 05:30–08:00 · **Anzeigen:** Board::countDirection, Board::hasWon und winningCells

### Sprechtext (Vorlesefassung)

> Das ist der zentrale Algorithmus meines Spiels. Nach jedem gültigen Zug kennt das Programm die Zeile, die Spalte und das Symbol des neu gesetzten Steins. „hasWon“ prüft zunächst, ob diese Position gültig ist und tatsächlich das erwartete Symbol enthält. Anschließend werden vier Richtungsvektoren durchlaufen: waagerecht, senkrecht und die beiden Diagonalen.
>
> > Die Hilfsmethode „countDirection“ läuft schrittweise entlang einer Richtung, solange sie innerhalb des Spielfelds bleibt und das nächste Feld dasselbe Symbol enthält. Für jede der vier Achsen wird sie zweimal aufgerufen, einmal vorwärts und einmal rückwärts. Dazu kommt eins für den zuletzt gesetzten Stein selbst. Sobald diese Summe mindestens vier beträgt, wird ein Sieg erkannt.
>
> > Die Gegenrichtung ist wichtig, weil der letzte Stein auch mitten in einer fertigen Viererreihe liegen kann. Die ergänzende Methode „winningCells“ ermittelt die Koordinaten der gesamten zusammenhängenden Gewinnlinie. Dadurch kann die Benutzeroberfläche die gewinnenden Felder besonders markieren.

**Aktion / Blickkontakt:** Die Arrays dr und dc zeigen; ein Beispiel 2 links + 1 neu + 1 rechts erklären.

**Merksatz:** Ein Stein + gleiche Nachbarn in beide Richtungen ergibt die Linienlänge.

---

## B05 · Spielzug, Zugwechsel und Undo
**Geplantes Zeitfenster:** 08:00–09:45 · **Anzeigen:** Game::play und history_ in Game.h

### Sprechtext (Vorlesefassung)

> Die Klasse „Game“ verbindet Eingabe und Regeln. In „Game::play“ wird die Benutzeranweisung ausgewertet. Handelt es sich um eine Spaltennummer, prüft die Steuerung zunächst, ob sie im erlaubten Bereich liegt und die Spalte noch Platz hat. Dann ruft sie „Board::drop“ auf. Nach einem erfolgreichen Zug wird ein „Move“ in der Historie gespeichert. Diese Struktur enthält Zeile, Spalte und den Spielerindex.
>
> > Anschließend folgt die Gewinnprüfung. Hat niemand gewonnen, prüft das Programm, ob das gesamte Feld voll ist. Nur wenn die Partie weiterläuft, wird mit „(current_ + 1) % Spieleranzahl“ zum nächsten Spieler gewechselt. Das Modulo sorgt dafür, dass nach dem letzten Spieler wieder Spieler null folgt.
>
> > Beim Undo-Befehl wird der letzte Zug aus der Historie gelesen und der entsprechende oberste Stein entfernt. Danach wird genau der Spieler wieder aktiv, der diesen Zug zuvor gesetzt hatte.

**Aktion / Blickkontakt:** Den Modulo-Ausdruck im Quellcode markieren und bei 3 Spielern mündlich durchgehen.

**Merksatz:** Ungültige Züge verändern weder Feld noch Zugreihenfolge.

---

## B06 · Speichern, Laden und Schutz vor Fehlern
**Geplantes Zeitfenster:** 09:45–11:45 · **Anzeigen:** Game::saveGame, Game::loadGame und include/AtomicFile.h

### Sprechtext (Vorlesefassung)

> Der Spielstand wird in einer Textdatei mit der Kennung „GYM1“ abgelegt. Darin stehen Spieleranzahl, aktueller Spieler, Feldabmessungen, Zugzahl, die Spielerinformationen, die komplette Zugliste und das Raster. Beim Laden ist mir die Trennung zwischen gelesenen Daten und tatsächlichem Spielzustand wichtig.
>
> > Die Methode „loadGame“ erzeugt zunächst ein vorläufiges Board. Sie kontrolliert unter anderem die Spielerzahl, das Format, die erlaubten Symbole, die Zugreihenfolge und die exakten Landepositionen. Die Züge werden erneut auf das vorläufige Feld angewendet und anschließend mit dem gespeicherten Raster verglichen. Stimmt etwas nicht, wird das Laden abgebrochen, ohne die laufende Partie zu ersetzen.
>
> > Beim Schreiben hilft „gym::writeSafely“. Zuerst wird der vollständige neue Inhalt in einer temporären Datei erstellt. Erst danach wird die bisherige Datei ersetzt. Das schützt bei bestimmten Schreibfehlern. Es ist jedoch keine vollständige Absicherung gegen zwei gleichzeitig schreibende Programmprozesse.

**Aktion / Blickkontakt:** Nicht den ganzen Parser vorlesen; Format GYM1 und Kandidatenprinzip erklären.

**Merksatz:** Niemals ungeprüfte Dateiinhalte direkt in den Spielzustand übernehmen.

---

## B07 · Teststrategie und Nachweise
**Geplantes Zeitfenster:** 11:45–13:15 · **Anzeigen:** tests/board_tests.cpp, CMakeLists.txt, GitHub Actions

### Sprechtext (Vorlesefassung)

> Guter Code muss nicht nur im Idealfall funktionieren. Deshalb habe ich automatisierte Testfälle im Projekt. Die „board_tests“ prüfen zum Beispiel, ob ein Stein unten landet, ob ungültige Spalten abgelehnt werden und ob horizontale, vertikale und beide diagonalen Gewinnlinien erkannt werden. Weitere Integrationstests prüfen Abläufe wie Speichern, Laden, die Spielereinrichtung, Revanche und Highscore.
>
> > CMake organisiert die verschiedenen Programmteile als Build-Ziele. CTest startet die dazugehörigen Tests. Über GitHub Actions laufen diese Prüfungen zusätzlich in mehreren Umgebungen. In der Entwicklung konnte ich damit Fehler in Windows- und Linux-Konfigurationen feststellen und nach Korrekturen erneut prüfen.
>
> > Wichtig ist die Grenze dieser Tests: Ein grüner automatischer Lauf beweist nur, dass die tatsächlich ausgeführten Prüfungen erfolgreich waren. Ob eine 3D-Ansicht optisch gut funktioniert oder ob ein Sound am Schulrechner zu hören ist, muss ich zusätzlich manuell testen.

**Aktion / Blickkontakt:** Auf einen konkreten verify-Test und den Actions-Lauf zeigen, nicht nur das grüne Symbol.

**Merksatz:** Automatische und manuelle Tests ergänzen sich.

---

## B08 · Gesamtfluss, Grenzen und Abschluss
**Geplantes Zeitfenster:** 13:15–15:00 · **Anzeigen:** Kurz zurück zu Game::play und Board::hasWon

### Sprechtext (Vorlesefassung)

> Ich fasse den technischen Ablauf noch einmal zusammen. Die Konsole liest eine Eingabe. „Game“ interpretiert sie und reicht einen gültigen Spaltenindex an „Board“ weiter. Das Board berechnet die Landeposition und setzt die Hantel. Anschließend speichert die Steuerung den Zug, prüft auf Gewinn oder Remis und bestimmt gegebenenfalls den nächsten Spieler. Die Ausgabe wird danach anhand des aktuellen Zustands neu erzeugt.
>
> > Diese Trennung hat einen praktischen Nutzen: Die Regeln funktionieren unabhängig davon, ob das Spiel im Terminal, in einer 2D-Oberfläche oder in der 3D-Arena dargestellt wird. Allerdings haben die Konsole und die grafischen Programme heute noch eigene Ablauf-Controller. Eine spätere Verbesserung wäre, auch diese Logik vollständig in einem gemeinsamen Kern zusammenzuführen. Ebenso könnte ich die großen grafischen Quelldateien in kleinere, klar abgegrenzte Klassen aufteilen.
>
> > Damit endet meine Codeerklärung. Die wichtigsten Prinzipien sind für mich Kapselung, nachvollziehbarer Programmfluss, gezielte Eingabeprüfung, sichere Datenverarbeitung und reproduzierbare Tests. Für Rückfragen kann ich die relevanten Methoden direkt im Quellcode öffnen.

**Aktion / Blickkontakt:** Bei exakt 30 Minuten Gesamtzeit diese Karte kürzen und drei Minuten für Fragen reservieren.

**Merksatz:** Regel verstehen, Zustand kontrollieren, Verhalten testen.


---

## Qualitäts- und Vorführhinweise
- Die schulspezifische Pflicht ist die objektorientierte **Konsolenversion**. 2D und 3D sind freiwillige Erweiterungen.
- Ein erfolgreicher CI-Job ist keine manuelle Windows-/Grafik-/Audio-Abnahme.
- Für Demospielstände nur isolierte Verzeichnisse mit `tests/fixtures/` verwenden; persönliche Spielstände nicht überschreiben.
- Die KI-Unterstützung transparent nennen; Formulierungen nicht als Nachweis eigenständig entwickelter Methoden ausgeben.
- Bei einer Fachfrage: zuerst den Algorithmus fachlich erklären, anschließend die konkrete Funktion im Quellcode zeigen.

**Weiterführend:** [Vortragsleitfaden](VORTRAGSLEITFADEN.md) · [Generalprobe](../docs/GENERALPROBE_2027.md) · [Schulanforderungen](../docs/SCHULANFORDERUNGEN_51_STATUS.md) · [Fachgespräch](FACHGESPRAECH_KURZANTWORTEN.md)
