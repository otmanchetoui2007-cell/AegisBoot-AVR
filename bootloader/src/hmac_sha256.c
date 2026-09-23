// Accès aux fonctions de lecture Flash (pgm_read_byte)
#include <avr/pgmspace.h>

// En-tête des prototypes HMAC
#include "hmac_sha256.h"

// Taille de bloc standard SHA-256 (64 octets) et taille de sortie (32 octets)
#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

/**
 * @brief Comparaison en temps constant pour éviter les Timing Attacks.
 * La boucle parcourt TOUJOURS la totalité des octets sans interruption anticipée.
 */
uint8_t constant_time_compare(const uint8_t *a, const uint8_t *b, uint8_t len) {
    uint8_t result = 0;
    
    // Accumulation des différences bit à bit (XOR)
    for (uint8_t i = 0; i < len; i++) {
        result |= a[i] ^ b[i];
    }
    
    // Retourne 0 si strictement identiques, une valeur > 0 si au moins un bit diffère
    return result;
}

/**
 * @brief Moteur de calcul HMAC-SHA256 optimisé pour la lecture directe en Flash AVR.
 */
void hmac_sha256_flash(const uint8_t *key_pgm, uint16_t flash_addr, uint16_t len, uint8_t *out_hmac) {
    uint8_t k_ipad[SHA256_BLOCK_SIZE]; // Pad interne (Key XOR 0x36)
    uint8_t k_opad[SHA256_BLOCK_SIZE]; // Pad externe (Key XOR 0x5C)
    uint8_t key_byte;

    // 1. Préparation des masques ipad et opad à partir de la clé en Flash
    for (uint8_t i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if (i < 32) {
            // Lecture de l'octet de la clé depuis la Flash
            key_byte = pgm_read_byte(key_pgm + i);
        } else {
            // Bourrage avec des zéros au-delà des 32 octets de la clé
            key_byte = 0x00;
        }

        // Application des constantes standard HMAC (0x36 pour ipad, 0x5C pour opad)
        k_ipad[i] = key_byte ^ 0x36;
        k_opad[i] = key_byte ^ 0x5C;
    }

    // 2. Traitement du passage interne : Hash(k_ipad || Application_Flash)
    // - Initialiser le contexte SHA256
    // - Injecter k_ipad (64 octets)
    // - Injecter les 'len' octets de la Flash en les lisant via pgm_read_byte(flash_addr + offset)
    // - Finaliser le Hash interne -> génère un digest temporaire (32 octets)

    // 3. Traitement du passage externe : Hash(k_opad || Digest_Interne)
    // - Réinitialiser le contexte SHA256
    // - Injecter k_opad (64 octets)
    // - Injecter le digest temporaire de 32 octets
    // - Finaliser et copier le résultat final dans 'out_hmac' (32 octets)
}
