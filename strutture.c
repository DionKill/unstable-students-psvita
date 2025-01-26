//
// Created by diego on 28/12/24.
//

#include "strutture.h"

/**
 *
 * @param nGiocatori Giocatori inseriti dall'utente
 * @return Passa come valore di ritorno la lista dei giocatori con nome
 */
Giocatore *inizializzaListaGiocatori (int nGiocatori) {
    Giocatore *lista = NULL;
    lista = aggiungiGiocatori(lista, nGiocatori);
    return lista;
}

/** Funzione ricorsiva per allocare la lista di Giocatori in memoria.
 *
 * @param nodo Il giocatore attuale
 * @param nGiocatori Il quantitativo di giocatori
 * @return Ritorna la lista
 */

Giocatore *aggiungiGiocatori (Giocatore *nodo, int nGiocatori) {
    if (nGiocatori == 0)
        return nodo;

    // Da rivedere, è brutto passare un valore non inizializzato e inizializzarlo dentro la funzione
    nodo = (Giocatore *) malloc(sizeof(Giocatore));
    if (nodo == NULL) exit(EXIT_FAILURE); // Esce se non puo' allocare memoria

    // Continua il codice, mettendo subito il prossimo nodo a NULL
    nodo->next = NULL;

    // Chiede all'utente il nome finché non e' valido
    do {
        printf ("\n"
                "Inserisci il nome del nuovo giocatore: ");
        scanf(" %" NOME_LENGTH_STR "[^\n]s", nodo->nome);
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
Giocatore *inserisciTestaInCoda (Giocatore *listaGiocatori) {
    Giocatore *tmp = listaGiocatori; // Variabile temporanea
    while (listaGiocatori->next != NULL)
        listaGiocatori = listaGiocatori->next;
    listaGiocatori->next = tmp;
    listaGiocatori = listaGiocatori->next;
    return listaGiocatori;
}

/** Alloca spazio in memoria per una singola carta.
 *
 * @return Ritorna la carta creata
 */
Carta *allocaCarta () {
    Carta *carta = NULL;
    carta = (Carta *) malloc(sizeof(Carta));

    if (carta == NULL) exit(EXIT_FAILURE);

    carta->next = NULL;

    return carta;
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
    tmp->effetto = c->effetto;
    tmp->quandoEffetto = c->quandoEffetto;
    tmp->puoEssereGiocato = c->puoEssereGiocato; // Disessere giocati

    tmp->next = copiaCarta(tmp, nCopie - 1);

    return tmp;
}
