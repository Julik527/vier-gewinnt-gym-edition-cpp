# JK AXIS | FACHGESPRÄCH · 30 PRÜFUNGSFRAGEN

**Projekt:** Vier Gewinnt | Gym Edition · C++17 · ITA 2026/27  
**Ausgabe:** GOLD MASTER | Fachgesprächstraining · Revision 02.11.2026  
**Anforderung:** kurze, korrekte Antworten, die sich im tatsächlichen Quellcode belegen lassen.

## Lernmethode

**Selbsttest:** Frage zuerst laut beantworten, dann Kurzantwort lesen und die genannte Codestelle öffnen. Der Prüfende kann nach Begriffen, Randfällen oder Entscheidungen fragen. Deshalb sollen die Antworten verstanden und nicht nur wortwörtlich auswendig gelernt werden.

**Zeittakt:** Basisfragen 15–20 Sekunden, Algorithmusfragen 25–40 Sekunden. Bei Fragen zur Eigenleistung tatsächliche Beiträge und KI-Nutzung korrekt darstellen.


---

## Grundlagen und OOP

### F01 | Was macht die Funktion main()?

**Antwort:** Sie ist der Einstiegspunkt der Konsolenanwendung. Sie erzeugt ein Game-Objekt, ruft run() auf und fängt unerwartete Exceptions ab.

**Codebeleg:** `src/main.cpp`

**Achtung:** main enthält weder die Spielregeln noch eine eigene Spielfeldschleife.

### F02 | Was ist der Vorteil einer Klasse gegenüber einer großen Funktion?

**Antwort:** Eine Klasse kann zusammengehörige Daten und Methoden kapseln. Im Projekt verwaltet Board den Feldzustand und die Regeln, Game den Konsolenablauf.

**Codebeleg:** `include/Board.h · include/Game.h`

**Achtung:** Objektorientierung bedeutet nicht automatisch, dass jeder Programmteil eine eigene Klasse braucht.

### F03 | Welche Aufgabe hat Player?

**Antwort:** Player speichert Name und Symbol. Beide Werte werden über Getter gelesen; die Spielfeldlogik gehört nicht in diese Klasse.

**Codebeleg:** `include/Player.h`

**Achtung:** Die Highscore-Werte liegen nicht in Player.

### F04 | Warum sind rows_, columns_ und grid_ private?

**Antwort:** Andere Klassen sollen die internen Daten nicht beliebig verändern. Öffentliche Methoden definieren kontrollierte Lese- und Schreibzugriffe.

**Codebeleg:** `include/Board.h`

**Achtung:** Private allein schützt nicht vor jeder fehlerhaften Implementierung.

### F05 | Wie funktioniert der Board-Konstruktor?

**Antwort:** Er validiert die Spielerzahl, berechnet die Feldabmessungen und initialisiert alle Zellen als leer.

**Codebeleg:** `src/Board.cpp · Board::Board`

**Achtung:** 2 Spieler → 7×6, 3 → 9×8, 4 → 11×10.

### F06 | Was ist ein zweidimensionaler vector?

**Antwort:** std::vector<std::vector<char>> speichert mehrere Reihen aus Zeichen. Das passt zur in Abhängigkeit von der Spielerzahl festgelegten Spielfeldgröße.

**Codebeleg:** `include/Board.h`

**Achtung:** Äußerer Index = Reihe, innerer Index = Spalte.

### F07 | Wozu dient const nach einer Methode?

**Antwort:** Es legt fest, dass die Methode die nicht-mutable Member ihres Objekts nicht verändert. Eine Abfrage wie nextFreeRow darf den Feldzustand nicht ändern.

**Codebeleg:** `include/Board.h · nextFreeRow`

**Achtung:** const macht eine Methode nicht automatisch nebenläufigkeitssicher.

### F08 | Warum startest du intern bei Index null?

**Antwort:** C++-Container verwenden nullbasierte Indizes. Die sichtbare Spaltennummer 4 wird vor dem Zugriff in den internen Index 3 umgerechnet.

**Codebeleg:** `src/Game.cpp · Game::play`

