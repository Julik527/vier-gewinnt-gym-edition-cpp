# Product Backlog – Vier Gewinnt | Gym Edition

**Version:** 1.1 · **Erstellt:** 25.10.2026 · **Basis:** Anforderungsanalyse (FR-01–18 / EX-01–07) und vorhandener C++17-Code.

Die Spalte „Stand“ unterscheidet **implementiert** von **abgenommen**. Noch nicht getestete Funktionen gelten nicht als fachlich freigegeben.

| ID | Prio | Backlog Item | Anforderung | Akzeptanzkriterium | Nachweis / Stand |
|:--|:--|:--|:--|:--|:--|
| PB-01 | MUST | Startmenü 0–6 | FR-01 | Ungültige Auswahl wird abgefangen | Code vorhanden; Smoke-Test bestanden |
| PB-02 | MUST | Spielerzahl 2–4 | FR-02 | Andere Eingaben abgewiesen | Code vorhanden, manueller Test offen |
| PB-03 | MUST | Eindeutige Spielernamen | FR-03 | Leere / doppelte Namen abgewiesen | Code vorhanden, manueller Test offen |
| PB-04 | MUST | Dynamische Feldgrößen | FR-04 | 7×6 / 9×8 / 11×10 | Board-Assertions bestanden |
| PB-05 | MUST | Zugabgabe und Schwerkraft | FR-05 | Stein fällt auf unterstes Feld | Board teilweise geprüft, Integration offen |
| PB-06 | MUST | Spielerwechsel | FR-06 | Nach ungültigem Zug gleicher Spieler | Code vorhanden, manueller Test offen |
| PB-07 | MUST | Gewinnerkennung | FR-07 | Vier horizontale/vertikale/diagonale Steine | Board-Assertions bestanden, E2E offen |
| PB-08 | MUST | Remis und Abschluss | FR-08/13 | Ohne Sieger voll = Remis | Code vorhanden, Abnahmetest offen |
| PB-09 | MUST | Ungültige Eingaben | FR-09 | Keine Zustandsänderung | Grenztests teils bestanden |
| PB-10 | MUST | Spiel speichern | FR-10 | Vollständige, lesbare Datei | Code vorhanden, Endtest offen |
| PB-11 | MUST | Sicher laden | FR-11/14 | Defekte Dateien ohne Zustandsbeschädigung abweisen | Negativtest offen |
| PB-12 | MUST | Highscore | FR-12 | Sieg genau +1, dauerhaft, sortiert | Persistenztest offen |
| PB-13 | MUST | GitHub-Versionierung | FR-15 | Öffentliches Repo, Quellcode, Commits | Dateien über GitHub-Integration hochgeladen; History des ursprünglichen lokalen ZIP weicht ab |
| PB-14 | MUST | KI-Prompts | FR-16 | Eingaben, Ergebnisse, eigene Kontrollen | `docs/KI_PROMPT_DOKUMENTATION.md` |
| PB-15 | MUST | Vortrag/Handout | FR-17/18 | Architektur und Code erklärbar | Präsentation erstellt; Generalprobe offen |
| PB-16 | SHOULD | Hantel-ASCII/Hilfe | EX-01/05 | Hilfe verändert Spielstand nicht | Code vorhanden, Sichttest offen |
| PB-17 | COULD | Vorschau, Undo, Speichern | EX-02–04 | Gültige Zustandsübergänge | Undo geprüft, andere Integration offen |
| PB-18 | COULD | Info & Animation | EX-06/07 | Abschaltbar, ohne Effekt spielbar | Terminaltest offen |

## Meilenstein Donnerstag, 29.10.2026
1. Anforderungen und Klassenmodell sind dokumentiert.
2. C++17-Konsolenmenü ist implementiert; dokumentierter Smoke-Test liegt vor.
3. Das öffentliche Repository enthält Quellcode, Tests, README und diese Planung.
4. Als Abschlussnachweis GitHub-Link, Commit-URL, Build- und Menü-Screenshot sichern.

## Statusregel
**Geplant:** noch nicht implementiert.
**Code vorhanden:** Implementierung erkennbar, aber nicht automatisch abgenommen.
**Teilweise geprüft:** nur Teilmenge der Akzeptanztests erfolgreich.
**Abgenommen:** alle zugehörigen Tests ausgeführt und protokolliert.

Optional können PB-01 bis PB-18 als GitHub Issues übernommen werden.
## Qualitätsfortschritt 28.10.2026

- Für PB-01 bis PB-12 liegen jetzt zusätzliche automatisierte Unit- und Integrationstestfälle vor (`tests/board_tests.cpp`, `tests/integration_smoke.py`).
- Speziell für PB-11 wurde die Ladefunktion um eine Prüfung gegen unerwartete Zusatzdaten ergänzt.
- GitHub Actions soll die Tests auf Linux und Windows in Debug und Release prüfen; der konkrete Erfolg ist im Workflow-Verlauf nachzuweisen.
- Die Dokumente werden durch einen separat eingerichteten Read-only-Preflight auf Integrität geprüft.

**Status ist nicht gleich Abnahme:** Neue Tests sind implementiert, ihre Ergebnisse auf dem Zielsystem bzw. in der CI-Matrix sind vor einem Statuswechsel auf „abgenommen“ zu kontrollieren.
