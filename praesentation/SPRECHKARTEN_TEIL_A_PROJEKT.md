# Sprechkarten Teil A: Projektvorstellung

**Projekt:** Vier Gewinnt | Gym Edition · C++17 · ITA 2026/27  
**Fassung:** Präsentationstraining · Revision 02.11.2026  
**Basis:** Entwicklungsbranch `quality/gameplay-persistence-2026-10-28`, geprüfter Quellcodestand 31.10.2026  
**Vortragsziel:** 15:00 Minuten für Teil A inklusive der angegebenen Aktionen. Die tatsächliche Redezeit ist noch zu messen.

> **Zum Ablesen:** Nur die Absätze unter **Sprechtext** gehören zum Vortrag. Die übrigen Zeilen sind Regiehinweise. Vor der Abgabe den Text dem tatsächlich gezeigten Programm und deiner eigenen Tätigkeit anpassen. Fachbegriffe lieber korrekt und langsam erklären als hastig sämtliche Sätze abzulesen.

### Zeitregie

| Karte | Thema | Zeitfenster | Endmarke |
|:--:|---|---:|---:|
| A01 | Begrüßung und Leitidee | 00:00–01:20 | 01:20 |
| A02 | Spielregeln und Anforderungen | 01:20–03:05 | 03:05 |
| A03 | Menü, Zugsteuerung und Spielkomfort | 03:05–04:50 | 04:50 |
| A04 | Objektorientierte Architektur | 04:50–06:40 | 06:40 |
| A05 | Konsolenversion, 2D und 3D | 06:40–08:30 | 08:30 |
| A06 | Spielstand, Highscore und Datensicherheit | 08:30–10:20 | 10:20 |
| A07 | Tests, GitHub und verantwortliche Entwicklung | 10:20–12:20 | 12:20 |
| A08 | Kurze Vorführung und Übergabe an den Code | 12:20–15:00 | 15:00 |

**Prüfungsvariante mit Fragen innerhalb von 30 Minuten:** Teil A und B jeweils auf ca. 13:30 Minuten kürzen und insgesamt 3 Minuten für das Fachgespräch reservieren. Dazu in A08 die Zusatzdemo streichen und B08 deutlich verkürzen. Beide Zeitpläne können nicht gleichzeitig vollständig ausgeführt werden.

---

## A01 · Begrüßung und Leitidee

**ZEIT:** 00:00–01:20  
**ZEIGEN:** Titelfolie · Projektname

### Sprechtext

Guten Tag. Ich stelle heute mein C++-Projekt „Vier Gewinnt | Gym Edition“ vor. Es basiert auf dem bekannten Vier-Gewinnt-Prinzip und verbindet die klassischen Spielregeln mit einem eigenen Fitness-Design: Statt normaler Spielsteine erscheinen Hantelsymbole. Mein Ziel war allerdings nicht nur eine andere Optik. Ich wollte ein vollständiges, verständlich aufgebautes Programm erstellen, bei dem Spielregeln, Bedienung, Datenspeicherung und Fehlerbehandlung zusammenarbeiten.

Der verbindliche Kern ist eine objektorientierte Konsolenanwendung in C++17. Zusätzlich gibt es im Entwicklungszweig eine 2D-Grafikversion und eine freiwillige 3D-Erweiterung. Im ersten Teil erkläre ich die Anforderungen, die Bedienung, den Aufbau und die Qualitätssicherung. Anschließend öffne ich den tatsächlichen Quellcode und zeige, wie ein Spielzug verarbeitet und ein Sieg erkannt wird.

**REGIE:** Blickkontakt nach dem ersten Satz; erst danach zur Projektübersicht wechseln.

**MERKSATZ:** Regeln zuerst, Gestaltung als Erweiterung.

**KURZFASSUNG BEI ZEITDRUCK:** Ein C++17-Vier-Gewinnt mit eigenem Design und nachvollziehbarer Spiellogik.


---

## A02 · Spielregeln und Anforderungen

**ZEIT:** 01:20–03:05  
**ZEIGEN:** Regeln · 7×6 / 9×8 / 11×10

### Sprechtext

Das Spiel unterstützt zwei, drei oder vier Personen. Je nach Spieleranzahl ändert sich das Spielfeld: Bei zwei Personen sind es sieben Spalten und sechs Reihen, bei drei Personen neun Spalten und acht Reihen und bei vier Personen elf Spalten und zehn Reihen. Vor dem Spiel werden die Anzahl und die Namen der Mitspielenden festgelegt.

