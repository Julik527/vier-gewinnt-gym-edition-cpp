# Meilenstein 1 und 2 – JK AXIS V6 (Leitfragenfassung)

**Projekt:** Vier Gewinnt | Gym Edition · Programmiertechnik (C++17), ITA 25/9  
**Quellenstand der Unterlagen:** 12.11.2026; das V6-Archiv basiert auf dem GitHub-Commit `1e6043659d2861f503da634e536cf91a90698ebc`.  
**Wichtig:** Die Dokumente sind vorbereitete Schulunterlagen. Eine Schul-PC-Vorführung, eine gemessene Sprechprobe oder eine Freigabe durch die Lehrkraft wird damit nicht bestätigt.

[Zur Meilensteinübersicht](../../README.md) · [Zur Projektstartseite](../../../README.md) · [Aktuelle Konsolenquellen](../../../src/) · [Schulabgabe](../../../school/README.md)

## Welcher Termin nutzt welche Unterlagen?

| Termin | Zweck | Kernunterlagen |
|---|---|---|
| 29.10.2026 | Meilenstein 1: Planung und Entwurf | [Projektplanung (PDF)](schulunterlagen/01_Meilenstein_1_Projektplanung_und_Entwurf_JK_AXIS_Abgabefassung_V5.pdf) · [DOCX](schulunterlagen/01_Meilenstein_1_Projektplanung_und_Entwurf_JK_AXIS_Abgabefassung_V5.docx) |
| 19.11.2026 | Einzelpräsentation, Ziel 10 Minuten, acht Folien | [PPTX](praesentationen/07_Einzelpraesentation_19_11_2026_10_Min_JK_AXIS.pptx) · [PDF](praesentationen/07_Einzelpraesentation_19_11_2026_10_Min_JK_AXIS.pdf) · [Sprechernotizen](vorfuehrung/14_19_11_Sprechernotizen.txt) · [Probenplan](vorfuehrung/11_Probeplan_19_11_2026_JK_AXIS_V5.pdf) |
| 26.11.2026 | Meilenstein 2: Zwei-Spieler-Konsole und Live-Demo | [M2-Dokumentation](schulunterlagen/02_Meilenstein_2_Grundversion_und_Tests_JK_AXIS_Abgabefassung_V5.pdf) · [Demoablauf](vorfuehrung/08_Live_Demo_26_11_2026.md) · [Testprotokoll](schulunterlagen/04_Test_und_Abnahmeprotokoll_M1_M2_JK_AXIS_Abgabefassung_V5.docx) |

## V6: Präsentationen und Leitfragen

- [Leitfragen und Antworten (TXT)](vorfuehrung/15_Leitfragen_und_Antworten_JK_AXIS_V6.txt)
- [12-Folien-Projektüberblick M1+M2 (PPTX)](praesentationen/06_Praesentation_Meilenstein_1_und_2_JK_AXIS.pptx) · [PDF](praesentationen/06_Praesentation_Meilenstein_1_und_2_JK_AXIS.pdf)
- [Sprechernotizen M1+M2 (TXT)](vorfuehrung/13_M1_M2_Sprechernotizen.txt) · [PDF](vorfuehrung/13_M1_M2_Sprechernotizen.pdf)
- [Sprechernotizen Einzelpräsentation (TXT)](vorfuehrung/14_19_11_Sprechernotizen.txt) · [PDF](vorfuehrung/14_19_11_Sprechernotizen.pdf)

**Abgrenzung:** Die 12-Folien-Präsentation ist ein Projektüberblick, nicht die 10-Minuten-Einzelpräsentation vom 19.11.2026. Die ursprünglichen V5-Berichte bleiben in V6 unverändert; Leitfragen und Notizen sind die Weiterentwicklung.

## Schulunterlagen und Vorführung

- [Gesamtdokument als Druckfassung (13 Seiten)](schulunterlagen/00_Gesamtdokument_Meilenstein_1_und_2_JK_AXIS_V5.pdf) – bewusst als **optionales Sammeldokument** enthalten, keine zweite Bearbeitungsquelle
- [Handout (DOCX)](schulunterlagen/03_Handout_Meilenstein_1_und_2_JK_AXIS_Abgabefassung_V5.docx) · [PDF](schulunterlagen/03_Handout_Meilenstein_1_und_2_JK_AXIS_Abgabefassung_V5.pdf)
- [Test-/Abnahmeprotokoll (DOCX)](schulunterlagen/04_Test_und_Abnahmeprotokoll_M1_M2_JK_AXIS_Abgabefassung_V5.docx) · [PDF](schulunterlagen/04_Test_und_Abnahmeprotokoll_M1_M2_JK_AXIS_Abgabefassung_V5.pdf)
- [Vortragsleitfaden/Fachgespräch (DOCX)](schulunterlagen/05_Vortragsleitfaden_und_Fachgespraech_JK_AXIS_Abgabefassung_V5.docx) · [PDF](schulunterlagen/05_Vortragsleitfaden_und_Fachgespraech_JK_AXIS_Abgabefassung_V5.pdf)
- [Probeplan 19.11. (DOCX)](vorfuehrung/11_Probeplan_19_11_2026_JK_AXIS_V5.docx) · [PDF](vorfuehrung/11_Probeplan_19_11_2026_JK_AXIS_V5.pdf)
- [Live-Demo 26.11. (DOCX)](vorfuehrung/12_Live_Demo_Offline_Abgabe_26_11_2026_JK_AXIS_V5.docx) · [PDF](vorfuehrung/12_Live_Demo_Offline_Abgabe_26_11_2026_JK_AXIS_V5.pdf)
- [Zeitplan und Abgabecheckliste](vorfuehrung/10_Zeitplan_und_Abgabecheckliste.md)

## Code und Prüfnachweise

- **Keine zweite C++-Quellcodekopie:** Das im ursprünglichen ZIP enthaltene Offline-Quellpaket aus Commit `1e6043659` wurde absichtlich nicht importiert. Der aktuelle Code liegt bereits unter [src/](../../../src/). Eine aktuelle isolierte Schulabgabe wird über [scripts/package-school-console.py](../../../scripts/package-school-console.py) erzeugt.
- [Historische Buildanleitung im V6-Archiv](vorfuehrung/09_Projekt_Bauen_und_Testen.md) – beschreibt explizit das **nicht mitimportierte** Original-Quellcode-ZIP.
- [Historischer Linux-CTest-Nachweis](nachweise/Lokaler_CTest_Linux_Release.txt) – nach Angaben des V6-Pakets sechs Tests bestanden; kein Beleg für den jetzigen `main`-Branch.
- [Repository und referenzierte CI-Läufe](nachweise/Repository_und_Quellen.txt)
- [Dateiprüfsummen des bereinigten V6-Imports](SHA256SUMS.txt)

## Datenhygiene

- Die temporäre LibreOffice-Datei `.~lock.*` wurde ausgeschlossen.
- Das veraltete, separate Konsolen-ZIP samt dessen SHA-Datei wurde **nicht** doppelt eingebracht.
- Die ursprüngliche SHA256-Liste wurde durch eine zur neuen Ordnerstruktur passende Liste ersetzt.
- DOCX/PDF und PPTX/PDF werden als unterschiedliche Ausgabeformate desselben Dokuments aufbewahrt, nicht als unabhängige Bearbeitungsstände.
- Keine bestehenden Dateien im Repository wurden für diesen Import gelöscht oder umbenannt.
