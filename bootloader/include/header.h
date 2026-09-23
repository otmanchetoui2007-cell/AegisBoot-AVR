#ifndef HEADER_H
#define HEADER_H

// Inclusion des types entiers standardisés à taille fixe (uint8_t, uint16_t, uint32_t)
#include <stdint.h>

// Mot magique 32 bits : 0x41454749 correspond à la chaîne ASCII "AEGI"
// Le suffixe UL (Unsigned Long) force le compilateur AVR à coder la constante sur 32 bits
#define MAGIC_WORD 0x41454749UL

/**
 * Structure de métadonnées du Header applicatif (Taille exacte : 40 octets).
 * L'attribut __attribute__((packed)) interdit au compilateur d'ajouter des octets 
 * de rembourrage (padding) d'alignement mémoire entre les champs.
 */
typedef struct __attribute__((packed)) {
    uint32_t magic;         // [Offset 0x00] Identifiant binaire magique (4 octets)
    uint16_t version;       // [Offset 0x04] Numéro de version pour l'Anti-Rollback (2 octets)
    uint16_t image_size;    // [Offset 0x06] Taille du code applicatif brut en octets (2 octets)
    uint8_t  hmac_tag[32];   // [Offset 0x08] Empreinte cryptographique HMAC-SHA256 (32 octets)
} FirmwareHeader_t;

#endif // HEADER_H
