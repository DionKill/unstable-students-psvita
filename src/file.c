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
        printf("\n"
           RED "Il file %s non è stato trovato!", path);
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
    Carta **tmp = mazzo;

    int quantita; // Il numero di copie della carta

    /* Il loop legge la quantità dal file.
     * Controlla il risultato della fscanf, basta che non sia arrivato alla fine del file (EOF) per continuare. */
    while ( fscanf(fp, "%d", &quantita) != EOF ) {
        // Alloca una carta temporanea che viene popolata dal file
        *tmp = allocaCarta();

        // Legge questi nome, descrizione, tipo e numero di effetti
        fscanf(fp, " %[^\n]s", (*tmp)->nome);
        fscanf(fp, " %[^\n]s", (*tmp)->descrizione);
        fscanf(fp, "%d", &(*tmp)->tipo);
        fscanf(fp, "%d", &(*tmp)->nEffetti);

        (*tmp)->effetto = NULL; // Allocazione a NULL per evitare problemi

        // Se il numero di effetti è maggiore di 0, alloca memoria e legge i dati
        if ((*tmp)->nEffetti > 0) {
            // Alloca un array di effetti dinamico, se non può esce con un errore
            (*tmp)->effetto = (Effetto *) malloc((*tmp)->nEffetti * sizeof(Effetto));
            if ((*tmp)->effetto == NULL) exit(EXIT_FAILURE);

            // For che legge gli effetti e li mette nell'array dinamico
            for (int i = 0; i < (*tmp)->nEffetti; i++)
                fscanf(fp, "%d %d %d", &(*tmp)->effetto[i].azione, &(*tmp)->effetto[i].targetGiocatori, &(*tmp)->effetto[i].tipo);
        }
        fscanf(fp, "%d", &(*tmp)->quandoEffetto);
        fscanf(fp, "%d", &(*tmp)->opzionale); // Disessere giocati

        // Se la quantità è maggiore di uno, copia la carta
        if (quantita > MIN_1)
            (*tmp)->next = copiaCarta(*tmp, quantita - 1); // -1 perché una c'è già (quando è stata letta dal file)

        // Scorre la lista in avanti
        while (*tmp != NULL)
            tmp = &(*tmp)->next;
    }
    // Chiude il file e restituisce la lista appena creata
    fclose(fp);
}

/** Funzione per il caricamento della partita
 * @return Se ritorna true, il salvataggio è caricabile, altrimenti va creato
 */
