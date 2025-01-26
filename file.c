//
// Created by diego on 30/12/24.
//

#include "file.h"

/** Legge tutte le carte comprese nel file di testo del mazzo.
 * @return Restituisce il mazzo completo dal file.
 */
Carta *leggiCarteDaFile () {
    // Apre il file mazzo
    FILE *fp = NULL;
    fp = fopen(MAZZO, "r");
    if (fp == NULL) {
        printf(RED "Il file del mazzo non e' stato trovato!");
        exit(EXIT_FAILURE);
    }

    // Crea una lista di carte e alloca lo spazio per una singola carta, e puntatore temporaneo per scorrere la lista
    Carta *testaCarte = allocaCarta();
    Carta *tmp = testaCarte;

    int quantita; // Il numero di copie della carta

    /* Legge la quantità dal file.
     * Controlla il risultato della fscanf, basta che non sia arrivato alla fine del file per continuare il loop. */
    while ( fscanf(fp, "%d", &quantita) != EOF ) {
        fscanf(fp, " %[^\n]s", &tmp->nome);
        fscanf(fp, " %[^\n]s", &tmp->descrizione);
        fscanf(fp, "%d", &tmp->tipo);
        fscanf(fp, "%d", &tmp->nEffetti);
        tmp->effetto = NULL;

        // Se il numero di effetti è maggiore di 0, alloca memoria e legge i dati
        if (tmp->nEffetti > 0) {
            tmp->effetto = (Effetto *) malloc(tmp->nEffetti * sizeof(Effetto)); // Alloca un array dinamico
            if (tmp->effetto == NULL) exit(EXIT_FAILURE);

            // For che legge gli effetti, e li mette nell'array dinamico
            for (int i = 0; i < tmp->nEffetti; i++)
                fscanf(fp, "%d %d %d", tmp->effetto[i].azione, tmp->effetto[i].targetGiocatori, tmp->effetto[i].tipo);
        }
        fscanf(fp, "%d", &tmp->quandoEffetto);
        fscanf(fp, "%d", &tmp->puoEssereGiocato); // Disessere giocati

        if (quantita > 1) {
            tmp->next = copiaCarta(tmp, quantita - 1); // -1 perché una c'è già (quando è stata letta dal file)
        while (tmp != NULL) tmp = tmp->next;
        }
    }
    // Chiude il file e restituisce la lista appena creata
    fclose(fp);
    return testaCarte;
}

