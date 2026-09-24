#include <avr/pgmspace.h>
#include <stdint.h>

// Comparaison à temps constant pour prévenir les attaques par canal auxiliaire (Side-Channel)
int constant_time_compare(const uint8_t *a, const uint8_t *b, uint16_t len) {
    uint8_t result = 0;
    for (uint16_t i = 0; i < len; i++) {
        result |= (a[i] ^ b[i]);
    }
    return (result == 0); // Renvoie 1 si identiques, 0 si différents
}
