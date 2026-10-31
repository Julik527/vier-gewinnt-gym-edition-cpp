# Release- und Abnahmecheckliste

**Projekt:** Vier Gewinnt | Gym Edition · C++17 / SFML 3.1
**Stand:** 31.10.2026. Diese Datei enthält keine erfundenen Testresultate.

## 1. Automatisierte Qualitätsprüfungen
- [ ] Letzter \`main\`-Commit: Linux/Windows Debug und Release erfolgreich
- [ ] Zusätzlicher MSYS2-UCRT64-Build in Debug und Release erfolgreich
- [ ] GUI unter Windows und Linux erfolgreich gebaut; headless GUI-Self-Test erfolgreich
- [ ] ScoreFile- und MatchSession-Negativtests erfolgreich
- [ ] Dokumenten- und Lizenzhinweise auf Vollständigkeit geprüft
- [ ] Release-ZIP und SHA-256 über CI erzeugt; Self-Test **im Paketordner** erfolgreich

Prüfnachweis: den **konkreten Workflow-Link und Commit-SHA** eintragen, nicht allein ein Badge.

## 2. Windows-Handprüfung (durch tatsächlichen Tester)
- [ ] \`docs/GUI_TESTPLAN.md\` komplett abgearbeitet, Status mit Datum belegt
- [ ] Min. zwei Bildschirmauflösungen getestet, inkl. 125/150 % Skalierung
- [ ] Maus, Tastatur, Namen mit Umlauten und Undo geprüft
- [ ] Sound bei F9 hörbar, Stumm-/Gerät-los-Verhalten geprüft
- [ ] Defekte Speicherdaten und parallele Programmstarts geprüft
- [ ] ZIP auf einem **anderen Windows-Rechner ohne Entwicklungswerkzeuge** entpackt und gestartet
- [ ] PowerPoint/Beamer/Schul-PC in einer echten Generalprobe getestet

## 3. Veröffentlichung
- [ ] Draft-PR fachlich reviewed und erst danach nach \`main\` übernommen
- [ ] GitHub-Beschreibung, Tags, Website-/Projektlinks gepflegt
- [ ] Zwei bis drei authentische Screenshots und eine kurze Gameplay-Demo eingebunden
- [ ] Quellcode-Lizenz durch den Rechteinhaber ausdrücklich festgelegt (oder Alle-Rechte-vorbehalten beibehalten)
- [ ] Rechte und Namensnennung externer Assets verifiziert
- [ ] GitHub Release erstellt (Versionsnummer, Changes, ZIP, SHA-256, Einschränkungen)
- [ ] Build des Release-Tags reproduziert und final dokumentiert

## 4. Ausstehende Architekturverbesserungen
- [ ] Getrennte Abläufe \`Game\`/\`MatchSession\` nach Regressionstests vereinheitlichen
- [ ] Optional: Replay, Spielerstatistik und GUI-Aufteilung nur mit sauberem Testnachweis
- [ ] Spielstandsordner und konkurrierende Schreibzugriffe für echte Mehrfachstarts definieren

**Freigaberegel:** Ein grüner CI-Lauf deckt Build-/Testanforderungen ab, aber keine vollumfängliche manuelle Windows-, Audio-, Grafik- und Vorführabnahme.
