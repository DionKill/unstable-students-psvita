//
// Created by diego on 30/12/24.
//

#ifndef FILE_H
#define FILE_H

#include "strutture.h"
#include "colori-ansi.h"
#include "gioco.h"

#define MAZZO "../mazzo.txt" // Il mazzo è nella cartella precedente
#define SALVATAGGIO "../saves/savegame.sav"

FILE *apriFile (char *path, char *mode);
void leggiCarteDaFile (Carta **mazzo);
void salvataggio (int nGiocatori, Giocatore *listaGiocatori, Carta *mazzoPesca,
                  Carta *mazzoScarti, Carta *mazzoAulaStudio, char *salvataggio);
void salvataggioMazzo (Carta *mazzo, FILE *fp);
void caricamento (int *nGiocatori, Giocatore **listaGiocatori, Carta **mazzoPesca,
                  Carta **mazzoScarti, Carta **mazzoAulaStudio, char *salvataggio);
void caricamentoMazzo(int size, Carta **mazzo, FILE *fp);

#endif //FILE_H
