# JK AXIS | Aufgabenblatt 51 | reproducible Windows CMake/CTest preflight.
# Only new build directories are written; no saved games or scores are touched.
param([switch]$WithGui)
$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest
$root = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot "..")).Path

function Stop-Preflight([string]$Message) {
    Write-Host "[FEHLER] $Message" -ForegroundColor Red
    exit 1
}
function Find-Tool([string]$Executable) {
    $local = Join-Path $ucrtBin $Executable
    if (Test-Path -LiteralPath $local -PathType Leaf) { return $local }
    $found = Get-Command -Name $Executable -CommandType Application -ErrorAction SilentlyContinue |
        Select-Object -First 1
    if ($null -ne $found) { return $found.Source }
    Stop-Preflight "$Executable ist nicht erreichbar. MSYS2-UCRT64 oder PATH pruefen."
}
function Invoke-Checked([string]$Executable, [string[]]$Arguments) {
    & $Executable @Arguments
    if ($LASTEXITCODE -ne 0) {
        Stop-Preflight "Befehl fehlgeschlagen (Exit $LASTEXITCODE): $Executable $($Arguments -join ' ')"
    }
}

# MSYS2/setup-msys2 can install UCRT64 under a runner-specific directory.
# Prefer explicit runner/user setting; otherwise find the compiler on PATH.
$ucrtBin = $null
if ($env:GYM_UCRT64_BIN) {
    $ucrtBin = $env:GYM_UCRT64_BIN
} elseif (Test-Path -LiteralPath "C:\msys64\ucrt64\bin\g++.exe" -PathType Leaf) {
    $ucrtBin = "C:\msys64\ucrt64\bin"
} else {
    $discovered = Get-Command -Name "g++.exe" -CommandType Application -ErrorAction SilentlyContinue |
        Select-Object -First 1
    if ($null -ne $discovered) { $ucrtBin = Split-Path -Parent $discovered.Source }
}
if (-not $ucrtBin) {
    Stop-Preflight "MSYS2-UCRT64 nicht gefunden. GYM_UCRT64_BIN setzen oder g++.exe in PATH aufnehmen."
}
$ucrtBin = [System.IO.Path]::GetFullPath($ucrtBin)
$gcc = Join-Path $ucrtBin "gcc.exe"
$gxx = Join-Path $ucrtBin "g++.exe"
if (-not (Test-Path -LiteralPath $gcc -PathType Leaf) -or
    -not (Test-Path -LiteralPath $gxx -PathType Leaf)) {
    Stop-Preflight "gcc.exe und g++.exe im ermittelten UCRT64-Ordner fehlen: $ucrtBin"
}
# Ensure CMake, CTest, Ninja and GCC come from one matching toolchain.
$env:Path = "$ucrtBin;$env:Path"
$cmake = Find-Tool "cmake.exe"
$ninja = Find-Tool "ninja.exe"
$ctest = Find-Tool "ctest.exe"
Write-Host "[INFO] UCRT64-Compiler: $gxx"
Write-Host "[INFO] CMake: $cmake / Ninja: $ninja"
if ($WithGui -and $root -match '[^\x00-\x7F]') {
    Stop-Preflight "GUI nicht im Unicode-Quellpfad bauen. Erst separat nach C:\Dev\GymEdition klonen."
}
$folder = if ($WithGui) { "build-school-gui-preflight" } else { "build-school-console-preflight" }
$build = Join-Path $root $folder
$guiSetting = if ($WithGui) { "ON" } else { "OFF" }

Write-Host "JK AXIS | Automatischer Schul-Vorabtest | $folder"
Write-Host "Quelle: $root"
Write-Host "Bestehende Spielstaende und regulaere Build-Verzeichnisse werden nicht veraendert."

Invoke-Checked -Executable $cmake -Arguments @(
    "-S", $root,
    "-B", $build,
    "-G", "Ninja",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_C_COMPILER=$gcc",
    "-DCMAKE_CXX_COMPILER=$gxx",
    "-DGYM_BUILD_GUI=$guiSetting"
)
Invoke-Checked -Executable $cmake -Arguments @("--build", $build, "--parallel", "2")
Invoke-Checked -Executable $ctest -Arguments @("--test-dir", $build, "--output-on-failure", "-C", "Release")

if ($WithGui) {
    $exe = Join-Path $build "vier-gewinnt-gui.exe"
    if (-not (Test-Path -LiteralPath $exe -PathType Leaf)) {
        Stop-Preflight "2D-EXE fehlt nach dem GUI-Build."
    }
    Invoke-Checked -Executable $exe -Arguments @("--path-self-test")
}
Write-Host "[OK] CMake-Build, CTest und automatische Vorpruefung erfolgreich." -ForegroundColor Green
Write-Host "[OFFEN] Echter Windows-F5-Start, Maus, Tastatur, Musik, Layout und persoenliche Codeerklaerung."