**Achtung:** Sichtbare Zählung und interner Index dürfen nicht verwechselt werden.


---

## Spiellogik und Algorithmen

### F09 | Wie entsteht die Schwerkraft?

**Antwort:** nextFreeRow durchsucht die gewünschte Spalte vom größten Reihenindex nach oben und liefert die erste leere Zelle. drop setzt den Stein nur dort ein.

**Codebeleg:** `src/Board.cpp · nextFreeRow / drop`

**Achtung:** Bei sechs Reihen liegt die unterste Zelle auf Index 5.

### F10 | Warum liefert nextFreeRow bei Fehler minus eins?

**Antwort:** Gültige Reihenindizes beginnen bei null. Minus eins kann deshalb eindeutig für eine ungültige oder volle Spalte stehen.

**Codebeleg:** `src/Board.cpp · nextFreeRow`

**Achtung:** Der Aufrufer muss diesen Rückgabewert prüfen.

### F11 | Was ist der Referenzparameter rowPlaced?

**Antwort:** drop liefert bool für Erfolg/Misserfolg zurück und schreibt bei Erfolg die tatsächliche Zielreihe über int& rowPlaced in die Variable des Aufrufers.

**Codebeleg:** `src/Board.cpp · drop`

**Achtung:** Bei einem fehlgeschlagenen Zug ist die Zielreihe -1.

### F12 | Wann wird ein Spieler weitergeschaltet?

**Antwort:** Nur nach einem erfolgreich gesetzten Stein, der weder zu einem Sieg noch zu einem Remis führt. Ein abgewiesener Zug lässt current_ unverändert.

**Codebeleg:** `src/Game.cpp · Game::play`

**Achtung:** Vor dem Wechsel wird der Spielstatus geprüft.

### F13 | Was bedeutet der Modulo-Operator beim Spielerwechsel?

**Antwort:** (current_ + 1) % spielerzahl führt die Indizes zyklisch durch die Spieler. Bei drei Personen entsteht 0, 1, 2, 0.

**Codebeleg:** `src/Game.cpp · Game::play`

**Achtung:** Nicht durch eine feste Wenn-Abfrage für nur zwei Personen ersetzen.

### F14 | Wie erkennt das Programm vier in einer Reihe?

**Antwort:** hasWon zählt vom zuletzt gesetzten Stein entlang von vier Achsen. Für jede Achse summiert es gleiche Symbole in beiden Richtungen plus den Ausgangsstein.

**Codebeleg:** `src/Board.cpp · hasWon / countDirection`

**Achtung:** Die Länge muss mindestens vier betragen.

### F15 | Warum sind für vier Achsen acht Suchrichtungen nötig?

**Antwort:** Jede Achse besitzt eine Vorwärts- und eine Rückwärtsrichtung. Nur so erkennt man eine Reihe, deren zuletzt gesetzter Stein in der Mitte liegt.

**Codebeleg:** `src/Board.cpp · hasWon`

**Achtung:** Horizontal ist nicht dasselbe wie nur nach rechts suchen.

### F16 | Wie weiß die Oberfläche, welche vier Steine gewonnen haben?

**Antwort:** winningCells liefert die Koordinaten der zusammenhängenden Gewinnlinie. Die Darstellung kann diese Zellen anschließend gesondert hervorheben.

**Codebeleg:** `src/Board.cpp · winningCells`

**Achtung:** Bei einer längeren Linie können mehr als vier Zellen markiert werden.

### F17 | Was passiert bei einem vollen Spielfeld?

**Antwort:** Nach einem gültigen Zug prüft die Steuerung zuerst auf Sieg und dann auf isFull(). Ist das Brett vollständig belegt und niemand hat gewonnen, endet die Runde als Remis.

**Codebeleg:** `src/Game.cpp · Game::play`

**Achtung:** Ein volles Feld allein bedeutet nicht zwingend Remis, wenn zugleich gewonnen wurde.

### F18 | Wie funktioniert Undo?

