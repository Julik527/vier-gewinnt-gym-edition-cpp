# Aufgabenblatt 51 - Anforderungs- und Nachweismatrix

**Stand:** 31.10.2026 · **Referenz:** Projektaufgabe von Dr. Michael Litera, 13 Seiten.  
**Automatisierter Basisnachweis:** Commit `466f5530`, [C++ QA 9/9 erfolgreich](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/41202025895), [Dokument-QA erfolgreich](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/41202025887).

`Code+CI` belegt eine Implementierung und automatisierte Tests, **nicht** eine persönliche Schulabnahme. `Vorbereitet` bedeutet, dass die Unterlage besteht; `Offen` verlangt reale Beobachtungen.

| Vorgabe aus AB51 | Konkreter Nachweis | Status / Rest |
|---|---|---|
| S. 1–2: OOP-Konsolenprogramm | `Game`, `Board`, `Player`, `src/main.cpp` | Code+CI; Erklärung offen |
| S. 2: Hauptmenü und Eingaben | `Game::run`, `integration_smoke.py` | Code+CI; manueller T01 offen |
| S. 2: 2 bis 4 Personen mit Namen | `configurePlayers`, `configureNames` | Code+CI |
| S. 2–3: Raster 7×6, 9×8, 11×10 | `Board::Board`, Board-Tests | Code+CI |
| S. 2–4: Schwerkraft, volle Spalte, 4er-Gewinn, Remis | `Board::drop`, `hasWon`, `isFull` | Code+CI; manuelle Vollpartie offen |
| S. 4: Spielstand GYM1, Laden, Dateiprüfung | `Game::saveGame`, `loadGame` | Code+CI; Neustart am Laptop offen |
| S. 4: dauerhafter Highscore, Sortierung | `addWin`, `showHighscores`, `ScoreFile` | Code+CI |
| S. 5: agiles Board, Review, Retrospektive | `PRODUCT_BACKLOG.md`, `SPRINT_REVIEW_2026-10-31.md` | Vorbereitet; Donnerstagsprotokolle fehlen |
| S. 6: Meilensteine bis 18.02.2027 | `PROJECT_CHARTER.md` und Sprintübersicht | Plan vorhanden; spätere Termine offen |
| S. 7: GitHub, Branches, Commits, Release | Repository und Draft-PR #1 | Repository vorhanden; Release/Tag offen |
| S. 8: Tests und Abnahmekriterien | `board_tests`, `integration_smoke`, `TESTPLAN.md` | CI grün; tatsächliche Handtests offen |
| S. 9–11: 30 Minuten + Quellcode und Demo | PPTX, Vortragsleitfaden, `GENERALPROBE_2027.md` | Vorbereitet; Probe 11.02.2027 offen |
| S. 11: Beispielstände und Rückfallstrategie | `tests/fixtures/`, `GENERALPROBE_2027.md` | Vorbereitet; Vor-Ort-Demo offen |
| S. 12: Handout | `HANDOUT_AUFGABENBLATT_51.md` | Inhalt erstellt; Druck/Formatprüfung offen |
| S. 2, 10, 12: KI-Prompts | `KI_PROMPT_DOKUMENTATION.md` | Register; Vollständigkeit kritisch prüfen |
| S. 13: reproduzierbarer Start | CI und Windows-Diagnose | **Lokaler GUI-Build zuletzt gescheitert**; Konsole separat prüfen |
| S. 13: Code selbst erklären | `VORTRAGSLEITFADEN.md` | **persönlicher mündlicher Nachweis offen** |

## Bewertungspriorität

**Programm 60 %, Präsentation 20 %, Handout 20 %.** Deshalb stehen Pflicht-Konsole, Qualität, Datenintegrität und Codeverständnis vor zusätzlichen GUI- und Musikfunktionen.

## Endfreigabe

**Noch nicht erteilt.** Vor der Abgabe sind die manuellen Testpläne mit realen Ist-Beobachtungen, eine funktionierende Demonstration, ein verständliches Fachgespräch und eine echte Generalprobe erforderlich. Grüne GitHub-Jobs belegen nur den dort ausgeführten automatisierten Prüfumfang.
