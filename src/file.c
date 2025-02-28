//
// Created by diego on 30/12/24.
//

#include "file.h"

/** Dato un percorso in ingresso, apre un file.
 *
 * @param path Il percorso dove si trova il file
 * @param mode
 * @return Ritorna il puntatore a file
 */
FILE *apriFile (char *path, char *mode) {
    // Apre il file
    FILE *fp = NULL;
    fp = fopen(path, mode);
    if (fp == NULL) {
        printf(RED "Il file non è stato trovato!");
        exit(EXIT_FAILURE);
    }
    return fp;
}

/** Legge tutte le carte comprese nel file di testo del mazzo.
 * @param mazzo Il mazzo che verrà popolato da carte lette dal file.
 */
void leggiCarteDaFile (Carta **mazzo) {
    FILE *fp = apriFile(MAZZO, "r");

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
            *mazzo = scorriLista;
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

/** Funzione responsabile del salvataggio dei dati su file.
 * Fin troppi parametri comunque
 *
 * @param nGiocatori Il numero di giocatori
 * @param listaGiocatori La lista dei giocatori da salvare
 * @param mazzoPesca Il mazzo da pesca
 * @param mazzoScarti Il mazzo delle carte che vengono giocate
 * @param mazzoAulaStudio Il mazzo delle carte laureando / studente che vengono giocate (credo)
 * @param salvataggio Il file da aprire (può essere savegame.sav oppure un file dato all'apertura del programma)
 */
void salvataggio (int nGiocatori, Giocatore *listaGiocatori, Carta *mazzoPesca,
                  Carta *mazzoScarti, Carta *mazzoAulaStudio, char *salvataggio) {

    // Apertura del file
    FILE *fp = apriFile(salvataggio, "wb");

    // Inizio scrittura dei dati su file usando apertura in binario
    fwrite(&nGiocatori, sizeof(int), 1, fp);

    // Scrive gli elementi del giocatore (comprese le carte in mano)
    for (int i = 0; i < nGiocatori; i++) {
        fwrite(listaGiocatori->nome, sizeof(listaGiocatori->nome), 1, fp);

        int length = contaCarte(listaGiocatori->carteGiocatore); // Int che ha la lunghezza della lista
        fwrite(&length, sizeof(int), 1, fp);

        salvataggioMazzo(listaGiocatori->carteGiocatore, &fp);
        salvataggioMazzo(listaGiocatori->carteAulaGiocatore, &fp);
        salvataggioMazzo(listaGiocatori->carteBonusMalusGiocatore, &fp);
    }

    // Scrive la grandezza di ogni altro mazzo rimanente, seguito da tutte le carte di quel mazzo
    int nCarte = contaCarte(mazzoPesca); // Intero che salva la lunghezza del mazzo
    fwrite(&nCarte, sizeof(int), 1, fp);
    salvataggioMazzo(mazzoPesca, &fp);

    nCarte = contaCarte(mazzoScarti); // Intero che salva la lunghezza del mazzo
    fwrite(&nCarte, sizeof(int), 1, fp);
    salvataggioMazzo(mazzoScarti, &fp);

    nCarte = contaCarte(mazzoAulaStudio); // Intero che salva la lunghezza del mazzo
    fwrite(&nCarte, sizeof(int), 1, fp);
    salvataggioMazzo(mazzoAulaStudio, &fp);

    // Chiude il file
    fclose(fp);
}

/** Salva un mazzo intero (o anche una singola carta) su file
 *
 * @param mazzo Il mazzo da salvare su file
 * @param fp Il puntatore a file, che va modificato
 */
void salvataggioMazzo (Carta *mazzo, FILE **fp) {
    while (mazzo != NULL) {
        fwrite(mazzo->nome, sizeof(mazzo->nome), 1, *fp); // Scrive il nome
        fwrite(mazzo->descrizione, sizeof(mazzo->descrizione), 1, *fp); // Scrive la descrizione
        fwrite(&mazzo->tipo, sizeof(TipologiaCarta), 1, *fp);
        fwrite(&mazzo->nEffetti, sizeof(mazzo->nEffetti), 1, *fp);

        // Se degli effetti sono presenti, scrive i loro attributi sul file
        for (int j = 0; j < mazzo->nEffetti; j++) {
            fwrite(&mazzo->effetto[j].azione, sizeof(Azione), 1, *fp); // Scrive l'azione
            fwrite(&mazzo->effetto[j].targetGiocatori, sizeof(TargetGiocatori), 1, *fp); // Scrive il target
            fwrite(&mazzo->effetto[j].tipo, sizeof(TipologiaCarta), 1, *fp); // Scrive la tipologia
        }

        fwrite(&mazzo->quandoEffetto, sizeof(Quando), 1, *fp); // Quando la carta effettua
        fwrite(&mazzo->puoEssereGiocato, sizeof(bool), 1, *fp); // Disessere giocati

        mazzo = mazzo->next; // Segue alla prossima carta
    }
}