# KI-Prompt-Dokumentation – Vier Gewinnt | Gym Edition

**Stand:** 25.10.2026 · **Anforderung:** FR-16 · **Design:** JK-CD-DE-2026-003 · **Technologie:** C++17

## Transparenz der Quellen
**O = Originalfrage** (direkt sichtbar oder durch sichtbare Verlaufsnotiz gestützt); **R = sinngemäß rekonstruierter Auftrag**, kein wörtliches Zitat. Dies ist eine strukturierte Dokumentation der verfügbaren Projektkommunikation und Dateien, **kein vollständiger technischer Chat-Export**. Verwendete KI-Ausgaben wurden projektbezogen eingeordnet; die tatsächliche Eigenprüfung und Abnahme bleiben gesondert nachzuweisen.

## Promptregister

### P-01 – Gym-Design und Bedienung
**Datum:** 22.10.2026 · **Art:** R.
**Anliegen:** Eine Spielanleitung und ein Gaming-UI-Konzept mit Kurzhanteln statt gewöhnlichen Spielsteinen sowie eine Sammlung von Programmablaufplänen.
**Ergebnis:** UI-Mockups, Kurzanleitung und PAP-Sammlung in der 84-seitigen Dokumentation.
**Kontrolle:** Grafikdesign und ausführbaren C++-Code ausdrücklich trennen.

### P-02 – Interaktives Inhaltsverzeichnis
**Datum:** 22.10.2026 · **Art:** O (Verlaufsnotiz).
**Prompt:** „Interaktives inhaltsverzeichniss“
**Ergebnis:** PDF-Navigation für Module, technische Kapitel und PAP.
**Kontrolle:** Sprungziele und Rücklinks im exportierten PDF prüfen.

### P-03 – Anforderungsanalyse
**Datum:** 22.10.2026 · **Art:** R.
**Anliegen:** Anforderungsanalyse für das Vier-Gewinnt-Schulprojekt auf Basis des Aufgabenblatts erstellen.
**Ergebnis:** 19-seitige Anforderungsanalyse, Anforderungen FR-01–FR-18, Testkonzept und Zuordnung zu PAP.
**Kontrolle:** Muss-/Zusatzanforderungen und bestätigte Tests voneinander trennen.

### P-04 – UML-Klassendiagramm
**Datum:** 22.10.2026 · **Art:** R.
**Anliegen:** Objektorientiertes Klassendiagramm als PDF und Abgleich mit den Header-Dateien.
**Ergebnis:** Sechsseitiges UML-Dokument zu Game, Board, Player, Move.
**Kontrolle:** Methoden, Attribute und Sichtbarkeit aus dem tatsächlichen C++-Quellcode übernehmen; keine GUI-Klasse hinzuerfinden.

### P-05 – Programmier-Lernpfad
**Datum:** 22.10.2026 · **Art:** R.
**Anliegen:** Schritt-für-Schritt-Anleitung zum eigenständigen C++-Programmieren, statt nur einer Spielanleitung.
**Ergebnis:** 23 Module von Toolchain bis Testing, Persistenz und GitHub.
**Kontrolle:** Code und Verständnis anhand von kleinen Übungen validieren.

### P-06 – main.cpp erläutern
**Datum:** 23.10.2026 · **Art:** O (Verlaufsnotiz).
**Prompt:** „Meine Antwort ist: Spiel. Prüfe sie und erkläre mir anschließend den nächsten Abschnitt von main.cpp auf Schülerniveau.“
**Ergebnis:** Begleitetes Erklären des Programmstarts und der Game-Instanz.
**Kontrolle:** Lernassistenz nicht mit eigenständig verfasstem Code gleichsetzen.

### P-07 – Vortragspräsentation
**Datum:** 25.10.2026 · **Art:** O.
**Anliegen:** Eine 30-Minuten-Präsentation mit 15 Minuten Projektvortrag und 15 Minuten C++-Live-Erklärung, JK-Corporate-Design, Schullogo und Renderprüfung.
**Ergebnis:** 17 Folien als PPTX/PDF und Vortragsleitfaden.
**Kontrolle:** Die Konsolenversion nicht mit dem UI-Mockup verwechseln. Originalwortlaut siehe unten.

### P-08 – PDF-Öffnungsfehler
**Datum:** 25.10.2026 · **Art:** O.
**Prompt:** „Die Präsentation als PDF konnte nicht geöffnet werden und Vortrags- und Leitfaden konnte auch nicht geöffnet werden.“
**Ergebnis:** PDFs unter kürzeren Namen, alternative ZIP-Datei.
**Kontrolle:** PDF technisch/visuell prüfen; die tatsächliche Öffnung auf dem Nutzergerät ist ein separater Nachweis.

### P-09 – Präsentationskorrektur
**Datum:** 25.10.2026 · **Art:** O.
**Anliegen:** PowerPoint lässt sich nicht öffnen; fehlende Agenden; gestauchtes Schullogo; erneute Renderkontrolle.
**Ergebnis:** Neu exportierte PPTX/PDF mit sechs Agenda-Blöcken, Kapitelnavigation und proportionalem Schullogo.
**Kontrolle:** Layoutüberlappungen korrigiert und exportierte Seiten gesichtet.

### P-10 – Meilensteinprüfung
**Datum:** 25.10.2026 · **Art:** O.
**Anliegen:** Prüfung der am 29.10. fälligen Anforderungen Projektstart, Product Backlog, GitHub, Klassenentwurf, Konsolenmenü; fehlende Teile vervollständigen.
**Ergebnis:** Prüfbericht, Product Backlog mit 18 Punkten, vorbereitete Git-Ordnerstruktur.
**Kontrolle:** C++-Kompilierung, Board-Assertions und Menü-Smoke-Test dokumentiert. Endabnahme noch offen.

