# Einmalig offizielles Schul-Logo von Wikimedia Commons lokal beziehen.
# Lizenz: CC BY-SA 4.0; Quelle und Urheber in docs/SCHOOL_LOGO.md.
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$dir = Join-Path $root "assets\school"
New-Item -ItemType Directory -Path $dir -Force | Out-Null
$target = Join-Path $dir "bernd-blindow-schulen.jpg"
$source = "https://upload.wikimedia.org/wikipedia/commons/7/7e/Logo_BBS_standard_quer.jpg"
Invoke-WebRequest -Uri $source -OutFile $target
$sha1 = (Get-FileHash -Algorithm SHA1 -Path $target).Hash.ToLowerInvariant()
if ($sha1 -ne "71bc2f41e221b85e09907cedaeecb861b025b047") {
    Remove-Item $target -Force -ErrorAction SilentlyContinue
    throw "Schullogo-Download hat eine unerwartete Pruefsumme. Bitte die Originalquelle pruefen."
}
Write-Host "Originales Schul-Logo gespeichert: $target"
Write-Host "Lizenz und Urheber: docs/SCHOOL_LOGO.md"
