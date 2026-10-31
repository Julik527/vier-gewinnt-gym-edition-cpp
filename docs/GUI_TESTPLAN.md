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

| G23 | Originalschullogo mit `scripts/install-school-logo.ps1` laden und GUI starten | Korrekt proportioniertes Original in weißem Bereich, Quellenangabe vorhanden | nicht geprüft | offen |
| G24 | Ohne Schullogo starten | Kein Absturz, eindeutiger Schulname als Text | nicht geprüft | offen |
| G25 | F9 mehrfach drücken | Originaler 116-BPM-Stereo-Soundtrack startet/stoppt ohne Spielstandsänderung | nicht geprüft | offen |
| G28 | F10/F11 und die Schalter -/+ bedienen | Lautstärke in 10-%-Schritten zwischen 0 und 100 %, kein Eingriff in den Spielstand | nicht geprüft | offen |
| G29 | GUI in 2/3/4-Spieler-Modi bei 100/125/150 % DPI testen | Navigation und Match-Status bleiben lesbar; kein Überlaufen von Buttons | nicht geprüft | offen |
| G30 | Startbildschirm/Spielbrett nach Redesign prüfen | JK-AXIS-Gestaltung, Spielvorschau, aktiver Spieler und Gewinner deutlich erkennbar | nicht geprüft | offen |
| G26 | Programm ohne Audioausgabegerät starten | Spiel bleibt spielbar, Hinweis statt Absturz | nicht geprüft | offen |
| G27 | `scripts/demo.ps1 -Mode Both` starten | Erst GUI, danach PowerShell-Spiel; keine gleichzeitigen Schreibzugriffe | nicht geprüft | offen |

Bei Testdurchführung Datum, Kompilerversion und echte Screenshots oder Fehlermeldungen eintragen. **CI-Tests sind kein Ersatz für diese interaktiven Prüfungen.**

## Automatisierte Nachweise und lokale Wiederholung

Am Ausgangsstand `02eb79f8` sind die GUI-Jobs unter Ubuntu und Windows erfolgreich. Geprüft wurden Release-Build, MatchSession und GUI-Kern. Unter Ubuntu kommt ein kurzer echter Fensterstart über Xvfb hinzu. Der Logo-Verpackungsschritt ist auf beiden Systemen erfolgreich. [Gesicherter CI-Snapshot](evidence/CI_SNAPSHOT_2026-10-28.json).

G01/G02 bleiben als Prüfungen der vorgesehenen lokalen Umgebung offen. Der Windows-CI-Job verwendet MSVC. Die lokale Konfiguration verwendet MSYS2 UCRT64, Ninja und Debug. Eine erfolgreiche GUI-Debug-Kompilierung wird vom bisherigen GUI-CI-Job nicht belegt.

Der zuletzt gemeldete F5-Fehler lautet „cmake wird nicht erkannt“. Die Tasks ergänzen inzwischen den UCRT64-Suchpfad. Nach der Installation aus der README beide F5-Profile erneut starten und das tatsächliche Ergebnis eintragen.

G23: Das mitgelieferte JPEG kann die dokumentierte skalierte Kopie sein. „Original“ meint das unveränderte Schulmotiv, nicht zwingend dieselbe Bytefolge wie bei Wikimedia. G25: Musik ist zunächst aus, F9 aktiviert sie. F10/F11 steuern die Lautstärke; der Soundtrack ist eine originale prozedurale Komposition. Ein Status „Playing“ ersetzt keinen Hörtest. G26: Ein fehlendes Audiogerät muss man tatsächlich provozieren, bevor der Fall als bestanden gilt.
