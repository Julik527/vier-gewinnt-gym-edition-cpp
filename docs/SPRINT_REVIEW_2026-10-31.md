# Kanban, Sprint-Review und Retrospektive - Stand 31.10.2026

**Projekt:** Vier Gewinnt | Gym Edition · **Quelle:** Aufgabenblatt 51, Abschnitt 6.  
Dies ist eine dokumentierte technische Bestandsaufnahme vom Samstag, **kein fingiertes Unterrichtsprotokoll**. Eine tatsächliche Vorführung am Donnerstag, 29.10.2026, wurde nicht nachgewiesen.

## Kanban

| Backlog | To Do | In Arbeit / Review | Nachgewiesen |
|---|---|---|---|
| Spätere optionale GUI-Funktionen | Konsolen-Testplan am Zielgerät ausfüllen | Windows-GUI mit kurzem ASCII-Dateipfad neu bauen | Board, Regeln, Speicherfunktion, Highscores |
| Meilenstein-Release und Tag | 30-Min.-Vortrag und Codeverständnis trainieren | Handout und Unterlagen auf AB51 abgleichen | Tests: 9/9 CI-Jobs am Commit 466f5530 |
| Gemeinsame Ablaufsteuerung für GUI/Konsole | Lehrkraft-Review tatsächlich einholen | Beispielstände und Backups für Schul-Demo prüfen | Repository, Entwicklungshistorie, Anforderungsdokumente |

## Nachweisbare technische Iterationen

| Zeitraum | Tatsächlich bearbeiteter Schwerpunkt | Erkenntnis |
|---|---|---|
| 22.–25.10.2026 | Konsolenspiel, Klassenentwurf, GitHub, Dokumentation | Spielkern und Schulprojektstruktur hergestellt |
| 26.–28.10.2026 | Menü, Testfälle, Dateischutz, SFML-2D-Ausgabe | Erweiterungen getrennt von Pflicht-Konsole halten |
| 29.–31.10.2026 | MSYS2, GDB, Unicode-Pfad, CI | Pfad mit `ß` beeinträchtigt lokale Abhängigkeiten; kurzer ASCII-Pfad empfohlen |
| 31.10.2026 | Neues UI und eigener Stereo-Soundtrack | Automatischer Build erfolgreich; praktischer Hörtest bleibt offen |
| 31.10.2026 | Abgleich mit Aufgabenblatt 51 | Handout, Abnahmematrix, Vorführdaten und Projekt-Reviews nachziehen |

## Retrospektive des technischen Zwischenstands

**Positiv:** OOP-Kern und Pflichtfunktionen besitzen zahlreiche automatisierte Tests auf Linux und Windows. Fehlerfälle bei Eingaben, Speicherdateien und Highscores werden ausdrücklich berücksichtigt.

**Hindernisse:** Der GUI-Neubau im lokalen Benutzerpfad scheiterte an der Zeichenkodierung und langen Abhängigkeitspfaden. MSYS2-Bash-Kommandos wurden in PowerShell ausgeführt. Grün gemeldete CI ersetzt keine lokale Schul-Demo.

**Verbesserung:** In einem kurzen ASCII-Pfad kompilieren; stabile Konsole zuerst vorführbereit halten; reale Testergebnisse mit Gerät, Datum und Beobachtung erfassen. Zusätzliche Effekte erst nach der Kernabnahme.

## Geplante Meilensteine

- **26.11.2026:** spielbare Zwei-Spieler-Fassung in der Schule vorführen.
- **17.12.2026:** horizontale, vertikale, diagonale Siege, Remis und Fehlerbehandlung erklären.
- **28.01.2027:** 2–4 Spieler, 3 Feldgrößen, Speichern/Laden und Highscore demonstrieren; Feature Freeze.
- **11.02.2027:** 30-minütige Generalprobe durchführen.
- **18.02.2027:** Präsentation, Codeerklärung und Handout.

## Ausfüllvorlage für den nächsten echten Donnerstags-Sprint

**Datum, Unterrichtsstunde:** offen  
**Sprintziel:** offen  
**Git-Commit/Build:** offen  
**Was wurde vorgeführt?** offen  
**Testresultat und beobachtete Fehler:** offen  
**Rückmeldung der Lehrkraft:** offen  
**Retrospektive: gut / schlecht / nächster Schritt:** offen

Erst nach Durchführung eintragen und im Kanban verschieben.
