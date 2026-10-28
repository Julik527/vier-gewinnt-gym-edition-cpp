# Meilenstein – Donnerstag, 29.10.2026

**Stand des Projektchecks:** 25.10.2026. Dokumentstatus von Testergebnissen und Remote-Nachweisen getrennt prüfen.

| Bereich | Nachweis | Offene Kontrolle |
|:--|:--|:--|
| Projektstart | Entwicklungsdateien seit 22.10.2026 | Falls nötig schriftlich bestätigen |
| Anforderungen | FR-01–FR-18, Anforderungsanalyse | Mit Lehrer abstimmen |
| Product Backlog | `docs/PRODUCT_BACKLOG.md` mit 18 Punkten | Priorität verfolgen |
| Klassenentwurf | Game, Board, Player, Move; UML-PDF unter `reference/` auf GitHub | Im Fachgespräch erklären |
| Menü-Prototyp | `Game::run()`, Hauptmenü 0–5, geführter Spielstart; neue Integrationstests vorbereitet | Am Schulrechner und in CI ausführen, Ergebnis dokumentieren |
| GitHub | Öffentliches Repository, Textdateien plus acht PDFs/PPTX-Dateien (Commit `53612455`) | Build vorführen und ggf. Download auf Schulrechner testen |
| KI-Dokumentation | Promptregister als Markdown und PDF unter `reference/` | Vollständige Chat-Exportpflicht ggf. separat klären |

**Hinweis:** Eine ältere PDF-Prüffassung entstand vor dem tatsächlichen GitHub-Upload. Der Repositorystatus ist in dieser Markdown-Datei aktualisiert.

## Erweiterung der Menüführung vom 28.10.2026

Das Projekt enthält jetzt einen Ergebnisbildschirm, Revanche mit denselben Spielern, einen aktiven Menüstatus sowie eine ruhigere Terminaldarstellung. Die ergänzten Akzeptanztests in `tests/integration_smoke.py` prüfen unter anderem zwei aufeinanderfolgende Siege und das korrekte Highscore-Verhalten. **Diese Ergänzung ist ein Implementierungsstand, keine bestätigte Schul- oder Windows-Abnahme.**

## Ausstehend
Manuelle vollständige Systemtests, Windows-Präsentation, finale Prüfung der erzeugten Dokumente, ggf. Aufgabenabnahme am 29.10.2026.
## Update vom 28.10.2026

Die Projektdateien wurden auf JK AXIS | GOLD MASTER gebrandet, die Quelltexte wurden um eine strengere Savegame-Prüfung ergänzt und ein CMake-/CTest-Projekt samt GitHub-Actions-Workflows eingerichtet. Diese Ergänzungen sind **keine rückwirkende schulische Freigabe**.

**Vor dem Termin am 29.10.2026:** Die GitHub-Actions-Ergebnisse prüfen, möglichst einen vollständigen Build und eine Spielrunde unter Windows vorführen, das Dokumenten-Preflight-Ergebnis sichern und die Visualisierung der Präsentation auf dem Vorführgerät testen. Siehe [Qualitätssicherung](QUALITY_AND_TESTS.md).
