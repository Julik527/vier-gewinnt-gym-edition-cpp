# Prüfung der Präsentations- und Referenzdateien

**Stand:** 11.11.2026. Dieses Dokument unterscheidet aktuelle Prüfungsunterlagen von historischen Quellen.

## Aktuelle Unterlagen für den 19.11.2026

- [PowerPoint: acht Folien mit Sprechernotizen](../praesentation/Vier_Gewinnt_Meilenstein_10_Minuten.pptx)
- [PDF: acht Seiten](../praesentation/Vier_Gewinnt_Meilenstein_10_Minuten.pdf)
- [Zehn-Minuten-Ablauf](../praesentation/MEILENSTEIN_10_MIN_2026-11-19.md)

Die Folien und das PDF wurden erfolgreich aus [dem gemeinsamen Generatorskript](../scripts/build_milestone_10min.py) erzeugt. Der technische Dokumentencheck prüft Folien-/Seitenzahlen, Notizen, Lesbarkeit der Dateiformate und lokale Markdown-Verweise. Der Textaudit meldete bei beiden aktuellen Dateien **null Fundstellen** für die definierten Werbebezeichnungen.

## Frühere 17-Folien-Präsentation

Die ältere PowerPoint-Datei und ihre PDF-Fassung wurden auf Basis des vorhandenen Decks bereinigt. Dabei wurden **17 Textabsätze** mit übertriebenen Qualitätsbezeichnungen neutralisiert. Die PDF wurde anschließend aus der bearbeiteten PPTX neu exportiert; 17 Folien/Seiten und extrahierbare Texte wurden kontrolliert. Der Originalstand bleibt über die Git-Historie rekonstruierbar.

Die alte Präsentation ist **nicht** die zeitlich passende Fassung für den 19.11.2026.

## Referenzdokumente und Originalnachweise

Die Dateien in [reference/](../reference/) umfassen ältere technische Ausarbeitungen und historische Projektkommunikation. Im ursprünglichen Audit wurden **15 Fundstellen im PDF mit alten KI-Prompts** und **eine Fundstelle in der Gesamtdokumentation** festgestellt. Die Originalzitate wurden nicht nachträglich verfälscht und sind keine aktuelle Qualitätsbeurteilung.

Die Skripte [Dokumentenprüfung](../scripts/document_preflight.py) und [Textaudit für PDF/PPTX](../scripts/artifact_language_audit.py) legen ihren Prüfungsumfang offen. Das Entfernen von Textlabels und ein erfolgreicher automatischer Check garantieren **keine** vollständige visuelle Prüfung. PowerPoint auf dem Schulrechner, Folienumbrüche, Beamer-Lesbarkeit und eine laut gestoppte Probe bleiben manuell abzunehmen.
