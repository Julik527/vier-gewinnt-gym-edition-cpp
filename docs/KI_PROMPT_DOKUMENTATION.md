# KI-Prompt-Dokumentation – Vier Gewinnt | Gym Edition

**Stand:** Arbeitstag 28.10.2026 · **Anforderung:** FR-16 · **Gestaltung:** JK AXIS GOLD MASTER · **Technologie:** C++17 mit optionaler SFML-2D-GUI

## Transparenz der Quellen
**O = Originalfrage** (direkt sichtbar oder durch sichtbare Verlaufsnotiz gestützt); **R = sinngemäß rekonstruierter Auftrag**, kein wörtliches Zitat. Dies ist eine strukturierte Dokumentation der verfügbaren Projektkommunikation und Dateien, **kein vollständiger technischer Chat-Export**. Verwendete KI-Ausgaben wurden projektbezogen eingeordnet; die tatsächliche Eigenprüfung und Abnahme bleiben gesondert nachzuweisen.

Die Einträge P-01 bis P-16 beschreiben den historischen Stand vom 22.–25.10.2026. Ihre damaligen Quellenkennzeichnungen bleiben erhalten. Neue Einträge ab P-17 beziehen sich auf den bereitgestellten heutigen Gesprächsauszug beziehungsweise ausdrücklich gekennzeichnete Rekonstruktionen. Für nicht sichtbare Teile des Verlaufs wird kein vollständiger Originalwortlaut behauptet. Einzelne genaue Uhrzeiten sind nicht durchgehend verfügbar.

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
- Aktueller automatischer Nachweis: sechs erfolgreiche C++-/GUI-Jobs und ein Dokumentenjob am Ausgangscommit `02eb79f8`. Siehe [CI-Snapshot](evidence/CI_SNAPSHOT_2026-10-28.json).
- Vollständige manuelle End-to-End-Abnahme auf dem Schulrechner, lokale Wiederholung nach dem CMake-Fehler, Hörtest und schulische Freigabe sind **nicht** belegt.
- Ein später hinzugefügter Prompt wird mit Datum, Originalwortlaut, Ergebnis, eigener Kontrolle und ggf. Commit-SHA ergänzt.

## Ergänzung vom 28.10.2026

Die folgenden Zitate stammen aus dem bereitgestellten Gesprächsauszug. **O** bezeichnet einen wörtlichen Beleg. **O-Auszug** bedeutet, dass nur ein ausdrücklich markierter Ausschnitt wiedergegeben wird. **R** ist eine sinngemäße Rekonstruktion ohne Anspruch auf Originalwortlaut. Fehlerausgaben werden auf den technischen Kern gekürzt. Persönliche IP-Adressen und Benutzerpfade werden nicht veröffentlicht.

Ergebnisangaben beruhen auf überprüften Dateien und Commits. Eine von der KI beschriebene Umsetzung wird damit nicht automatisch zur Eigenleistung des Schülers. Der Abschnitt Kontrolle nennt belegte automatische Prüfungen oder noch nötige eigene Prüfung.

### P-17: JK-AXIS-Logo in den Projektdateien

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Ich meinte in den Dateien Logo wechseln und dann auf 10 von 10 First Class machen. Sehr professionell, sehr vollkommen und sehr penibel und sehr präzise

**Ergebnis:** Logo-Migration der acht PDF-/PPTX-Dateien und Brandingbericht. SVG-Fassungen liegen unter assets/brand/.

**Kontrolle und Grenze:** Historische Migration `ea8bbef8`. Alte Designkennungen auf historischen PDF-Seiten werden im Dokumentenaudit ausdrücklich eingeordnet. Der heutige Abgleich verwendet zusätzlich Textauszug und Rendering.


### P-18: Gesamte Projektqualität verbessern

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Okay, aber dann mach es bitte auf zehn von zehn First Class. Sehr professionell, sehr vollkommen und sehr penibel und sehr präzise auf Gold- Standard-Niveau

**Ergebnis:** Qualitätsarbeit an Tests, Dokumenten, Menü und Quellcodestruktur.

**Kontrolle und Grenze:** Keine numerische Bewertung als Abnahmebeleg. Tatsächliche CI-Ergebnisse und verbleibende Zielgeräteprüfungen stehen in QUALITY_AND_TESTS.md.


### P-19: Kommentare und Dateiköpfe auf Schülerniveau

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Ach, was noch fehlt, sind Kommentare und Head-Beschreibungen für die Programmiercodes und so weiter auch so professionell am besten mit meinem neuen Corporate Design Guide gestalten und so weiter. Kommentare sollen so geschrieben sein wie von einem Schüler.

**Ergebnis:** Einheitliche Kommentarrichtlinien, Dateiköpfe und Aufgabenbeschreibungen für die ursprünglichen Projektdateien.