void iniziaPartita(Giocatore **listaGiocatori, int *nGiocatori, Carta **mazzoPesca,
                   Carta **mazzoScarti, Carta **mazzoAulaStudio, int *turno, char *path) {
    FILE *fp = NULL;
    fp = fopen(path, "rb");

    // Se il salvataggio non esiste, esce dal programma
    if (fp == NULL) {
        // Se non esiste, ma è il salvataggio normale, lo crea
        if (strcmp(path, SALVATAGGIO) == 0) {
            creaNuovaPartita(listaGiocatori, nGiocatori, mazzoPesca, turno);
        }
        // Il salvataggio passato come parametro non esiste, perciò esce dal programma
        else exit(EXIT_FAILURE);
    }

    // Se il salvataggio esiste, carica la partita
    else {
        caricamento(listaGiocatori, nGiocatori, mazzoPesca, mazzoScarti, mazzoAulaStudio, turno, path);
        fclose(fp);
    }
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
 * @param turno
 */
void salvataggio (int nGiocatori, Giocatore *listaGiocatori, Carta *mazzoPesca,
                  Carta *mazzoScarti, Carta *mazzoAulaStudio, char *salvataggio, int *turno) {

    // Apertura del file
    FILE *fp = apriFile(salvataggio, "wb");

    // Inizio scrittura dei dati su file usando apertura in binario
    fwrite(&nGiocatori, sizeof(int), 1, fp);

    // Scrive gli elementi del giocatore (comprese le carte in mano)
    for (int i = 0; i < nGiocatori; i++) {
        fwrite(listaGiocatori, sizeof(Giocatore), 1, fp);

        int length = contaCarte(listaGiocatori->carteGiocatore); // Int che ha la lunghezza della lista
        fwrite(&length, sizeof(int), 1, fp);
        salvataggioMazzo(listaGiocatori->carteGiocatore, fp);

        length = contaCarte(listaGiocatori->carteAulaGiocatore); // Int che ha la lunghezza della lista
        fwrite(&length, sizeof(int), 1, fp);
        salvataggioMazzo(listaGiocatori->carteAulaGiocatore, fp);

        length = contaCarte(listaGiocatori->carteBonusMalusGiocatore); // Int che ha la lunghezza della lista
        fwrite(&length, sizeof(int), 1, fp);
        salvataggioMazzo(listaGiocatori->carteBonusMalusGiocatore, fp);
        listaGiocatori = listaGiocatori->next;
    }

    // Scrive la grandezza di ogni altro mazzo rimanente, seguito da tutte le carte di quel mazzo
    int nCarte = contaCarte(mazzoPesca); // Intero che salva la lunghezza del mazzo
    fwrite(&nCarte, sizeof(int), 1, fp);
    salvataggioMazzo(mazzoPesca, fp);

    nCarte = contaCarte(mazzoScarti); // Intero che salva la lunghezza del mazzo
    fwrite(&nCarte, sizeof(int), 1, fp);
    salvataggioMazzo(mazzoScarti, fp);

    nCarte = contaCarte(mazzoAulaStudio); // Intero che salva la lunghezza del mazzo
    fwrite(&nCarte, sizeof(int), 1, fp);
    salvataggioMazzo(mazzoAulaStudio, fp);

    /* Salvataggio di dettagli extra */
    // Segnalatore
    int extra = 0; // False perché in C ogni valore diverso 0 è vero
    fwrite(&extra, sizeof(int), 1, fp);
    fwrite(turno, sizeof(int), 1, fp);

    // Chiude il file
    fclose(fp);
}

/** Salva un intero mazzo (o anche una singola carta) su file
 *
 * @param mazzo Il mazzo da salvare su file
 * @param fp Il puntatore a file, che va modificato
 */
void salvataggioMazzo (Carta *mazzo, FILE *fp) {
    while (mazzo != NULL) {
        fwrite(mazzo, sizeof(Carta), 1, fp);

        // Se degli effetti sono presenti, scrive i loro attributi sul file
        for (int j = 0; j < mazzo->nEffetti; j++) {
            fwrite(&mazzo->effetto[j].azione, sizeof(Azione), 1, fp); // Scrive l'azione
            fwrite(&mazzo->effetto[j].targetGiocatori, sizeof(TargetGiocatori), 1, fp); // Scrive il target
            fwrite(&mazzo->effetto[j].tipo, sizeof(TipologiaCarta), 1, fp); // Scrive la tipologia
        }

        mazzo = mazzo->next; // Segue alla prossima carta
    }
}

/** Funzione responsabile del caricamento dei dati da file.
 *
 * @param listaGiocatori La lista dei giocatori da salvare
 * @param nGiocatori Il numero di giocatori
 * @param mazzoPesca Il mazzo da pesca
 * @param mazzoScarti Il mazzo delle carte che vengono giocate
 * @param mazzoAulaStudio Il mazzo delle carte laureando / studente che vengono giocate (credo)
 * @param turno
 * @param salvataggio Il file da aprire (può essere savegame.sav oppure un file dato all'apertura del programma)
 */
void caricamento (Giocatore **listaGiocatori, int *nGiocatori, Carta **mazzoPesca,
                  Carta **mazzoScarti, Carta **mazzoAulaStudio, int *turno, char *salvataggio) {
    // Apertura del file
    FILE *fp = apriFile(salvataggio, "rb");
    int length; // Int che ha contiene lunghezza della lista da leggere

    // Legge il numero dei giocatori dal file
    fread(nGiocatori, sizeof(int), 1, fp);

    // Doppio puntatore temporaneo alla lista dei giocatori, per poterla scorrere
    Giocatore **tmpGiocatore = listaGiocatori;

    // Alloca lo spazio in memoria per i giocatori, e legge le carte dal file aggiungendole ai mazzi del giocatore
    for (int i = 0; i < *nGiocatori; i++) {
        // Allocazione dinamica dei giocatori, altrimenti esce con errore
        *tmpGiocatore = (Giocatore *) malloc(sizeof(Giocatore));
        if (*tmpGiocatore == NULL) exit(EXIT_FAILURE);

        fread(*tmpGiocatore, sizeof(Giocatore), 1, fp);

        fread(&length, sizeof(int), 1, fp);
        caricamentoMazzo(length, &(*tmpGiocatore)->carteGiocatore, fp);

        fread(&length, sizeof(int), 1, fp);
        caricamentoMazzo(length, &(*tmpGiocatore)->carteAulaGiocatore, fp);

        fread(&length, sizeof(int), 1, fp);
        caricamentoMazzo(length, &(*tmpGiocatore)->carteBonusMalusGiocatore, fp);

        (*tmpGiocatore)->next = NULL;
        tmpGiocatore = &(*tmpGiocatore)->next;
    }

    // Legge la grandezza di ogni altro mazzo rimanente, seguito da tutte le carte di quel mazzo
    fread(&length, sizeof(int), 1, fp);
    caricamentoMazzo(length, mazzoPesca, fp);

    fread(&length, sizeof(int), 1, fp);
    caricamentoMazzo(length, mazzoScarti, fp);

    fread(&length, sizeof(int), 1, fp);
    caricamentoMazzo(length, mazzoAulaStudio, fp);

    /* Robe extra dei salvataggi */
    // Intero che verrà salvato dentro il file per segnalare l'utilizzo di cose extra
    int extra;
    fread(&extra, sizeof(int), 1, fp);

    // Se extra è diverso da End Of File
    if (extra == 0) {
        fread(turno, sizeof(int), 1, fp);
    }
    else {
        *turno = 1;
    }

    // Rende la lista circolare
    rendiListaGiocatoriCircolare(*listaGiocatori);

    // Chiude il file
    fclose(fp);
}

/** Carica un intero mazzo (o anche una singola carta) da file
 *
 * @param size La lunghezza della lista. Serve per poter allocare nuovamente spazio in memoria
 * @param mazzo
 * @param fp Il puntatore a file, che va modificato
*/
void caricamentoMazzo(int size, Carta **mazzo, FILE *fp) {
    // Carte temporanee per scorrere il mazzo
    Carta **tmp = mazzo;

    for (int i = 0; i < size; i++) {
        *tmp = allocaCarta();

        // Legge il blocco della carta
        fread(*tmp, sizeof(Carta), 1, fp);

        // Imposta gli effetti della carta a NULL per evitare problemi
        (*tmp)->effetto = NULL;

        // Se il numero degli effetti è maggiore di 0, alloca l'array dinamico
        if ((*tmp)->nEffetti > 0) {
            // Alloca spazio in memoria
            (*tmp)->effetto = (Effetto *) malloc(sizeof(Effetto) * (*tmp)->nEffetti);
            if ((*tmp)->effetto == NULL) exit(EXIT_FAILURE);
        }

        // Se degli effetti sono presenti, scrive i loro attributi sul file
        for (int j = 0; j < (*tmp)->nEffetti; j++) {
            fread(&(*tmp)->effetto[j].azione, sizeof(Azione), 1, fp); // Scrive l'azione
            fread(&(*tmp)->effetto[j].targetGiocatori, sizeof(TargetGiocatori), 1, fp); // Scrive il target
            fread(&(*tmp)->effetto[j].tipo, sizeof(TipologiaCarta), 1, fp); // Scrive la tipologia
        }

        // Il temporaneo scorre alla prossima carta
        tmp = &(*tmp)->next;
    }
}
