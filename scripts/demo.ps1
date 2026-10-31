# Schulvorfuehrung: grafisches Spiel und die PowerShell-Konsole nacheinander.
# Keine Spielstand-Kollisionen durch gleichzeitige Prozesse.
param(
    [ValidateSet("Both", "GUI", "Terminal")]
    [string]$Mode = "Both",
    [switch]$NoBuild
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$compiler = "C:\msys64\ucrt64\bin\g++.exe"
$logo = Join-Path $root "assets\school\bernd-blindow-schulen.jpg"
Push-Location $root
$originalPath = $env:Path
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
try {
    if ($Mode -in @("GUI", "Both")) {
        if (-not (Test-Path (Join-Path $root "src/gui/Main.cpp"))) {
            throw "2D-Quellcode fehlt. Pruefe zuerst 'git status --short', dann 'git fetch origin' und den Entwicklungsbranch. Anleitung: docs/WINDOWS_QUICKSTART.md"
        }
        if (-not (Test-Path $logo)) {
            try {
                & (Join-Path $root "scripts\install-school-logo.ps1")
            } catch {
                Write-Warning "Schullogo konnte nicht geladen werden. Schulname bleibt sichtbar."
            }
        }
        $gui = Join-Path $root "build-gui-ucrt64\vier-gewinnt-gui.exe"
        if (-not $NoBuild) {
            if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
                throw "CMake fehlt. Im MSYS2-UCRT64-Terminal: pacman -Syu; danach pacman -S mingw-w64-ucrt-x86_64-cmake"
            }
            if (-not (Get-Command ninja -ErrorAction SilentlyContinue)) {
                throw "Ninja fehlt. Im MSYS2-UCRT64-Terminal: pacman -S mingw-w64-ucrt-x86_64-ninja"
            }
            if (-not (Test-Path $compiler)) {
                throw "MSYS2 UCRT64 fehlt: $compiler"
            }
            & cmake -S . -B build-gui-ucrt64 -G Ninja -DGYM_BUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release "-DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe" "-DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe"
            if ($LASTEXITCODE -ne 0) { throw "GUI-Konfiguration fehlgeschlagen." }
            & cmake --build build-gui-ucrt64 --target vier-gewinnt-gui --parallel 2
            if ($LASTEXITCODE -ne 0) { throw "GUI-Build fehlgeschlagen." }
        }
        if (-not (Test-Path $gui)) { throw "GUI-EXE fehlt: $gui" }
        Write-Host "Grafische Gym Edition starten. F9 schaltet Musik an oder aus."
        # Pfad fuer SFML- und UCRT64-DLLs nur fuer diese PowerShell-Sitzung ergaenzen.
        Start-Process -FilePath $gui -WorkingDirectory $root -Wait
    }
    if ($Mode -in @("Terminal", "Both")) {
        if (-not (Test-Path $compiler)) { throw "MSYS2 UCRT64 g++ fehlt: $compiler" }
        $terminal = Join-Path $root "vier-gewinnt.exe"
        if (-not $NoBuild) {
            & $compiler -std=c++17 -O0 -g -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Board.cpp src/Game.cpp src/ConsoleUI.cpp -o $terminal
            if ($LASTEXITCODE -ne 0) { throw "Konsolen-Build fehlgeschlagen." }
        }
        if (-not (Test-Path $terminal)) { throw "Konsolen-EXE fehlt: $terminal" }
        Write-Host "PowerShell-Version: JK AXIS / Bernd-Blindow-Schulen."
        & $terminal
    }
} finally {
    $env:Path = $originalPath
    Pop-Location
}
