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

#define COMANDO_GIOCA_CARTA 1
#define COMANDO_PESCA_CARTA 2
#define COMANDO_MOSTRA_CARTE 3
#define COMANDO_ESCI 4
#define COMANDO_CONFERMA 13 // Invio
#define COMANDO_ANNULLA 27 // ESC

void gioco ();
void giocaCarta (Giocatore *giocatore);
int scegliAzione ();

#endif //GIOCO_H