Die Spieler setzen ihre Hanteln abwechselnd in eine Spalte. Ein Stein fällt nicht auf eine frei gewählte Position, sondern landet auf dem untersten freien Feld dieser Spalte. Gewonnen hat, wer vier eigene Symbole ohne Unterbrechung waagerecht, senkrecht oder diagonal verbindet. Eine volle Spalte darf keinen weiteren Stein annehmen; ebenso müssen ungültige Eingaben abgefangen werden. Ist das komplette Feld belegt und niemand hat gewonnen, endet die Runde unentschieden. Genau diese Regeln stehen im Zentrum der schulischen Pflichtfassung.

**REGIE:** Die drei Feldgrößen klar unterscheiden; die vier Gewinnrichtungen sichtbar zeigen.

**MERKSATZ:** Zwei bis vier Spieler, Schwerkraft und vier Gewinnachsen.

**KURZFASSUNG BEI ZEITDRUCK:** Die Regeln sind unabhängig von der späteren grafischen Darstellung.


---

## A03 · Menü, Zugsteuerung und Spielkomfort

**ZEIT:** 03:05–04:50  
**ZEIGEN:** Konsolenmenü oder Arena-Screenshot

### Sprechtext

Beim Start führt das Programm durch zwei einfache Schritte: Zuerst wird die Spielerzahl gewählt und danach werden die Namen nacheinander eingegeben. Im Hauptmenü gibt es unter anderem „Neues Spiel“, „Fortsetzen“, „Spielstand laden“, „Bestenliste“ und „Anleitung“.

Während einer Partie reicht eine Spaltennummer für einen normalen Zug. Zusätzlich kann man „V 4“ eingeben. Dadurch erscheint eine Vorschau für Spalte vier, ohne dass ein Stein tatsächlich gesetzt wird. Mit „U“ nimmt man den letzten Zug zurück, mit „S“ speichert man und mit „M“ kehrt man ins Hauptmenü zurück, ohne die aktive Runde automatisch zu verlieren. Nach einem Sieg oder einem Remis folgt ein eigener Ergebnisbildschirm mit Revanche und Neustart. So bleibt das Spiel auch bei Unterbrechungen nachvollziehbar. Besonders wichtig ist dabei, dass eine ungültige Aktion den Spielstand nicht unbemerkt verändert.

**REGIE:** Vorschau und Undo nur dann live demonstrieren, wenn das Programm bereits geprüft und gestartet ist.

**MERKSATZ:** Jede Aktion verändert den Zustand nur, wenn sie gültig ist.

**KURZFASSUNG BEI ZEITDRUCK:** Die Bedienung ist bewusst kurz und fehlertolerant.


---

## A04 · Objektorientierte Architektur

**ZEIT:** 04:50–06:40  
**ZEIGEN:** UML oder Projektstruktur · Board, Game, Player, ConsoleUI

### Sprechtext

Die Architektur ist objektorientiert, weil verschiedene Klassen unterschiedliche Verantwortlichkeiten besitzen. „Player“ speichert den Namen und das Symbol eines Spielers. „Board“ verwaltet die Zellen des Spielfelds und die grundlegenden Regeln. „Game“ koordiniert in der Konsole das Menü, den Spielerwechsel und den Spielverlauf. „ConsoleUI“ bereitet die Informationen so auf, dass sie im Terminal lesbar angezeigt werden.

Die grafischen Anwendungen verwenden dagegen einen eigenen Ablauf-Controller namens „MatchSession“. Das ist eine wichtige Unterscheidung: Konsole und Grafik haben noch nicht dieselbe gesamte Ablaufsteuerung, aber sie nutzen dieselbe Klasse „Board“ für die Spielregeln. Dadurch muss ich die Gewinnerkennung nicht für jede Oberfläche neu programmieren. Eine langfristige Verbesserung wäre, auch die Ablaufsteuerungen weiter zusammenzuführen. Die aktuelle Trennung hilft mir bereits dabei, den Code nach Aufgabenbereichen zu lesen, zu testen und zu erklären.

**REGIE:** Auf Beziehungen zeigen, nicht die komplette UML vorlesen. Den Unterschied Game/MatchSession ausdrücklich nennen.

