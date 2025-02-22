//
// Created by diego on 22/02/25.
//

#include "misc.h"

/** Funzione che libera l'input buffer.
 * Con questa non vengono aggiunti gli invii e caratteri speciali durante l'inserimento di testo.
 */
void flushInputBuffer () {
    while (getchar() != '\n');
}

/** Libera la memoria usata dal programma prima dell'uscita
 * TODO: completare la funzione
 *
 * @param testaMazzo Il mazzo da liberare
 */
void liberaMemoria (Carta *testaMazzo) {
    while (testaMazzo != NULL) {
        Carta *tmp = testaMazzo;
        testaMazzo = testaMazzo->next;

        free(tmp->effetto);
        free(tmp);
    }
}
