# Installiert das originale Schulkennzeichen (unveraenderte bzw. verkleinerte Fassung).
# 429-robust: eine bekannte alternative Quelle, keine Wiederholungsschleife.
# Attribution und Lizenz: docs/SCHOOL_LOGO.md
param([string]$LocalFile = "")

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$dir = Join-Path $root "assets\school"
$target = Join-Path $dir "bernd-blindow-schulen.jpg"
New-Item -ItemType Directory -Force -Path $dir | Out-Null

function Test-SchoolJpeg([string]$Path) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) { return $false }
    if ((Get-Item -LiteralPath $Path).Length -lt 10000) { return $false }
    $stream = $null
    try {
        $stream = [System.IO.File]::OpenRead($Path)
        $head = New-Object byte[] 3
        $read = $stream.Read($head, 0, 3)
        return $read -eq 3 -and $head[0] -eq 255 -and $head[1] -eq 216 -and $head[2] -eq 255
    } catch {
        return $false
    } finally {
        if ($null -ne $stream) { $stream.Dispose() }
    }
}

if (Test-SchoolJpeg $target) {
    Write-Host "Schullogo ist bereits installiert: $target"
    exit 0
}

$temporary = Join-Path $dir "school-logo-download.part"
try {
    if ($LocalFile) {
        if (-not (Test-SchoolJpeg $LocalFile)) { throw "Die angegebene lokale Datei ist kein gueltiges JPEG." }
        Copy-Item -LiteralPath $LocalFile -Destination $temporary -Force
        Write-Host "Schullogo aus lokaler Datei uebernommen."
    } else {
        $sources = @(
            @{
                Name = "Schulprofil (verkleinerte Logo-Kopie)"
                Url  = "https://storage.googleapis.com/media-ossnar82/media/images/Logo_BBS_standard_quer.max-1200x1200.jpg"
                OfficialHash = ""
            },
            @{
                Name = "Wikimedia Commons (Original)"
                Url  = "https://upload.wikimedia.org/wikipedia/commons/7/7e/Logo_BBS_standard_quer.jpg"
                OfficialHash = "71bc2f41e221b85e09907cedaeecb861b025b047"
            }
        )
        $downloaded = $false
        foreach ($source in $sources) {
            Remove-Item -LiteralPath $temporary -Force -ErrorAction SilentlyContinue
            try {
                Invoke-WebRequest -Uri $source.Url -OutFile $temporary -TimeoutSec 25 -ErrorAction Stop
                if (-not (Test-SchoolJpeg $temporary)) { throw "Antwort enthaelt kein gueltiges JPEG." }
                if ($source.OfficialHash) {
                    $digest = (Get-FileHash -LiteralPath $temporary -Algorithm SHA1).Hash.ToLowerInvariant()
                    if ($digest -ne $source.OfficialHash) { throw "Original-Datei hat falsche Pruefsumme." }
                }
                Write-Host "Schullogo bezogen von: $($source.Name)"
                $downloaded = $true
                break
            } catch {
                Write-Warning "Quelle nicht verfuegbar: $($source.Name) ($($_.Exception.Message))"
            }
        }
        if (-not $downloaded) {
            throw "Beide Quellen sind nicht verfuegbar. Speichere das Logo manuell und verwende -LocalFile <Pfad>."
        }
    }
    if (-not (Test-SchoolJpeg $temporary)) { throw "Schullogo konnte nicht verifiziert werden." }
    Move-Item -LiteralPath $temporary -Destination $target -Force
    Write-Host "Schullogo installiert: $target"
    Write-Host "Urheber/Lizenz: siehe docs/SCHOOL_LOGO.md"
} finally {
    Remove-Item -LiteralPath $temporary -Force -ErrorAction SilentlyContinue
}
