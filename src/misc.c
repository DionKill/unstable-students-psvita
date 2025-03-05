//
// Created by diego on 26/02/25.
//

#include "misc.h"

/** Una semplice funzione che chiede di premere invio per continuare.
 * Utile per fare in modo che l'utente possa leggere su schermo prima di andare avanti.
 */
void premiInvioPerContinuare () {
    printf("\n"
           "Premi "
           BGRN
           "INVIO"
           RESET
           " per continuare"
           CURSORE_INPUT);
    flushInputBuffer();
}

int richiediInput (int min, int max) {
    int input;
    do {
        printf(CURSORE_INPUT);

        // Inserisce l'opzione da tastiera
        scanf("%d", &input);
        flushInputBuffer();
        if (input < min || input > max)
            printf("Non hai inserito un opzione corretta!");
    } while (input < min || input > max);
    return input;
}

/** Funzione che libera l'input buffer.
 * Con questa non vengono aggiunti gli invii e caratteri speciali durante l'inserimento di testo.
 */
void flushInputBuffer () {
    while (getchar() != '\n');
}
