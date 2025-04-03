//
// Created by diego on 26/02/25.
//

#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdlib.h>
#include "colori-ansi.h"

// Grafica e cursori non incluse in grafica.c per inclusioni circolari. Solo per decorazione.
#define CURSORE_INPUT "\n" BYEL ">" RESET ": "
#define LINEA_BIANCA WHTB "\t\t\t\t\t\t\t\t\t\t" RESET "\n"

#define P1 1
#define P2 2
#define P3 3
#define P4 4
#define P0 0

int inserisciNumero (int min, int max);
void premiInvioPerContinuare ();
void flushInputBuffer ();

#endif //MISC_H
