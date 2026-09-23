MCU = atmega328p
F_CPU = 16000000UL
BOOT_LOADER_ADDRESS = 0x7000

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Iinclude
LDFLAGS = -mmcu=$(MCU) "-Wl,--section-start=.text=$(BOOT_LOADER_ADDRESS)"

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

ifeq ($(OS),Windows_NT)
	MKDIR = if not exist build mkdir build
	RMDIR = if exist build rmdir /s /q build
else
	MKDIR = mkdir -p build
	RMDIR = rm -rf build
endif

all: create_build build/bootloader.hex

create_build:
	@$(MKDIR)

build/%.o: src/%.c
	avr-gcc $(CFLAGS) -c $< -o $@

build/bootloader.elf: $(OBJ)
	avr-gcc $(LDFLAGS) $^ -o $@

build/bootloader.hex: build/bootloader.elf
	avr-objcopy -O ihex -R .eeprom $^ $@

clean:
	@$(RMDIR)