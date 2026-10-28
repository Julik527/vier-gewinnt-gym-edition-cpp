# Einmalig offizielles Schul-Logo von Wikimedia Commons lokal beziehen.
# Lizenz: CC BY-SA 4.0; Quelle und Urheber in docs/SCHOOL_LOGO.md.
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$dir = Join-Path $root "assets\school"
New-Item -ItemType Directory -Path $dir -Force | Out-Null
$target = Join-Path $dir "bernd-blindow-schulen.jpg"
$source = "https://upload.wikimedia.org/wikipedia/commons/7/7e/Logo_BBS_standard_quer.jpg"
Invoke-WebRequest -Uri $source -OutFile $target
if ((Get-Item $target).Length -lt 10000) { throw "Logo-Download ist unvollstaendig." }
Write-Host "Originales Schul-Logo gespeichert: $target"
Write-Host "Lizenz und Urheber: docs/SCHOOL_LOGO.md"
