// Accès aux registres d'E/S de l'ATmega328P (MCUCR, SP, RAMEND, IVCE, IVSEL)
#include <avr/io.h>

// Accès aux fonctions de contrôle des interruptions globales (cli)
#include <avr/interrupt.h>

// Prototype de la fonction de saut
#include "jump.h"

/**
 * @brief Effectue la transition sécurisée du Bootloader vers l'Application.
 */
void execute_user_app(void) {
    // 1. Désactivation absolue des interruptions globales pour éviter tout crash pendant la transition
    cli();

    // 2. Redirection de la Table des Vecteurs d'Interruption vers l'Adresse Application (0x0000)
    // Étape A : Activer le bit de modification IVCE (Interrupt Vector Change Enable)
    MCUCR = (1 << IVCE);

    // Étape B : Effacer le bit IVSEL dans les 4 cycles d'horloge suivants pour pointer vers 0x0000
    MCUCR &= ~(1 << IVSEL);

    // 3. Réinitialisation du Pointeur de Pile (Stack Pointer) au sommet de la SRAM (0x08FF sur ATmega328P)
    SP = RAMEND;

    // 4. Instruction Assembleur : Saut absolu vers le vecteur de Reset de l'application
    asm volatile ("jmp 0x0000");
}