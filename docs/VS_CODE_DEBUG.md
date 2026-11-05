# VS Code: C++17 bauen und mit F5 debuggen

**Stand:** 05.11.2026 · Projekt: Vier Gewinnt | Gym Edition

## Aktuell offiziell unterstützte Startkonfiguration

Im öffentlichen Repository gibt es **die lauffähige C++-Konsolenversion**. Der definierte CMake-Target heißt `vier-gewinnt`. Die vorhandenen Quelldateien sind `src/main.cpp`, `src/Game.cpp`, `src/Board.cpp` und `src/ConsoleUI.cpp`.

Die `.vscode/launch.json` enthält dazu den Eintrag **Vier Gewinnt | Gym Edition** und verwendet den Build-Task **Vier Gewinnt bauen**. Der Task verwendet GCC aus MSYS2 UCRT64 sowie relative Pfade zu Quell- und Ausgabedatei. Der Debugger ist `C:/msys64/ucrt64/bin/gdb.exe`.

## Voraussetzungen

- Windows mit MSYS2 UCRT64, `C:/msys64/ucrt64/bin/g++.exe` und `gdb.exe`
- VS Code mit Microsofts C/C++-Erweiterung (`cppdbg`)
- **Den gesamten Repository-Stammordner** in VS Code öffnen, nicht nur eine einzelne `.cpp`-Datei

## Vorhandene Konsolenversion starten

1. Projektordner öffnen.
2. Mit **Strg + Shift + B** den Task `Vier Gewinnt bauen` ausführen.
3. Anschließend mit **F5** das Programm starten.
4. Im integrierten Terminal Spalten oder Spielbefehle eingeben.

Die Kompilierung muss `vier-gewinnt.exe` im Repository-Stammordner erzeugen. Die laufenden Spielstände werden im Unterordner `data/` gespeichert.

## 2D und 3D: noch keine Build-Ziele im Repository

Die auf einem lokalen Rechner eventuell schon existierenden Startprofile `Gym Edition 2D` und `Gym Edition 3D` beziehen sich auf `build-gui/vier-gewinnt-gui.exe` und `build-3d/vier-gewinnt-3d.exe`. Der öffentliche GitHub-Stand enthält jedoch weder zugehörige C++-Quellen noch definierte CMake-Ziele oder die Build-Tasks `Gym Edition 2D bauen` und `Gym Edition 3D bauen`.

**Deshalb wurden diese Profile nicht als funktionierende Startoptionen in GitHub veröffentlicht.** Lokale, nicht hochgeladene Grafikversionen bleiben davon unberührt. Vor der Integration müssen die tatsächlichen Quellordner, die verwendete Grafikbibliothek, Compiler- und Linkerparameter und die Build-Zielnamen bekannt sein. Erst dann die passenden `tasks.json`- und `launch.json`-Einträge ergänzen und testen.

## Windows-Pfade und `Illegal byte sequence`

Manche Kombinationen aus MSYS2/MinGW und Windows-Dateipfaden scheitern bei Unicode-Zeichen im Benutzernamen. Der Task verwendet deshalb ein **relatives** `-o vier-gewinnt.exe` und relative `.cpp`-Pfade. Sollte das nicht helfen, das Projekt in ein Verzeichnis mit einfachen ASCII-Zeichen klonen, etwa `C:\Dev\vier-gewinnt-gym-edition-cpp`, und dort starten. Ein Windows-Benutzerkonto muss dafür nicht umbenannt werden.

## Wenn du lokal bereits weitere Dateien geändert hast

GitHub-Änderungen wirken **nicht automatisch** auf deinen PC. Vor einem `git pull --ff-only origin main` zuerst `git status` prüfen und deine abweichenden lokalen `.vscode`-Dateien sichern. Bei einer Konfliktmeldung nicht blind zurücksetzen, damit lokale 2D-/3D-Arbeit nicht verloren geht.

## Qualitätskontrolle

`tests/vscode_config_test.py` kontrolliert, dass das Debug-Profil auf den vorhandenen Build-Task zeigt und die Quelldateiliste mit CMake übereinstimmt. Die GitHub-Actions-Matrix überprüft das Projekt zusätzlich unter Linux und Windows.
