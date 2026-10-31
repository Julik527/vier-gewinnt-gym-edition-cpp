# Entwicklungsprotokoll vom 28.10.2026

**Projekt:** Vier Gewinnt | Gym Edition · **Entwickler:** Julian Krauß · **Sprache:** C++17
**Geprüfter Ausgangsstand:** [`02eb79f8`](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/commit/02eb79f8c432ef4578cf650ca2b74efa4d448769)
**Arbeitsbranch:** `quality/gameplay-persistence-2026-10-28` · **Übergabe:** [Draft-Pull-Request #1](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/1)

## Einordnung

Der Arbeitstag umfasst die JK-AXIS-Markenintegration in die Projektunterlagen und die Weiterentwicklung des Spiels von der Konsole zur zusätzlichen 2D-Anwendung. Dieses Protokoll beschreibt nur nachweisbare Arbeiten an diesem Repository. Andere Tagesprojekte, etwa die Portfolio-Website, gehören nicht zur Spielabgabe.

Die Umsetzung ist weit fortgeschritten. Die endgültige Abnahme am Schulrechner steht noch aus. Die Bezeichnung GOLD MASTER gehört zur Gestaltung und ersetzt kein Testergebnis. Die Dokumentation entstand mit KI-Unterstützung. Eine vollständig selbstständige Entwicklung oder bereits bestandene Lehrkraftprüfung wird nicht behauptet.

## 1. Markenintegration und verständliche Dokumentation

- Das aktuelle JK-AXIS-Präzisionssymbol liegt als helle und inverse SVG-Fassung unter `assets/brand/` und steht in der README sowie den Designleitfäden.
- Der frühere Dokumentenumbau ersetzte die Logo-Instanzen in den acht PDF-/PPTX-Dateien. Einige historische PDF-Seiten enthalten weiterhin die alte Design-ID. Diese Kennungen gehören zum archivierten Planungsstand und werden im aktuellen Nachtrag erklärt.
- Der Kommentarleitfaden beschreibt kurze Dateiköpfe und verständliche Erklärungen auf Schülerniveau. Neue GUI-Dateien hatten zum Prüfzeitpunkt teilweise kürzere beziehungsweise englische Kommentare. Der Leitfaden führt sie deshalb ausdrücklich auf, ohne eine lückenlose sprachliche Überarbeitung zu behaupten.
- Fehlende heutige Prompts, Entscheidungen, Fehlerberichte und Kontrollgrenzen werden in [KI_PROMPT_DOKUMENTATION.md](KI_PROMPT_DOKUMENTATION.md) nachgetragen. Originalaussagen und Rekonstruktionen bleiben unterscheidbar.

## 2. Konsole: Menü und Spielerlebnis

`Neues Spiel` führt durch Spielerzahl und Namen. Zwei Personen spielen auf 7×6, drei auf 9×8 und vier auf 11×10 Feldern. Namen müssen eindeutig sein und passen in das 24-Byte-Limit. Nach der Einrichtung beginnt die Terminalpartie direkt.

Das Hauptmenü zeigt eine aktive Runde. `M` unterbricht sie, `2` setzt sie fort. Nach Sieg oder Remis gibt es Revanche, neue Einrichtung oder Hauptmenü. Die Revanche behält Namen und Symbole und leert das Raster. Highscores zählen nur tatsächliche Siege.

Die Trainings-Arena verwendet Hantelsymbole, Spielerfarben, einen klaren Zugstatus und eine markierte Gewinnreihe. Vorschau und Meldungen bleiben lesbar. Die optionale Fallanimation erzeugt mehrere unterschiedliche Spielfeldbilder. Die Konsole startet ohne Animation und bleibt ohne ANSI-Farben bedienbar.

## 3. Datensicherheit und Fehlerkorrekturen

| Ausgangsproblem | Änderung | Nachweis |
|:--|:--|:--|
| Direktes Überschreiben konnte einen alten Spielstand bei Schreibfehlern verlieren | Erst in `.tmp` schreiben, Schreibstatus prüfen, dann im selben Ordner ersetzen | `include/AtomicFile.h`, Integrationstests und `match_session_tests` |
| Vorschau verschwand vor der nächsten Eingabe | Kein sofortiges Neuzeichnen nach der Vorschau | `src/Game.cpp`, `tests/integration_smoke.py` |
| Vier lange Namen überfüllten die Statuszeile | Namen untereinander anzeigen | Integrationstest mit 24-Byte-Namen |
| Steuerzeichen konnten ins Terminal gelangen | Steuerzeichen bei Eingabe und Laden ablehnen | Integrations- und MatchSession-Tests |
| Testplan enthielt Tabellenfragmente und wörtliche Backslash-n-Sequenzen | 47 Fälle repariert, Markdown-Prüfung ergänzt | `tests/testplan_lint.py` |
| Früherer F5-Build ließ `ConsoleUI.cpp` aus | Quelldatei und Debug-Symbole ergänzen | `tests/vscode_config_test.py` |

Das Format `GYM1` bleibt erhalten. Konsole und GUI verwenden dieselben relativen Datenpfade. Die Programme werden in der Vorführung nacheinander gestartet. Ein sicherer Dateiersatz ist kein Schutz gegen zwei gleichzeitig schreibende Spielprozesse und keine Zusicherung gegen sämtliche Stromausfallfälle.

## 4. Zusätzliche grafische Gym Edition

Das optionale CMake-Ziel `vier-gewinnt-gui` verwendet SFML 3.1.0 für Fenster, Darstellung, Eingaben und Audio. Es ergänzt die bestehende Konsole. `Board` ist der gemeinsame Regelkern. Die GUI verwendet `MatchSession` für die Partie. Die Konsole bleibt bei `Game` und `console_ui`. Beide Frontends teilen damit Board und Datei-Hilfsfunktion, aber noch keinen einzigen gemeinsamen Spielcontroller.

Die GUI bietet Maus- und Tastaturbedienung, Spielerübersicht, animierte Hanteln, Gewinnmarkierung, Speichern/Laden, Bestenliste und Revanche. Der Startassistent hat nach Spielerzahl und Namen zusätzlich eine Bestätigungsansicht. Die grafische Fallanimation ist standardmäßig an. Alte 3D-Bilder in der Gesamtdokumentation bleiben Design-Mockups und zeigen keine implementierte 3D-Version.

## 5. Schullogo und Musik

Das Schullogo ist zusätzlich zur JK-AXIS-Projektmarke eingebunden. Zum geprüften Stand liegt bereits eine skalierte JPEG-Kopie unter `assets/school/bernd-blindow-schulen.jpg`. Sie stammt aus dem dokumentierten alternativen Bildhost und ist nicht bytegleich mit dem Wikimedia-Original. Quelle, Urheber, Lizenzangabe und Skalierung stehen in [SCHOOL_LOGO.md](SCHOOL_LOGO.md).

Die GUI sucht das Bild im Projekt- und EXE-Umfeld. CMake kopiert ein gültiges Bild neben die EXE und versucht nur bei fehlendem Bild einen Download. Wegen HTTP 429 wird zuerst die alternative Quelle verwendet. Das PowerShell-Skript unterstützt außerdem `-LocalFile`. Ohne Bild bleibt der Schulname sichtbar.

`GymBeat` erzeugt einen achtsekündigen 120-BPM-Loop aus Wellenformen. Es wird keine Aufnahme eines empfohlenen kommerziellen Songs mitgeliefert. Musik startet ausgeschaltet und lässt sich mit F9 oder der Schaltfläche einschalten. Der SoundBuffer wird vor dem Sound geladen, die Lautstärke beträgt 65 %. Eine Melodie im mittleren Frequenzbereich verbessert die Hörbarkeit auf Notebook-Lautsprechern. `--audio-devices` hilft bei der Diagnose.

Ein Abspielstatus belegt keine tatsächlich hörbare Ausgabe. Der Hörtest mit F9, dem ausgewählten Windows-Ausgabegerät und dem App-Lautstärkemixer ist noch offen.

## 6. Letzter gemeldeter Windows-Fehler: CMake nicht gefunden

Die VS-Code-Aufgabe brach bereits bei `cmake -S . ...` ab. PowerShell fand das Programm nicht. Ein C++-Compilerfehler war damit noch nicht erreicht. Die Installation beziehungsweise der lokale Suchpfad war anhand der Fehlermeldung allein nicht eindeutig feststellbar.

Die GUI-Aufgaben in `.vscode/tasks.json` ergänzen jetzt `C:/msys64/ucrt64/bin` und behalten den übrigen Suchpfad. Die ähnliche Angabe in `launch.json` wirkt erst beim Programmstart und reicht für den vorherigen Build nicht aus. Änderung: [`ab83df11`](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/commit/ab83df11c8972e9ae424279deb28d8944e02062f).

Die README enthält die Installation der UCRT64-Pakete mit `pacman`, die Versionskontrolle und den erneuten F5-Start. Eine Rückmeldung über den erfolgreichen lokalen Build liegt noch nicht vor. Die GitHub-Windows-GUI nutzt MSVC und beweist daher nicht die Funktionsfähigkeit der lokalen MSYS2-UCRT64-Konfiguration.

## 7. Verifizierter automatischer Prüfstand

Am Ausgangscommit `02eb79f8` sind [alle sechs C++-/GUI-Jobs](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/40779797878) und der [Dokumentenjob](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/40779798112) erfolgreich abgeschlossen. Die abgefragten Job- und Schrittstatus liegen als [CI-Snapshot](evidence/CI_SNAPSHOT_2026-10-28.json) vor. Zeitstempel dieses Nachweises sind ausdrücklich UTC. Die Läufe endeten lokal nach Mitternacht am 29.10.2026.

| Prüfung | Ergebnis | Aussagegrenze |
|:--|:--|:--|
| Ubuntu Debug und Release | Erfolgreich | Automatisierter Runner |
| Windows Debug und Release | Erfolgreich | Automatisierter Runner |
| SFML-GUI unter Ubuntu und Windows | Erfolgreich | GUI-Build und ausgewählte CTests |
| Linux-Fensterstart mit Xvfb | Im GUI-CTest erfasst | Kurzer virtueller Fensterstart, keine vollständige Bedienprüfung |
| Verpacktes Schullogo | Validierungsschritt erfolgreich | Dateiformat/Abmessungen, keine visuelle Vorführung |
| Dokumente und Markdown-Testplan | Erfolgreich | Technische Integrität des damaligen Dokumentbestands |
| Lokales F5, Mausbedienung und Audio | Offen | Nur am echten Zielgerät abschließend prüfbar |

Die vorliegende Dokumentationsrunde verändert den beschriebenen C++-Quellcode nicht. Ihr eigener Dokumentencheck wird nach Erzeugung der aktualisierten Dateien protokolliert.

## 8. Was vor der Abgabe noch fehlt

1. Aktuellen Entwicklungsbranch holen, CMake/Ninja lokal verfügbar machen und beide F5-Profile starten.
2. Eine vollständige GUI- und Terminalpartie mit Gewinn, Revanche, Speichern/Laden und Neustart durchführen. GUI und Konsole nacheinander verwenden.
3. Schullogo auf dem Bildschirm und Musik über das tatsächliche Windows-Ausgabegerät prüfen.
4. Präsentation in Microsoft PowerPoint und auf dem vorgesehenen Beamer öffnen. Programmstart und Schriftgrößen vorab prüfen.
5. Erklärungen zu `Board`, `Game`, `MatchSession`, `GymBeat` und sicherem Speichern in eigenen Worten üben.
6. Beobachtungen in [TESTPLAN.md](TESTPLAN.md) und [GUI_TESTPLAN.md](GUI_TESTPLAN.md) eintragen. Die Lehrkraft entscheidet über die schulische Abnahme.

Der Draft-Pull-Request wird durch diese Dokumentation nicht automatisch nach `main` übernommen.
