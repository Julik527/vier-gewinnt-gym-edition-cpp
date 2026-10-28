# Product Backlog: Vier Gewinnt | Gym Edition

**Version:** 2.0 · **Aktualisiert:** 28.10.2026 · **Anforderungen:** FR-01–18 und EX-01–07
**Belegstand:** `02eb79f8` auf `quality/gameplay-persistence-2026-10-28`

Die Zeilen unterscheiden automatische Prüfung und manuelle Freigabe. **CI bestanden** bedeutet nur die tatsächlich abgedeckten Kontrollen des verlinkten Laufes. Eine vollständige Schulabnahme bleibt offen.

| ID | Prio | Thema | Anforderung | Akzeptanzkriterium | Nachweis und Stand |
|:--|:--|:--|:--|:--|:--|
| PB-01 | MUST | Hauptmenü | FR-01 | Hauptmenü 0–5, geführter Start, ungültige Auswahl abfangen | Konsolen-Integration in CI bestanden |
| PB-02 | MUST | Spielerzahl | FR-02 | Nur 2, 3 oder 4, Einrichtung abbrechbar | Konsolen-Integration für alle drei Größen bestanden |
| PB-03 | MUST | Namen | FR-03 | Nacheinander, nicht leer, eindeutig, 24 Bytes, keine Steuerzeichen | Integrations- und MatchSession-Prüfungen bestanden |
| PB-04 | MUST | Feldgrößen | FR-04 | 7×6, 9×8 oder 11×10 | Board-Tests und Konsolen-Integration bestanden |
| PB-05 | MUST | Schwerkraft | FR-05 | Unterstes freies Feld, volle Spalte ablehnen | Board-Tests bestanden |
| PB-06 | MUST | Spielerwechsel | FR-06 | Wechsel nur nach gültigem Zug | Unit- und Integrationstests bestanden |
| PB-07 | MUST | Sieg | FR-07 | Waagerecht, senkrecht, zwei Diagonalen | Board-Tests sowie gespielte Siegfolgen bestanden |
| PB-08 | MUST | Remis und Rundenende | FR-08/13 | Volles Feld ohne Sieg, Ergebnisnavigation | Board-Fülltests und Ergebnisnavigation bestanden, vollständige manuelle Remispartie offen |
| PB-09 | MUST | Eingabekontrolle | FR-09 | Fehler ändern keine gültige Partie | Negative Tests bestanden, interaktive Gesamtprüfung offen |
| PB-10 | MUST | Speichern | FR-10 | Lesbares GYM1, alter Stand bei getestetem Schreibfehler erhalten | Integration und MatchSession bestanden |
| PB-11 | MUST | Laden | FR-11/14 | Defekte Datei ablehnen und aktive Partie erhalten | Negative Integrations- und MatchSession-Fälle bestanden |
| PB-12 | MUST | Highscore | FR-12 | Sieg +1, sortiert, nach Neustart erhalten | Zwei Siegfolgen, Neustart und Schreibfehler geprüft |
| PB-13 | MUST | GitHub | FR-15 | Quellcode, Dokumente und nachvollziehbare Commits | Öffentliches Repository vorhanden, Zusatzentwicklung in Draft-PR #1 |
| PB-14 | MUST | KI-Prompts | FR-16 | Original oder Rekonstruktion, Ergebnis, Kontrolle und Grenzen | Register um den heutigen Verlauf ergänzt, kein vollständiger Chat-Export |
| PB-15 | MUST | Vortrag und Handout | FR-17/18 | Beide Versionen und Architektur verständlich erklären | Unterlagen aktualisiert, Generalprobe und Schulabnahme offen |
| PB-16 | SHOULD | Hanteln und Hilfe | EX-01/05 | Eindeutige Symbole, Hilfe verändert keine Partie | UI- und Integrationstests bestanden, Sichtprüfung offen |
| PB-17 | COULD | Vorschau, Undo und S | EX-02–04 | Korrekte Zustandsübergänge | Automatische Kontrollen bestanden |
| PB-18 | COULD | Info und Animation | EX-06/07 | Optional, unterschiedliche Fallbilder, ohne Farbe lesbar | ConsoleUI-Frames geprüft, visuelle Wirkung am Zielgerät offen |
| PB-19 | SHOULD | Zusätzliche 2D-GUI | GUI-01 | Separates Ziel, Maus/Tastatur, 2–4 Personen, Board weiterverwenden | Linux-/Windows-GUI-Build und Kernprüfungen bestanden, Bedienabnahme offen |
| PB-20 | SHOULD | Menü und Revanche | UI-01–05 | Fortsetzen, Ergebnis, gleiche Spieler, exakt ein Siegpunkt | Konsolen-Integration und MatchSession bestanden |
| PB-21 | COULD | Gym-Beat | AUDIO-01 | F9/Schalter, Standard aus, Fehlermeldung bei fehlendem Gerät | Code und Audio-Linkage vorhanden, tatsächlicher Hörtest offen |
| PB-22 | SHOULD | Schullogo | BRAND-01 | Originalmotiv, proportional, Quellen/Lizenz, lokaler Fallback | JPEG liegt im Repository, CI-Verpackung bestanden, Sichtprüfung offen |
| PB-23 | MUST | Lokales Windows-Setup | DEV-01 | CMake/Ninja und beide F5-Profile funktionieren | Task-PATH korrigiert, lokale Wiederholung nach CMake-Fehler offen |
| PB-24 | MUST | Vollständige Dokumentation | DOC-01 | Markdown, PDFs und PPTX widerspruchsfrei zum Prüfstand | [Dokumentenaudit](DOCUMENTATION_AUDIT_2026-10-28.md), aktualisierte Nachträge und Folien |

## Belege und nächste Abnahme

[C++-/GUI-Lauf](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/40779797878), [Dokumentenlauf](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/40779798112), [gesicherte Jobstatus](evidence/CI_SNAPSHOT_2026-10-28.json). Die historischen 18 Backlog-IDs bleiben erhalten. PB-19 bis PB-24 sind projektinterne Ergänzungen und keine nachträglich erfundenen Pflichtpunkte des Aufgabenblatts.

Zum Meilenstein am 29.10.2026 müssen Projekt, Anforderungen, Backlog, Repository, Klassenentwurf und Menü vorgeführt werden. Die GUI ist Zusatzumfang. Vor der Vorführung den lokalen CMake-/F5-Fehler beheben, beide Programme nacheinander testen und die tatsächlichen Beobachtungen in den Testplänen eintragen.
