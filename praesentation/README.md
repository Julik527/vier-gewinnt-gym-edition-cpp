# Präsentation und Codeerklärung

**Projekt:** Vier Gewinnt | Gym Edition · C++17 · ITA 2026/27  
**Ausgabe:** Vorbereitungsunterlagen · Revision 02.11.2026  
**Ziel:** Die Pflicht-Konsole verständlich präsentieren, den tatsächlichen C++-Code fachlich erklären und typische Rückfragen belegen.

> **AKTUELLE PRÜFUNG (19.11.2026):** Dr. Litera verlangt laut E-Mail vom 11.11.2026 **eine Einzelpräsentation von ca. 10 Minuten**. Die aktuelle, darauf abgestimmte Fassung steht in **[Meilenstein 10 Minuten](MEILENSTEIN_10_MIN_2026-11-19.md)**. Die weiter unten dokumentierten **15 + 15 Minuten** beziehen sich auf ein anderes bzw. späteres Vortragsformat und sind für diesen Klausurtag **nicht** die Zielzeit. Die 10 Minuten sind geplant, nicht bereits mit Stoppuhr bestätigt.

## Die fünf Arbeitsdokumente

| Ressource | Zweck | Umfang |
|---|---|---|
| **[Sprechkarten A: Projektvorstellung](SPRECHKARTEN_TEIL_A_PROJEKT.md)** | Vorlesefassung mit Zeitmarken, Aktionen und Kurzfassungen | 8 Karten, 15 Minuten |
| **[Sprechkarten B: C++-Codeerklärung](SPRECHKARTEN_TEIL_B_CODE.md)** | Quellcodegestütztes Vortragen von Klassen, Algorithmen und Datenhaltung | 8 Karten, 15 Minuten |
| **[Druckversion im Browser](SPRECHKARTEN_DRUCKVERSION.html)** | Karten im einheitlichen JK-AXIS-Layout, A5 quer und digitaler Einzelkartenmodus | 16 Druckseiten |
| **[Fachgespräch mit Kurzantworten](FACHGESPRAECH_KURZANTWORTEN.md)** | Wiederholung, Codebelege und typische Denkfehler | 31 Fragen |
| **[Checkliste zur Prüfung und Generalprobe](PRUEFUNG_CHECKLISTE.md)** | Zeitplan, Technik, Schul-PC, persönliche Nachweise und Fallback | 1 Checkliste |

## Druckversion verwenden

1. Die Datei `SPRECHKARTEN_DRUCKVERSION.html` auf GitHub öffnen und über **Raw / Download** lokal speichern.
2. Die gespeicherte HTML-Datei im Browser öffnen. Mit **Einzelkarten** durch die Karten blättern oder alle untereinander lesen.
3. Zum Drucken **Drucken / PDF** wählen, Papierformat **A5 quer**, **100 %** und **Hintergrundgrafiken** aktivieren. In der Druckvorschau prüfen, dass es **16 vollständig lesbare Seiten** sind. Falls der Druckertreiber kein A5 quer anbietet, `Als PDF speichern` verwenden und anschließend passend drucken.
4. Karten A und B getrennt stapeln. Karte A01 bzw. B01 liegt jeweils oben.

Die HTML-Ausgabe verwendet Standardschriftarten und benötigt weder Internet noch zusätzliche Schriftdateien. Die Markdown-Dateien bleiben die direkt bearbeitbaren Textfassungen; die Druckversion ist eine abgestimmte Momentaufnahme davon. Bei späteren Textänderungen muss sie erneut erzeugt werden.

## Zeitregie

**Variante 1: 15 + 15 Minuten.** Teil A beginnt bei 00:00 und endet nach 15:00; Teil B startet für sich wieder bei 00:00 und endet nach 15:00. Fragen folgen nur dann zusätzlich, wenn die Lehrkraft diese Zeit einräumt.

**Variante 2: 30 Minuten einschließlich Fragen.** Teil A auf 13:30 und Teil B auf 13:30 verkürzen, 03:00 für Rückfragen einplanen. Dazu insbesondere A08 und B08 kürzen. Die Fragezeit nicht durch hastiges Vorlesen „einsparen“.

## Empfehlungen für die Generalprobe

**Durchgang 1 – lesen:** Alle 16 Karten laut sprechen und Zeitfenster notieren. Unnatürliche Formulierungen an deinen eigenen Stil anpassen.

**Durchgang 2 – verstehen:** Nach jeder Karte nur mit dem Merksatz sprechen. Fachbegriffe bei Bedarf im Code nachsehen und nicht auswendig nachsprechen.

**Durchgang 3 – belegen:** VS Code öffnen und zu den in Teil B genannten Methoden springen. Einen Debugger-Haltepunkt in `Board::drop` beziehungsweise `Board::hasWon` setzen und die Zustandsänderung erläutern.

**Durchgang 4 – komplett proben:** Vortragsfolien, laufende Konsole, Code und mögliche Rückfragen wie in der Prüfung am tatsächlichen Gerät zeigen. Zeit, Git-Commit, Fehler und Verbesserungen in der [Prüfungs-Checkliste](PRUEFUNG_CHECKLISTE.md) dokumentieren.

## Nachweise und Grenzen

- **Pflicht:** Objektorientiertes Konsolenspiel in C++17. 2D und 3D sind optionale Erweiterungen; die stabile Konsole hat Priorität.
- **Quellcode:** Die Karten beziehen sich auf den Entwicklungsbranch `quality/gameplay-persistence-2026-10-28`, nicht automatisch auf die ältere `main`-Version.
- **CI:** [Letzter in diesen Unterlagen geprüfter C++-Lauf vom 31.10.2026](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/actions/runs/41221586099) mit neun erfolgreichen Jobs; kein Ersatz für einen manuellen Test am Vorführgerät.
- **Pull Request:** [PR #1](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/1) ist offen und der Entwicklungszweig wird durch die Karten nicht automatisch nach `main` übernommen.
- **KI-Nutzung:** Diese Lern- und Sprechunterlagen wurden mit KI-Unterstützung erarbeitet. Aussagen zu eigener Implementierung und Tests müssen mit deiner tatsächlichen Arbeit übereinstimmen.
- **Freigabe:** Keine schulische Bewertung, keine erfolgreiche Generalprobe und keine Betriebssicherheit auf deinem Schulrechner werden hier vorweggenommen.

## Weitere Projektdokumente

[Vortragsleitfaden](VORTRAGSLEITFADEN.md) · [Generalprobe](../docs/GENERALPROBE_2027.md) · [Schulanforderungen 51](../docs/SCHULANFORDERUNGEN_51_STATUS.md) · [Architektur](../docs/ARCHITECTURE.md) · [3D-Lernpfad](../docs/3D_LERNPFAD.md) · [KI-Prompt-Dokumentation](../docs/KI_PROMPT_DOKUMENTATION.md)
