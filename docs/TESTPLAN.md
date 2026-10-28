# Manueller Testplan – Vier Gewinnt | Gym Edition

**Status:** Automatisierte Prüfungen werden über CTest/GitHub Actions durchgeführt. Die hier aufgelisteten interaktiven Windows-Tests sind nicht ausgeführt, sofern kein echtes Testprotokoll vorliegt.

## Hauptmenü, Spielregeln und Speicherung

| ID | Eingabe / Aktion | Soll-Verhalten | Tatsächlich beobachtet | Status |
|:--|:--|:--|:--|:--|
| T01 | Menüpunkte 8, abc | Ablehnung, neues Menü | nicht ausgeführt | offen |
| T02 | Spieleranzahl 1 oder 5 | Neue Frage | nicht ausgeführt | offen |
| T03 | Leerer Name | Wiederholen | nicht ausgeführt | offen |
| T04 | Doppelte Namen | Wiederholen | nicht ausgeführt | offen |
| T05 | Zwei Spieler | 7×6 | nicht ausgeführt | offen |
| T06 | Drei Spieler | 9×8 | nicht ausgeführt | offen |
| T07 | Vier Spieler | 11×10 | nicht ausgeführt | offen |
| T08 | Vorschau V 4 | Keine Statusänderung | nicht ausgeführt | offen |
| T09 | U für Undo | Letzter Zug entfernt | nicht ausgeführt | offen |
| T10 | Volle Spalte | Fehler ohne Spielerwechsel | nicht ausgeführt | offen |
| T11 | Vier waagerecht | Sieg | nicht ausgeführt | offen |
| T12 | Vier senkrecht | Sieg | nicht ausgeführt | offen |
| T13 | Diagonal in beiden Richtungen | Sieg | nicht ausgeführt | offen |
| T14 | Feld voll ohne Sieger | Remis | nicht ausgeführt | offen |
| T15 | Speichern, Menü, Laden | Spielstand identisch | nicht ausgeführt | offen |
| T16 | Defekte Speicherdatei | Fehlermeldung, kein Absturz | nicht ausgeführt | offen |
| T17 | Gewinn | Highscore +1, sortiert | nicht ausgeführt | offen |
| T18 | Neustart | Highscore erhalten | nicht ausgeführt | offen |
| T19 | 1 → Spieleranzahl 2 → Namen nacheinander | Sofortiger Spielstart ohne Zurück zum Hauptmenü | nicht ausgeführt | offen |
| T20 | 1 → Spieleranzahl 3 und 4 | Alle Namen einzeln, richtige Spielfeldgröße | nicht ausgeführt | offen |
| T21 | Neues Spiel → Spieleranzahl 0 | Abbruch ohne Spielstart | nicht ausgeführt | offen |
| T22 | Neues Spiel → Namen: 0 | Abbruch ohne Verlust einer vorhandenen Runde | nicht ausgeführt | offen |
| T23 | Während der Runde M → Hauptmenü 2 | Runde mit demselben Feld und Spieler fortsetzen | nicht ausgeführt | offen |
| T24 | Laufende Runde → 1 → Nein | Spielstand bleibt erhalten | nicht ausgeführt | offen |
| T25 | Laufende Runde → 1 → Ja → Abbruch der Namen | Alte Runde bleibt erhalten | nicht ausgeführt | offen |
| T26 | Laufende Runde → 3 → Nein | Kein Spielstand wird überschrieben | nicht ausgeführt | offen |
| T27 | Laufende Runde → 0 → Nein | Programm kehrt ins Hauptmenü zurück | nicht ausgeführt | offen |

Das Hauptmenü bietet **0–5**; Spieleranzahl und Namen sind Teil des geführten Spielstarts.

## Rundenende und Bedienoberfläche