**Antwort:** Die Historie enthält pro Zug Reihe, Spalte und Spieler. undo entfernt den zuletzt gesetzten, obersten Stein und aktiviert wieder den Spieler dieses Zugs.

**Codebeleg:** `include/Game.h · src/Game.cpp · Board::undo`

**Achtung:** Die aktuelle Implementierung nimmt während einer aktiven Runde zurück, nicht nach einem bereits abgeschlossenen Sieg.


---

## Dateien, Fehler und Persistenz

### F19 | Welche Daten enthält das Format GYM1?

**Antwort:** Kennung, Spielerzahl, nächster Spieler, Feldgröße, Zuganzahl, Namen, Symbole, Zugliste und Raster. Damit kann der Loader eine laufende Runde rekonstruieren.

**Codebeleg:** `src/Game.cpp · saveGame`

**Achtung:** Nicht behaupten, dass die Rundennummer gespeichert wird; sie wird beim Laden zurückgesetzt.

### F20 | Warum spielt der Loader die Züge erneut nach?

**Antwort:** Das Kandidaten-Brett überprüft Schwerkraft, Reihenfolge und das tatsächlich entstehende Raster. Erst danach wird der echte Zustand ersetzt.

**Codebeleg:** `src/Game.cpp · loadGame`

**Achtung:** Eine bloße Zeichentabelle wäre leichter manipulierbar.

### F21 | Was geschieht bei einer beschädigten Savegame-Datei?

**Antwort:** loadGame liefert false und übernimmt die Kandidatendaten nicht. Eine bereits aktive Partie bleibt daher unverändert.

**Codebeleg:** `src/Game.cpp · loadGame`

**Achtung:** Auch überflüssige Daten am Datei-Ende werden zurückgewiesen.

### F22 | Wie schützt writeSafely die bisherige Datei?

**Antwort:** Der neue Inhalt wird in eine separate temporäre Datei geschrieben. Erst nach erfolgreichem Schreiben erfolgt der Austausch mit der Zieldatei.

**Codebeleg:** `include/AtomicFile.h`

**Achtung:** Das verhindert nicht alle Systemausfälle und ist kein Lock gegen mehrere Prozesse.

### F23 | Warum darf ein Highscore nicht doppelt gezählt werden?

**Antwort:** Ein beendeter Gewinn darf nur einen Zählerzuwachs auslösen. Die grafische MatchSession kann einen gescheiterten Dateischreibversuch wiederholen, ohne denselben Sieg erneut zu vergeben.

**Codebeleg:** `src/MatchSession.cpp · retryPendingScoreSave`

**Achtung:** Ein Retry speichert vorhandene Daten, er erzeugt keinen neuen Sieg.

### F24 | Wie verhindert das Programm fehlerhafte Namen?

**Antwort:** Es lehnt leere oder zu lange Namen und Steuerzeichen ab; beim Einrichten werden doppelte Namen innerhalb einer Partie verhindert.

**Codebeleg:** `src/Game.cpp · validName / configureNames`

**Achtung:** Die Länge wird in Bytes geprüft, nicht als vollständige Unicode-Zeichenzahl.


---

## Qualität, Grafik und Fachgespräch

### F25 | Was ist der Unterschied zwischen Unit- und Integrationstest?

**Antwort:** Ein Unit-Test prüft eine überschaubare Funktion oder Klasse, zum Beispiel Board::hasWon. Ein Integrationstest prüft das Zusammenspiel mehrerer Teile beim Spielstart oder Speichern.

**Codebeleg:** `tests/board_tests.cpp · tests/integration_smoke.py`

**Achtung:** Beide Testarten sind notwendig, aber nicht identisch.

### F26 | Welche Rolle spielen CMake, CTest und GitHub Actions?

**Antwort:** CMake beschreibt Builds, CTest führt registrierte Tests aus und GitHub Actions startet definierte Prüfabläufe auf externen Systemen.

**Codebeleg:** `CMakeLists.txt · .github/workflows/cpp-quality.yml`

**Achtung:** Ein erfolgreicher Build ist noch kein vollständiger Bedienungstest.

### F27 | Was unterscheidet Game von MatchSession?

