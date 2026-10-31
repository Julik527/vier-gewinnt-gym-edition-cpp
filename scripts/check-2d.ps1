# JK AXIS | Gym Edition 2D | Read-only Windows environment diagnosis.
# Does not install packages, switch Git branches or delete any files.
param(
    [switch]$Strict
)
$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest
$root = Split-Path -Parent $PSScriptRoot
$toolRoot = "C:\msys64\ucrt64\bin"
$issues = New-Object System.Collections.Generic.List[string]

function Show-Check {
    param([string]$Label, [bool]$Ok, [string]$Message)
    $state = if ($Ok) { "OK" } else { "FEHLT" }
    Write-Host ("[{0}] {1}: {2}" -f $state, $Label, $Message)
    if (-not $Ok) { $script:issues.Add($Label + ": " + $Message) }
}

Write-Host "JK AXIS | Vier Gewinnt 2D | Diagnose"
Write-Host "Projekt: $root"
Write-Host ""

$source = Join-Path $root "src\gui\Main.cpp"
$sourceOk = Test-Path -LiteralPath $source -PathType Leaf
Show-Check "2D-Quellcode" $sourceOk $(if ($sourceOk) { "src/gui/Main.cpp vorhanden" } else { "Du bist vermutlich noch auf main. Entwicklungsbranch auschecken." })

$gitExe = Get-Command git -ErrorAction SilentlyContinue
if ($null -ne $gitExe) {
    $ref = (& git -C $root branch --show-current 2>$null | Select-Object -First 1)
    if (-not $ref) { $ref = "(kein Branch erkannt)" }
    Write-Host "Git-Branch: $ref"
    if (-not $sourceOk) {
        Write-Host "VOR BRANCHWECHSEL: git status --short"
        Write-Host "Danach, nur wenn lokale Aenderungen gesichert sind:"
        Write-Host "git fetch origin"
        Write-Host "git switch --track origin/quality/gameplay-persistence-2026-10-28"
    }
} else {
    Show-Check "Git" $false "Git ist im Terminal nicht erreichbar."
}

$tools = @("g++.exe", "gdb.exe", "cmake.exe", "ninja.exe")
foreach ($name in $tools) {
    $path = Join-Path $toolRoot $name
    $found = Test-Path -LiteralPath $path -PathType Leaf
    Show-Check $name $found $(if ($found) { $path } else { "MSYS2 UCRT64 installieren/aktualisieren." })
}

$containsUnicode = $root -match '[^\x00-\x7F]'
if ($containsUnicode) {
    Write-Warning "Der Projektpfad enthaelt Sonderzeichen. GDB kann bei -environment-cd scheitern."
    Write-Host "Das Repository in VS Code ueber ein ASCII-Laufwerk (z. B. V:\) oeffnen."
    Write-Host 'Beispiel im Projektordner: subst V: "$((Get-Location).Path)"'
    Write-Host "V: vorher nicht ueberschreiben, wenn der Buchstabe schon belegt ist."
}

$exe = Join-Path $root "build-gui\vier-gewinnt-gui.exe"
$exeExists = Test-Path -LiteralPath $exe -PathType Leaf
Write-Host ("[INFO] F5-GUI-EXE: {0}" -f $(if ($exeExists) { "vorhanden" } else { "noch nicht gebaut" }))
$cachePath = Join-Path $root "build-gui\CMakeCache.txt"
if (Test-Path -LiteralPath $cachePath -PathType Leaf) {
    $cache = Get-Content -LiteralPath $cachePath -Raw
    $generator = ($cache -split "\r?\n" | Where-Object { $_ -match '^CMAKE_GENERATOR:INTERNAL=' } | Select-Object -First 1)
    if ($generator) { Write-Host "CMake: $generator" }
}
Write-Host ""
if ($issues.Count -eq 0) {
    Write-Host "Grundvoraussetzungen vorhanden. In VS Code: Profil 'Vier Gewinnt | Gym Edition 2D' und F5."
} else {
    Write-Host ("{0} Voraussetzung(en) fehlen. Hinweise oben befolgen." -f $issues.Count)
    if ($Strict) { exit 1 }
}
