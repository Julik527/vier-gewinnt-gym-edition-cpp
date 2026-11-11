# Sprechkarten Teil B: C++-Codeerklärung

**Projekt:** Vier Gewinnt | Gym Edition · C++17 · ITA 2026/27  
**Fassung:** Präsentationstraining · Revision 02.11.2026  
**Basis:** Entwicklungsbranch `quality/gameplay-persistence-2026-10-28`, geprüfter Quellcodestand 31.10.2026  
**Vortragsziel:** 15:00 Minuten für Teil B inklusive der angegebenen Aktionen. Die tatsächliche Redezeit ist noch zu messen.

> **Zum Ablesen:** Nur die Absätze unter **Sprechtext** gehören zum Vortrag. Die übrigen Zeilen sind Regiehinweise. Vor der Abgabe den Text dem tatsächlich gezeigten Programm und deiner eigenen Tätigkeit anpassen. Fachbegriffe lieber korrekt und langsam erklären als hastig sämtliche Sätze abzulesen.

### Zeitregie

| Karte | Thema | Zeitfenster | Endmarke |
|:--:|---|---:|---:|
| B01 | Einstiegspunkt und Programmablauf | 00:00–01:20 | 01:20 |
| B02 | Datenmodell und Feldgröße | 01:20–03:00 | 03:00 |
| B03 | Schwerkraft, Grenzen und erfolgreicher Zug | 03:00–05:15 | 05:15 |
| B04 | Gewinnerkennung und Richtungsvektoren | 05:15–08:00 | 08:00 |
| B05 | Spielerwechsel, Undo und Zustände | 08:00–09:50 | 09:50 |
| B06 | Dateiformat und kontrolliertes Laden | 09:50–12:05 | 12:05 |
| B07 | Unit-Tests, Integrationstests und CI | 12:05–13:25 | 13:25 |
| B08 | Zusammenfassung und fachlicher Abschluss | 13:25–15:00 | 15:00 |

**Prüfungsvariante mit Fragen innerhalb von 30 Minuten:** Teil A und B jeweils auf ca. 13:30 Minuten kürzen und insgesamt 3 Minuten für das Fachgespräch reservieren. Dazu in A08 die Zusatzdemo streichen und B08 deutlich verkürzen. Beide Zeitpläne können nicht gleichzeitig vollständig ausgeführt werden.

---

## B01 · Einstiegspunkt und Programmablauf

**ZEIT:** 00:00–01:20  
**ZEIGEN:** src/main.cpp → Game::run → Game::play

### Sprechtext

Ich starte die Codeerklärung in der Datei „src/main.cpp“. Die Funktion „main“ ist der Einstiegspunkt der Konsolenanwendung. Hier wird ein Objekt der Klasse „Game“ erzeugt. Mit „game.run()“ beginnt anschließend die Menüsteuerung. Der umgebende „try-catch“-Block fängt unerwartete Ausnahmen ab und sorgt dafür, dass eine Fehlermeldung ausgegeben werden kann.

In „Game::run“ befindet sich die Hauptmenüschleife. Sie prüft, welchen Menüpunkt eine Person auswählt. Startet oder lädt sie eine Partie, übernimmt „Game::play“ die eigentliche Zugschleife. Dadurch bleibt die „main“-Funktion sehr klein. Sie ist nicht für die Regeln oder die Darstellung zuständig. Genau diese Aufgabentrennung macht den Ablauf einfacher nachvollziehbar.

**REGIE:** Auf Game game, game.run und den catch-Block zeigen; danach mit der Funktionssuche zu Game::run wechseln.

**MERKSATZ:** main startet, Game steuert.

**KURZFASSUNG BEI ZEITDRUCK:** Der Programmstart übergibt die Kontrolle an die Klasse Game.

**CODESTELLE:** `src/main.cpp · Game::run()`


---

## B02 · Datenmodell und Feldgröße

**ZEIT:** 01:20–03:00  
**ZEIGEN:** include/Board.h → Board::Board

### Sprechtext

In „Board.h“ sehen wir den Zustand des Spielfelds. „rows_“ enthält die Anzahl der Reihen, „columns_“ die Anzahl der Spalten. Das Raster selbst liegt in „grid_“. Sein Typ ist „std::vector<std::vector<char>>“: Ein äußerer Vektor enthält mehrere innere Vektoren, und jedes Zeichen repräsentiert eine Zelle. Ein Punkt steht für ein leeres Feld.

