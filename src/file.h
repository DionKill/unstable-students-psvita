//
// Created by diego on 30/12/24.
//

#ifndef FILE_H
#define FILE_H

#include "strutture.h"
#include "colori-ansi.h"
#include "gioco.h"

#define MAZZO "../mazzo.txt" // Il mazzo è nella cartella precedente

FILE *apriFile (char path[]);
void leggiCarteDaFile (Carta **c);

#endif //FILE_H
