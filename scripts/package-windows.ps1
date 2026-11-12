# Projekt: Vier Gewinnt | Gym Edition
# Datei:   scripts/package-windows.ps1
# Autor:   Julian Krauß
# Schule:  Bernd-Blindow-Schule Ravensburg
# Klasse:  ITA25/9
# Fach:    Programmiertechnik (C++)
# Lehrer:  Dr. Litera
# Stand:   12.11.2026 (Dokumentation)
# Aufgabe: Stellt eine Windows-Ausgabe des Programms zusammen.
# ------------------------------------------------------------

# JK AXIS | Portable Windows release assembly (MSVC build).
# Packages real build output, validates command-line self-test, emits SHA-256.
param(
    [string]$BuildDirectory = "build-gui"
)
$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root $BuildDirectory
$dist = Join-Path $root "dist"
$stage = Join-Path $dist "Gym-Edition-Windows"
$archive = Join-Path $dist "Gym-Edition-Windows.zip"

if (-not (Test-Path $build -PathType Container)) {
    throw "Build-Verzeichnis fehlt: $build"
}
$binary = Get-ChildItem -Path $build -Filter "vier-gewinnt-gui.exe" -Recurse -File |
    Where-Object { $_.FullName -notmatch "[\\/]_deps[\\/]" } |
    Select-Object -First 1
if ($null -eq $binary) { throw "GUI-EXE wurde nicht gebaut." }

if (Test-Path $stage) { Remove-Item $stage -Recurse -Force }
New-Item -ItemType Directory -Force -Path $stage | Out-Null
Copy-Item -LiteralPath $binary.FullName -Destination $stage
# The tested binary must be traceable to its exact Git source revision.
$sourceSha = (& git -C $root rev-parse HEAD).Trim().ToLowerInvariant()
if ($LASTEXITCODE -ne 0 -or $sourceSha -notmatch '^[0-9a-f]{40}$') {
    throw "Git-Commit-SHA fuer Release nicht ermittelbar."
}
Set-Content -LiteralPath (Join-Path $stage "VERSION.txt") -Value $sourceSha -Encoding ascii

# Runtime dependencies copied by CMake sit next to the executable.
Get-ChildItem -LiteralPath $binary.DirectoryName -Filter "*.dll" -File |
    ForEach-Object { Copy-Item -LiteralPath $_.FullName -Destination $stage }

$logoSource = Join-Path $binary.DirectoryName "assets/school/bernd-blindow-schulen.jpg"
if (-not (Test-Path $logoSource)) {
    $logoSource = Join-Path $root "assets/school/bernd-blindow-schulen.jpg"
}
if (Test-Path $logoSource) {
    $logoTarget = Join-Path $stage "assets/school"
    New-Item -ItemType Directory -Force -Path $logoTarget | Out-Null
    Copy-Item -LiteralPath $logoSource -Destination $logoTarget
}


# Optional verified SRG774 CC0 OGG sound pack. CI builds without the
# audio assets are silent, not ready-to-play soundtrack releases.
$audioSource = Join-Path $root "assets/audio/sci-fi"
if (Test-Path -LiteralPath (Join-Path $audioSource "title.ogg") -PathType Leaf) {
    if (Test-Path -LiteralPath (Join-Path $audioSource "PROFILE.txt") -PathType Leaf) {
        & python (Join-Path $root "scripts/install-gym-phonk-v4.py") --destination $audioSource --verify
    } else {
        & python (Join-Path $root "scripts/install-sci-fi-audio.py") --destination $audioSource --verify
    }
    if ($LASTEXITCODE -ne 0) { throw "Audio-Profil oder SHA256-Pruefung fehlgeschlagen." }
    $audioDest = Join-Path $stage "assets/audio/sci-fi"
    New-Item -ItemType Directory -Force -Path $audioDest | Out-Null
    Get-ChildItem -LiteralPath $audioSource -File |
        Where-Object { $_.Extension -eq ".ogg" -or $_.Name -eq "PROFILE.txt" } |
        ForEach-Object { Copy-Item -LiteralPath $_.FullName -Destination $audioDest }
}

@"
Vier Gewinnt | Gym Edition 2D
JK AXIS · Windows-Release-Paket

1. Das ZIP in einen beschreibbaren Ordner entpacken (z. B. Downloads).
2. vier-gewinnt-gui.exe starten.
3. Mit acht geprueften OGG-Dateien startet die Musik automatisch.
   Unterstuetzt: Gym Phonk V4 und originales SRG774-Sci-Fi-Paket.
   F9 pausiert/aktiviert; F10/F11 regeln die Lautstaerke.
   Fehlen Audio-Dateien, bleibt das Spiel absichtlich stumm.
   Die alte 116-BPM-Musik wird NICHT mehr abgespielt.
4. Spielstände werden derzeit relativ zum Startordner unter data/ abgelegt.

Technischer Hinweis:
Dieses Paket wurde aus dem Windows-CI-Build erstellt und einem
automatischen Self-Test unterzogen. Eine Sicht-/Audio-/Schulrechner-
Abnahme und ein Test auf einem sauberen zweiten PC sind noch nötig.
Windows VC++-Laufzeitkomponenten können erforderlich sein.
Quelle und vollständige Anleitung:
https://github.com/Julik527/vier-gewinnt-gym-edition-cpp
"@ | Set-Content -Path (Join-Path $stage "START_HIER.txt") -Encoding UTF8

$checkScript = @'
@echo off
cd /d "%~dp0"
"vier-gewinnt-gui.exe" --audio-check
echo.
echo 8/8 = Sci-Fi-Paket erkannt. 0/8 = Musikdateien fehlen.
pause
'@
Set-Content -LiteralPath (Join-Path $stage "AUDIO_PRUEFEN.cmd") -Value $checkScript -Encoding ascii

Push-Location $stage
try {
    & (Join-Path $stage "vier-gewinnt-gui.exe") --self-test
    if ($LASTEXITCODE -ne 0) {
        throw "Der Self-Test des verpackten Spiels ist fehlgeschlagen."
    }
} finally {
    Pop-Location
}

if (Test-Path $archive) { Remove-Item $archive -Force }
Compress-Archive -Path (Join-Path $stage "*") -DestinationPath $archive -Force
$hash = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
"$hash  Gym-Edition-Windows.zip" |
    Set-Content -Path (Join-Path $dist "SHA256SUMS.txt") -Encoding ascii
Write-Host "Releasepaket und SHA-256 erfolgreich erstellt: $archive"
