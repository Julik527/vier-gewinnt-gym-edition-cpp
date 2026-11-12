# JK AXIS | 3D Windows portable package, based on tested MSVC output.
# Does not modify source files or existing 2D release ZIP.
param([string]$BuildDirectory="build-gui")
$ErrorActionPreference="Stop"
Set-StrictMode -Version Latest

$root=Split-Path -Parent $PSScriptRoot
$build=Join-Path $root $BuildDirectory
$dist=Join-Path $root "dist"
$stage=Join-Path $dist "Gym-Edition-3D-Windows"
$archive=Join-Path $dist "Gym-Edition-3D-Windows.zip"
$checksum=Join-Path $dist "SHA256SUMS_3D.txt"

if(-not (Test-Path -LiteralPath $build -PathType Container)) {
    throw "3D-Build-Verzeichnis fehlt: $build"
}
$binary=Get-ChildItem -Path $build -Filter "vier-gewinnt-3d.exe" -Recurse -File |
    Where-Object { $_.FullName -notmatch '[\\/]_deps[\\/]' } |
    Select-Object -First 1
if($null -eq $binary) {throw "Keine kompilierte 3D-EXE gefunden."}
if(Test-Path -LiteralPath $stage) {Remove-Item -LiteralPath $stage -Force -Recurse}
New-Item -ItemType Directory -Force -Path $stage | Out-Null
Copy-Item -LiteralPath $binary.FullName -Destination $stage
Get-ChildItem -LiteralPath $binary.DirectoryName -Filter "*.dll" -File |
    ForEach-Object {Copy-Item -LiteralPath $_.FullName -Destination $stage}


# Optional verified SRG774 CC0 OGG sound pack. CI builds without the
# audio assets are silent, not ready-to-play soundtrack releases.
$audioSource = Join-Path $root "assets/audio/sci-fi"
if (Test-Path -LiteralPath (Join-Path $audioSource "title.ogg") -PathType Leaf) {

    if (Test-Path -LiteralPath (Join-Path $audioSource "PROFILE.txt") -PathType Leaf) {
        & python (Join-Path $root "scripts/install-gym-phonk-v4.py") --destination $audioSource --verify
    } else {
        & python (Join-Path $root "scripts/install-sci-fi-audio.py") --destination $audioSource --verify
    }
    if ($LASTEXITCODE -ne 0) { throw "Audio-Paket: Profile oder SHA-256-Pruefung fehlgeschlagen." }
    $audioDest = Join-Path $stage "assets/audio/sci-fi"
    New-Item -ItemType Directory -Force -Path $audioDest | Out-Null
    Get-ChildItem -LiteralPath $audioSource -File |
        Where-Object { $_.Name -eq "PROFILE.txt" -or $_.Extension -eq ".ogg" } |
        ForEach-Object { Copy-Item -LiteralPath $_.FullName -Destination $audioDest }
}

@"
JK AXIS | VIER GEWINNT | GYM EDITION 3D

1. ZIP vollständig in einen beschreibbaren Ordner entpacken.
2. vier-gewinnt-3d.exe starten.
3. 2, 3 oder 4 Spieler auswählen und Namen eingeben.
4. Links klicken: Stein setzen. Rechts ziehen: Kamera drehen.
5. Mausrad: Zoom. A/D, Q/E: Kamera. C: Kamera zurücksetzen.
6. Mit acht installierten/verifizierten OGG-Dateien startet der Sound automatisch:
   Gym Phonk V4 oder originales SRG774-Sci-Fi-Paket.
   F9: Pause/Fortsetzen; F10/F11: Lautstärke.
   Ohne OGG-Dateien: Spiel bleibt stumm, nicht die alte 116-BPM-Musik.
7. Speichern mit S, Undo mit U; Spielstände unter data/.

Wichtig: Erfordert einen OpenGL-kompatiblen Windows-Grafiktreiber.
Auf einem zweiten Windows-PC kann die VC++-Laufzeit benötigt werden.
Automatischer CI-Self-Test ist kein Hör-, Grafik- oder Schul-PC-Test.
Die 2D-Version und die Konsolenversion bleiben separat erhalten.

Quelle:
https://github.com/Julik527/vier-gewinnt-gym-edition-cpp
"@ | Set-Content -LiteralPath (Join-Path $stage "START_HIER.txt") -Encoding UTF8

$checkScript = @'
@echo off
cd /d "%~dp0"
"vier-gewinnt-3d.exe" --audio-check
echo.
echo 8/8 = Sci-Fi-Paket erkannt. 0/8 = Musikdateien fehlen.
pause
'@
Set-Content -LiteralPath (Join-Path $stage "AUDIO_PRUEFEN.cmd") -Value $checkScript -Encoding ascii

Push-Location $stage
try {
    & (Join-Path $stage "vier-gewinnt-3d.exe") --self-test
    if($LASTEXITCODE -ne 0){throw "Der Paket-Self-Test der 3D-Version ist fehlgeschlagen."}
} finally {Pop-Location}
if(Test-Path -LiteralPath $archive){Remove-Item -LiteralPath $archive -Force}
Compress-Archive -Path (Join-Path $stage "*") -DestinationPath $archive -Force
$hash=(Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
"$hash  Gym-Edition-3D-Windows.zip" | Set-Content -LiteralPath $checksum -Encoding ascii
Write-Host "[OK] 3D-Windows-ZIP und SHA-256 erzeugt: $archive"