Im Konstruktor wird die Größe aus der Spielerzahl berechnet. Die Formel für die Reihen lautet sechs plus zweimal die Anzahl der Spieler über zwei hinaus. Die Spaltenzahl ist immer um eins größer. Das ergibt sechs mal sieben, acht mal neun oder zehn mal elf Zellen. Wichtig für die nächsten Methoden: Die Indizes beginnen nicht bei eins, sondern bei null. Bei sechs Reihen besitzt die unterste Reihe daher den Index fünf.

**REGIE:** Im Editor rows_, columns_ und grid_ markieren; für drei Spieler die Rechnung 6+2×(3−2)=8 vormachen.

**MERKSATZ:** Ein zweidimensionaler Vektor bildet die Zellen ab.

**KURZFASSUNG BEI ZEITDRUCK:** Die Spielfeldgröße hängt vollständig von der Spielerzahl ab.

**CODESTELLE:** `include/Board.h · src/Board.cpp: Board::Board()`


---

## B03 · Schwerkraft, Grenzen und erfolgreicher Zug

**ZEIT:** 03:00–05:15  
**ZEIGEN:** Board::nextFreeRow → Board::drop

### Sprechtext

Die nächste zentrale Methode heißt „Board::nextFreeRow“. Sie erhält den Index einer Spalte. Zuerst prüft sie, ob der Index innerhalb des Spielfelds liegt. Anschließend läuft eine „for“-Schleife von „rows_ minus eins“ rückwärts bis zur obersten Reihe. Dadurch sucht sie von unten nach oben. Sobald sie eine Zelle mit dem Zeichen Punkt findet, gibt sie den Reihenindex zurück. Gibt es keine freie Position oder war die Spalte ungültig, liefert sie minus eins.

„Board::drop“ verwendet genau dieses Ergebnis. Ist keine Position frei, wird kein Stein eingetragen. Andernfalls schreibt die Funktion das Symbol in das Raster und gibt die tatsächlich verwendete Reihe über den Referenzparameter „rowPlaced“ zurück. Der Rückgabewert „bool“ zeigt zusätzlich, ob der Zug erfolgreich war. So bleiben die Schwerkraftregeln unabhängig davon, ob später eine Animation angezeigt wird.

**REGIE:** Optional Breakpoint bei return row setzen; konkret erklären: sichtbare Spalte 4 entspricht intern Index 3.

**MERKSATZ:** Erst gültige Position ermitteln, dann einsetzen.

**KURZFASSUNG BEI ZEITDRUCK:** nextFreeRow sucht unten; drop setzt nur bei Erfolg.

**CODESTELLE:** `src/Board.cpp: Board::nextFreeRow(), Board::drop()`


---

## B04 · Gewinnerkennung und Richtungsvektoren

**ZEIT:** 05:15–08:00  
**ZEIGEN:** Board::countDirection → Board::hasWon → winningCells

### Sprechtext

Das ist der wichtigste Algorithmus der Anwendung. Nach einem erfolgreichen Zug sind die genaue Reihe, Spalte und das Symbol bekannt. „Board::hasWon“ prüft von diesem letzten Stein aus, ob mindestens vier gleiche Symbole zusammenhängen.

Dazu verwendet die Methode zwei kleine Arrays mit Zeilen- und Spaltenänderungen: „dr“ und „dc“. Die Kombinationen stehen für waagerecht, senkrecht und beide Diagonalen. Für jede Achse wird „countDirection“ zweimal aufgerufen: einmal in die positive und einmal in die negative Richtung. Solange die nächste Position im Spielfeld liegt und dasselbe Symbol trägt, erhöht die Methode einen Zähler.

Die Länge einer zusammenhängenden Reihe ergibt sich aus eins für den zuletzt gesetzten Stein, plus den gleichen Nachbarn in beiden Richtungen. Ab einem Wert von vier ist die Partie gewonnen. Beide Richtungen sind notwendig, weil der letzte Stein auch in der Mitte einer Reihe liegen kann. Die Methode „winningCells“ liefert zusätzlich die zu markierenden Zellen für die Anzeige.

**REGIE:** Beispiel zeigen: zwei gleiche Steine links, letzter Stein, einer rechts = vier. Die Werte dr/dc einzeln nachvollziehen.

