# Release- und Abnahmecheckliste

**Projekt:** Vier Gewinnt | Gym Edition · C++17 / SFML 3.1
**Stand:** 31.10.2026. Diese Datei enthält keine erfundenen Testresultate.

## Schulische Freigabe nach Aufgabenblatt 51 (31.10.2026)

Die verbindliche Nachweisgrundlage liegt in [SCHULANFORDERUNGEN_51_STATUS.md](SCHULANFORDERUNGEN_51_STATUS.md). **Programm 60 %, Präsentation 20 %, Handout 20 %.**

- [x] Eigenes Handout in [Markdown](HANDOUT_AUFGABENBLATT_51.md) erarbeitet und eine dreiseitige PDF-Fassung für die Abgabe erstellt (PDF separat bereitgestellt).
- [x] Prüfbarkeit der Pflichtanforderungen in einer Nachweismatrix dokumentiert.
- [x] Zeitplan, offene Freigaben und aktuelles Kanban/Review als [Bestandsaufnahme](SPRINT_REVIEW_2026-10-31.md) dokumentiert.
- [x] 30-Min.-Vorführablauf, Sicherheitskonzept und isolierte Beispiel-Spielstände vorbereitet.
- [ ] Handout durch den Schüler selbst gelesen, ausgedruckt und auf fachliche Erklärbarkeit geprüft.
- [ ] Konsolen-Testplan mit tatsächlichen Windows-Beobachtungen ausgefüllt.
- [ ] Konsole auf dem Schulgerät demonstriert; Schüler kann Board/Game/Player/Algorithmus erklären.
- [ ] Tatsächliche Donnerstagssprints samt Lehrkraftfeedback protokolliert.
- [ ] Generalprobe 11.02.2027 mit Stoppuhr durchgeführt; Abschluss 18.02.2027.
- [ ] Entsprechenden schulischen Meilensteinrelease/tag nach Freigabe erstellt.

**Wichtig:** Die Checkboxen „erstellt“ sind Dateinachweise, keine bestandene Schulevaluation.

## 1. Automatisierte Qualitätsprüfungen
- [ ] Letzter `main`-Commit: Linux/Windows Debug und Release erfolgreich
- [ ] Zusätzlicher MSYS2-UCRT64-Build in Debug und Release erfolgreich
- [ ] GUI unter Windows und Linux erfolgreich gebaut; headless GUI-Self-Test erfolgreich
- [ ] ScoreFile- und MatchSession-Negativtests erfolgreich
- [ ] Dokumenten- und Lizenzhinweise auf Vollständigkeit geprüft
- [ ] Release-ZIP und SHA-256 über CI erzeugt; Self-Test **im Paketordner** erfolgreich

Prüfnachweis: den **konkreten Workflow-Link und Commit-SHA** eintragen, nicht allein ein Badge.

## Reproduzierbare lokale Vorabprüfung

- [ ] Unter `C:\Dev\GymEdition` die Konsole mit `scripts/preflight-school.ps1` kompilieren; CTest-Ausgabe und Commit dokumentieren.
- [ ] Optional `scripts/preflight-school.ps1 -WithGui` ausführen und den Pfad-Selbsttest nachweisen.
- [ ] Die neue `scripts/check-2d.ps1`-Diagnose auf dem Zielrechner ausführen und die gefundenen CMake-/Ninja-Pfade festhalten.

Diese automatischen Schritte ersetzen keinen manuellen GUI-Hör-, Sicht- oder Bedienungstest.

## 2. Windows-Handprüfung (durch tatsächlichen Tester)
- [ ] `docs/GUI_TESTPLAN.md` komplett abgearbeitet, Status mit Datum belegt
- [ ] Min. zwei Bildschirmauflösungen getestet, inkl. 125/150 % Skalierung
- [ ] Maus, Tastatur, Namen mit Umlauten und Undo geprüft
- [ ] Sound bei F9 hörbar, Stumm-/Gerät-los-Verhalten geprüft
- [ ] Defekte Speicherdaten und parallele Programmstarts geprüft
- [ ] ZIP auf einem **anderen Windows-Rechner ohne Entwicklungswerkzeuge** entpackt und gestartet
- [ ] PowerPoint/Beamer/Schul-PC in einer echten Generalprobe getestet

## 3. Veröffentlichung
- [ ] Draft-PR fachlich reviewed und erst danach nach `main` übernommen
- [ ] GitHub-Beschreibung, Tags, Website-/Projektlinks gepflegt
- [ ] Zwei bis drei authentische Screenshots und eine kurze Gameplay-Demo eingebunden
- [ ] Quellcode-Lizenz durch den Rechteinhaber ausdrücklich festgelegt (oder Alle-Rechte-vorbehalten beibehalten)
- [ ] Rechte und Namensnennung externer Assets verifiziert
- [ ] GitHub Release erstellt (Versionsnummer, Changes, ZIP, SHA-256, Einschränkungen)
- [ ] Build des Release-Tags reproduziert und final dokumentiert

## 4. Ausstehende Architekturverbesserungen
- [ ] Getrennte Abläufe `Game`/`MatchSession` nach Regressionstests vereinheitlichen
- [ ] Optional: Replay, Spielerstatistik und GUI-Aufteilung nur mit sauberem Testnachweis
- [ ] Spielstandsordner und konkurrierende Schreibzugriffe für echte Mehrfachstarts definieren

**Freigaberegel:** Ein grüner CI-Lauf deckt Build-/Testanforderungen ab, aber keine vollumfängliche manuelle Windows-, Audio-, Grafik- und Vorführabnahme.
