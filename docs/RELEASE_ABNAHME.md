# Release- und Abnahmecheckliste | Vier Gewinnt – Gym Edition

**Stand:** 11.11.2026 · **Zweck:** Technische und manuelle Freigabekriterien für die Veröffentlichung.

## A. Automatisierte Prüfungen (je Release-Kandidat)

- [ ] Konkrete Commit-SHA, Branch und CMake-Konfiguration dokumentiert.
- [ ] C++-Quality-Workflow für **genau diese SHA** erfolgreich (Linux, Windows, Debug, Release, UCRT64, GUI und 3D).
- [ ] Document-&-Brand-QA für dieselbe SHA erfolgreich, einschließlich Repository-Konsistenztest.
- [ ] Bei Fehlversuchen Logs gesichert, Ursachen behoben und dieselben Prüfungen erneut bestanden.
- [ ] Portable Windows-2D-/3D-Pakete mit SHA-256 erzeugt und eindeutig dem Commit zugeordnet.

**Grenze:** CI-Erfolg ist kein Beleg für flüssige Grafik, hörbaren Ton oder die tatsächliche Unterrichtsvorführung.

## B. Echte Windows-Abnahme

Für **jeden** Test Ergebnis mit Datum, Commit, Windows-Version, Grafikkarte, Auflösung, DPI, genauen Schritten, Ist-Ergebnis und Beleg (Screenshot/Log) festhalten.

- [ ] Pflicht-Konsole auf dem vorgesehenen Rechner mit `scripts/preflight-school.ps1` gebaut und gestartet.
- [ ] 2/3/4 Spieler, alle vier Gewinnrichtungen, Remis, volle Spalten, ungültige Eingaben, Undo und Revanche live geprüft.
- [ ] Speichern, Neustart, Laden und Highscores mit echten Partien nachvollzogen.
- [ ] Defekte Save-/Highscore-Dateien und verweigerte Schreibrechte sicher behandelt.
- [ ] 2D und 3D aus VS Code per F5 erfolgreich gestartet; Maus- und Tastaturbedienung vollständig geprüft.
- [ ] Auflösungen sowie Windows-Skalierung 100 %, 125 % und 150 % visuell kontrolliert.
- [ ] 3D-Kamera: Rotation, Zoom, äußerste Spalten, Ray-Picking, Fokusverlust, Fallanimation und Gewinn-Effekte geprüft.
- [ ] Musik F9/F10/F11 tatsächlich gehört, Stumm-Verhalten und fehlende Audiogeräte geprüft.
- [ ] UTF-8-Namen/Umlaute, Schulkennzeichen und Windows-Pfade mit `ß` praktisch geprüft.
- [ ] 2D-/3D-Paket auf einem zweiten Windows-Rechner ohne Entwicklungswerkzeuge ausgepackt und gestartet.
- [ ] PowerPoint-Datei und PDF auf dem Schulbeamer geprüft; 30-Minuten-Generalprobe absolviert.

## C. Architektur- und Veröffentlichungsprüfung

- [ ] `Board` ohne SFML-/OpenGL-Abhängigkeit; Regeln werden zentral getestet.
- [ ] Unterschiede zwischen Konsolen-`Game` und GUI-`MatchSession` bleiben ausdrücklich dokumentiert.
- [ ] OpenGL-Kompatibilitätsprofil wird nicht fälschlich als moderne Shader-/PBR-Pipeline bezeichnet.
- [ ] Keine Garantie gegen Stromausfall oder gleichzeitige Schreibzugriffe behauptet.
- [ ] Urheber- und Nutzungsrechte für Quellcode, Logo, Schullogo, Bilder und Ton geklärt.
- [ ] Authentische Screenshots und Gameplay-Videos aus realen Programmläufen erstellt, keine Mockups als Beweis.
- [ ] Versionstag, Downloadpakete, SHA-256-Prüfsummen und Release Notes nach Freigabe veröffentlicht.

## D. Schulabnahme und Codeverständnis

- [ ] Konstruktoren, Kapselung, `std::vector`, Spielzug, Gewinnerkennung und Fehlerbehandlung frei erklärt.
- [ ] Codeabschnitte auf Nachfrage live gefunden und mit eigenen Worten erläutert.
- [ ] Präsentation, Handout und Aufgabenblatt 51 inhaltlich abgeglichen.
- [ ] KI-Unterstützung transparent und nachvollziehbar dokumentiert.
- [ ] Fachgespräch und Vorführung real durchgeführt; keine fiktiven Freigaben eingetragen.

## Freigabeentscheidung

**Noch nicht freigegeben.** Die Kästchen bleiben offen, solange keine realen Belege vorliegen. Ein grüner Pull Request ist ein technischer Zwischenstand, keine schulische Abnahme oder Bewertung.

Siehe auch [Release-Checkliste](RELEASE_CHECKLIST.md), [GUI-Testplan](GUI_TESTPLAN.md), [3D-Testplan](3D_TESTPLAN.md), [Anforderungsmatrix](SCHULANFORDERUNGEN_51_STATUS.md).
