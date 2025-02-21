//
// Created by diego on 30/12/24.
//

#ifndef GIOCO_H
#define GIOCO_H

#include <stdio.h>
#include <stdlib.h>
#include "colori-ansi.h"
#include "strutture.h"

#define N_CARTE_PER_GIOCATORE 5

void menu ();
void gioco ();

int creaGiocatori(Giocatore **listaGiocatori);
void flushInputBuffer ();

#endif //GIOCO_H
