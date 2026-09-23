param(
    [int]$Version = 1,
    [string]$ComPort = "COM3",
    [switch]$FlashBootloader,
    [switch]$FlashApp
)

# Interrompre le script immédiatement à la moindre erreur
$ErrorActionPreference = "Stop"

# 1. Définition des chemins du projet sur le Bureau
$rootDir     = "$HOME\Desktop\AegisBoot-AVR"
$appDir      = "$rootDir\app"
$bootDir     = "$rootDir\bootloader"
$toolsDir    = "$rootDir\tools"
$outputDir   = "$rootDir\output"

$appSrc      = "$appDir\src\main.c"
$appElf      = "$appDir\build\app.elf"
$appBin      = "$appDir\build\app.bin"
$signedBin   = "$outputDir\signed_firmware.bin"
$keyPath     = "$toolsDir\config\secret.key"

Write-Host "=== [AEGISBOOT-AVR] DÉBUT DE LA CHAÎNE COMPILATION ET SIGNATURE ===" -ForegroundColor Cyan

# 2. ÉTAPE DE COMPILATION : Conversion du code C applicatif en binaire brut
Write-Host "[1/4] Compilation de l'application avec avr-gcc..." -ForegroundColor Yellow

if (-not (Test-Path "$appDir\build")) {
    New-Item -ItemType Directory -Path "$appDir\build" -Force | Out-Null
}

# Compilation C -> ELF (Option -Wl mise entre guillemets pour éviter l'erreur PowerShell MissingArgument)
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -I"$appDir\include" "-Wl,--section-start=.text=0x0028" $appSrc -o $appElf

# Extraction du binaire brut (.elf -> .bin)
avr-objcopy -O binary -R .eeprom $appElf $appBin

Write-Host "  -> Binaire applicatif brut créé : $appBin" -ForegroundColor Green

# 3. ÉTAPE DE SIGNATURE : Calcul HMAC-SHA256 et ajout du Header
Write-Host "[2/4] Génération du Header de 40 octets et signature HMAC-SHA256..." -ForegroundColor Yellow

& "$toolsDir\sign_firmware.ps1" -AppBinPath $appBin -Version $Version -SecretKeyPath $keyPath -OutputPath $signedBin

# 4. ÉTAPE OPTIONNELLE : Flashage du Bootloader
if ($FlashBootloader) {
    Write-Host "[3/4] Flashage du Bootloader sur la cible ($ComPort)..." -ForegroundColor Yellow
    make -C $bootDir clean
    make -C $bootDir
    & "$toolsDir\flash_target.ps1" -HexPath "$bootDir\build\bootloader.hex" -ComPort $ComPort
} else {
    Write-Host "[3/4] Flashage Bootloader ignoré (Utiliser -FlashBootloader pour forcer)." -ForegroundColor Gray
}

# 5. ÉTAPE OPTIONNELLE : Flashage de l'Application Signée
if ($FlashApp) {
    Write-Host "[4/4] Flashage de l'image signée sur la cible ($ComPort)..." -ForegroundColor Yellow
    & "$toolsDir\flash_target.ps1" -HexPath $signedBin -ComPort $ComPort
} else {
    Write-Host "[4/4] Flashage Application ignoré (Utiliser -FlashApp pour forcer)." -ForegroundColor Gray
}

Write-Host "=== [AEGISBOOT-AVR] OPÉRATION TERMINÉE AVEC SUCCÈS ===" -ForegroundColor Green