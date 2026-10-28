# Manueller Testplan – Vier Gewinnt | Gym Edition

**Kennzeichnung: NOCH AUSZUFÜHREN.** Ergebnisse nach eigener Ausführung ergänzen.

| ID | Eingabe / Aktion | Soll-Verhalten | Ist / Beleg | Bestanden? |
|:--|:--|:--|:--|:--|
| T01 | Menüpunkte 8, abc | Ablehnung, neues Menü | | |
| T02 | Spieleranzahl 1 oder 5 | Neue Frage | | |
| T03 | Leerer Name | Wiederholen | | |
| T04 | Doppelte Namen | Wiederholen | | |
| T05 | Zwei Spieler | 7×6 | | |
| T06 | Drei Spieler | 9×8 | | |
| T07 | Vier Spieler | 11×10 | | |
| T08 | Vorschau V 4 | Keine Statusänderung | | |
| T09 | U für Undo | Letzter Zug entfernt | | |
| T10 | Volle Spalte | Fehler ohne Spielerwechsel | | |
| T11 | Vier waagerecht | Sieg | | |
| T12 | Vier senkrecht | Sieg | | |
| T13 | Diagonal in beiden Richtungen | Sieg | | |
| T14 | Feld voll ohne Sieger | Remis | | |
| T15 | Speichern, Menü, Laden | Spielstand identisch | | |
| T16 | Defekte Speicherdatei | Fehlermeldung, kein Absturz | | |
| T17 | Gewinn | Highscore +1, sortiert | | |
| T18 | Neustart | Highscore erhalten | | |
| T19 | 1 → Spieleranzahl 2 → Namen nacheinander | Sofortiger Spielstart ohne Zurück zum Hauptmenü | | |
| T20 | 1 → Spieleranzahl 3 und 4 | Alle Namen einzeln, richtige Spielfeldgröße | | |
| T21 | Neues Spiel → Spieleranzahl 0 | Abbruch ohne Spielstart | | |
| T22 | Neues Spiel → Namen: 0 | Abbruch ohne Verlust einer vorhandenen Runde | | |
| T23 | Während der Runde M → Hauptmenü 2 | Runde mit demselben Feld und Spieler fortsetzen | | |
| T24 | Laufende Runde → 1 → Nein | Spielstand bleibt erhalten | | |
| T25 | Laufende Runde → 1 → Ja → Abbruch der Namen | Alte Runde bleibt erhalten | | |
| T26 | Laufende Runde → 3 → Nein | Kein Spielstand wird überschrieben | | |
| T27 | Laufende Runde → 0 → Nein | Programm kehrt ins Hauptmenü zurück | | |

Für das neue Hauptmenü gilt **0–5**. Die Punkte Spieleranzahl und Namen befinden sich im geführten Ablauf unter **1 Neues Spiel**, nicht als eigene Hauptmenüpunkte.

Belege in `docs/evidence/` ablegen. Manuelle Tests erst nach der tatsächlichen Durchführung als bestanden markieren.