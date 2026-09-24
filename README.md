# AegisBoot-AVR

**AegisBoot-AVR** is an ultra-lightweight secure bootloader designed for the Microchip/Atmel ATmega328P microcontroller (Arduino Uno R3). It guarantees firmware integrity and authenticity using **HMAC-SHA256** signature verification prior to handing over execution control to the main application.

---

## 📌 Key Features

* **Cryptographic Authentication (HMAC-SHA256)**: Automatically rejects unsigned, corrupted, or tampered application images.
* **Side-Channel Attack Resistance**: Employs constant-time comparison (`constant_time_compare`) during signature validation to eliminate timing leaks.
* **Hardware Hardening & Anti-Floating Pin Protection**: Enforces strict pin state configurations (enabled pull-ups, anti-floating measures) to prevent fault injection and unintended resets caused by electromagnetic noise.
* **Automated CI/CD Pipeline**: Fully orchestrated via PowerShell scripts (`build_and_sign.ps1`), managing AVR-GCC compilation, header encapsulation, Python signing, and AVRDUDE deployment.

---

## 📐 Memory Map

The ATmega328P provides 32 KB of Flash memory. AegisBoot resides in the high Bootloader section (entry address `.text=0x7000`):

| Address Range (Hex) | Size | Description |
| :--- | :--- | :--- |
| `0x0000 - 0x0027` | 40 bytes | Application Interrupt Vector Table |
| `0x0028 - 0x6FFF` | ~28 KB | Reserved Space for User Application |
| `0x7000 - 0x7FFF` | 4 KB | **AegisBoot Bootloader** (Entry point at `0x7000`) |

---

## 📁 Repository Structure

```text
AegisBoot-AVR/
├── app/                    # User application source code
│   ├── include/            # Application header files
│   ├── src/                # Application main.c
│   └── Makefile            # Application binary build (.bin)
├── bootloader/             # Secure Bootloader source code
│   ├── include/            # Headers (hmac_sha256.h, header_reader.h, etc.)
│   ├── src/                # C Logic (HMAC, constant-time, jump, main)
│   └── Makefile            # Bootloader linker setup (-Wl,--section-start=.text=0x7000)
├── images/                 # Execution proofs and architecture diagrams
│   └── test_true.png       # Validated flashing pipeline screenshot
├── output/                 # Output directory for final signed binaries
├── tools/                  # Automation pipeline scripts
│   ├── build_and_sign.ps1  # Master script (Build, Sign, Flash)
│   ├── flash_target.ps1    # Space-tolerant AVRDUDE wrapper script
│   └── sign_firmware.py    # Header generator and HMAC-SHA256 calculator
└── README.md               # Project documentation

---

## ⚙️ Technical Details

- **Microcontroller**: ATmega328P (8-bit AVR @ 16 MHz)
- **Flash Memory**: 32 KB total (4 KB allocated for Bootloader at x7000)
- **Communication Protocol**: UART / Serial (115200 baud, 8N1) via vrdude
- **Security Header**: 40-byte custom header (32-byte HMAC-SHA256 signature + 8-byte version metadata)
- **Cryptographic Engine**: Software HMAC-SHA256 with constant-time comparison


 ## 🛠️ Prerequisites & Toolchain Setup

Ensure the following tools are installed and accessible in your system's PATH:

    1)AVR-GCC Toolchain (avr-gcc, avr-objcopy, make)

    2)AVRDUDE (Microcontroller flashing utility)

    3)Python 3.x (Used for cryptographic firmware signing)

    4)PowerShell 5.1+ (For executing the build automation pipeline)

 ## 🚀 Usage & Automated Pipeline

The repository provides an automated end-to-end pipeline to compile, sign, and flash the device with a single command.
Run Full Pipeline (Bootloader + Application):
 & "tools/build_and_sign.ps1" -Version 1 -ComPort COM3 -FlashBootloader -FlashApp

Pipeline Execution Flow:

    1.Application Compilation: Compiles the raw binary app/build/app.bin.

    2. Cryptographic Signing: sign_firmware.py calculates the HMAC-SHA256 digest, prepends the header, and outputs output/signed_firmware.bin.

    3.Bootloader Compilation: Builds bootloader/build/bootloader.hex linked to start address 0x7000.

    4.Hardware Deployment: flash_target.ps1 deploys both the bootloader and signed application sequentially via AVRDUDE.
## 🛡️ Security & Implementation Details

   - HMAC Signature Verification: The bootloader inspects the firmware header before jumping to application memory. If the locally computed digest does not match the header, execution is halted.

    -Constant-Time Comparison:

  uint8_t constant_time_compare(const uint8_t *a, const uint8_t *b, size_t len) {
    uint8_t result = 0;
    for (size_t i = 0; i < len; i++) {
        result |= a[i] ^ b[i];
    }
    return result; // Returns 0 if and only if arrays match
}
    -Memory Isolation: Bootloader memory bounds are strictly locked to prevent application code from overwriting critical boot logic.

## 📜 License

Distributed under the MIT License. Free to use, modify, and distribute.
