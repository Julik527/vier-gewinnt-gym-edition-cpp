# C++-Dateiköpfe: Struktur und Zweck

Alle **36 C++-Dateien** im Projekt (Header, Implementierungen, Tests und
Übungen) besitzen einen einheitlichen, deutschsprachigen Dateikopf im
**Doxygen-Format**. Die Informationen sind bewusst auf den tatsächlichen
Dateiinhalt abgestimmt.

## Felder

| Doxygen-Feld | Bedeutung |
|---|---|
| `@file` | Eindeutiger relativer Pfad der Quellcodedatei |
| `@brief` | Ein Satz zur Hauptaufgabe dieser Datei |
| `@details` | Zentrale Funktionen, Zusammenarbeit und Datenfluss |
| `@par Abhängigkeiten` | Verwendete Klassen, Standardbibliothek, SFML oder OpenGL |
| `@par Wichtige Randbedingungen` | Grenzen, Fehlerbehandlung und überprüfbare Besonderheiten |

**Beispiel:** In `include/Board.h` erklärt der Kopf die Feldregeln,
Indizierung und zulässigen Symbole. In `src/gui/GymAudio.h` erklärt
er das OGG-Streaming, Crossfades und den Verzicht auf den alten
Synthesizer-Fallback. Die Details stehen jeweils im zugehörigen Code.

## Was nicht in den Dateikopf gehört

- Eine erfundene Versionsnummer, ein ungeprüftes Qualitätsversprechen
  oder ein automatisch veraltendes Tagesdatum.
- Eine pauschale Lizenzbehauptung. Die Nutzung des Quellcodes und der
  zugelieferten Audiodateien wird getrennt in `docs/LICENSING.md`
  und den Audio-Unterlagen beschrieben.
- Zeilenweise Nacherzählung offensichtlicher Syntax.

Für die konkrete Funktionsweise gelten weiterhin die Kommentare direkt an
Methoden und wichtigen Verzweigungen. Der
[Quellcode-Leitfaden](QUELLCODE_KOMMENTARE.md) erklärt die Klassenfolge vom
Konsolen-Einstieg bis zur 3D-Grafik.

## Dauerhafte Prüfung

Der Test `tests/source_prologues_test.py` kontrolliert die Doxygen-Felder
und die korrekten `@file`-Pfade. Er verwendet nur Python 3 und verändert
keine Dateien:

```bash
python tests/source_prologues_test.py
```

Der Test gehört zu den GitHub-Dokumentationsprüfungen. Ein bestandener Test
belegt die formale Struktur, nicht die vollständige inhaltliche Richtigkeit
jedes erklärenden Satzes.
