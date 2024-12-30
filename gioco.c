//
// Created by diego on 30/12/24.
//

#include "gioco.h"
#include "strutture.h"

void nuovaPartita () {
    Giocatore *listaGiocatori = NULL;
    int nGiocatori = 2;
    printf("Quanti giocatori giocheranno?)"
        "\n"
        "[2-4] (vuoto per 2): ");
    scanf("%d", &nGiocatori);

    for (int i = 0; i < nGiocatori; i++) {

    }

    listaGiocatori = allocaListaGiocatori(nGiocatori);
    listaGiocatori->next = inserisciGiocatoreInCoda(listaGiocatori);
}