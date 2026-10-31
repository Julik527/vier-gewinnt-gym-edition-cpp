# JK AXIS | Aufgabenblatt 51 | reproducible Windows CMake/CTest preflight.
# Only new build directories are written; no saved games or scores are touched.
param([switch]$WithGui)
$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest
$root = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot "..")).Path
$ucrtBin = "C:\msys64\ucrt64\bin"

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

$cmake = Find-Tool "cmake.exe"
$ninja = Find-Tool "ninja.exe"
$ctest = Find-Tool "ctest.exe"
$gcc = Join-Path $ucrtBin "gcc.exe"
$gxx = Join-Path $ucrtBin "g++.exe"
if (-not (Test-Path -LiteralPath $gcc -PathType Leaf) -or
    -not (Test-Path -LiteralPath $gxx -PathType Leaf)) {
    Stop-Preflight "gcc.exe und g++.exe aus C:\msys64\ucrt64\bin fehlen."
}
if ($WithGui -and $root -match '[^\x00-\x7F]') {
    Stop-Preflight "GUI nicht im Unicode-Quellpfad bauen. Erst separat nach C:\Dev\GymEdition klonen."
}
$env:Path = "$ucrtBin;$env:Path"
$folder = if ($WithGui) { "build-school-gui-preflight" } else { "build-school-console-preflight" }
$build = Join-Path $root $folder
$guiSetting = if ($WithGui) { "ON" } else { "OFF" }

Write-Host "JK AXIS | Automatischer Schul-Vorabtest | $folder"
Write-Host "Quelle: $root"
Write-Host "Bestehende Spielstaende und regulaere Build-Verzeichnisse werden nicht veraendert."

Invoke-Checked $cmake @(
    "-S", $root,
    "-B", $build,
    "-G", "Ninja",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_C_COMPILER=$gcc",
    "-DCMAKE_CXX_COMPILER=$gxx",
    "-DGYM_BUILD_GUI=$guiSetting"
)
Invoke-Checked $cmake @("--build", $build, "--parallel", "2")
Invoke-Checked $ctest @("--test-dir", $build, "--output-on-failure", "-C", "Release")

if ($WithGui) {
    $exe = Join-Path $build "vier-gewinnt-gui.exe"
    if (-not (Test-Path -LiteralPath $exe -PathType Leaf)) {
        Stop-Preflight "2D-EXE fehlt nach dem GUI-Build."
    }
    Invoke-Checked $exe @("--path-self-test")
}
Write-Host "[OK] CMake-Build, CTest und automatische Vorpruefung erfolgreich." -ForegroundColor Green
Write-Host "[OFFEN] Echter Windows-F5-Start, Maus, Tastatur, Musik, Layout und persoenliche Codeerklaerung."
