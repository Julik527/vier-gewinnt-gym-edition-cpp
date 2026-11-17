# Standalone-Konsolenversion bauen und testen

Projekt: **Vier Gewinnt | Gym Edition** · Julian Krauß · ITA25/9

## Ohne Internet arbeiten

1. `04_Quellcode/Vier-Gewinnt-Schulabgabe-Konsole.zip` entpacken.
2. In den entpackten Ordner wechseln. Dort liegt `CMakeLists.txt`.
3. Einen C++17-Compiler und CMake **ab Version 3.16** benötigen; Python 3 schaltet die beiden Integrationstests frei.
4. Diese Befehle in PowerShell oder im Terminal ausführen:

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Die Datei heißt je nach Generator `build/vier-gewinnt.exe`, `build/Debug/vier-gewinnt.exe` oder unter Linux `build/vier-gewinnt`. Bei Fehlern die Build-Ausgabe beachten. Windows (MSYS2 UCRT64 oder MSVC) ist vorzubereiten, nicht erst im Unterricht zu installieren.

## Quellcode und Prüfsummen

Das ZIP ist die unveränderte **Source-only-Schulabgabe** aus GitHub Actions, Commit `1e6043659d2861f503da634e536cf91a90698ebc`. ZIP und die `.sha256`-Datei liegen nebeneinander. Unter Linux lässt sich die Datei mit `sha256sum -c Vier-Gewinnt-Schulabgabe-Konsole.zip.sha256` prüfen; unter Windows mit `Get-FileHash` und Abgleich des SHA-256-Werts.

## Tatsächlicher technischer Nachweis dieser Vorbereitung

Das Paket wurde unter Linux im Release-Modus mit g++ und CMake gebaut. CTest meldete **6/6 bestanden, 0 fehlgeschlagen**. Das Protokoll liegt unter `05_Nachweise/Lokaler_CTest_Linux_Release.txt`. Eine echte Windows-/Schul-PC-Vorführung ist nicht damit nachgewiesen.
