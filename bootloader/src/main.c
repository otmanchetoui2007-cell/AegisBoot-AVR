#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

void execute_user_app(void);
int constant_time_compare(const uint8_t *a, const uint8_t *b, uint16_t len);
void hmac_sha256_flash(uint8_t *out_hmac);

int main(void) {
    // ⚡ 0. VERROUILLAGE IMMÉDIAT DE LA BROCHE 13 (PB5) À L'ÉTAT ÉTEINT (0V)
    // Cela supprime l'état flottant et l'allumage aléatoire au branchement USB
    DDRB |= (1 << PB5);    // Broche 13 en SORTIE
    PORTB &= ~(1 << PB5);  // Broche 13 forcée à 0V (ÉTEINTE)

    uint8_t computed_hmac[32];
    uint8_t expected_hmac[32];

    // 1. Lecture de la signature attendue dans le Header (adresse 0x0000)
    for (uint16_t i = 0; i < 32; i++) {
        expected_hmac[i] = pgm_read_byte(i);
    }

    // 2. Calcul du HMAC sur le binaire applicatif
    hmac_sha256_flash(computed_hmac);

    // 3. Vérification de la signature
    if (constant_time_compare(computed_hmac, expected_hmac, 32)) {
        // ✅ SIGNATURE VALIDE -> Saut vers l'application (qui fera clignoter la LED)
        execute_user_app();
    }

    // 🚨 SIGNATURE INVALIDE (Attaque) -> Allumer la LED en FIXE (Mode Alarme)
    PORTB |= (1 << PB5);

    while (1); // Boucle d'arrêt de sécurité
    return 0;
}
