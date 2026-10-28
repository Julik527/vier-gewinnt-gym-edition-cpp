# Gym Edition 2D – Windows-11-Praxistest

**Status:** Noch nicht interaktiv durchgeführt. Automatische Builds und fensterlose CTest-Läufe werden getrennt in GitHub Actions dokumentiert.

**Testumgebung:** Windows 11, VS Code, CMake 3.28+, Ninja, MSYS2 UCRT64 (GCC/GDB), SFML 3.1.0 über CMake. Alternativ ein unterstützter Visual-Studio-MSVC-Build.

| ID | Testschritt | Erwartetes Ergebnis | Tatsächlich beobachtet | Status |
|:--|:--|:--|:--|:--|
| G01 | CMake mit `GYM_BUILD_GUI=ON` konfigurieren | SFML 3.1.0 wird gefunden/gebaut | nicht geprüft | offen |
| G02 | `vier-gewinnt-gui` mit Debug und Release bauen | Keine Compiler- oder Linkerfehler | nicht geprüft | offen |
| G03 | Neues F5-Profil `Gym Edition 2D` in VS Code starten | Eigenständiges 2D-Fenster erscheint | nicht geprüft | offen |
| G04 | Fenster minimieren, wiederherstellen, größer/kleiner ziehen | UI bleibt proportional und bedienbar | nicht geprüft | offen |
| G05 | Neues Spiel, zwei Personen, Namen eingeben | Zuordnung sichtbar, 7x6-Feld | nicht geprüft | offen |
| G06 | Spiel mit drei Personen starten | 9x8-Feld, korrekte Zugfolge | nicht geprüft | offen |
| G07 | Spiel mit vier Personen starten | 11x10-Feld, alle Spalten anklickbar | nicht geprüft | offen |
| G08 | Namen mit Umlauten eingeben und korrigieren | UTF-8-Namen sichtbar, Backspace korrekt | nicht geprüft | offen |
| G09 | Eingaben abbrechen, vorhandene Runde fortsetzen | Vorhandenes Spielfeld bleibt erhalten | nicht geprüft | offen |
| G10 | Mit Maus Hantel in gewählte Spalte setzen | Stein landet durch Schwerkraft korrekt | nicht geprüft | offen |
| G11 | Mit Pfeiltasten/Leertaste dieselbe Aktion ausführen | Dieselben Spielregeln und Spielerwechsel | nicht geprüft | offen |
| G12 | Animation aktiviert, volle Spalte anklicken | Sichtbarer Fall in Frames, volle Spalte abgewiesen | nicht geprüft | offen |
| G13 | Während Fall mehrere Eingaben ausführen | Keine doppelten Züge oder beschädigte Zustände | nicht geprüft | offen |
| G14 | Animation ausschalten, Zug wiederholen | Direkter Zug ohne Fall, gleicher Endzustand | nicht geprüft | offen |
| G15 | Undo, Menü, Fortsetzen, Anleitung | Keine ungewollten Zugänderungen | nicht geprüft | offen |
| G16 | GYM1-Spielstand speichern, Programm neu starten und laden | Spieler, Raster, Zugfolge stimmen | nicht geprüft | offen |
| G17 | Beschädigte Speicherdatei laden | Warnung, kein Verlust der laufenden Partie | nicht geprüft | offen |
| G18 | Viererreihe horizontal, vertikal, diagonal spielen | Gewinnername, vier markierte Felder | nicht geprüft | offen |
| G19 | Spiel gewinnen, Revanche starten | Gleiche Spieler, leeres Feld, Gesamtpunkt bleibt | nicht geprüft | offen |
| G20 | Zweimal gewinnen, GUI neu starten, Bestenliste öffnen | Exakt zwei Gesamtsiege und korrekte Sortierung | nicht geprüft | offen |
| G21 | Bei laufender Partie `Neues Spiel` oder `Laden` wählen und ablehnen | Kein Datenverlust | nicht geprüft | offen |
| G22 | Programm über Fenster-X oder Menü beenden | Fenster schließt sicher, ggf. mit Bestätigung | nicht geprüft | offen |

Bei Testdurchführung Datum, Kompilerversion und echte Screenshots oder Fehlermeldungen eintragen. **CI-Tests sind kein Ersatz für diese interaktiven Prüfungen.**
