//
// Created by diego on 30/12/24.
//

#include "file.h"

/** Legge tutte le carte comprese nel file di testo del mazzo.
 * Quando compilato, bisogna avere il file "mazzo.txt" insieme all'eseguibile,
 * che sarà dentro la cartella cmake-build-debug.
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
    Carta *carta = allocaCarta();
    Carta *tmp = carta; // Puntatore temporaneo per creare la lista
    int quantita;

    /*
     *Problema attuale: quando ne deve aggiungere un altro, continua a leggere e non si copia. bisogna anche far scendere la quantità che in questo momento non succede
     */
    while (tmp != NULL) {
        quantita = fscanf(fp, "%d", &quantita); // Il numero di copie della carta
        fscanf(fp, " %[^\n]s", &carta->nome);
        fscanf(fp, " %[^\n]s", &carta->descrizione);
        fscanf(fp, "%d", &carta->tipo);
        fscanf(fp, "%d", &carta->nEffetti);

        carta->effetto = NULL;

        if (carta->nEffetti > 0) {
            carta->effetto = (Effetto *) malloc(carta->nEffetti * sizeof(Effetto));
            if (carta->effetto == NULL) exit(EXIT_FAILURE);

            for (int i = 0; i < carta->nEffetti; i++)
                fscanf(fp, "%d", carta->effetto[carta->nEffetti].azione);
            for (int i = 0; i < carta->nEffetti; i++)
                fscanf(fp, "%d", carta->effetto[carta->nEffetti].targetGiocatori);
            for (int i = 0; i < carta->nEffetti; i++)
                fscanf(fp, "%d", carta->effetto[carta->nEffetti].tipo);
        }

        fscanf(fp, "%d", &carta->quandoEffetto);
        fscanf(fp, "%d", &carta->puoEssereGiocato); // Disessere giocati

        // Va avanti nella lista
        tmp->next = allocaCarta();
    }
    return carta;
}