**Kontrolle und Grenze:** CODE_STYLE_JK_AXIS.md wurde mit der später hinzugekommenen GUI ergänzt. Dort sind kürzere und teilweise englische Kommentare noch vorhanden. Keine vollständig abgeschlossene Kommentarmigration behauptet.


### P-20: Zusammenhängendes Spielmenü

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Ich würde jetzt auf jeden Fall das Spielmenü verbessern und professioneller machen. Zum Beispiel, wenn man auf neues Spiel drückt, muss das... Ich muss kurz ausführen. Zum Beispiel, wenn man auf neues Spiel drückt, sollte man auch die Spieleranzahl festlegen und Name eingeben, auch nacheinander. Und mach es einfach professioneller, wie es in richtigen Spielmenü ist und nicht so abwechslungsreich immer neues Menü, neues Menü. Mach es 10 von 10 First Class und mach es einmalig, sehr professionell, sehr vollkommen und sehr penibel und sehr präzise auf Gold-Niveau.

**Ergebnis:** Game führt durch Spielerzahl und Namen. Der direkte Terminalstart ersetzt getrennte Einstellungsmenüs.

**Kontrolle und Grenze:** Integration für zwei, drei und vier Spieler, Abbruch, Fortsetzen und ungültige Namen. Die CI am Ausgangsstand ist erfolgreich. Manueller Schulrechner-Test offen.


### P-21: Die fünf Menüverbesserungen umsetzen

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Okay, dann verbessere alle genannten Punkte auf 10 von 10 First Class. Sehr professionell und sehr vollkommen und sehr penibel und sehr präzise auf Gold-Niveau.

**Ergebnis:** Aktiver Menüstatus, geführter Start, Ergebnisbildschirm, Revanche, ruhigere Darstellung und einheitliche Bedienbegriffe.

**Kontrolle und Grenze:** Der Auftrag bezieht sich auf den unmittelbar zuvor besprochenen Menüentwurf. Ergebnisnavigation, zwei Siegpartien und korrektes Highscore-Zählen sind in integration_smoke.py enthalten.


### P-22: Gym-Identität und eigentliche Spieloberfläche

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Okay, dann mache alles Ganze auf 10 von 10 First Class. Es soll ja auch Gym Edition sein. sehr professionell und sehr vollkommen und sehr penibel und sehr präzise, dass es auf Gold-Standard-Niveau ist.

**Ergebnis:** Trainings-Arena, Hantelsymbole, Spielerfarben, Status und hervorgehobene Gewinnlinie. Später zusätzlicher 2D-Spielbildschirm.

**Kontrolle und Grenze:** Die einzelnen Umsetzungsschritte sind am Code und in ARCHITECTURE.md getrennt dokumentiert. Terminalgestaltung, 2D-Programm und historische 3D-Mockups nicht gleichsetzen.


### P-23: Fehlerbericht zur „Lounge JSON“

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Aber die Lounge JSON funktioniert irgendwie nicht mehr. Was ist da los?

**Ergebnis:** Als Hinweis auf launch.json beziehungsweise den F5-Start eingeordnet. Build-Aufgabe um ConsoleUI.cpp und Debugparameter ergänzt.

**Kontrolle und Grenze:** „launch.json“ ist hier eine Interpretation der Formulierung, kein korrigiertes Originalzitat. vscode_config_test.py prüft die Verknüpfungen. Ein erfolgreicher lokaler F5-Neustart ist gesondert nötig.


### P-24: Gezielte P0/P1-Stabilisierung

**Datum:** 28.10.2026, Sitzungstag · **Art:** R

**Rekonstruierter Auftrag:** Sinngemäßer, aus dem Projektverlauf rekonstruierter Auftrag: Auf dem vorhandenen Entwicklungsbranch konkrete Schwachstellen bei Datensicherheit und Bedienung beheben, Tests ergänzen und nicht automatisch nach main mergen.

**Ergebnis:** Sicherer Dateiersatz, dauerhafte Vorschau, Steuerzeichenprüfung und lesbare lange Namen. Später gemeinsamer AtomicFile-Helfer.

**Kontrolle und Grenze:** Kein wörtlicher Originalprompt verfügbar. Nachweis sind unter anderem `30ea62f`, `99af262`, `65a5d81` und die Integrationsprüfungen.


### P-25: Zusätzliche grafische Gym Edition

**Datum:** 28.10.2026, Sitzungstag · **Art:** R

**Rekonstruierter Auftrag:** Die zuvor besprochenen Verbesserungen der Gym Edition umsetzen. Im anschließenden Entwicklungsverlauf wurde eine eigenständige grafische Oberfläche ergänzt. Ein zusätzlicher, davon getrennter wörtlicher GUI-Auswahlprompt ist im bereitgestellten Auszug nicht belegt.

