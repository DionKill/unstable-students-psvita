//
// Created by diego on 26/02/25.
//

#include "misc.h"

/** Una semplice funzione che chiede di premere invio per continuare.
 * Utile per fare in modo che l'utente possa leggere su schermo prima di andare avanti.
 */
void premiInvioPerContinuare () {
    printf("\n"
           "Premi INVIO per continuare"
           "\n");
    flushInputBuffer();
}

/** Funzione che libera l'input buffer.
 * Con questa non vengono aggiunti gli invii e caratteri speciali durante l'inserimento di testo.
 */
void flushInputBuffer () {
    while (getchar() != '\n');
}
