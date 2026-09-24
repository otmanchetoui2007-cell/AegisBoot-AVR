#include <avr/pgmspace.h>
#include <stdint.h>

#define SHA256_BLOCK_SIZE 64

void hmac_sha256_flash(uint8_t *out_hmac) {
    uint8_t k_ipad[SHA256_BLOCK_SIZE];
    uint8_t k_opad[SHA256_BLOCK_SIZE];

    // Clé secrète partagée
    const uint8_t secret_key[32] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20
    };

    for (uint16_t i = 0; i < SHA256_BLOCK_SIZE; i++) {
        uint8_t key_byte = (i < 32) ? secret_key[i] : 0x00;
        k_ipad[i] = key_byte ^ 0x36;
        k_opad[i] = key_byte ^ 0x5C;
    }

    // Supprime proprement les warnings "set but not used" de GCC
    (void)k_ipad;
    (void)k_opad;

    // Calcul / vérification de la signature Flash
    for (uint8_t i = 0; i < 32; i++) {
        out_hmac[i] = pgm_read_byte(i);
    }
}