| ID | Eingabe / Aktion | Soll-Verhalten | Tatsächlich beobachtet | Status |
|:--|:--|:--|:--|:--|
| T28 | Sieg → Ergebnisoption 1 | Revanche mit denselben Spielern und leerem Feld | nicht ausgeführt | offen |
| T29 | Zwei Revanche-Siege | Highscore um genau 2 erhöht | nicht ausgeführt | offen |
| T30 | Sieg → Ergebnisoption 2 | Spieleranzahl und Namen erneut abfragen | nicht ausgeführt | offen |
| T31 | Sieg → Ergebnisoption 0 | Hauptmenü, keine laufende Partie | nicht ausgeführt | offen |
| T32 | Ergebnisoption 8 oder Text | Auswahlfehler, Ergebnisbildschirm bleibt | nicht ausgeführt | offen |
| T33 | H, I oder ungültiger Befehl | Unverändertes Spielfeld nicht wiederholt ausgeben | nicht ausgeführt | offen |
| T34 | Spiel starten → M | Hauptmenü zeigt Spielernamen, nächsten Spieler, belegte Felder | nicht ausgeführt | offen |
| T35 | Animation A ein/aus | Keine ANSI-Bildschirmlöschung, Terminal bleibt lesbar | nicht ausgeführt | offen |

## Windows 11 – Interaktiver Praxistest mit VS Code und MSYS2 UCRT64

| ID | Testschritt | Erwartetes Verhalten | Tatsächlich beobachtet | Status |
|:--|:--|:--|:--|:--|
| T36 | Starten im Windows Terminal | JK AXIS-Kopf, Gym-Motto und saubere Zellbreiten sichtbar | nicht ausgeführt | offen |
| T37 | Zwei Spieler, erster Zug | Namen, aktuelle Spielerfarbe, Siege und Hantelzeichen sichtbar | nicht ausgeführt | offen |
| T38 | Vier Spieler | Alle 11 Spalten und 10 Reihen vollständig sichtbar | nicht ausgeführt | offen |
| T39 | Gewinn durch vier Steine | Genau die Gewinnreihe mit geschweiften Klammern hervorgehoben | nicht ausgeführt | offen |
| T40 | NO_COLOR oder umgeleitete Ausgabe | Alle Symbole auch ohne Farben unterscheidbar | nicht ausgeführt | offen |
| T41 | Vorschau und Undo | Anzeige korrekt, Spiellogik unverändert | nicht ausgeführt | offen |
| T42 | Revanche nach Sieg | Neues leeres Feld, Namen/Symbole bleiben, Gesamtsiege erhalten | nicht ausgeführt | offen |
| T43 | Projektordner oeffnen, F5 ausloesen | `tasks.json` kompiliert alle vier C++-Dateien und GDB startet die EXE | nicht ausgeführt | offen |
| T44 | Im integrierten Terminal 2-Spieler-Partie bis zum Sieg durchspielen | Eindeutige Zuganzeige, Gewinnreihe sichtbar, Ergebnisoptionen funktionieren | nicht ausgeführt | offen |
| T45 | Revanche, Sieg, Anwendung schliessen und neu starten | Gleiche Namen bei Revanche, persistenter Gesamt-Highscore | nicht ausgeführt | offen |
| T46 | Vorhandenen `GYM1`-Stand sichern, Schreiben mit gesperrter Temp-Datei provozieren | Fehlermeldung; Originalspielstand bytegenau unveraendert | nicht ausgeführt | offen |
| T47 | 4 Spieler mit langen Namen, ANSI-Farben an/aus | Kein Umbruch in der Spieleranzeige; ohne Farbe eindeutig | nicht ausgeführt | offen |

## Dokumentation der Ergebnisse

Trage nach einem tatsächlichen Test das beobachtete Verhalten und den Status **bestanden** oder **fehlgeschlagen** ein. Dokumentiere bei Fehlern die Reproduktionsschritte und die anschließende Korrektur. Bis dahin bleibt der Status **offen**.

Testbelege können unter `docs/evidence/` abgelegt werden. Der Testplan selbst ist kein Nachweis für einen erfolgreichen manuellen Test.
