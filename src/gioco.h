//
// Created by diego on 30/12/24.
//

#ifndef GIOCO_H
#define GIOCO_H

#include <stdio.h>
#include <stdlib.h>
#include "strutture.h"
#include "file.h"
#include "grafica.h"
#include "misc.h"

#define N_CARTE_PER_GIOCATORE 5

void menu ();
void gioco ();
int scegliAzione ();

int creaGiocatori(Giocatore **listaGiocatori);

#endif //GIOCO_H