### P-11 – GitHub-Repository suchen
**Datum:** 25.10.2026 · **Art:** O.
**Prompt:** „Ich habe hier jetzt ein GitHub Repository reingemacht.“
**Ergebnis:** Repository `Julik527/vier-gewinnt-gym-edition-cpp` identifiziert.
**Kontrolle:** Besitzer, Standardbranch und Sichtbarkeit geprüft.

### P-12 – Öffentlich veröffentlichen
**Datum:** 25.10.2026 · **Art:** O.
**Prompt:** „Ja, sie soll öffentlich bleiben. Kannst du das hochladen, bitte? Oder wie soll ich das machen?“
**Ergebnis:** GitHub-fertige ZIP und Anleitung erstellt, Remote-Adresse gesetzt.
**Kontrolle:** In dieser Phase war ein Git-Push noch nicht nachgewiesen.

### P-13 – KI-Nutzung als Projektakte
**Datum:** 25.10.2026 · **Art:** O.
**Prompt:** „Die verwendeten KI-Prompts werden nachvollziehbar gesammelt und als Projektdokumentation abgegeben. Bitte für das ganze was wir bis jetzt gemacht haben wieder pdf first class“
**Ergebnis:** 16-seitiges Prompt-PDF mit Originalzitaten, Rekonstruktionen und Statusangaben; diese Markdown-Fassung zur Recherche auf GitHub.
**Kontrolle:** Authentische und rekonstruierte Texte kennzeichnen.

### P-14 – Corporate Design
**Datum:** 22.10.2026 · **Art:** R.
**Anliegen:** Dokumente in JK-Design und mit Schullogo; Farbigkeit und Layout konsistent halten.
**Ergebnis:** Wiederkehrende Navy-/Blaugestaltung, Logos und Dokumentkennungen.
**Kontrolle:** Schullogo unverzerrt platzieren.

### P-15 – Selbstständiges C++-Lernen
**Datum:** 23.10.2026 · **Art:** R.
**Anliegen:** Vorhandenen Code selbst verstehen, erklären und schulische Meilensteine berücksichtigen.
**Ergebnis:** Lernschritte zu Game, Board, Player, Move und Debugging.
**Kontrolle:** Eigenleistung, KI-Vorschläge und durchgeführte Tests kenntlich machen.

### P-16 – Alle Projektdateien auf GitHub hochladen
**Datum:** 25.10.2026 · **Art:** O.
**Prompt:** „bitte kannst du alles auf GitHub hochladen?“
**Ergebnis:** Quellcode, README, Tests, Backlog und Dokumentation über die verbundene GitHub-Integration übertragen.
**Kontrolle:** Textdateien auf GitHub gezielt abrufen. Binärdateien (PDF/PPTX) sind getrennt zu ergänzen; sie dürfen nicht als hochgeladen behauptet werden.

## Ausgewählte vollständige Originalfragen

**P-07, 25.10.2026**

> Mach jetzt daraus eine sehr professionelle und sehr vollkommene, sehr präzise, sehr penible Präsentation. Ich denke so 15 Minuten und 15 Minuten den Code selber in Visual Studio erklären, ja. Genau. Zehn von zehn First Class auf Gold-Niveau und Rendering muss perfekt sein. Keine Formatierungsfehler, keine Darstellungsfehler, wirklich so mit Lupe anschauen und Rendering machen und natürlich mit meinem Corporate Design gestalten und Schullogo professionell reinmachen und penibel umgehen und präzise.

**P-09, 25.10.2026**

> Und die normale Präsentation kann sie auch nicht geöffnet werden auf einmal. Und überall fehlen Agendas, also Inhaltsverzeichnisse. Genau. Das ist auch sehr wichtig. Und das Schui-Logo sieht komisch aus, sieht so eingequetscht aus. Bitte nochmal mit Lupe überprüfen.

**P-10, 25.10.2026**

> Oktober 2026 Donnerstag, 29.10.2026 Projektstart, Anforderungen, Product Backlog, GitHubRepository, Klassenentwurf und Konsolenmenü Repository angelegt; Anforderungen und Architektur dokumentiert; Menü-Prototyp läuft. Kontrolliere, ob ich das schon für mein Projekt gemacht habe oder nicht. Falls nicht, können wir das jetzt schnell machen. Sehr professionell, sehr vollkommen, sehr penibel und sehr präzise. Zehn von zehn, First Class. PDF oder so machen und direkt diese Repository in GitHub. Ja, genau.

**P-12, 25.10.2026**

> Ja, sie soll öffentlich bleiben. Kannst du das hochladen, bitte? Oder wie soll ich das machen?

## Nachweise und Grenzen
- Quellcode: `src/`, `include/`, `tests/`.
- Board-Testprotokoll: `docs/evidence/BUILD_AND_TEST_LOG_2026-10-25.txt`.
- Schulische Anforderungen: Anforderungsanalyse (19 Seiten, FR-16).
- UML (6 Seiten), Gesamtdokumentation (84 Seiten).
- Vollständige manuelle End-to-End-Abnahme, Windows-Wiederholung und schulische Freigabe sind **nicht** belegt.
- Ein später hinzugefügter Prompt wird mit Datum, Originalwortlaut, Ergebnis, eigener Kontrolle und ggf. Commit-SHA ergänzt.

## Fortlaufendes Prompt-Protokoll
| Datum | Originalfrage | Ergebnis / Datei | Eigene Änderung | Test / Commit |
|:--|:--|:--|:--|:--|
| | | | | | |