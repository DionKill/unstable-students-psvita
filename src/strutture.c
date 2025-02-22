//
// Created by diego on 28/12/24.
//

#include "strutture.h"

#include "gioco.h"

/*--- Gestione dei giocatori ---*/

/** Funzione ricorsiva per allocare la lista di Giocatori in memoria.
 *
 * @param nodo Il giocatore attuale
 * @param nGiocatori Il quantitativo di giocatori
 * @return Ritorna la lista
 */
Giocatore *aggiungiGiocatori (Giocatore *nodo, int nGiocatori) {
    if (nGiocatori == 0)
        return nodo;

    // Crea un nuovo giocatore e lo alloca
    nodo = (Giocatore *) malloc(sizeof(Giocatore));
    if (nodo == NULL) exit(EXIT_FAILURE); // Esce se non puo' allocare memoria

    // Continua il codice, mettendo subito il prossimo nodo a NULL
    nodo->next = NULL;

    // Chiede all'utente il nome finché non è valido
    do {
        printf ("\n"
                "Inserisci il nome del nuovo giocatore: ");
        scanf(" %" NOME_LENGTH_STR "[^\n]s", nodo->nome);
        flushInputBuffer();
    } while (strlen(nodo->nome) < 0);

    // Si richiama da solo finché non ha finito di aggiungere i giocatori
    nodo->next = aggiungiGiocatori(nodo->next, nGiocatori - 1);

    // Ritorna la lista
    return nodo;
}

/** Inserisce la testa come prossimo nodo in coda, per poter ciclare all'infinito, creando una lista circolare
 *
 * @param listaGiocatori La lista dei giocatori
 * @return La lista dei giocatori ma che ha, come ultimo nodo, il primo della lista
 */
Giocatore *rendiListaGiocatoriCircolare (Giocatore *listaGiocatori) {
    Giocatore *tmp = listaGiocatori; // Variabile temporanea
    while (listaGiocatori->next != NULL)
        listaGiocatori = listaGiocatori->next;
    listaGiocatori->next = tmp;
    listaGiocatori = listaGiocatori->next;
    return listaGiocatori;
}

/*--- Gestione delle carte ---*/

/** Alloca spazio in memoria per una singola carta, altrimenti esce
 * @return Ritorna la carta allocata
 */
Carta *allocaCarta () {
    Carta *carta = NULL;
    carta = (Carta *) malloc(sizeof(Carta));

    if (carta == NULL) exit(EXIT_FAILURE);

    carta->next = NULL;

    return carta;
}

/** Cicla il mazzo e conta quante carte ci sono
 *
 * @param c Il mazzo di cui bisogna contare le carte
 * @return Ritorna il numero di carte nel mazzo
 */
int contaCarte (Carta *c) {
    int i = 0;
    while (c != NULL) {
        c = c->next;
        i++;
    }
    return i;
}

/** Funzione ricorsiva che prende in ingresso un nodo Carta, alloca la successiva, e copia i contenuti nella successiva.
 *
 * @param c Il nodo carta da copiare
 * @param nCopie Quante volte deve essere ancora copiato
 * @return Ritorna la carta
 */
Carta *copiaCarta (Carta *c, int nCopie) {
    if (nCopie == 0)
        return NULL;

    // Crea un nodo temporaneo alias del prossimo nodo (il primo di nCopie dove bisogna copiare)
    Carta *tmp = allocaCarta();

    // Copia i dati un parametro alla volta
    strcpy(tmp->nome, c->nome);
    strcpy(tmp->descrizione, c->descrizione);
    tmp->tipo = c->tipo;
    tmp->nEffetti = c->nEffetti;

    // Copia i parametri degli effetti
    if (tmp->nEffetti > 0) {
        tmp->effetto = (Effetto *) malloc(tmp->nEffetti * sizeof(Effetto)); // Alloca un array dinamico

        if (tmp->effetto == NULL) exit(EXIT_FAILURE);

        // For che legge gli effetti e li mette nell'array dinamico
        for (int i = 0; i < tmp->nEffetti; i++) {
            tmp->effetto[i].azione = c->effetto[i].azione;
            tmp->effetto[i].tipo = c->effetto[i].tipo;
            tmp->effetto[i].targetGiocatori = c->effetto[i].targetGiocatori;
        }
    }

    tmp->quandoEffetto = c->quandoEffetto;
    tmp->puoEssereGiocato = c->puoEssereGiocato; // Disessere giocati

    tmp->next = copiaCarta(tmp, nCopie - 1);

    return tmp;
}

/** Divide il mazzo in un mazzo normale e uno matricola.
 * Si basa sul fatto che le carte matricola sono sempre all'inizio della lista, dopo averle lette dal file.
 *
 * @param mazzo Il mazzo da cui vengono estratte le carte matricola. Viene anch'esso modificato.
 * @return Ritorna un nuovo mazzo contenente solo matricole.
 */
Carta *dividiMazzoMatricola (Carta **mazzo) {
    if (*mazzo == NULL) return NULL;

    Carta *mazzoMatricole = NULL;

    // Finché ci sono matricole scorre avanti e le sposta in un nuovo mazzo
    while ((*mazzo)->tipo == MATRICOLA)
        spostaCarta(mazzo, *mazzo, &mazzoMatricole);

    return mazzoMatricole;
}

/** Funzione che sposta tutti gli elementi della lista originale a una nuova lista, in modo casuale.
 *
 * L'algoritmo è fin troppo complesso, articolato, intricato, macchinoso, contorto, tortuoso,
 * c'erano altri metodi, è lento, ma lo volevo fare così senza passare per un array, quindi...
 *
 * @param mazzoOriginale Il mazzo che andrà modificato
 */
void shuffleCarte (Carta **mazzoOriginale) {
    Carta *mazzoRandomizzato = NULL;

    //Se il mazzo originale è nullo, esce
    if (*mazzoOriginale == NULL) return;

    Carta *pre = NULL; // La carta precedente a quella che dobbiamo cambiare, così è possibile rimuoverla dalla lista
    int lunghezza = contaCarte(*mazzoOriginale); // La lunghezza della lista

    // Finché nel *mazzoOriginale ci sono ancora carte, continua
    while (*mazzoOriginale != NULL) {
        int rnd = rand() % lunghezza;
        Carta *tmp = *mazzoOriginale; // Carta temporanea che scorrerà fino a quella da spostare al nuovo mazzo
        pre = NULL;

        // Scorre fino all'elemento da spostare
        for (int i = 0; i < rnd; i++) {
            pre = tmp;
            tmp = tmp->next;
        }

        // Rimuove l'elemento dal *mazzoOriginale, collegando il nodo precedente a quello successivo di tmp
        if (pre != NULL) {
            pre->next = tmp->next;
        } else {
            *mazzoOriginale = tmp->next;
        }

        // Aggiunge la carta alla testa del mazzo (così non bisogna scorrere fino alla fine ogni volta)
        tmp->next = mazzoRandomizzato;
        mazzoRandomizzato = tmp;

        lunghezza--;
    }
    *mazzoOriginale = mazzoRandomizzato;
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
