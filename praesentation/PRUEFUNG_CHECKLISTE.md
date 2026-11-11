# JK AXIS | PRÜFUNGSVORBEREITUNG UND GENERALPROBE

**Projekt:** Vier Gewinnt | Gym Edition · C++17 · ITA 2026/27  
**Version:** Arbeitscheckliste vom 02.11.2026  
**Zweck:** Reproduzierbare Vorbereitung, Zeitmanagement, Belegsicherung und Störfallplan. Alle Checkboxen bleiben offen, bis sie tatsächlich nachgewiesen wurden.

## 1. Zwei zulässige Zeitmodelle

| Variante | Projekt | Code | Fachfragen / Reserve | Summe |
|---|---:|---:|---:|---:|
| A: 15 + 15 | 15:00 | 15:00 | anschließend, falls vorgesehen | 30:00 ohne Fragen |
| B: Fragen in 30 Minuten | 13:30 | 13:30 | 03:00 | 30:00 |

**Klärung vor der Prüfung:** Welches Modell die Lehrkraft tatsächlich verlangt, ist verbindlich. Die ausformulierten Karten A01–A08 und B01–B08 verwenden Modell A. Für Modell B A08 und B08 verkürzen, nicht einfach schneller sprechen.

## 2. Fachliche Zuordnung

| Prüfungsziel | Kartennummer | Vorführort |
|---|---|---|
| Ziel, Anforderungen, Feldgrößen | A01–A02 | Titel, Regeln |
| Bedienung, Eingabevalidierung | A03, A08 | Konsolenmenü, Vorschau, Undo |
| Kapselung und Klassen | A04, B01–B02 | `include/`, `src/main.cpp` |
| Schwerkraft | B03 | `Board::nextFreeRow`, `Board::drop` |
| Gewinn in vier Achsen | B04 | `Board::hasWon`, `countDirection` |
| Spielsteuerung und Undo | B05 | `Game::play`, `history_` |
| Speichern, Laden und Highscore | A06, B06 | `Game::loadGame`, `AtomicFile.h` |
| Qualität und GitHub | A07, B07 | `tests/`, `CMakeLists.txt`, GitHub Actions |
| 2D und 3D als Erweiterung | A05 | screenshots/optional vorgetestete Demo |
| Fachgespräch | F01–F31 | `FACHGESPRAECH_KURZANTWORTEN.md` |

## 3. Technische Vorbereitung am Zielgerät

- [ ] Aktuellen Git-Branch und Commit dokumentiert: `git branch --show-current`, `git rev-parse --short HEAD`.
- [ ] Quellcode und Projektordner vollständig vorhanden; C++-Pflicht-Konsole lässt sich **ohne Internet** starten.
- [ ] Die benötigten EXE-Dateien sind **vor** dem Vortrag gebaut und getestet.
- [ ] Unter Windows bei problematischen Umlaut-/ß-Pfaden das dokumentierte kurze Arbeitsverzeichnis `C:\Dev\GymEdition` verwenden, sofern vorhanden.
- [ ] Konsole mit zwei Spielern, Vorschau `V 4`, gültigem Zug, Undo, Speichern und Laden persönlich durchgespielt.
- [ ] Ein vorbereiteter, isolierter Demo-Spielstand führt nachvollziehbar zu einem Gewinn.
- [ ] Demo-Datenordner ist getrennt von persönlichen Highscores und Spielständen; Backup angelegt.
- [ ] 2D-/3D-Programm, Grafik, DPI-Skalierung, Audio und Mausklicks nur bei tatsächlicher Vorführung **am Zielgerät** getestet.
- [ ] Beide Anzeigen bzw. Beamer kontrolliert: vollständiger Bildschirm, lesbare Schrift, keine störenden Fenster.
- [ ] VS Code vorab geöffnet; Dateireihenfolge und Suchbegriffe für die Methoden bekannt.
- [ ] Offline-Kopie der Präsentation, des Codes und der Sprechkarten lokal verfügbar.

## 4. Code-Fachgespräch vorbereiten

- [ ] `Board::nextFreeRow` ohne Textvorlage mit einem konkreten 7×6-Feld erklären.
- [ ] `Board::drop` und den `int&`-Referenzparameter begründen.
- [ ] Alle vier Gewinnachsen sowie die Suche in beide Richtungen am Code erklären.
- [ ] `(current_ + 1) % Spieleranzahl` mit 2, 3 und 4 Spielern demonstrieren.
- [ ] `GYM1` und den sicheren Ladeablauf anhand eines Fehlerfalls erläutern.
- [ ] Die Grenzen der automatisierten Tests und des Dateischutzes ehrlich benennen.
- [ ] Die tatsächliche KI-Unterstützung korrekt darstellen.

## 5. Störfallplan

| Problem | Sofortmaßnahme | Sachlich richtige Aussage |
|---|---|---|
| SFML/3D startet nicht | Keine Neuinstallation im Vortrag; Pflicht-Konsole verwenden | Grafik war Zusatz; manueller Test am Vorführgerät nicht erfolgreich |
| Bild am Beamer unlesbar | Anzeige skalieren oder vorbereiteten Screenshot öffnen | Code nicht aus winzigen Zeilen erklären |
| Speicherdatei nicht lesbar | Separaten vorbereiteten Teststand verwenden; Fehlermeldung zeigen | Aktuelle Partie wird bei ungültigen Dateien nicht ersetzt |
| Zu wenig Zeit | A08-Demo kürzen, B08-Zusammenfassung kürzen | Kernalgorithmus und Abnahmestatus bleiben erhalten |
| Unerwartete Frage | Begriff definieren, passende Methode öffnen, konkretes Beispiel nennen | Unsicherheit offen eingestehen, keine Behauptung erfinden |

## 6. Messprotokoll für die echte Generalprobe

| Versuch | Datum | Git-Commit | A (mm:ss) | B (mm:ss) | Fragen (mm:ss) | Fehler / Korrektur |
|---|---|---|---|---|---|---|
| 1 | offen | offen | offen | offen | offen | offen |
| 2 | offen | offen | offen | offen | offen | offen |
| 3 | offen | offen | offen | offen | offen | offen |

**Freigaberegel:** Erst nach einem erfolgreichen Test am Zielgerät, einem verständlichen Fachgespräch und einer real gemessenen Generalprobe die jeweiligen Punkte abhaken. Diese Datei bestätigt keinerlei Abschlussprüfung.

**Querverweise:** [Karten A](SPRECHKARTEN_TEIL_A_PROJEKT.md) · [Karten B](SPRECHKARTEN_TEIL_B_CODE.md) · [Fachgespräch](FACHGESPRAECH_KURZANTWORTEN.md) · [Vortragsleitfaden](VORTRAGSLEITFADEN.md) · [Generalprobe 2027](../docs/GENERALPROBE_2027.md).
