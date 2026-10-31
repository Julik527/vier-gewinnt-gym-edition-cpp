# Windows 11: 2D-Spiel starten und Fehler richtig eingrenzen

**Projekt:** JK AXIS · Vier Gewinnt | Gym Edition · C++17 / SFML 3.1.0  
**Ziel:** MSYS2 UCRT64 (GCC 16.2, GDB 18.1), CMake 3.28+, Ninja, Visual Studio Code.

## 1. Zwei Branches, zwei Programme

Das GitHub-Repository hat den Standardbranch `main` mit der Konsole. Die grafische 2D-Anwendung befindet sich derzeit im **offenen [Draft-PR #1](https://github.com/Julik527/vier-gewinnt-gym-edition-cpp/pull/1)** auf `quality/gameplay-persistence-2026-10-28`.

Öffne das *PowerShell-Terminal* im Repository-Ordner und kontrolliere:

```powershell
git status --short
git branch --show-current
Test-Path .\src\gui\Main.cpp
```

Wenn `main` und `False` angezeigt werden, kann die 2D-EXE nicht existieren. **Zuerst ungespeicherte/uncommittete Arbeit sichern.** Dann:

```powershell
git fetch origin
git switch --track origin/quality/gameplay-persistence-2026-10-28
```

Falls der lokale Entwicklungsbranch schon existiert:

```powershell
git switch quality/gameplay-persistence-2026-10-28
git pull --ff-only
```

Wenn Git beim Wechsel lokale Änderungen meldet, **nicht** mit `--force` fortfahren. Vorher klären, welche Dateien gesichert oder committed werden sollen.

## 2. MSYS2 UCRT64 (nicht MSYS, MINGW64 oder CLANG64)

Im *MSYS2 UCRT64*-Terminal, wenn nötig:

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
cmake --version
ninja --version
g++ --version
gdb --version
```

Ein Systemupdate bei Aufforderung abschließen und MSYS2 gegebenenfalls neu starten, **bevor** Einzelpakete installiert werden. Keine `--nodeps`-, `-Rdd`- oder erzwungenen Versionsmischungen.

**Wichtig:** SFML 3.1.0 wird beim ersten GUI-CMake-Build über FetchContent geladen. Nicht als vermeintliche Lösung einfach das MSYS2-SFML-3.0.x-Paket installieren.

## 3. Unicode-Pfad: GDB-Fehler `-environment-cd`

Unter Windows können Sonderzeichen wie `ß` im Projektpfad dazu führen, dass der GDB-Debugger den Ordner nicht öffnen kann. Der Compiler muss deshalb **nicht** defekt sein.

Wenn der Projektordner im PowerShell-Terminal geöffnet ist, kann ein freier Laufwerksbuchstabe verwendet werden:

```powershell
subst
subst V: "$((Get-Location).Path)"
Test-Path V:\.vscode\launch.json
code V:\
```

**Nur verwenden, wenn V: noch nicht belegt ist.** Falls V: bereits auf den richtigen Projektordner zeigt, `subst V:` nicht erneut ausführen. Das Laufwerk bleibt normalerweise bis zum Neustart bestehen. In VS Code muss anschließend der Arbeitsordner **V:\** und nicht der Benutzerprofilpfad geöffnet sein.

## 4. F5 und manueller 2D-Build

Im **PowerShell-Terminal** von VS Code:

```powershell
.\scripts\check-2d.ps1
```

Dann **Ausführen und Debuggen** → `Vier Gewinnt | Gym Edition 2D` → **F5**. Die bestehenden VS-Code-Aufgaben konfigurieren CMake/Ninja und kompilieren die GUI automatisch. Der erste FetchContent-Build benötigt Internet und kann mehrere Minuten beanspruchen.

Wenn der F5-Build scheitert, liefert die getrennte Demo-Konfiguration einen unabhängigen Test:

```powershell
.\scripts\demo.ps1 -Mode GUI
```

Dieses Skript baut im separaten Verzeichnis `build-gui-ucrt64`, das F5-Profil dagegen in `build-gui`. Nicht wahllos CMake-Caches löschen oder zwischen MSVC und GCC im selben Build-Verzeichnis mischen.

## 5. Fehlersuche: Datei oder Debugger?

```powershell
Test-Path .\src\gui\Main.cpp
Test-Path .\build-gui\vier-gewinnt-gui.exe
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
& .\build-gui\vier-gewinnt-gui.exe --self-test
Write-Host "Exit-Code: $LASTEXITCODE"
```

- **Quellcode `False`:** falscher Git-Branch.
- **Quellcode `True`, EXE `False`:** GUI-Build ist fehlgeschlagen oder noch nicht gelaufen.
- **Self-Test erfolgreich, F5 scheitert:** Debugger-/Pfad-Konfiguration prüfen.
- **Self-Test erfolgreich, Fenster öffnet sich nicht:** Windows-Treiber/OpenGL, Fonts und Laufzeitbibliotheken prüfen; die Self-Test-Option prüft nicht die Darstellung.
- **Fehlende DLL:** Build-Ausgabe und UCRT64-Laufzeitpfad prüfen; nicht beliebige DLLs aus dem Internet laden.

Für Test- und Präsentationsnachweise gilt weiterhin [GUI_TESTPLAN.md](GUI_TESTPLAN.md). Ein bestandener CI-Lauf ist kein Nachweis für korrektes Rendering, Tastatur, Maus oder Musik auf deinem eigenen Rechner.