**MERKSATZ:** Vier Achsen, jeweils beide Richtungen, Summe mindestens vier.

**KURZFASSUNG BEI ZEITDRUCK:** 1 + links + rechts beziehungsweise oben + unten und beide Diagonalen.

**CODESTELLE:** `src/Board.cpp: countDirection(), hasWon(), winningCells()`


---

## B05 · Spielerwechsel, Undo und Zustände

**ZEIT:** 08:00–09:50  
**ZEIGEN:** Game::play · Move · history_

### Sprechtext

Die Klasse „Game“ verbindet die Eingaben mit dem Spielfeld. In „Game::play“ werden zunächst Befehle oder Spaltennummern ausgewertet. Nur ein gültiger Spaltenwert führt zum Einwerfen eines Steins. Nach dem erfolgreichen „Board::drop“ speichert das Programm einen Eintrag der Struktur „Move“ in der Zugliste „history_“. Ein Eintrag enthält Reihe, Spalte und den Index des Spielers.

Danach prüft das Programm zuerst auf Sieg und anschließend auf ein volles Spielfeld. Nur wenn die Partie weiterläuft, wechselt es zum nächsten Spieler. Die Formel lautet „(current_ + 1) % Spieleranzahl“. Durch den Modulo-Operator beginnt die Reihenfolge nach dem letzten Spieler wieder bei null.

Beim Undo holt die Steuerung den letzten Zug aus der Historie, entfernt den obersten Stein und setzt den ursprünglichen Spieler wieder an die Reihe. Dadurch bleiben Zugfolge und Spielfeld konsistent.

**REGIE:** Den Modulo-Fall für drei Spieler nennen: 0 → 1 → 2 → 0. Dann history_.back() zeigen.

**MERKSATZ:** Erst Zug prüfen, dann speichern, danach Ergebnis und Wechsel.

**KURZFASSUNG BEI ZEITDRUCK:** Ungültige Aktionen dürfen keinen Spielerwechsel auslösen.

**CODESTELLE:** `src/Game.cpp: Game::play() · include/Game.h: Move`


---

## B06 · Dateiformat und kontrolliertes Laden

**ZEIT:** 09:50–12:05  
**ZEIGEN:** Game::saveGame → Game::loadGame → gym::writeSafely

### Sprechtext

Zum Schluss der eigentlichen Spiellogik zeige ich das Speichern und Laden. Die Spielstanddatei beginnt mit der Kennung „GYM1“. Danach folgen unter anderem Spieleranzahl, aktueller Spieler, Dimensionen, Namen, Symbole, Zugliste und Raster. Dadurch enthält eine gespeicherte Partie genügend Informationen, um ihren Zustand zu rekonstruieren.

In „loadGame“ wird nicht direkt in die bestehende Partie geschrieben. Die Methode erzeugt ein neues Board als Kandidaten und kontrolliert zuerst die Metadaten. Dann spielt sie die gespeicherten Züge nacheinander durch und prüft, ob die tatsächlichen Landepositionen und das entstandene Raster zu den Daten passen. Erst wenn alles korrekt ist, ersetzt sie den bisherigen Spielzustand.

Beim Speichern nutzt der Entwicklungszweig die gemeinsame Funktion „gym::writeSafely“. Sie schreibt zunächst eine temporäre Datei und ersetzt die alte erst nach erfolgreichem Schreiben. Das begrenzt das Risiko eines Datenverlusts bei Schreibfehlern. Es koordiniert aber keine gleichzeitig laufenden Anwendungen.

**REGIE:** Die beiden getrennten Phasen markieren: Kandidat prüfen → Zustand übernehmen. Nicht den ganzen Parser lesen.

**MERKSATZ:** Sichere Datenübernahme statt blindem Dateilesen.

**KURZFASSUNG BEI ZEITDRUCK:** Beim Fehler bleibt die vorhandene Partie erhalten.

**CODESTELLE:** `src/Game.cpp: saveGame(), loadGame() · include/AtomicFile.h`


---

## B07 · Unit-Tests, Integrationstests und CI

**ZEIT:** 12:05–13:25  
**ZEIGEN:** tests/board_tests.cpp · CMakeLists.txt · Actions-Lauf

### Sprechtext

