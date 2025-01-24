//
// Created by diego on 05/01/25.
//

#include "debug.h"

#include "strutture.h"

void stampaGiocatori () {

}

void stampaCarte(Carta *carta) {
    Carta *tmp = carta;
    while (tmp != NULL) {
        printf("\n"
        "-----");
        printf("\n"
               "Nome: %s", tmp->nome);
        printf("\n"
               "Descrizione: %s", tmp->descrizione);
        printf("\n"
               "Tipo: %d", tmp->tipo);
        printf("\n"
               "Numero di effetti: %d", tmp->nEffetti);
        for (int i = 0; i < tmp->nEffetti; i++) {
               printf("\n"
                      "\tAzione: %d", tmp->effetto->azione);
               printf("\n"
                      "\tTarget: %d", tmp->effetto->targetGiocatori);
               printf("\n"
                      "\tTipo: %d", tmp->effetto->tipo);

        }
        printf("\n"
               "Quando: %d", tmp->quandoEffetto);
        printf("\n"
               "\nDisessere giocati: %d", tmp->puoEssereGiocato);
        printf("\n"
               "-----");
    }
}