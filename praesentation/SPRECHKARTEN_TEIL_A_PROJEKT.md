# Sprechkarten · Teil A: Projektvorstellung

**Projekt:** Vier Gewinnt | Gym Edition · C++17 · ITA-Programmierprojekt 2026/27  
**Ausgabe:** JK AXIS | GOLD MASTER · Übungsfassung vom 02.11.2026  
**Quelle:** Entwicklungsbranch `quality/gameplay-persistence-2026-10-28` (Stand 31.10.2026)  
**Dauer:** ca. 15 Minuten für Teil A; exakte Sprechzeit muss mit Stoppuhr geprobt werden.

> **Benutzung:** Den Text unter **Sprechtext** darfst du zum Üben ablesen. Vor der Abgabe auf deinen echten Wortlaut, deine tatsächliche Eigenleistung und die endgültigen Folien anpassen. **Aktion** bedeutet: etwas im Programm, in VS Code oder auf einer Folie zeigen. Alles, was nicht am Zielrechner überprüft wurde, ausdrücklich als noch offen bezeichnen.
>
> **Zeitreserve:** Falls die gesamten 30 Minuten die Rückfragen einschließen, die letzten Karten kürzen und etwa 3 Minuten für das Fachgespräch freilassen. Die Zeitfenster sind Orientierung, keine bewiesenen Sprechzeiten.

---

## A01 · Einstieg und Projektidee
**Geplantes Zeitfenster:** 00:00–01:30 · **Anzeigen:** Titelfolie mit Projektname

### Sprechtext (Vorlesefassung)

> Guten Tag. Ich stelle heute mein C++-Projekt „Vier Gewinnt | Gym Edition“ vor. Die Grundidee ist das bekannte Spiel Vier Gewinnt, aber mit einer eigenen Darstellung im Fitness-Stil. Anstelle klassischer Spielsteine verwende ich Hantelsymbole. Die Idee dahinter ist, ein verständliches Spiel nicht nur nachzuprogrammieren, sondern es um eine eigene Benutzerführung und zusätzliche technische Funktionen zu erweitern.
>
> > Der wichtigste Teil ist eine objektorientierte Konsolenanwendung in C++17. Zusätzlich gibt es im Entwicklungszweig eine grafische 2D-Version und eine experimentelle echte 3D-Arena. Für die schulischen Anforderungen steht die Konsole im Mittelpunkt. Mein Ziel heute ist, zunächst das Spiel und seine Architektur zu zeigen. Danach gehe ich direkt in den Quellcode und erkläre die wichtigsten Algorithmen, damit nachvollziehbar ist, wie das Programm tatsächlich arbeitet.

**Aktion / Blickkontakt:** Nicht gleich in 3D starten. Erst Ziel, Pflichtumfang und Ablauf nennen.

**Merksatz:** Ein eigener Spielkern statt nur einer hübschen Oberfläche.

---

## A02 · Anforderungen und Spielregeln
**Geplantes Zeitfenster:** 01:30–03:15 · **Anzeigen:** Anforderungsfolie; bei Bedarf das Raster zeigen

### Sprechtext (Vorlesefassung)

> Die Anwendung ist für zwei, drei oder vier Personen ausgelegt. Die Größe des Spielfelds passt sich automatisch an. Bei zwei Spielern sind es sieben Spalten und sechs Reihen, bei drei Spielern neun Spalten und acht Reihen und bei vier Spielern elf Spalten und zehn Reihen.
>
> > Vor einer Partie werden die Anzahl der Spieler und ihre Namen abgefragt. Danach setzen die Personen abwechselnd ihre Hanteln in eine Spalte. Die Schwerkraft wird simuliert: Ein neuer Stein landet immer auf dem untersten freien Feld. Ziel ist es, vier eigene Symbole ohne Unterbrechung waagerecht, senkrecht oder diagonal anzuordnen. Das Programm muss volle Spalten und ungültige Eingaben abweisen. Ist das gesamte Feld belegt und niemand hat gewonnen, endet die Partie unentschieden. Genau diese Regeln bilden den verbindlichen Kern des Projekts.