**Ergebnis:** Eigenständiges SFML-3.1.0-Ziel, MatchSession, Maus/Tastatur, Hantelfall, Revanche und GYM1. Die Konsole bleibt erhalten.

**Kontrolle und Grenze:** Die Umsetzung ist durch `17d5aee`, `fa594f3` und den aktuellen Code belegt. Die allgemeine Nutzerzustimmung steht bei P-22. Hier wird weder eine zweite identische Originalfrage erfunden noch ein exakter Wortlaut ergänzt.

### P-26: Umsetzung statt erneuter Bewertung

**Datum:** 28.10.2026, Sitzungstag · **Art:** R

**Rekonstruierter Auftrag:** Sinngemäßer Auftrag aus dem vorliegenden Verlauf: Einen konkreten Arbeitsauftrag für tatsächliche Verbesserungen und erneute Tests formulieren, statt weitere Bewertungen ohne Umsetzung zu geben.

**Ergebnis:** Fehlerhafte Testplantabellen repariert, Prüfskript ergänzt, echte Fallbilder und GUI-Arbeiten fortgeführt.

**Kontrolle und Grenze:** Der vollständige damalige Arbeitsauftrag liegt hier nicht als überprüfbarer Originaltext vor. Deshalb Rekonstruktion. Commits `681b663`, `bb96456`, `4558ba3` und `3a13d28` belegen die Änderungen.


### P-27: Projekt erklären und Lehrerfragen vorbereiten

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Das ist ja mega, aber wie findest du es jetzt? Das ist ja... ich bin begeistert. Nur jetzt kurz erklären können und dann die Fragen vom Lehrer beantworten und die Eins bekommen.

**Ergebnis:** Kurzvorstellung und Erklärungen zu Klassen, SFML, Schwerkraft, Gewinnlogik und getrennter Darstellung. Aktualisierter Vortragsleitfaden.

**Kontrolle und Grenze:** Die Formulierungen dienen der Vorbereitung. Eine Note wird weder garantiert noch als bereits vergeben dokumentiert. Eigenleistung und Verständnis müssen im Fachgespräch gezeigt werden.


### P-28: Musik und beide Vorführwege

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Ich würde noch so richtig coole Musik reinmachen, aber welche Songs sind so richtig cool, motivierend, die auch zu mir passt, die jeder mag und so weiter. Und noch das coole Logo reinmachen. Ich würde gerne beides zeigen: einmal grafisch und einmal von Dings, von PowerShell-Ansicht, also von Terminal.

**Ergebnis:** Synthetischer Gym-Beat und Demo-Skript für GUI, danach Konsole. Keine kommerziellen Songaufnahmen im Repository.

**Kontrolle und Grenze:** Bekannte Songs waren Gesprächsempfehlungen, kein Bestandteil des Programms. Der tatsächlich erzeugte Loop steht in GymBeat.h. Die Audioausgabe ist manuell zu prüfen.


### P-29: Korrektur: Schullogo

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Sorry, ich meinte Schullogo.

**Ergebnis:** Bernd-Blindow-Schullogo zusätzlich zur JK-AXIS-Marke, Schulname in der Konsole, Quellennachweis.

**Kontrolle und Grenze:** Originalmotiv von Bernd Blindow Gruppe. Verwendete Datei ist eine skalierte Kopie, Quelle/Lizenz/Prüfsumme in SCHOOL_LOGO.md.


### P-30: Keine Musik und fehlendes Schullogo

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Aber ich höre irgendwie keine Musik. Warum denn? Und bei dieser grafischen UI fehlt das Logo. Von Schullogo, ja.

**Ergebnis:** Musikschalter deutlicher, Sound nach SoundBuffer laden, 65 % Lautstärke, hörbarer Melodiebereich und --audio-devices. Logo-Suche in Projekt- und EXE-Umgebung, CMake-Verpackung.

**Kontrolle und Grenze:** Audio beginnt weiterhin ausgeschaltet. Codekorrektur und Build sind belegt, tatsächlicher Hör- und Sichttest am Nutzergerät noch offen. Belege `97d1201`, `3c897c8`, `ff83551`, `274d98a`.


### P-31: Wikimedia meldet HTTP 429

**Datum:** 28.10.2026, Sitzungstag · **Art:** O-Auszug

**Beleg (Auszug):**

> Error: 429, Too many requests.

**Ergebnis:** Alternative Bildquelle vor Wikimedia, begrenzte Downloadversuche, JPEG-Prüfung und -LocalFile. Eine gültige skalierte JPEG-Kopie wurde in den Branch aufgenommen.

**Kontrolle und Grenze:** Dies ist nur der technische Ausschnitt der vom Nutzer eingefügten Fehlermeldung. IP-Adresse und persönliche Pfade wurden ausgelassen. Belege `b7c9f44`, `7c50fef`, `702bdf7`; aktuelle Logo-Verpackungsjobs erfolgreich.


