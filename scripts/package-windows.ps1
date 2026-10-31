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

@"
Vier Gewinnt | Gym Edition 2D
JK AXIS · Windows-Release-Paket

1. Das ZIP in einen beschreibbaren Ordner entpacken (z. B. Downloads).
2. vier-gewinnt-gui.exe starten.
3. F9 schaltet den eigens komponierten Stereo-Soundtrack ein/aus.
   F10/F11 regeln die Musiklautstaerke in 10-Prozent-Schritten.
4. Spielstände werden derzeit relativ zum Startordner unter data/ abgelegt.

Technischer Hinweis:
Dieses Paket wurde aus dem Windows-CI-Build erstellt und einem
automatischen Self-Test unterzogen. Eine Sicht-/Audio-/Schulrechner-
Abnahme und ein Test auf einem sauberen zweiten PC sind noch nötig.
Windows VC++-Laufzeitkomponenten können erforderlich sein.
Quelle und vollständige Anleitung:
https://github.com/Julik527/vier-gewinnt-gym-edition-cpp
"@ | Set-Content -Path (Join-Path $stage "START_HIER.txt") -Encoding UTF8

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
