# Manuelles Testprotokoll | Konsolen-Schulabgabe

**Datum:** __________  **Commit/ZIP SHA-256:** __________  
**Windows/Linux:** __________  **Compiler:** __________  
**Rechner / Terminal:** __________  **Testperson:** __________

Die Spalte **Ist-Ergebnis** bleibt bis zur echten Durchführung leer. Nicht aus automatischen CI-Tests abschreiben.

| ID | Durchführung | Erwartetes Ergebnis | Ist-Ergebnis | OK/Fehler |
|---|---|---|---|---|
| K01 | Programm starten, Hauptmenü anzeigen | Menü vollständig, keine Fehlermeldung | | |
| K02 | 2 Spieler und eindeutige Namen eingeben | 7 × 6, Start bei Spieler 1 | | |
| K03 | 3 Spieler neu starten | 9 × 8, richtige Reihenfolge | | |
| K04 | 4 Spieler neu starten | 11 × 10, alle Spalten sichtbar | | |
| K05 | Ungültige Spielerzahl, doppelte/leere Namen | Verständliche Ablehnung, keine beschädigte Partie | | |
| K06 | Ungültige Spaltenzahlen und volle Spalte | Kein Stein, kein falscher Spielerwechsel | | |
| K07 | Vier horizontale Steine zum Sieg bringen | Richtige Siegmeldung | | |
| K08 | Vertikalen Sieg erzeugen | Richtige Siegmeldung | | |
| K09 | Sieg über beide Diagonalrichtungen getrennt erzeugen | Jeweils korrekte Gewinnerkennung | | |
| K10 | Remis-Situation vorführen | Unentschieden ohne falschen Sieger | | |
| K11 | Zugvorschau und Undo prüfen | Vorschau verändert nichts, Undo korrekt | | |
| K12 | Laufende Partie speichern, Programm schließen, neu starten, laden | Raster, Reihenfolge und Spielernamen korrekt | | |
| K13 | Highscore nach Sieg und Neustart öffnen | Sieger genau einmal gezählt, Liste sortiert | | |
| K14 | Beschädigte GYM1-Datei laden | Fehler erkannt, laufender gültiger Zustand erhalten | | |
| K15 | Schreibziel schreibschützen | Verständliche Fehlermeldung, vorhandene Datei bleibt erhalten | | |
| K16 | Hilfe, Revanche, Spielabbruch | Menüs nachvollziehbar, keine unerwartete Beendigung | | |
| K17 | Handout, Quellcode und Demo am Schul-PC | Schrift und vollständiges Spielfeld lesbar | | |
| K18 | Fachgespräch ohne Skript führen | Algorithmus, Kapselung und Fehlerfälle erklärbar | | |

## Beobachtungen und Korrekturen

**Test-ID / Fehler:** ______________________________  
**Schritte zur Reproduktion:** _____________________  
**Tatsächliches Verhalten:** _______________________  
**Behoben in Commit:** _____________________________  
**Nachtest am:** ___________________________________

## Freigabe

- [ ] Alle Pflichtfälle tatsächlich durchgeführt.
- [ ] Offene Fehler dokumentiert und fachlich bewertet.
- [ ] Programm, Handout und Präsentation stimmen mit der Schulaufgabe überein.
- [ ] Persönliche Vorführung erfolgreich.

**Freigabevermerk / Datum:** ________________________
