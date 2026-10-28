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

| T28 | Sieg → Ergebnisoption 1 | Revanche mit denselben Spielern und leerem Feld | | |
| T29 | Zwei Revanche-Siege | Highscore um genau 2 erhöht | | |
| T30 | Sieg → Ergebnisoption 2 | Spieleranzahl und Namen erneut abfragen | | |
| T31 | Sieg → Ergebnisoption 0 | Hauptmenü, keine laufende Partie | | |
| T32 | Ergebnisoption 8 oder Text | Auswahlfehler, Ergebnisbildschirm bleibt | | |
| T33 | H, I oder ungültiger Befehl | Unverändertes Spielfeld nicht wiederholt ausgeben | | |
| T34 | Spiel starten → M | Hauptmenü zeigt Spielernamen, nächsten Spieler, belegte Felder | | |
| T35 | Animation A ein/aus | Keine ANSI-Bildschirmlöschung, Terminal bleibt lesbar | | |

| T36 | Starten im Windows Terminal | JK AXIS-Kopf, Gym-Motto und saubere Zellbreiten sichtbar | | |
| T37 | Zwei Spieler, erster Zug | Namen, aktuelle Spielerfarbe, Siege und Hantelzeichen sichtbar | | |
| T38 | Vier Spieler | Alle 11 Spalten und 10 Reihen vollständig sichtbar | | |
| T39 | Gewinn durch vier Steine | Genau die Gewinnreihe mit geschweiften Klammern hervorgehoben | | |
| T40 | NO_COLOR oder umgeleitete Ausgabe | Alle Symbole auch ohne Farben unterscheidbar | | |
| T41 | Vorschau und Undo | Anzeige korrekt, Spiellogik unverändert | | |
| T42 | Revanche nach Sieg | Neues leeres Feld, Namen/Symbole bleiben, Gesamtsiege erhalten | | |

Belege in `docs/evidence/` ablegen. Manuelle Tests erst nach der tatsächlichen Durchführung als bestanden markieren.