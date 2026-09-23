param(
    [string]$HexPath,
    [string]$ComPort = "COM3",
    [string]$Programmer = "arduino",
    [int]$BaudRate = 115200
)

$ErrorActionPreference = "Stop"

if (-not (Get-Command "avrdude" -ErrorAction SilentlyContinue)) {
    throw "Erreur: 'avrdude' n'est pas installé ou n'est pas dans le PATH."
}

if (-not (Test-Path $HexPath)) {
    throw "Erreur: Fichier introuvable à l'emplacement : $HexPath"
}

Write-Host "[AVRDUDE] Flashage de $HexPath sur $ComPort..." -ForegroundColor Cyan

$avrdudeArgs = @(
    "-c", $Programmer,
    "-p", "m328p",
    "-P", $ComPort,
    "-b", $BaudRate,
    "-U", "flash:w:${HexPath}:raw"
)

Start-Process -FilePath "avrdude" -ArgumentList $avrdudeArgs -NoNewWindow -Wait
