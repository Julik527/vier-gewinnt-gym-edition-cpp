# Meilenstein-Präsentation | Vier Gewinnt | Gym Edition | 10 Minuten

**Stand: 11.11.2026.** Prüfungsgrundlage: E-Mail von Dr. Michael Litera, 11.11.2026, „Klausur am 19.11.2026“; **PP: Einzelpräsentationen ca. 10 Minuten pro Schüler**. Diese zehnminütige Prüfungsvariante **ersetzt für den 19.11.2026 den bisherigen Ablauf von 15 + 15 Minuten**. Der ältere 30-Minuten-Vortrag bleibt als anderes Präsentationsformat archiviert. **Hinweis:** Der folgende Plan ist minutengenau verteilt, aber die tatsächliche Sprechdauer muss mit Stoppuhr bestätigt werden.

## Exakter Zeitplan (Ziel 10:00)

| Folie | Zeit | Sekunden | Inhalt / notwendiger Nachweis |
|---|---|---:|---|
| 01 | 00:00–00:40 | 40 | Ziel, C++17, Pflicht-Konsole, aktueller Meilenstein |
| 02 | 00:40–01:50 | 70 | Anforderungen 2–4 Personen, Feldgrößen, Schwerkraft |
| 03 | 01:50–03:10 | 80 | OOP: `Player`, `Board`, `Game`, `ConsoleUI` |
| 04 | 03:10–04:40 | 90 | `nextFreeRow` und `drop`, Referenzparameter |
| 05 | 04:40–06:10 | 90 | `hasWon`, vier Achsen × zwei Richtungen |
| 06 | 06:10–07:15 | 65 | `GYM1`, Validierung, temporäre Datei |
| 07 | 07:15–08:50 | 95 | Pflichtkonsole: Eingabe, Vorschau, Stein, Undo |
| 08 | 08:50–10:00 | 70 | CI-Status, Risiken, offene Abnahme, nächster Schritt |
| **Gesamt** | **00:00–10:00** | **600** | **Zielzeit vor Generalprobe** |

## Sprechregie, fachlich verbindlich an den Code gebunden

### 01 | Einstieg
„Guten Tag. Ich stelle den aktuellen Meilenstein meines C++17-Projekts *Vier Gewinnt | Gym Edition* vor. Der schulische Kern ist ein objektorientiertes Konsolenspiel für zwei bis vier Personen. Mir geht es nicht allein um Gestaltung, sondern um verständliche Regeln, Datensicherheit und überprüfbare Programmteile. Ich erläutere kurz den Aufbau, die zwei zentralen Algorithmen und die Nachweise.“

### 02 | Anforderungen
„Das Feld wächst mit der Spielerzahl: zwei Personen spielen auf sechs mal sieben, drei auf acht mal neun, vier auf zehn mal elf Zellen. Jeder Stein fällt in der gewählten Spalte bis zum untersten freien Feld. Vier gleiche Symbole in einer Reihe bilden einen Gewinn. Ungültige Eingaben und volle Spalten dürfen keinen Stein setzen. Hinzu kommen Undo, Spielstand und Highscore. Die grafischen Varianten sind freiwillig, nicht Voraussetzung für die Pflichtabgabe.“

### 03 | Klassen und Dateien
„`Player` speichert Name und Symbol. `Board` enthält das Raster, Schwerkraft und Gewinnerkennung. `Game` koordiniert die Konsole, die Spielerreihenfolge und Menüfunktionen. `ConsoleUI` ist für die Textdarstellung zuständig. Das Hauptprogramm erzeugt ein `Game`-Objekt und ruft `run()` auf. Die Schnittstelle von Board steht in `include/Board.h`, die Methoden in `src/Board.cpp`. So kann ich den Regelkern separat testen.“

### 04 | Schwerkraft
„`Board::nextFreeRow` kontrolliert zuerst die Spaltengrenzen. Dann sucht eine Schleife von der letzten Reihe rückwärts bis zur ersten nach einer freien Zelle `'.'`. Bei einer vollen oder unzulässigen Spalte kommt `-1` zurück. `Board::drop` nutzt diesen Wert, setzt bei Erfolg genau einen Stein und schreibt die Landereihe über `int& rowPlaced` an den Aufrufer zurück. Das ist ein konkreter C++-Referenzparameter.“

