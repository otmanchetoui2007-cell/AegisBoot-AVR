#ifndef HMAC_SHA256_H
#define HMAC_SHA256_H

// Types entiers standardisés (uint8_t, uint16_t, uint32_t)
#include <stdint.h>

/**
 * @brief Calcule le tag HMAC-SHA256 d'un bloc de données résidant en mémoire Flash.
 * 
 * @param key_pgm    Pointeur Flash vers la clé secrète de 32 octets (déclarée avec PROGMEM).
 * @param flash_addr Adresse de départ du bloc à hacher dans la mémoire Flash.
 * @param len        Longueur du bloc applicatif à hacher (en octets).
 * @param out_hmac   Buffer de destination en RAM recevant le tag HMAC final (32 octets).
 */
void hmac_sha256_flash(const uint8_t *key_pgm, uint16_t flash_addr, uint16_t len, uint8_t *out_hmac);

/**
 * @brief Compare deux buffers de taille 'len' en temps constant.
 * 
 * @param a   Premier buffer d'octets.
 * @param b   Second buffer d'octets.
 * @param len Nombre d'octets à comparer.
 * @return 0 si les buffers sont identiques, valeur non nulle sinon.
 */
uint8_t constant_time_compare(const uint8_t *a, const uint8_t *b, uint8_t len);

#endif // HMAC_SHA256_H