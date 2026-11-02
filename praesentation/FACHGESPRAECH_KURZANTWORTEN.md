# Fachgespräch · Kurzantworten für die Prüfung

**Projekt:** Vier Gewinnt | Gym Edition · C++17  
**Fassung:** JK AXIS | GOLD MASTER · 02.11.2026  
**Ziel:** Ohne auswendig gelernte Floskeln in 10–30 Sekunden fachlich korrekt antworten und anschließend den Quellcode zeigen.

> **Übung:** Erst die Frage lesen, dann **ohne auf die Antwort zu schauen** mündlich beantworten. Anschließend die Musterantwort vergleichen und die genannte Quellcode-Stelle öffnen. Diese Antworten sind Hilfen zum Lernen und kein Nachweis bereits vorhandener Eigenleistung.
>
> **Wichtig:** Die Pflicht-Konsole zuerst beherrschen. Bei Grafik-/Audiofragen zwischen implementiertem Code, automatischem Build-Test und offener Handprüfung auf dem Zielrechner unterscheiden.

## F01 · Warum ist das Projekt objektorientiert?

**Kurzantwort:** Player kapselt Name/Symbol, Board den Rasterzustand und Regeln, Game die Konsolensteuerung. Jede Klasse hat eine nachvollziehbare Hauptverantwortung.

**Im Code zeigen:** `include/Player.h · include/Board.h · include/Game.h`

## F02 · Wieso verwendest du vector<vector<char>>?

**Kurzantwort:** Die Zahl der Zeilen und Spalten hängt von der Spielerzahl ab. Ein zweidimensionaler vector erlaubt passende Größen; ein char pro Zelle genügt für Punkt oder Spielsymbol.

**Im Code zeigen:** `include/Board.h · Board::Board`

## F03 · Was bedeuten private und public?

**Kurzantwort:** Private Member können von außen nicht unmittelbar verändert werden. Public Methoden definieren, welche kontrollierten Aktionen mit einem Objekt möglich sind.

**Im Code zeigen:** `include/Board.h`

## F04 · Was ist die Aufgabe des Konstruktors?

**Kurzantwort:** Er setzt einen gültigen Ausgangszustand, berechnet Feldabmessungen und initialisiert alle Zellen als leer.

**Im Code zeigen:** `Board::Board`

## F05 · Warum verwendest du rows_ - 1 beim Einwerfen?

**Kurzantwort:** Die Rasterindizes beginnen oben bei null. Die unterste Zeile hat deshalb den höchsten Index. Von dort aus wird das erste freie Feld gesucht.

**Im Code zeigen:** `Board::nextFreeRow`

## F06 · Warum bedeutet -1 bei nextFreeRow ein Fehler?

**Kurzantwort:** Jeder gültige Zeilenindex liegt zwischen null und rows_-1. Minus eins ist daher ein eindeutiger Rückgabewert für ungültige oder volle Spalten.

**Im Code zeigen:** `Board::nextFreeRow`

## F07 · Warum ist rowPlaced eine Referenz?

**Kurzantwort:** drop gibt per bool an, ob das Setzen erfolgreich war. Der Referenzparameter liefert zusätzlich die tatsächliche Zeile an den Aufrufer zurück.

**Im Code zeigen:** `Board::drop`

## F08 · Was bedeutet const bei einer Methode?

**Kurzantwort:** Eine const-Methode darf den beobachtbaren Objektzustand nicht verändern; das ist z. B. bei der Suche nach der freien Zeile sinnvoll.

**Im Code zeigen:** `Board::nextFreeRow`

## F09 · Was ist Modulo beim Spielerwechsel?

**Kurzantwort:** (current_ + 1) % spielerzahl lässt die Indizes zyklisch von null bis spielerzahl-1 laufen und nach dem letzten Spieler wieder bei null starten.

**Im Code zeigen:** `Game::play`

## F10 · Warum werden vier Achsen, aber acht Richtungen geprüft?

**Kurzantwort:** Horizontal, vertikal und beide Diagonalen sind vier Achsen. Jeweils vorwärts und rückwärts gezählt wird die gesamte zusammenhängende Linie um den letzten Zug erkannt.

**Im Code zeigen:** `Board::hasWon · Board::countDirection`

## F11 · Was passiert, wenn eine Spalte voll ist?

**Kurzantwort:** nextFreeRow liefert -1. Der Zug wird abgewiesen; Zugliste, Spielerwechsel und Raster bleiben unverändert.

**Im Code zeigen:** `Board::nextFreeRow · Game::play`

## F12 · Wie funktioniert Undo?

