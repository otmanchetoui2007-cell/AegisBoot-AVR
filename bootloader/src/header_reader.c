// Accès aux macros de lecture de la mémoire programme (Flash) sur l'architecture AVR
#include <avr/pgmspace.h>

// Structure binaire du Header définie dans l'Étape 1
#include "header.h"

/**
 * @brief Lit le Header applicatif de 40 octets situé au début de la mémoire Flash (0x0000).
 * 
 * @param dest_header Pointeur vers la structure en mémoire SRAM recevant les métadonnées.
 */
void read_firmware_header(FirmwareHeader_t *dest_header) {
    // memcpy_P est la fonction spécifique AVR pour copier des données depuis la Flash vers la SRAM :
    // 1. dest_header            : Adresse destination en mémoire RAM (SRAM)
    // 2. (const void*)0x0000    : Adresse source absolue dans la mémoire Flash
    // 3. sizeof(FirmwareHeader_t): Nombre d'octets à transférer (exactement 40 octets)
    memcpy_P(dest_header, (const void*)0x0000, sizeof(FirmwareHeader_t));
}
