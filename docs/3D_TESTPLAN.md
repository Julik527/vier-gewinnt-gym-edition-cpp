# Gym Edition 3D - manueller Testplan

**Status:** Vor-Ort-Tests noch offen. CI-Ergebnisse zählen nur für die dort tatsächlich ausgeführten automatisierten Prüfungen.

| ID | Test / Erwartung | Ergebnis |
|---|---|---|
| D01 | Frischer Windows-UCRT64-3D-Build unter kurzem ASCII-Pfad | offen |
| D02 | F5-Start aus dem echten VS-Code-Workspace `C:\Dev\GymEdition` | offen |
| D03 | Intro läuft etwa vier Sekunden und ist überspringbar | offen |
| D04 | 2/3/4 Spieler und 7×6, 9×8, 11×10 Feldgrößen | offen |
| D05 | Alle Spieler geben eindeutige Namen ein | offen |
| D06 | Linksklick auf äußerste Spalten funktioniert | offen |
| D07 | Pfeiltasten/Leertaste setzen denselben Zug wie Maus | offen |
| D08 | Kein doppelter Zug während der Fallanimation | offen |
| D09 | Volle Spalte ändert keine Spielreihenfolge | offen |
| D10 | Kameradrehung mit rechter Maus, A/D und Q/E | offen |
| D11 | Zoom und Kameragrenzen ohne Sichtfehler | offen |
| D12 | C stellt die Kamera zurück | offen |
| D13 | Tiefe, Licht und Material ohne störendes Flimmern | offen |
| D14 | Gewinn in vier Richtungen und animierter Siegereffekt | offen |
| D15 | Undo und Revanche erhalten korrekte Spiellogik | offen |
| D16 | Speichern, Neustart, Laden, Highscore | offen |
| D17 | Beschädigte Speicherdatei wird abgewiesen | offen |
| D18 | F9/F10/F11 und Betrieb ohne Audiogerät | offen |
| D19 | Fenster-X bei laufender Runde fragt nach Bestätigung | offen |
| D20 | Windows-Skalierung 100/125/150 % und Größenänderung | offen |
| D21 | 30-Minuten-Probe auf Schul-PC und Beamer | offen |

Nach realer Durchführung eintragen: Datum, Commit, Compiler, GPU/Treiber, Windows-Version, DPI, erwartetes und tatsächliches Verhalten, Screenshot/Fehlermeldung und Status. Keine Häkchen ohne Nachweis.

## Neue Abnahmetests: Kamera und Datensicherheit

| ID | Test / Erwartung | Ergebnis |
|---|---|---|
| D22 | Mit gedrehter/gekippter Kamera die äußersten und unteren Zellen anklicken | Richtige Spalte; kein Zug außerhalb des Brettes | offen |
| D23 | Maus ohne Klick über das Brett bewegen | Auswahl folgt dem Cursor, kein Stein wird gesetzt | offen |
| D24 | Beim Kameradrehen Fensterfokus verlieren und Maus lösen | Keine dauerhaft blockierte Drag-Geste | offen |
| D25 | `data` absichtlich schreibschützen und „Sichern und beenden“ wählen | Schreibfehler hält die laufende Partie offen; keine stille Datenlöschung | offen |
| D26 | Highscore-Dateiziel blockieren, einen Sieg erzielen und nach Freigabe „Highscore erneut sichern“ betätigen | Warnung bei Fehler, danach korrekter gespeicherter Sieg ohne Doppelzählung | offen |


## CC0-Audio in der 3D-Version

| ID | Test / Erwartung | Ergebnis |
|---|---|---|
| D27 | Acht originale OGG-Dateien installieren, F9 aktivieren | Sci-Fi-Audio startet ohne Interaktionsverlust | offen |
| D28 | Intro, Hauptmenü, Namenseingabe, Spiel, Ergebnis | Musik folgt der Szene, Übergänge ohne harte Sprünge | offen |
| D29 | Nach mindestens 75 % belegten Feldern weiter spielen | Tense-Stimmung wechselt zu Urgent ohne Spielstandsänderung | offen |
| D30 | Sieg auslösen und Menübuttons anklicken | Victory/hover hörbar und angenehm abgemischt | offen |
| D31 | OGG-Dateien fehlen oder Audiogerät nicht erreichbar | Originales Musikthema als Fallback beziehungsweise stille Spielbarkeit | offen |