### P-32: CMake in der VS-Code-Buildaufgabe nicht gefunden

**Datum:** 28.10.2026, Sitzungstag · **Art:** O-Auszug

**Beleg (Auszug):**

> cmake: Der Begriff „cmake“ wird nicht als Name eines Cmdlets, einer Funktion, einer Skriptdatei oder eines ausführbaren Programms erkannt.

**Ergebnis:** GUI-Konfigurations- und Build-Aufgabe ergänzen den UCRT64-Suchpfad. README erklärt Paketinstallation und erneuten F5-Start.

**Kontrolle und Grenze:** Wörtlicher Ausschnitt der aktuellen Fehlermeldung. Ohne Systemzugriff bleibt offen, ob CMake fehlte oder nur nicht auffindbar war. Commit `ab83df11`. Konfigurationstest bestanden, lokaler Wiederholungsversuch nicht belegt.


### P-33: Heutige Arbeit vollständig dokumentieren

**Datum:** 28.10.2026, Sitzungstag · **Art:** O

**Beleg:**

> Jetzt haben wir heute zwei große Sachen gemacht. Einmal Projekt fertig gemacht, aber fast fertig. Aber bitte dokumentier alles, wo es rein soll. Prüfe alle Dokumente in GitHub und ergänze, wenn was fehlt, Prompt oder so.

**Ergebnis:** Alle Markdown-Unterlagen, sieben PDFs und die PPTX mit dem Entwicklungsstand abgeglichen. Register, Architektur, Tagesprotokoll, aktuelle Testnachweise und Unterlagen ergänzt. Historische Planungsstände bleiben als solche erkennbar.

**Kontrolle und Grenze:** Dokumentenaudit mit Dateiliste, Render- und Integritätsprüfung sowie GitHub-Commit. Der Umfang bezieht sich auf das Spiel-Repository. Andere Tagesprojekte werden nicht ohne eigene Quellen als erledigt behauptet.


### P-34: Bewertungsfragen und Abnahmegrenze

**Datum:** 28.10.2026, Sitzungstag · **Art:** O-Auszug

**Beleg (Auszug):**

> Aber wie viele Punkte gibst du mir?

**Ergebnis:** Im Gespräch wurden Einschätzungen abgegeben. Sie sind keine Messwerte und kein schulisches Prüfergebnis.

**Kontrolle und Grenze:** Wiederholte Fragen nach dem Projektstand werden hier als Rückmeldungsphase dokumentiert. Verbindlich für den Bericht sind Quellcode, Nachweise und offene Prüfungen, keine pauschale 10/10-Zusage.


## Quellen, Vollständigkeit und Eigenprüfung

Die heutigen Originalbelege stammen aus dem für diese Bearbeitung bereitgestellten Gesprächsauszug. P-24, P-25 und P-26 sind ausdrücklich rekonstruierte Arbeitsaufträge. Frühere Einträge behalten die damaligen Quellenangaben. Ein späterer technischer Chat-Export kann zusätzliche Eingaben enthalten. Dieses Register ist deshalb eine nachvollziehbare Projektakte, kein als vollständig bescheinigter Chat-Export.

Die Datumsangabe bezeichnet den Arbeitstag 28.10.2026. Einzelne GitHub-Prüfungen liefen in UTC noch am 28.10., in Europe/Berlin bereits am 29.10.2026. Der CI-Snapshot enthält die originalen UTC-Zeitstempel.

Die dokumentierten Ergebnisse entstanden mit KI-Unterstützung. Welche Stellen der Schüler selbst geschrieben, geändert oder verstanden hat, lässt sich aus einem Commit des verbundenen Kontos allein nicht ableiten. Persönliche Eigenprüfung sollte mit einem tatsächlich ausgeführten Versuch, einer Beobachtung und einer Erklärung am Code ergänzt werden.

| Noch eigene Prüfung | Wo dokumentieren? |
|:--|:--|
| Beide F5-Profile nach der CMake-Korrektur starten | TESTPLAN.md und GUI_TESTPLAN.md |
| Schullogo tatsächlich sehen und F9 tatsächlich hören | GUI_TESTPLAN.md |
| Speichern, Laden, Revanche und Neustart selbst durchführen | Beide Testpläne |
| Board gegenüber Game und MatchSession erklären | Vortragsleitfaden und Fachgespräch |
| KI-Unterstützung und eigene Beiträge ehrlich abgrenzen | Diese Projektakte und eigene Testnotizen |

Weitere spätere Eingaben fortlaufend mit P-35 usw. ergänzen. Dafür Original oder Rekonstruktion, Datum, überprüftes Ergebnis und tatsächlichen Kontrollstand angeben.
