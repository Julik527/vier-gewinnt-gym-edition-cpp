# Upload-Nachweis: PDF- und Präsentationsdateien

**Aktualisierter Stand: 28.10.2026.** Alle acht vorgesehenen PDF-/PPTX-Dateien sind vorhanden. Der erste Upload bleibt mit dem historischen Commit belegt. Die aktuellen Referenz-PDFs enthalten neue Nachträge und ausdrücklich historische Originalteile.

**Nachweis-Commit:** https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/commit/53612455ee21bf4363b7d5627fe8cd388a8c7e8b

## Ordner `reference/` im Repository-Stamm

- [x] [Anforderungsanalyse](../reference/Anforderungsanalyse_JK-CD-DE-2026-003.pdf) (23 Seiten)
- [x] [UML-Klassendiagramm](../reference/UML_Klassendiagramm_JK-CD-DE-2026-003.pdf) (11 Seiten)
- [x] [Meilenstein-Prüfbericht](../reference/Meilenstein_2026-10-29_Pruefbericht.pdf) (10 Seiten)
- [x] [KI-Prompt-Dokumentation](../reference/KI_Prompt_Dokumentation_JK-CD-DE-2026-003.pdf) (27 Seiten)
- [x] [Gesamtdokumentation](../reference/Gesamt_Dokumentation_Vier_Gewinnt_Gym_Edition.pdf) (92 Seiten)

## Ordner `praesentation/` im Repository-Stamm

- [x] [PowerPoint](../praesentation/Vier_Gewinnt_Praesentation_FINAL.pptx)
- [x] [Präsentation als PDF](../praesentation/Vier_Gewinnt_Praesentation_FINAL.pdf)
- [x] [Vortragsleitfaden](../praesentation/Vier_Gewinnt_Vortragsleitfaden_FINAL.pdf)

Die Dateien wurden im Root-Bereich `reference/` und `praesentation/` hochgeladen, **nicht** unter `docs/`. Das ist gültig. Die Dateiliste wurde anhand des GitHub-Commits geprüft; separate visuelle Prüfung der binären Downloads am Zielgerät steht ggf. noch aus.

## Branding-Update vom 28.10.2026

Die acht publizierten Dateien wurden am 28.10.2026 durch einen einmaligen GitHub-Actions-Migrationslauf verarbeitet (Commit [`ea8bbef8`](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/commit/ea8bbef88c2579c73c701c8f81828b202c36f46d)). Der schreibende Migrations-Workflow wurde anschließend entfernt. Der Nachweis und die Abgrenzung der noch offenen Sichtprüfung stehen im [Branding-QA-Bericht](BRAND_QA_REPORT.md). Die neue [Dokumenten-Prüfroutine](../scripts/document_preflight.py) arbeitet nur lesend. Die ursprünglichen Dateipfade blieben zur Wahrung bestehender Links erhalten. Ein unveränderter Dateiname mit älterer Design-ID bedeutet nicht, dass im Dokument weiterhin das alte Logo verwendet wird.

## Weitere Schritte
Den Build und die Spielabläufe vor der schulischen Abnahme in der vorgesehenen Windows-Umgebung durchführen. Keine EXE-Dateien, persönlichen Spielstände oder lokalen Highscores veröffentlichen.


## Aktuelle Dokumentationsrunde

Die 17 Folien der PowerPoint und ihre PDF-Fassung wurden auf Konsole plus SFML-2D-GUI aktualisiert. Der Vortragsleitfaden umfasst jetzt drei Seiten. Das KI-PDF enthält P-01 bis P-34 und den historischen Originalteil. UML enthält zusätzlich ein aktuelles vektorbasiertes Klassenmodell. Die fünf Referenz-PDFs behalten ihre früheren Seiten als gekennzeichnetes Archiv.

Der Gesamtbestand umfasst 183 PDF-Seiten und 17 PPTX-Folien. [Dateigrößen, Seitenzahlen und SHA-256](evidence/DOCUMENT_ARTIFACTS_2026-10-28.json) sowie [vollständiges Audit](DOCUMENTATION_AUDIT_2026-10-28.md). Der lokale PowerPoint-/Beamer- und Windows-Spieltest bleibt davon getrennt.
