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
    if (nGiocatori < 0)
        return nodo;

    nodo = (Giocatore *) malloc(sizeof(Giocatore));
    if (nodo == NULL) exit(EXIT_FAILURE); // Esce se non può allocare memoria

    // Continua il codice, mettendo subito il prossimo nodo a NULL
    nodo->next = NULL;

    // Chiede all'utente il nome finché non è valido
    do {
        printf ("\n"
                "Inserisci il nome del nuovo giocatore: ");
        scanf(" %" NOME_LENGTH_STR "[^\n]s", nodo->nome);
    } while (strlen(nodo->nome) < 0);

    // Si richiama da solo finché non ha finito
    nodo->next = aggiungiGiocatori(nodo->next, nGiocatori - 1);
    return nodo;
}