**MERKSATZ:** Board verwaltet Regeln, Game den Konsolenablauf.

**KURZFASSUNG BEI ZEITDRUCK:** Jede Klasse hat eine klar begrenzte Hauptaufgabe.


---

## A05 · Konsolenversion, 2D und 3D

**ZEIT:** 06:40–08:30  
**ZEIGEN:** Nur verifizierte Screenshots; optional kurze 2D-/3D-Demo

### Sprechtext

Die Konsolenanwendung erfüllt den schulisch geforderten Kern und benötigt keine SFML-Grafikbibliothek. Im Entwicklungszweig habe ich das Projekt zusätzlich um eine 2D-Anwendung mit SFML 3.1 ergänzt. Dort gibt es ein eigenes Fenster, Maus- und Tastatursteuerung, eine Fallanimation und einen optionalen, im Code erzeugten Soundtrack. Die Musik ist standardmäßig ausgeschaltet.

Daneben existiert eine 3D-Arena auf Basis von OpenGL. Sie zeichnet räumliche Körper, verwendet Perspektive und Tiefentest und erlaubt das Drehen und Zoomen der Kamera. Eine interessante technische Aufgabe war die Mausauswahl: Ein Strahl von der Kameraposition wird mit der Ebene des Spielbretts geschnitten, um die richtige Spalte zu finden. Diese Erweiterungen zeigen, wie dieselbe Regelklasse in unterschiedlichen Oberflächen eingesetzt werden kann. Sie ersetzen aber nicht die Pflicht-Konsole und sind erst nach einem echten Sicht-, Hör- und Funktionstest auf dem Zielrechner vorführbereit.

**REGIE:** Zwischen implementiertem Code, erfolgreichem CI-Build und noch offenen manuellen Tests unterscheiden.

**MERKSATZ:** Die Grafik ist optional; die Spielregeln bleiben gleich.

**KURZFASSUNG BEI ZEITDRUCK:** Die 2D-/3D-Oberflächen sind freiwillige Erweiterungen.


---

## A06 · Spielstand, Highscore und Datensicherheit

**ZEIT:** 08:30–10:20  
**ZEIGEN:** Grafik: GYM1 → vorläufiges Board → Prüfungen → Übernahme

### Sprechtext

Das Projekt kann einen Spielstand speichern und später wieder laden. Die Datei beginnt mit der Kennung „GYM1“ und enthält neben der Spielerzahl auch den aktuellen Spieler, die Namen, die Spielfeldgröße, die Zugliste und das Raster. Beim Laden wird der Inhalt nicht blind übernommen. Das Programm erstellt zunächst ein vorläufiges Spielfeld und spielt die gespeicherten Züge darauf nach. Stimmen Spielregeln, Zugfolge und Raster nicht überein, wird die Datei abgewiesen. Die laufende Partie bleibt dabei erhalten.

Für das Schreiben gibt es einen gemeinsamen Dateihelfer: Der neue Inhalt wird zuerst vollständig in eine temporäre Datei geschrieben und erst danach an die Stelle der bisherigen Datei gesetzt. Das vermindert das Risiko, bei einem fehlgeschlagenen Schreibversuch den vorherigen Spielstand zu verlieren. Zusätzlich führt das Spiel eine Bestenliste mit dauerhaft gespeicherten Siegen. Diese Funktionen zeigen, dass Datensicherheit auch bei einem kleinen Spiel wichtig ist.

**REGIE:** Keine vollständige Transaktions- oder Mehrprozesssicherheit behaupten; der Dateihelfer ersetzt kein Locking.

**MERKSATZ:** Zuerst validieren, dann übernehmen; zuerst schreiben, dann ersetzen.

**KURZFASSUNG BEI ZEITDRUCK:** Beschädigte Speicherdateien dürfen die aktive Runde nicht zerstören.


---

## A07 · Tests, GitHub und verantwortliche Entwicklung

**ZEIT:** 10:20–12:20  
**ZEIGEN:** tests/ · CMakeLists.txt · GitHub Actions

### Sprechtext

Um Fehler früh zu finden, habe ich für wichtige Funktionen automatisierte Tests im Repository. Dazu gehören die verschiedenen Feldgrößen, die Schwerkraft, alle vier Gewinnrichtungen, volle Spalten, Undo sowie Speicher- und Ladefunktionen. Für die grafischen Versionen gibt es zusätzlich Tests der gemeinsamen Spielsteuerung und einzelner 3D-Berechnungen.

