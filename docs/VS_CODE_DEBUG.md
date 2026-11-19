# VS Code: Konsole, 2D und 3D bauen und debuggen

**Stand:** 05.11.2026. Projekt: Vier Gewinnt | Gym Edition (C++17).

## Was war kaputt?

Nach einem lokalen `git merge origin/main` können gleichzeitig veränderte Dateien
Konfliktmarkierungen enthalten:

```text
<<<<<<< HEAD
...
=======
...
>>>>>>> origin/main
```

Das ist weder gültiges JSON noch gültiger C++-Quellcode und darf nicht
im Projekt verbleiben. Ein lokaler, nicht abgeschlossener Merge kann **nicht**
durch einen Remote-Commit auf GitHub repariert werden. Wenn der Merge nicht
bereits bearbeitet wurde, zunächst lokal `git merge --abort` ausführen.
Eigene uncommittete Änderungen vor weiteren Git-Schritten sichern.

Zweiter Fehler: Auf dem Entwicklungsbranch waren die 2D/3D-Aufgaben in
`.vscode/tasks.json` vorhanden, jedoch fehlten die `preLaunchTask`-Verweise
in `.vscode/launch.json`. So wurde F5 ohne Neubau ausgeführt.

## Aktuelle drei Startprofile

| F5-Profil | Build-Task | Ausgabedatei |
|---|---|---|
| Vier Gewinnt | Gym Edition | Vier Gewinnt bauen | `vier-gewinnt.exe` |
| Vier Gewinnt | Gym Edition 2D | Gym Edition 2D bauen | `build-gui/vier-gewinnt-gui.exe` |
| Vier Gewinnt | Gym Edition 3D | Gym Edition 3D bauen | `build-3d/vier-gewinnt-3d.exe` |

Die Grafik-Aufgaben rufen zuerst die passende CMake-Konfiguration auf
(`GYM_BUILD_GUI=ON` bzw. `GYM_BUILD_3D=ON`) und kompilieren anschließend
mit Ninja. Die 2D- und 3D-Quellcodes befinden sich in `src/gui/Main.cpp`
bzw. `src/gui/Main3D.cpp`.

## Ein VS-Code-Fenster: kein manuelles cd, GitHub integriert (Windows)

Die gemeinsame Datei `.vscode/settings.json` startet **neue** integrierte
PowerShell-Terminals im geöffneten Projektordner (`${workspaceFolder}`).
Sie aktiviert außerdem `git.autofetch`, damit VS Code regelmäßig die
Remote-Informationen abruft. Das **pusht keine Änderungen** automatisch.

1. Das Repository unter `C:\Dev\GymEdition` einmal in VS Code öffnen:
   **Datei → Ordner öffnen** und im bestehenden Fenster bestätigen. Alternativ
   aus einer PowerShell: `code --reuse-window C:\Dev\GymEdition`.
2. Bereits offene Terminals mit altem Verzeichnis schließen und mit
   **Terminal → Neues Terminal** neu öffnen. Die Eingabeaufforderung sollte
   `PS C:\Dev\GymEdition>` zeigen; ein `cd` ist nicht mehr notwendig.
3. **Ausführen und Debuggen** öffnen, eines der drei Profile (Konsole / 2D /
   3D) auswählen und **F5** drücken. `preLaunchTask` erledigt den Build.
   Die vorhandenen `launch.json`- und `tasks.json`-Pfade sind
   projektordnerbezogen und benötigen keine Anpassung für einzelne Benutzer.
4. Für GitHub **Quellcodeverwaltung** (`Strg+Umschalt+G`) nutzen.
   Lokale Änderungen prüfen, committen und mit **Push/Synchronisieren**
   übertragen. Vorher nötigenfalls Pull ausführen. Die `origin`-Verbindung
   wird beim Klonen im lokalen `.git/config` gespeichert; sie muss nicht
   vor jedem Start neu eingerichtet werden.

**Wichtig:** Diese Einstellungen wirken nur, wenn der richtige Repository-Ordner
geöffnet ist. Sie verschieben kein bereits offenes Terminal, konfigurieren
nicht automatisch ein zweites lokales Repository und ändern nicht die
benutzerweite VS-Code-Fensteroption. Wer `C:\Dev\GymEdition` nicht
im selben Fenster öffnet, kann weiter im alten Projektpfad landen.
Der erste 2D-/3D-Build benötigt weiterhin CMake, Ninja, MSYS2 und SFML.
Optional installierte OGG-Musikdateien werden nicht durch Git mitgeliefert.

## Windows-Voraussetzungen

- VS Code mit Microsoft C/C++-Erweiterung (`cppdbg`)
- MSYS2 UCRT64 mit `g++.exe`, `gdb.exe` und `gcc.exe`
- CMake **3.28 oder neuer** und Ninja für 2D und 3D
- Für 2D/3D SFML 3.1 und seine Build-Abhängigkeiten; falls SFML fehlt,
  lädt CMake die konfigurierte Version 3.1.0 über FetchContent
- OpenGL-Treiber und nutzbares Grafikgerät für die interaktive 3D-Version

## Vorgehen

1. VS Code im **Repository-Stammordner** öffnen.
2. Im Bereich *Run and Debug* das gewünschte F5-Profil wählen.
3. Erst nach erfolgreich beendetem Build das Programm bedienen.
4. Bei Problemen zunächst die zugehörige Build-Aufgabe über
   *Terminal → Buildaufgabe ausführen* separat ausführen und die erste
   konkrete Fehlermeldung prüfen.

Wenn im Windows-Benutzerpfad Unicode-Zeichen (z. B. `ß`) stehen und
das SFML-/CMake-Setup mit `Illegal byte sequence` scheitert, das
Repository **als neue Kopie** in einen Pfad mit ASCII-Zeichen klonen, z. B.
`C:\\Dev\\GymEdition`. Keinesfalls die bestehenden lokalen Arbeiten löschen.

## Prüfungen und Einschränkungen

`tests/vscode_config_test.py` vergleicht die drei F5-Profile mit den
verfügbaren Build-Tasks, den C++-Quellen und CMake. GitHub Actions prüft
zusätzlich Konsole und Grafikziele auf mehreren Systemen. Ein grüner
Headless-Selbsttest ersetzt **nicht** den manuellen Test des Windows-
Fensters, der Audioausgabe, der Mausklicks oder der Grafiktreiber.

Auf den Schulen-Rechnern muss die Umgebung real geprüft werden.
