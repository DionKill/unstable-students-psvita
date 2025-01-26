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

    /*
     * Dati nel mazzo di carte:
     * QUANTITA', NOME, DESCRIZIONE, TIPO CARTA, NUM EFFETTI, EFFETTO, QUANDO, OPZIONALE
     */
    // Crea una lista di carte e alloca lo spazio per una singola carta
    Carta *testaCarte = allocaCarta();
    Carta *tmp = testaCarte; // Puntatore temporaneo per creare la lista

    int quantita; // Il numero di copie della carta

    /* Legge la quantita' dal file.
     * Controlla il risultato della fscanf, che in caso sia un numero valido (c'e') continua il loop.
     * Basta che la quantita' sia diversa da 0 o inferiore per uscire dal while.
     */
    while ( fscanf(fp, "%d", &quantita) != EOF ) {
        // Legge gli effetti uno alla volta
        fscanf(fp, " %[^\n]s", &tmp->nome);
        fscanf(fp, " %[^\n]s", &tmp->descrizione);
        fscanf(fp, "%d", &tmp->tipo);
        fscanf(fp, "%d", &tmp->nEffetti);
        tmp->effetto = NULL;
        // Se il numero di effetti e' maggiore di 0, alloca memoria e legge i dati
        if (tmp->nEffetti > 0) {
            // Alloca memoria per un array dinamico, altrimenti esce
            tmp->effetto = (Effetto *) malloc(tmp->nEffetti * sizeof(Effetto));
            if (tmp->effetto == NULL) exit(EXIT_FAILURE);
            // Diversi for che leggono dal file i dati (uno alla volta e non una riga intera)
            for (int i = 0; i < tmp->nEffetti; i++)
                fscanf(fp, "%d", tmp->effetto[tmp->nEffetti].azione);
            for (int i = 0; i < tmp->nEffetti; i++)
                fscanf(fp, "%d", tmp->effetto[tmp->nEffetti].targetGiocatori);
            for (int i = 0; i < tmp->nEffetti; i++)
                fscanf(fp, "%d", tmp->effetto[tmp->nEffetti].tipo);
        }
        fscanf(fp, "%d", &tmp->quandoEffetto);
        fscanf(fp, "%d", &tmp->puoEssereGiocato); // Disessere giocati

        if (quantita > 1) {
            // Copia la carta in base al numero di quantita, e temp va avanti fino alla prossima carta.
            tmp->next = copiaCarta(tmp, quantita); // -1 perche' una c'e' gia' (quando e' stata letta dal file)
        while (tmp != NULL) tmp = tmp->next;
        }
    }
    // Chiude il file e restituisce
    fclose(fp);
    return testaCarte;
}
