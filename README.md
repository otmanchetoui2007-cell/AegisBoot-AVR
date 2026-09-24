# 🛡️ AegisBoot-AVR: Secure Bootloader with HMAC-SHA256 for ATmega328P

[![AVR GCC](https://img.shields.io/badge/Compiler-AVR--GCC-orange.svg)](https://gcc.gnu.org/)
[![Target MCU](https://img.shields.io/badge/MCU-ATmega328P-blue.svg)](https://www.microchip.com/)
[![Security](https://img.shields.io/badge/Security-HMAC--SHA256-green.svg)]()
[![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)](LICENSE)

**AegisBoot-AVR** is a lightweight, robust, and secure bootloader designed for the 8-bit **ATmega328P** microcontroller (Arduino Uno). It ensures firmware **integrity and authenticity** through **HMAC-SHA256** cryptographic verification before executing any user application code.

If the firmware is modified, corrupted, or not signed with the shared secret key, the bootloader halts system execution and triggers a visual security alarm.

---

## 🚀 Key Features

- 🔐 **Cryptographic Authentication (HMAC-SHA256)**: Verifies the application firmware signature stored in a dedicated 40-byte header at startup.
- 🛡️ **Side-Channel & Timing Attack Resistance**: Implements a constant-time comparison algorithm (`constant_time_compare`) to prevent timing side-channel leaks regarding the secret key.
- 🚨 **Tamper Detection & Alarm Mode**:
  - **Valid Firmware**: Passes control to the application (LED blinks normally at 500 ms intervals).
  - **Invalid Firmware / Attack**: Permanently locks the microcontroller in an infinite halt loop with a **solid alarm LED indicator (PB5 / Pin 13)**.
- ⚡ **Boot-Time Anti-Floating Protection**: Forces pin PB5 to $0\text{ V}$ (LOW) on the very first instruction cycle to eliminate floating pin noise and random LED flickers upon USB connection.
- 🛠️ **Automated Toolchain**: Automated PowerShell pipeline (`build_and_sign.ps1`) handling application compilation, header generation, HMAC signing, and flashing via `avrdude`.

---

## 📐 Flash Memory Map (ATmega328P)

The 32 KB Flash memory of the ATmega328P is partitioned as follows:

```text
    +-----------------------------------------+ 0x0000
    |   Security Header (40 Bytes)            |
    |   - HMAC-SHA256 Signature (32 bytes)    |
    |   - Metadata / Version (8 bytes)        |
    +-----------------------------------------+ 0x0028
    |                                         |
    |   User Application Firmware             |
    |                                         |
    +-----------------------------------------+ 0x7000 (Boot Section)
    |                                         |
    |   AegisBoot-AVR Secure Bootloader       |
    |                                         |
    +-----------------------------------------+ 0x7FFF
🛠️ Project Structure
AegisBoot-AVR/
├── app/                      # User application source code
│   ├── src/main.c            # Main application code (e.g., LED Blink)
│   └── Makefile              # Application build rules
├── bootloader/               # Secure Bootloader source code
│   ├── include/              # Header files (hmac_sha256.h, etc.)
│   ├── src/                  # HMAC implementation, header reader, main entry
│   └── Makefile              # Bootloader build rules (Boot section at 0x7000)
├── output/                   # Generated output binaries
│   └── signed_firmware.bin   # Signed application binary with 40-byte security header
├── tools/                    # Automation scripts
│   └── build_and_sign.ps1    # Build, signing, and flashing pipeline
├── .gitignore
└── README.md
📋 Prerequisites
Hardware
   . Arduino Uno R3 (or ATmega328P standalone board)
   . USB Type-B cable
Software
   . AVR GCC Toolchain (avr-gcc, avr-objcopy)
   . Make (GnuWin32 / MinGW)
   . AVRDUDE
   . PowerShell 5.1+

## ⚙️ Technical Details

- Microcontroller: ATmega328P (8-bit AVR  16 MHz)
- Flash Memory**: 32 KB total (4 KB allocated for Bootloader at `0x7000`)
- Communication Protocol: UART / Serial (`115200 baud`, 8N1) via `avrdude`
- Security Header: 40-byte custom header (32-byte HMAC-SHA256 signature + 8-byte version metadata)
- Cryptographic Engine: Software HMAC-SHA256 with constant-time comparison


🔧 Build, Sign & Flash
Run the automated pipeline script providing your COM port:
 & "tools\build_and_sign.ps1" -Version 1 -ComPort COM3 -FlashBootloader -FlashApp

 The script automatically:
   1) Compiles the user application binary.
   2) Calculates the HMAC-SHA256 payload digest with the secret key.
   3) Generates output/signed_firmware.bin.
   4) Compiles and flashes the bootloader at 0x7000.
   5)Flashes the signed firmware at 0x0000.
🧪 Security & Attack Testing

🟢 Normal Execution (Valid Firmware)
   . Bootloader reads the expected HMAC from Flash 0x0000.
   . Computes the HMAC over the application image.
   . Match confirmed -> Jumps to application (0x0028).
   . Visual Result: LED blinks regularly (500 ms).
🔴 Attack Simulation (Binary Corruption)

To prove tamper-resistance against bit-flips or unauthorized code injection:
   1) Corrupt 1 byte in the signed binary:
  $bytes = [System.IO.File]::ReadAllBytes("output\signed_firmware.bin")
  $bytes[45] =$bytes[45] -bxor 0xFF
  [System.IO.File]::WriteAllBytes("output\signed_firmware.bin", $bytes)
   2)Flash the corrupted binary:
  avrdude -c arduino -p m328p -P COM3 -b 115200 -U "flash:w:output\signed_firmware.bin:r"

Visual Result:
  .  Bootloader detects the signature mismatch.
  .  Execution is halted permanently.
  .  LED remains SOLID ON (Alarm Mode).


📄 License

This project is licensed under the MIT License.
