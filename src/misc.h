//
// Created by diego on 26/02/25.
//

#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdlib.h>
#include "colori-ansi.h"

#define CURSORE_INPUT "\n>: "

void premiInvioPerContinuare ();
int richiediInput (int min, int max);
void flushInputBuffer ();

#endif //MISC_H
