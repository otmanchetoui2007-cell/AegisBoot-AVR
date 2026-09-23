#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // 1. Configurer la broche PB5 (Pin 13 / LED "L") en SORTIE
    DDRB |= (1 << PB5);

    // 2. Boucle infinie de clignotement
    while (1) {
        PORTB ^= (1 << PB5); // Inverse l'état de la LED
        _delay_ms(500);       // Attend 500 ms
    }

    return 0;
}
