#ifndef JUMP_H
#define JUMP_H

/**
 * @brief Prépare le processeur et exécute un saut absolu vers l'application (0x0000).
 * Cette fonction ne retourne jamais.
 */
void execute_user_app(void);

#endif // JUMP_H