**Aktion / Blickkontakt:** Auf den Unterschied zwischen sichtbarer Spaltennummer 1–7 und internem Index 0–6 achten.

**Merksatz:** Eine Regel, drei Feldgrößen, zwei bis vier Spieler.

---

## A03 · Bedienung und Spielkomfort
**Geplantes Zeitfenster:** 03:15–05:00 · **Anzeigen:** Konsolenmenü beziehungsweise Arena-Screenshot

### Sprechtext (Vorlesefassung)

> Der Spielstart läuft in zwei Schritten ab. Zuerst wird die Spielerzahl ausgewählt, anschließend werden die Namen nacheinander eingetragen. Im Hauptmenü kann eine neue Partie begonnen, eine unterbrochene Runde fortgesetzt, ein gespeicherter Spielstand geladen oder die Bestenliste angesehen werden.
>
> > Während der Partie steht neben der Spaltenauswahl eine kleine Sammlung von Befehlen bereit. Mit „V 4“ kann man beispielsweise die vierte Spalte als Vorschau ansehen, ohne den Spielstand zu verändern. Mit „U“ wird der letzte Zug zurückgenommen. „S“ speichert und „M“ führt zurück zum Hauptmenü, ohne die laufende Runde sofort zu löschen. Nach Sieg oder Remis gibt es eine Ergebnisanzeige und die Möglichkeit einer Revanche. Namen und Symbole bleiben dann bestehen. Ich habe die Steuerung bewusst so organisiert, dass eine Person das Spiel ohne lange Anleitung bedienen kann.

**Aktion / Blickkontakt:** Falls live gezeigt wird: Vorschau → ein Zug → Undo. Keine persönlichen Savegames überschreiben.

**Merksatz:** Benutzbarkeit ist eine technische Anforderung, nicht nur Gestaltung.

---

## A04 · Objektorientierter Aufbau
**Geplantes Zeitfenster:** 05:00–06:45 · **Anzeigen:** UML-Diagramm oder Quellcode-Struktur

### Sprechtext (Vorlesefassung)

> Bei der Architektur habe ich darauf geachtet, dass nicht eine einzige Funktion alle Aufgaben übernimmt. Die Klasse „Player“ verwaltet Namen und Spielsymbol. „Board“ speichert das zweidimensionale Raster und ist für die grundlegenden Spielregeln zuständig. Die Klasse „Game“ steuert in der Konsolenversion Menüs, Zugwechsel, Rundenzustand und Dateifunktionen. „ConsoleUI“ erzeugt die Terminaldarstellung.
>
> > Bei den grafischen Versionen kommt eine eigene Klasse namens „MatchSession“ hinzu. Sie übernimmt den Spielzustand für 2D und 3D, verwendet aber dieselbe „Board“-Klasse für die Regeln. Damit bleibt die Schwerkraft und die Gewinnprüfung unabhängig davon, ob man Text oder Grafik anzeigt. Vollständig vereinheitlicht ist die Ablaufsteuerung zwischen Konsole und GUI noch nicht. Diese Grenze kann ich benennen und wäre auch ein sinnvoller Punkt für eine spätere Überarbeitung.

**Aktion / Blickkontakt:** Im Klassendiagramm nur vier Kernverantwortlichkeiten erläutern; nicht jede Methode ablesen.

**Merksatz:** Board kennt Regeln; Game und MatchSession kennen den Ablauf.

---

## A05 · Grafische Erweiterungen: 2D und 3D
**Geplantes Zeitfenster:** 06:45–08:30 · **Anzeigen:** 2D-/3D-Screenshots oder vorab geprüfte kurze Live-Ansicht

### Sprechtext (Vorlesefassung)