Der Build wird mit CMake organisiert; CTest führt registrierte Tests aus. GitHub Actions wiederholt diese Prüfungen in mehreren Windows- und Linux-Umgebungen. Der von mir zuletzt geprüfte C++-Lauf vom 31. Oktober 2026 war mit neun erfolgreichen Jobs abgeschlossen. Das ist ein belegter technischer Zwischenschritt, aber kein Ersatz für die reale Prüfung auf dem Schulrechner.

Bei Entwurf, Überarbeitung und Dokumentation wurde KI-Unterstützung verwendet. Diese Hilfe sollte transparent benannt werden. Entscheidend für meine mündliche Leistung ist, dass ich den Code selbst nachvollziehen, Fehlerfälle erklären und die Funktionen am Programm belegen kann.

**REGIE:** Nur den konkret geprüften CI-Lauf nennen: GitHub Actions Run 41221586099; keine schulische Freigabe behaupten.

**MERKSATZ:** Automatisierte Nachweise und eigene Codekenntnis gehören zusammen.

**KURZFASSUNG BEI ZEITDRUCK:** Ein grüner CI-Test ist notwendig, aber nicht die gesamte Abnahme.


---

## A08 · Kurze Vorführung und Übergabe an den Code

**ZEIT:** 12:20–15:00  
**ZEIGEN:** Bereits gestartetes Konsolenspiel; anschließend VS Code

### Sprechtext

Zum Abschluss zeige ich die Kernfunktionen kurz im laufenden Programm. Ich wähle zwei Spieler, trage zwei Namen ein und starte eine Runde. Anschließend verwende ich „V 4“ für eine Vorschau, setze einen Stein und mache diesen Zug mit „U“ rückgängig. Damit lässt sich direkt beobachten, wie die Steuerung auf unterschiedliche Eingaben reagiert.

Für einen vollständigen Gewinn habe ich zusätzlich einen gesonderten Testspielstand vorbereitet. Diesen führe ich nur in einem isolierten Demo-Verzeichnis vor, damit keine persönlichen Daten überschrieben werden. Falls die Vorführung auf dem Zielrechner unerwartet scheitert, kann ich den vorgesehenen Ablauf anhand des vorab geprüften Quellcodes und der Testnachweise erläutern.

Damit ist der Überblick abgeschlossen. Im zweiten Teil öffne ich die Klassen in Visual Studio Code und zeige Schritt für Schritt, wie aus einer Spalteneingabe ein gültiger Zug wird und wie die Gewinnerkennung arbeitet.

**REGIE:** Zeit strikt kontrollieren: zwei Namen, V 4, ein Zug, U; Gewinnfixture nur mit verbliebener Zeit. Nicht während der Prüfung neu bauen.

**MERKSATZ:** Von der Bedienung zur tatsächlichen Implementierung wechseln.

**KURZFASSUNG BEI ZEITDRUCK:** Der nächste Teil erklärt den Weg Eingabe → Board → Gewinnprüfung.


---

## Vortragskontrolle und verantwortliche Aussagen

- [ ] Pro Karte Anfang und Endzeit mit Stoppuhr geprüft; alle 15:00 Minuten eingehalten.
- [ ] Originalfolien, Bildschirmauflösung, Schriftgrößen und Übergänge am Präsentationsgerät überprüft.
- [ ] Funktionen tatsächlich vorgeführt oder ausdrücklich als noch nicht manuell getestet bezeichnet.
- [ ] CI-Nachweise nur für nachweisbar ausgeführte Jobs genannt: [C++-Prüflauf vom 31.10.2026](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/41221586099).
- [ ] Die Formulierungen zu KI-Unterstützung und eigener Arbeit sind sachlich zutreffend.
- [ ] Spielstände ausschließlich aus einem separaten Demo-Verzeichnis verwendet; Konsolenprogramm und GUI schreiben nicht gleichzeitig in denselben Datenordner.

**Nächster Schritt:** [Druckkarten](SPRECHKARTEN_DRUCKVERSION.html) · [Lehrerfragen](FACHGESPRAECH_KURZANTWORTEN.md) · [Generalprobe](../docs/GENERALPROBE_2027.md) · [Schulanforderungen](../docs/SCHULANFORDERUNGEN_51_STATUS.md).
