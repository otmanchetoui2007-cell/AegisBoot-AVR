param(
    [string]$AppBinPath,
    [int]$Version = 1,
    [string]$SecretKeyPath,
    [string]$OutputPath
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $AppBinPath)) {
    throw "Fichier binaire applicatif introuvable : $AppBinPath"
}

# 1. Vérification / Création automatique de la clé si absente
if (-not (Test-Path $SecretKeyPath)) {
    $keyDir = Split-Path -Path $SecretKeyPath
    if (-not (Test-Path $keyDir)) { New-Item -ItemType Directory -Path $keyDir -Force | Out-Null }
    $defaultKey = [byte[]](0..31)
    [System.IO.File]::WriteAllBytes($SecretKeyPath, $defaultKey)
}

# 2. Création du dossier de sortie si absent
$outDir = Split-Path -Path $OutputPath
if (-not (Test-Path $outDir)) { New-Item -ItemType Directory -Path $outDir -Force | Out-Null }

# 3. Lecture du binaire applicatif et de la clé secrète
$appBytes = [System.IO.File]::ReadAllBytes($AppBinPath)
$keyBytes = [System.IO.File]::ReadAllBytes($SecretKeyPath)

# 4. Calcul HMAC-SHA256 sur l'application
$hmacSha256 = [System.Security.Cryptography.HMACSHA256]::new($keyBytes)
$hmacTag = $hmacSha256.ComputeHash($appBytes)

# 5. Construction du Header de 40 octets (Magic: 0x41454749, Version, ImageSize, HMAC Tag)
$magicBytes   = [BitConverter]::GetBytes([uint32]0x41454749)
$versionBytes = [BitConverter]::GetBytes([uint16]$Version)
$sizeBytes    = [BitConverter]::GetBytes([uint16]$appBytes.Length)

$headerBytes = New-Object byte[] 40
[Array]::Copy($magicBytes, 0, $headerBytes, 0, 4)
[Array]::Copy($versionBytes, 0, $headerBytes, 4, 2)
[Array]::Copy($sizeBytes, 0, $headerBytes, 6, 2)
[Array]::Copy($hmacTag, 0, $headerBytes, 8, 32)

# 6. Fusion : Header (40 octets) + Application binaire
$finalImage = New-Object byte[] ($headerBytes.Length + $appBytes.Length)
[Array]::Copy($headerBytes, 0, $finalImage, 0, 40)
[Array]::Copy($appBytes, 0, $finalImage, 40, $appBytes.Length)

# 7. Écriture du fichier final
[System.IO.File]::WriteAllBytes($OutputPath, $finalImage)

Write-Host "  -> Header 40 octets généré avec succès (HMAC-SHA256)." -ForegroundColor Green
Write-Host "  -> Fichier signé créé : $OutputPath ($($finalImage.Length) octets)" -ForegroundColor Green
