# JK AXIS | Gym Edition 2D | read-only Windows diagnosis
param([switch]$Strict)
$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$root = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot "..")).Path
$ucrtBin = "C:\msys64\ucrt64\bin"
$issues = New-Object 'System.Collections.Generic.List[string]'

function Report([string]$Label, [bool]$Okay, [string]$Details) {
    $state = if ($Okay) { "OK" } else { "FEHLT" }
    Write-Host ("[{0}] {1}: {2}" -f $state, $Label, $Details)
    if (-not $Okay) { $script:issues.Add($Label + ": " + $Details) }
}
function Find-Tool([string]$Name) {
    $fixed = Join-Path $ucrtBin $Name
    if (Test-Path -LiteralPath $fixed -PathType Leaf) { return $fixed }
    $result = Get-Command -Name $Name -CommandType Application -ErrorAction SilentlyContinue |
        Select-Object -First 1
    if ($null -ne $result) { return $result.Source }
    return $null
}

Write-Host "JK AXIS | 2D-Diagnose ohne Aenderungen"
Write-Host "Projekt: $root"
Write-Host ""
$source = Test-Path -LiteralPath (Join-Path $root "src\gui\Main.cpp") -PathType Leaf
Report "2D-Quellcode" $source $(if ($source) { "vorhanden" } else { "fehlt; Git-Branch pruefen" })

$git = Find-Tool "git.exe"
if ($git) {
    $branch = (& $git -C $root branch --show-current 2>$null | Select-Object -First 1)
    Write-Host ("[INFO] Git-Branch: {0}" -f $(if ($branch) { $branch } else { "(unbekannt)" }))
    $changes = @(& $git -C $root status --short 2>$null)
    Write-Host ("[INFO] Git-Status: {0}" -f $(if ($changes.Count -eq 0) { "sauber" } else { "lokale Aenderungen vorhanden" }))
} else { Report "Git" $false "git.exe fehlt" }

$gxx = Find-Tool "g++.exe"
$gdb = Find-Tool "gdb.exe"
$cmake = Find-Tool "cmake.exe"
$ninja = Find-Tool "ninja.exe"
Report "G++" ($null -ne $gxx) $(if ($gxx) { $gxx } else { "g++.exe fehlt" })
Report "GDB" ($null -ne $gdb) $(if ($gdb) { $gdb } else { "gdb.exe fehlt" })
Report "CMake" ($null -ne $cmake) $(if ($cmake) { $cmake } else { "cmake.exe weder in MSYS2 noch PATH" })
Report "Ninja" ($null -ne $ninja) $(if ($ninja) { $ninja } else { "ninja.exe weder in MSYS2 noch PATH" })

if ($cmake) {
    try {
        $version = (& $cmake --version | Select-Object -First 1)
        Write-Host "[INFO] $version"
        if ($version -match 'version (\d+)\.(\d+)') {
            $major = [int]$Matches[1]
            $minor = [int]$Matches[2]
            Report "CMake >= 3.28" (($major -gt 3) -or ($major -eq 3 -and $minor -ge 28)) $version
        }
    } catch { Report "CMake-Start" $false $_.Exception.Message }
}

if ($root -match '[^\x00-\x7F]') {
    Write-Warning "Unicode im Quellpfad: SFML-FetchContent kann fehlerhafte Include-Pfade erzeugen."
    Write-Host "Empfohlen: separate Git-Kopie unter C:\Dev\GymEdition anlegen und dort frisch bauen."
    Write-Host "subst V: behebt nicht sicher jeden Unicode- oder langen Abhaengigkeitspfad."
}
if ($root.Length -gt 90) {
    Write-Warning "Langer Quellpfad ($($root.Length) Zeichen); moeglichst kurz klonen."
}
foreach ($name in @("build-gui", "build-gui-signature", "build-gui-unicode")) {
    $cache = Join-Path (Join-Path $root $name) "CMakeCache.txt"
    if (Test-Path -LiteralPath $cache -PathType Leaf) {
        $home = Get-Content -LiteralPath $cache |
            Where-Object { $_ -match '^CMAKE_HOME_DIRECTORY:INTERNAL=' } |
            Select-Object -First 1
        if ($home -and $home -match '[^\x00-\x7F]') {
            Write-Warning "Veralteter CMake-Cache in $name mit Unicode-Quellpfad; nicht in neuen Klon uebernehmen."
        }
    }
}
$exe = Join-Path $root "build-gui\vier-gewinnt-gui.exe"
Write-Host ("[INFO] 2D-EXE: {0}" -f $(if (Test-Path -LiteralPath $exe -PathType Leaf) { "vorhanden, Aktualitaet ungeprueft" } else { "noch nicht gebaut" }))
Write-Host ""
if ($issues.Count -eq 0) {
    Write-Host "[OK] Grundwerkzeuge erkannt. Der echte GUI-Start ist damit noch nicht abgenommen."
} else {
    Write-Host ("{0} technische Voraussetzung(en) offen." -f $issues.Count)
    if ($Strict) { exit 1 }
}