Die automatische Qualitätssicherung ist ein eigener Teil des Projekts. In den Board-Tests wird beispielsweise geprüft, ob eine Hantel auf dem untersten freien Feld landet, ob eine volle Spalte abgewiesen wird und ob jede Gewinnrichtung erkannt wird. Integrationstests kontrollieren zusätzlich längere Abläufe wie Spielstart, Rückgängig, Speichern und Laden.

CMake sorgt für reproduzierbare Build-Ziele; CTest startet die Testprogramme. GitHub Actions führt Prüfungen in verschiedenen Betriebssystemumgebungen aus. Ich zeige hier einen tatsächlich abgeschlossenen Prüflauf. Wichtig ist, präzise zu bleiben: Automatische Prüfungen ersetzen nicht den Bedienungs-, Grafik- und Hörtest am Schulrechner.

**REGIE:** In board_tests.cpp einen konkret lesbaren verify-Aufruf zeigen. Nur tatsächlich grüne Jobs als erfolgreich bezeichnen.

**MERKSATZ:** Testfall, Erwartung und beobachtetes Ergebnis gehören zusammen.

**KURZFASSUNG BEI ZEITDRUCK:** CMake baut, CTest testet, GitHub Actions automatisiert.

**CODESTELLE:** `tests/board_tests.cpp · CMakeLists.txt · .github/workflows/cpp-quality.yml`


---

## B08 · Zusammenfassung und fachlicher Abschluss

**ZEIT:** 13:25–15:00  
**ZEIGEN:** Zurück zu Game::play und Board::hasWon; keine neue Datei öffnen

### Sprechtext

Ich fasse den Weg eines einzelnen Spielzugs zusammen: Die Konsole liest eine Eingabe. „Game“ erkennt, ob es sich um eine zulässige Spaltennummer handelt. Das Board sucht die richtige Landeposition und setzt den Stein. Die Steuerung trägt den Zug in die Historie ein, prüft den möglichen Sieg oder das Unentschieden und wechselt gegebenenfalls zum nächsten Spieler. Anschließend zeigt die Oberfläche den neuen Zustand an.

Damit sind die Verantwortlichkeiten erkennbar getrennt: „Board“ entscheidet über die Regeln, „Game“ über die Abläufe und „ConsoleUI“ über die Textdarstellung. Eine weitere Verbesserung wäre eine gemeinsame Ablaufsteuerung auch für die grafischen Versionen. Wichtig ist mir, dass nicht nur die Oberfläche funktioniert, sondern dass ich die entscheidenden Funktionen und ihre Fehlerfälle direkt im Quellcode erklären kann. Vielen Dank. Ich beantworte gern Ihre Fragen.

**REGIE:** Beende spätestens nach 15 Minuten. Wenn Rückfragen in der Gesamtzeit enthalten sind, diese Karte stark verkürzen.

**MERKSATZ:** Eingabe → gültiger Zug → Siegprüfung → neuer Zustand.

**KURZFASSUNG BEI ZEITDRUCK:** Ein nachvollziehbarer Spielkern ist wichtiger als Effekte.

**CODESTELLE:** `src/Game.cpp: Game::play() · src/Board.cpp: hasWon()`


---

## Vortragskontrolle und verantwortliche Aussagen

- [ ] Pro Karte Anfang und Endzeit mit Stoppuhr geprüft; alle 15:00 Minuten eingehalten.
- [ ] Originalfolien, Bildschirmauflösung, Schriftgrößen und Übergänge am Präsentationsgerät überprüft.
- [ ] Funktionen tatsächlich vorgeführt oder ausdrücklich als noch nicht manuell getestet bezeichnet.
- [ ] CI-Nachweise nur für nachweisbar ausgeführte Jobs genannt: [C++-Prüflauf vom 31.10.2026](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/41221586099).
- [ ] Die Formulierungen zu KI-Unterstützung und eigener Arbeit sind sachlich zutreffend.
- [ ] Spielstände ausschließlich aus einem separaten Demo-Verzeichnis verwendet; Konsolenprogramm und GUI schreiben nicht gleichzeitig in denselben Datenordner.

**Nächster Schritt:** [Druckkarten](SPRECHKARTEN_DRUCKVERSION.html) · [Lehrerfragen](FACHGESPRAECH_KURZANTWORTEN.md) · [Generalprobe](../docs/GENERALPROBE_2027.md) · [Schulanforderungen](../docs/SCHULANFORDERUNGEN_51_STATUS.md).
