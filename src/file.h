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
#define LOG "../saves/log.txt"
#define N_ARG_PARAM 1

FILE *apriFile (char *path, char *mode);
void leggiCarteDaFile (Carta **mazzo);
void iniziaPartita(Giocatore **listaGiocatori, int *nGiocatori, Carta **mazzoPesca,
                   Carta **mazzoScarti, Carta **mazzoAulaStudio, int *turno, char *path);
void salvataggio (int nGiocatori, Giocatore *listaGiocatori, Carta *mazzoPesca,
                  Carta *mazzoScarti, Carta *mazzoAulaStudio, char *salvataggio, int *turno);
void salvataggioMazzo (Carta *mazzo, FILE *fp);
void caricamento (Giocatore **listaGiocatori, int *nGiocatori, Carta **mazzoPesca,
                  Carta **mazzoScarti, Carta **mazzoAulaStudio, int *turno, char *salvataggio);
void caricamentoMazzo(int size, Carta **mazzo, FILE *fp);

void fileLog (char *giocante, char *giocatoreAffetto, char *carta, Azione azione);
void fileLogTurni (char *giocatore, int turno, Quando momento);

#endif //FILE_H