> Die Konsolenfassung bildet den Pflichtumfang. Darüber hinaus habe ich das Projekt in einem separaten Entwicklungszweig um Grafik erweitert. Für die 2D-Version nutze ich SFML 3.1. Dort gibt es ein eigenes Fenster, Maus- und Tastaturbedienung, eine Fallanimation und einen optionalen, im Programm erzeugten Soundtrack. Dieser ist standardmäßig ausgeschaltet und kann mit F9 aktiviert werden.
>
> > Zusätzlich existiert eine experimentelle 3D-Version. Hier zeichnet OpenGL echte räumliche Körper mit Perspektive, Beleuchtung und Tiefentest. Die Kamera lässt sich drehen und zoomen. Um die richtige Spalte bei unterschiedlichen Kamerawinkeln zu treffen, wird ein Mausstrahl mit der Ebene des Spielfelds geschnitten. Entscheidend ist aber: Die grafische Darstellung verändert nicht die grundlegenden Spielregeln. Ein erfolgreicher automatischer Grafik-Build beweist außerdem noch nicht, dass die Oberfläche am Vorführrechner fehlerfrei aussieht und die Musik hörbar ist. Deshalb bleibt die geprüfte Konsole meine sichere Rückfalllösung.

**Aktion / Blickkontakt:** Nur bereits auf dem konkreten Vorführgerät getestete GUI live starten; sonst Screenshots zeigen.

**Merksatz:** 3D ist Zusatzleistung, keine Voraussetzung für die Pflichtaufgabe.

---

## A06 · Speichern, Laden und Highscores
**Geplantes Zeitfenster:** 08:30–10:30 · **Anzeigen:** Ablaufgrafik: validieren → speichern beziehungsweise laden

### Sprechtext (Vorlesefassung)

> Ein wichtiges technisches Merkmal ist die dauerhafte Speicherung. Ein Spielstand enthält nicht nur die sichtbaren Hanteln, sondern unter anderem die Spieler, den aktuellen Zug, die Spielfeldgröße und die Zugreihenfolge. Das Format trägt die Kennung „GYM1“. Beim Laden wird die Datei nicht einfach ungeprüft übernommen. Stattdessen wird ein neues, vorläufiges Spielfeld erstellt. Die gespeicherten Züge werden nachgespielt und mit dem gespeicherten Raster verglichen. Erst wenn sämtliche Prüfungen erfolgreich waren, wird der aktuelle Spielzustand ersetzt.
>
> > Für das Schreiben gibt es im Entwicklungsbranch einen gemeinsamen Helfer, der zuerst in eine temporäre Datei schreibt und die bestehende Datei erst nach erfolgreichem Abschluss ersetzt. Das reduziert das Risiko beschädigter Dateien bei einem Schreibfehler. Daneben gibt es eine Bestenliste mit dauerhaft gespeicherten Siegen. Ein Sieg wird einmal pro abgeschlossener Partie gezählt. Diese Umsetzung zeigt, dass ein Spiel auch mit fehlerhaften Daten und Dateizugriffsproblemen umgehen können muss.

**Aktion / Blickkontakt:** Nicht behaupten, dass der Helfer gleichzeitige Programmstarts vollständig absichert.

**Merksatz:** Erst prüfen, dann übernehmen. Erst vollständig schreiben, dann ersetzen.

---

## A07 · Tests, GitHub und Entwicklung
**Geplantes Zeitfenster:** 10:30–12:45 · **Anzeigen:** GitHub-Dateibaum und GitHub-Actions-Lauf

### Sprechtext (Vorlesefassung)