### 05 | Gewinnprüfung
„`Board::hasWon` startet beim letzten Stein. Vier Achsen werden untersucht: horizontal, vertikal und beide Diagonalen. Für jede Achse zählt `countDirection` die gleichen Steine in beiden Gegenrichtungen. Hinzu kommt eins für den neuen Stein. Ist die Summe mindestens vier, liegt ein Gewinn vor. Beide Richtungen sind nötig, weil der zuletzt gesetzte Stein in der Mitte einer Reihe liegen kann.“

### 06 | Dateioperationen
„Ein Spielstand beginnt mit `GYM1`. Laden bedeutet nicht, beliebige Werte direkt in eine laufende Partie zu schreiben. Erst erstellt das Programm ein Kandidatenboard, prüft die Daten und spielt die Züge nach. Nur ein konsistenter Kandidat ersetzt den aktuellen Zustand. Beim Speichern wird über `gym::writeSafely` erst eine temporäre Datei geschrieben. Das verbessert den Fehlerschutz, garantiert aber keine vollständige Mehrprozess-Transaktion.“

### 07 | Vorführung
„Ich zeige die bereits vorab getestete Konsolenfassung. Zuerst wähle ich ein neues Spiel für zwei Personen. Danach demonstriere ich eine Vorschau mit `V 4`, einen regulären Zug in Spalte 4 und das Rückgängigmachen mit `U`. Die Vorschau darf den Zustand nicht verändern, der echte Zug verändert eine Zelle, Undo stellt den vorherigen Zustand wieder her. Ich verwende nur einen separaten Demo-Datenordner. Wird die Vorführung technisch gestört, erkläre ich den Ablauf anhand des Codes und der Tests.“

### 08 | Teststand und Abschluss
„Auf dem Hauptbranch gab es einen erfolgreichen C++-Qualitätslauf. Für die getrennte Schul-Konsolenversion in PR #14 liefen vier Build- und Testkonfigurationen erfolgreich auf Windows und Linux. Beim GUI-Refactoring-PR #13 scheiterte der erste Linux-GUI-Job am 11.11. wegen einer DNS-Störung bei der Ubuntu-Paketinstallation; die anderen neun Jobs waren erfolgreich. Die Wiederholung des fehlgeschlagenen Jobs wurde erfolgreich abgeschlossen; damit ist der gesamte betreffende C++-CI-Lauf erfolgreich. Ein grüner CI-Test ersetzt keine echte Schul-PC-Generalprobe. Meine nächsten Schritte sind eine gemessene zehnminütige Probe und die eigene Codeerklärung.“

## Bedieneinsätze und exakte Stoppmarken

- **00:00:** Titelfolie bereits offen, Programm und Editor vorgestartet.
- **01:50:** Architekturfolie; keine dritte Bildschirm-App öffnen.
- **03:10:** Codefenster `src/Board.cpp`, Methode `nextFreeRow` zeigen.
- **04:40:** `Board::hasWon` markieren, die vier Richtungen erklären.
- **07:15:** Vorbereitete Konsole zeigen; **keinen neuen Build** anstoßen.
- **08:50:** Demo beenden, Statusfolie sichtbar.
- **10:00:** Vortrag beendet; Rückfragen nur zusätzlich, wenn die Lehrkraft sie einräumt.

## Abnahmekriterien vor der Präsentation

- [ ] Drei tatsächliche Stoppuhr-Durchläufe laut gesprochen; Zeiten pro Folie dokumentiert.
- [ ] Vortrag wirklich ungefähr 10 Minuten, nicht 15+15 Minuten.
- [ ] Der Desktop hat die Konsolenversion, Quellcode und Folien offline griffbereit.
- [ ] Auf dem Schul-PC wurden die Pflichteingaben **persönlich** geprüft.
- [ ] Die Demo verändert keine echten Savegames; Beispielstände sind isoliert.
- [ ] Alle genannten Methoden existieren im tatsächlich gezeigten Commit.
- [ ] CI-Status und offene Fehler auf dem Präsentationstag neu geprüft.
- [ ] KI-Unterstützung korrekt benannt; keine erfundene Eigenleistung.
- [ ] 2D/3D ausdrücklich freiwillig; im Prüfungstermin keine riskante Live-GUI-Demo.

**Quellen:** Lehrer-E-Mail 11.11.2026; Repository `Julik527/vier-gewinnt-gym-edition-cpp`, `include/Board.h`, `src/Board.cpp`, `src/Game.cpp`, `praesentation/VORTRAGSLEITFADEN.md`; CI-Läufe `42038424974`, `43062599385`, `43058713971`. Zeitplan und Vortragstext sind neue Lern- und Vorbereitungsunterlagen. Keine reale Probe, keine Schulnote und keine Lehrkraftfreigabe behauptet.
