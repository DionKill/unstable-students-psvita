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

void premiInvioPerContinuare ();
int inserisciNumero (int min, int max);
void flushInputBuffer ();

#endif //MISC_H