**Antwort:** Game steuert die Konsole. MatchSession verwaltet die Spielzustände für 2D und 3D. Beide verwenden dieselbe Board-Klasse, sind aber noch keine gemeinsame Ablaufsteuerung.

**Codebeleg:** `include/Game.h · include/MatchSession.h`

**Achtung:** Nicht behaupten, die gesamte Spiellogik sei bereits vereinheitlicht.

### F28 | Wie wird in 3D die getroffene Spalte berechnet?

**Antwort:** Aus dem Pixel und der Kamera wird ein Strahl berechnet. Dessen Schnittpunkt mit der Brettebene liefert eine Weltkoordinate, aus der die Spalte abgeleitet wird.

**Codebeleg:** `src/gui/Gym3DPicking.h · pickColumn`

**Achtung:** Die 3D-Funktion ist freiwillig und verlangt einen passenden Grafiktest.

### F29 | Warum werden während der Fallanimation weitere Eingaben blockiert?

**Antwort:** Die visuelle Bewegung ist noch nicht abgeschlossen. Ohne Sperre könnten mehrere Klicks unerwünschte Spielzüge auslösen, bevor der aktuelle Stein übernommen wurde.

**Codebeleg:** `src/gui/Main3D.cpp · startDrop`

**Achtung:** Rendering und endgültige Spielzustandsänderung sind getrennte Schritte.

### F30 | Wie gehst du mit KI-Unterstützung bei der Präsentation um?

**Antwort:** Ich benenne KI-Unterstützung bei Entwurf, Codeüberarbeitung und Dokumentation zutreffend. Für die Prüfung erkläre ich nur Abläufe, die ich tatsächlich selbst nachvollzogen und getestet habe.

**Codebeleg:** `docs/KI_PROMPT_DOKUMENTATION.md`

**Achtung:** Keine unbelegte Aussage über vollständig eigenständige Implementierung machen.

### F31 | Was würdest du im nächsten Entwicklungsschritt verbessern?

**Antwort:** Ich würde zuerst die manuellen Schul-PC-Tests abschließen. Danach wäre die Vereinheitlichung der Controller und die Aufteilung großer GUI-Dateien sinnvoller als zusätzliche Effekte.

**Codebeleg:** `docs/RELEASE_CHECKLIST.md · docs/ARCHITECTURE.md`

**Achtung:** Priorität: funktionsfähige Pflicht-Konsole und nachvollziehbarer Code.

---

## Drei kurze mündliche Übungssituationen

**Situation A · Zwei Minuten:** Starte bei `Game::play` und erkläre Eingabeprüfung → `drop` → `hasWon` → Spielerwechsel. Zeige jeden Schritt in der passenden Datei.

**Situation B · Zwei Minuten:** Erläutere anhand einer manipulierten `GYM1`-Datei, weshalb erst ein Kandidaten-Board geprüft wird. Dateien nicht an persönlichen Spielständen verändern.

**Situation C · Drei Minuten:** Beantworte F14, F19, F25, F27 und F30 frei. Ein selbst überprüfter CI-Link darf gezeigt werden; offene Handtests sind als offen zu benennen.

## Selbstbewertung

| Kriterium | 0 | 1 | 2 |
|---|---|---|---|
| Fachbegriff korrekt? | falsch | teilweise | präzise |
| Beispiel aus dem Projekt? | keines | allgemein | konkret |
| Codebeleg schnell gezeigt? | nein | mit Hilfe | selbstständig |
| Grenzen ehrlich benannt? | nein | teilweise | vollständig |

**Interpretation:** 7–8 Punkte je Übungsfrage sind das Trainingsziel, keine prognostizierte Schulnote.

**Weitere Materialien:** [Karten A](SPRECHKARTEN_TEIL_A_PROJEKT.md) · [Karten B](SPRECHKARTEN_TEIL_B_CODE.md) · [Prüfungs-Checkliste](PRUEFUNG_CHECKLISTE.md) · [Schulanforderungen](../docs/SCHULANFORDERUNGEN_51_STATUS.md).
