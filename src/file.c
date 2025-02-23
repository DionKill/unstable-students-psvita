//
// Created by diego on 30/12/24.
//

#include "file.h"

/** Dato un percorso in ingresso, apre un file.
 *
 * @param path Il percorso dove si trova il file
 * @return Ritorna il puntatore a file
 */
FILE *apriFile (char *path) {
    // Apre il file
    FILE *fp = NULL;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf(RED "Il file non è stato trovato!");
        exit(EXIT_FAILURE);
    }
    return fp;
}

/** Legge tutte le carte comprese nel file di testo del mazzo.
 * @param c Il mazzo che verrà popolato da carte lette dal file.
 */
void leggiCarteDaFile (Carta **c) {
    FILE *fp = apriFile(MAZZO);

    // Crea una lista di carte e alloca lo spazio per una singola carta, e puntatore temporaneo per scorrere la lista
    Carta *scorriLista = NULL;

    int quantita; // Il numero di copie della carta

    /* Il loop legge la quantità dal file.
     * Controlla il risultato della fscanf, basta che non sia arrivato alla fine del file (EOF) per continuare. */
    while ( fscanf(fp, "%d", &quantita) != EOF ) {
        // Alloca una carta temporanea che viene popolata dal file
        Carta *tmp = allocaCarta();

        fscanf(fp, " %[^\n]s", tmp->nome);
        fscanf(fp, " %[^\n]s", tmp->descrizione);
        fscanf(fp, "%d", &tmp->tipo);
        fscanf(fp, "%d", &tmp->nEffetti);

        tmp->effetto = NULL;
        // Se il numero di effetti è maggiore di 0, alloca memoria e legge i dati
        if (tmp->nEffetti > 0) {
            tmp->effetto = (Effetto *) malloc(tmp->nEffetti * sizeof(Effetto)); // Alloca un array dinamico

            if (tmp->effetto == NULL) exit(EXIT_FAILURE);

            // For che legge gli effetti e li mette nell'array dinamico
            for (int i = 0; i < tmp->nEffetti; i++)
                fscanf(fp, "%d %d %d", &tmp->effetto[i].azione, &tmp->effetto[i].targetGiocatori, &tmp->effetto[i].tipo);
        }
        fscanf(fp, "%d", &tmp->quandoEffetto);
        fscanf(fp, "%d", &tmp->puoEssereGiocato); // Disessere giocati

        if (quantita > 1)
            tmp->next = copiaCarta(tmp, quantita - 1); // -1 perché una c'è già (quando è stata letta dal file)

        // Se la lista è vuota, allora la lista del parametro viene impostata al primo elemento della nuova lista
        if (scorriLista == NULL) {
            scorriLista = tmp;
            *c = scorriLista;
        }
        // Altrimenti, se la lista è già esistente, appende alla lista i nuovi elementi appena letti dal file
        else {
            scorriLista->next = tmp;
            while (scorriLista->next != NULL) scorriLista = scorriLista->next;
        }
    }
    // Chiude il file e restituisce la lista appena creata
    fclose(fp);
}