**Kurzantwort:** Die Historie speichert Zeile, Spalte und Spielerindex. Beim Rückgängig-Machen wird der letzte zulässige Stein entfernt und der ursprüngliche Spieler erneut aktiviert.

**Im Code zeigen:** `Game.h (Move) · Game::play`

## F13 · Warum reicht es nicht, das gespeicherte Raster direkt einzulesen?

**Kurzantwort:** Eine fehlerhafte oder manipulierte Datei könnte unzulässige Zustände enthalten. Der Loader spielt die Züge auf einem Kandidatenfeld nach und übernimmt erst vollständig geprüfte Daten.

**Im Code zeigen:** `Game::loadGame · MatchSession::load`

## F14 · Was macht eine temporäre Datei beim Speichern besser?

**Kurzantwort:** Der neue Inhalt wird zuerst vollständig geschrieben. Erst dann ersetzt die Anwendung die alte Datei. Bei typischen Schreibfehlern bleibt so die vorherige Version erhalten; parallele Prozesse werden nicht automatisch synchronisiert.

**Im Code zeigen:** `include/AtomicFile.h`

## F15 · Welche Tests gibt es?

**Kurzantwort:** Board-Unit-Tests und weitere Integrationstests für Regeln, Spielstart, Speichern/Laden, Undo und Highscore. Im Entwicklungsbranch zusätzlich GUI-Kern- und 3D-Geometrietests.

**Im Code zeigen:** `tests/ · CMakeLists.txt`

## F16 · Was beweist ein grüner GitHub-Actions-Lauf nicht?

**Kurzantwort:** Er beweist weder, dass die Vorführung am konkreten Schulrechner funktioniert, noch dass Grafik, Sound und Präsentationskompetenz manuell abgenommen sind.

**Im Code zeigen:** `docs/3D_TESTPLAN.md · docs/RELEASE_CHECKLIST.md`

## F17 · Wie unterscheiden sich Game und MatchSession?

**Kurzantwort:** Game steuert die Pflicht-Konsole. MatchSession verwaltet die Spielzustände der grafischen Versionen. Gemeinsam ist die Board-Regelklasse; beide Ablaufsteuerungen sind noch nicht vereinheitlicht.

**Im Code zeigen:** `include/Game.h · include/MatchSession.h`

## F18 · Wie funktioniert die 3D-Mausauswahl?

**Kurzantwort:** Die Klickposition wird mit der Kameraposition in einen Strahl umgerechnet. Dessen Schnittpunkt mit der Brettebene liefert die passende Welt-x-Koordinate und damit die Spalte.

**Im Code zeigen:** `src/gui/Gym3DPicking.h`

## F19 · Warum blockierst du während einer Fallanimation weitere Züge?

**Kurzantwort:** Die grafische Darstellung darf nicht mehrere Spielzüge auslösen, während der aktuelle Stein noch unterwegs ist. Erst nach Animationsabschluss wird der Zustand aktualisiert.

**Im Code zeigen:** `src/gui/Main.cpp · src/gui/Main3D.cpp`

## F20 · Welche Rolle spielte KI und was ist dein Anteil?

**Kurzantwort:** KI-gestützte Entwürfe, Überarbeitungen und Dokumentation offen nennen. Eigene Leistung nur insoweit behaupten, wie sie wirklich erbracht wurde; zentrale Funktionen selbst erläutern und testen können.

**Im Code zeigen:** `docs/KI_PROMPT_DOKUMENTATION.md`

---

## Übungen für die Generalprobe

1. Die ersten zehn Fragen in zufälliger Reihenfolge **frei** beantworten.
2. `Board::nextFreeRow`, `Board::drop` und `Board::hasWon` mit einem Beispiel im Debugger erklären.
3. Einen ungültigen Zug vorführen und zeigen, dass der aktuelle Spieler erhalten bleibt.
4. Einen vorbereiteten Gewinnstand ausschließlich im Demo-Datenordner laden.
5. Ein beschädigtes GYM1-Savegame erklären: Warum wird es zurückgewiesen?
6. Offen sagen, welche Funktionen noch **nicht manuell** auf dem Schul-PC geprüft wurden.
7. Eine dreiminütige Fragerunde mit Stoppuhr durchführen.

**Verweise:** [Sprechkarten A](SPRECHKARTEN_TEIL_A_PROJEKT.md) · [Sprechkarten B](SPRECHKARTEN_TEIL_B_CODE.md) · [Vortragsleitfaden](VORTRAGSLEITFADEN.md) · [Generalprobe](../docs/GENERALPROBE_2027.md)
