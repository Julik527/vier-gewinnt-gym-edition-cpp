# Meilensteine | Vier Gewinnt – Gym Edition

[Projektstartseite](../README.md) · [Präsentationsfassungen](PRAESENTATION_VERSIONEN.md) · [Abgaben](../docs/schule/ABGABEDATEIEN.md) · [Qualität](../docs/pruefung/README.md)

Dieser Bereich führt chronologisch durch Planung, Präsentation und Live-Demo. **Die datierten Ordner sind Wegweiser:** Sie verlinken auf Originaldateien, statt PDF-, DOCX- oder PPTX-Kopien anzulegen. Das vollständige V6-Archiv liegt einmalig unter [`meilenstein-1-und-2/v6/`](meilenstein-1-und-2/v6/README.md).

## Termine und dokumentierter Stand

| Datum | Ziel / Meilenstein | Nachweislage | Unterlagen |
|:--|:--|:--|:--|
| **29.10.2026** | Planung und früher Entwicklungsstand | Historische Dokumente vorhanden; keine aktuelle Freigabe | [Meilenstein 1](2026-10-29/README.md) |
| **19.11.2026** | Einzelpräsentation, ca. zehn Minuten | Folien und Lernunterlagen vorhanden; Generalprobe/Schul-PC offen | [Präsentation](2026-11-19/README.md) |
| **26.11.2026** | Zwei-Spieler-Konsole mit Live-Demo | Demoplan und Testvorlage vorhanden; Zielgerätetest offen | [Meilenstein 2](2026-11-26/README.md) |

## Zuerst auswählen: Präsentationsfassung

Für **19.11.2026** existieren zwei **verschiedene** 8-Folien-Präsentationen. Die im bestehenden Ordner `praesentation/` gepflegte Fassung bleibt die **aktuelle Repository-Arbeitsfassung**; die später importierte V6-Leitfragenversion ist eine **Alternative**. Der zwölf Folien umfassende M1+M2-Projektüberblick ist wiederum ein anderes Format. Alle Pfade, Leitfäden und Freigabeschritte stehen im [Präsentationsregister](PRAESENTATION_VERSIONEN.md).

## Neu: V7.1 mit Schullogo und Lehrerfeedback

Die [V7.1-Fassung](meilenstein-1-und-2/v7.1/README.md) liegt **getrennt** von V6 vor: PowerPoint und PDF mit acht Kernfolien plus optionalem Feedback für den 19.11. sowie zwölf Kernfolien plus Feedback für den M1+M2-Überblick. Originales Schullogo und Herr Funke sind integriert. Die schulische Endfassung ist noch nicht als freigegeben dokumentiert.

## V6-Archiv und Dokumentenpflege

- [Dateiindex mit Word-, PDF-, PowerPoint- und TXT-Fassungen](meilenstein-1-und-2/v6/README.md)
- [Meilenstein 1+2 und Importstatus](meilenstein-1-und-2/README.md)
- [Herkunft der unverändert übernommenen Originale](meilenstein-1-und-2/v6/HERKUNFT.md)
- [SHA-256-Manifest](meilenstein-1-und-2/v6/SHA256SUMS.txt)

Der Import wurde mit [PR #28](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/28) in `main` übernommen. Die [automatische V6-Integritätsprüfung](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/44171452189) war **für den damaligen Import-Commit** erfolgreich. Das ist **keine** bestandene Präsentation, Windows-Vorführung oder Lehrkraftabnahme.

## Qualitäts- und Freigabestufen

| Stufe | Benötigter Nachweis | Stand 17.11.2026 |
|:--|:--|:--|
| Dateiintegrität V6 | SHA-256, Dokumentcontainer, Dateiliste, relative Links | Beim Import bestanden |
| Schul-PC-Funktion | Tatsächlicher Build und Demo mit Testprotokoll | Noch nicht dokumentiert |
| Vortrag | Ausgewählte PPTX/PDF, Beamerkontrolle, gemessene Dauer | Noch nicht dokumentiert |
| Schulische Abnahme | Tatsächliche Rückmeldung der Lehrkraft | Noch nicht dokumentiert |

Neue Meilensteine bekommen einen Ordner `JJJJ-MM-TT/README.md` mit **Ziel, Ergebnis, nachgewiesenem Stand, offenen Prüfungen und relevantem Commit**. Historische Nachweise werden nicht als aktueller CI-Status ausgegeben. Originaldateien nur bei dokumentiertem Anlass ersetzen, nicht wegen eines ähnlichen Dateinamens löschen.