> Für die technische Qualität verwende ich CMake als Build-System und CTest für automatisierte Prüfungen. Die Tests untersuchen unter anderem Feldgrößen, Schwerkraft, volle Spalten, alle vier Gewinnrichtungen, Undo, Spielstände und die Highscore-Datei. Im Entwicklungszweig gibt es zusätzlich Prüfungen für die grafischen Programmkerne und die 3D-Mathematik.
>
> > Das Projekt liegt in einem öffentlichen GitHub-Repository. Über Branches und Pull Requests kann ich neue Arbeiten getrennt von der stabileren Hauptversion entwickeln. GitHub Actions baut und testet das Projekt automatisiert unter verschiedenen Windows- und Linux-Umgebungen. Ein von mir geprüfter Lauf vom 31. Oktober 2026 hatte neun erfolgreiche C++-Prüfjobs. Das ist ein guter Nachweis für Kompilierung und getestete Funktionen, aber noch keine persönliche Schulabnahme oder vollständige Vor-Ort-Prüfung.
>
> > KI-Werkzeuge habe ich bei Entwurf, Überarbeitung und Dokumentation genutzt. Für mich ist wichtig, ihre Unterstützung transparent zu benennen. Die eigentliche Fachleistung bei der Präsentation besteht darin, den Code zu verstehen, Entscheidungen erklären und Fehler systematisch prüfen zu können.

**Aktion / Blickkontakt:** CI-Lauf bei Bedarf zeigen: actions/runs/41221586099. Nichts als manuell getestet ausgeben, was noch offen ist.

**Merksatz:** Tests belegen definierte Fälle. Fachverständnis zeige ich selbst.

---

## A08 · Demonstration, Grenzen und Übergang
**Geplantes Zeitfenster:** 12:45–15:00 · **Anzeigen:** Vorbereitete Konsole mit isoliertem Demo-Datenordner

### Sprechtext (Vorlesefassung)

> Zum Abschluss des ersten Teils zeige ich die Pflichtversion kurz in der Praxis. Ich starte das Hauptmenü, wähle zwei Spieler aus und trage die Namen ein. Auf dem Spielfeld kann ich die vierte Spalte mit „V 4“ zunächst nur als Vorschau markieren. Danach setze ich eine Hantel und nehme den Zug mit „U“ wieder zurück. So lässt sich bereits erkennen, dass die Bedienoberfläche die Regelprüfung nicht selbst übernimmt, sondern Funktionen des Spielkerns aufruft.
>
> > Für eine Gewinnerdemonstration sind vorbereitete, geprüfte Testdaten vorhanden. Ich verwende dafür ein separates Demo-Verzeichnis, damit private oder ältere Spielstände nicht überschrieben werden. Die grafische 3D-Version ist als Erweiterung interessant, aber meine Priorität für die Schule bleibt ein sicher ausführbares und erklärbares Konsolenprogramm. Als nächsten Schritt öffne ich nun in Visual Studio Code die zentralen Klassen und erkläre den Weg eines Zuges vom Benutzereingabewert bis zur Gewinnerkennung.

**Aktion / Blickkontakt:** Bei knappem Zeitbudget nur Vorschau, Zug und Undo zeigen; die Gewinnerdemo in den Codeteil oder die Reserve legen.

**Merksatz:** Jetzt wechseln wir von der Bedienung zur Implementierung.


---

## Qualitäts- und Vorführhinweise
- Die schulspezifische Pflicht ist die objektorientierte **Konsolenversion**. 2D und 3D sind freiwillige Erweiterungen.
- Ein erfolgreicher CI-Job ist keine manuelle Windows-/Grafik-/Audio-Abnahme.
- Für Demospielstände nur isolierte Verzeichnisse mit `tests/fixtures/` verwenden; persönliche Spielstände nicht überschreiben.
- Die KI-Unterstützung transparent nennen; Formulierungen nicht als Nachweis eigenständig entwickelter Methoden ausgeben.
- Bei einer Fachfrage: zuerst den Algorithmus fachlich erklären, anschließend die konkrete Funktion im Quellcode zeigen.

**Weiterführend:** [Vortragsleitfaden](VORTRAGSLEITFADEN.md) · [Generalprobe](../docs/GENERALPROBE_2027.md) · [Schulanforderungen](../docs/SCHULANFORDERUNGEN_51_STATUS.md) · [Fachgespräch](FACHGESPRAECH_KURZANTWORTEN.md)
