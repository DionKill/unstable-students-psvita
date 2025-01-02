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

/** Funzione ricorsiva per allocare la lista di Giocatori in memoria (non mi veniva nessun altro modo per risolverla)
 *
 * @param nodo Il giocatore attuale
 * @param nGiocatori Il quantitativo di giocatori
 * @return Ritorna la lista
 */
Giocatore *aggiungiGiocatori (Giocatore *nodo, int nGiocatori) {
    if (nGiocatori == 0)
        return nodo;

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
