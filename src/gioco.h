//
// Created by diego on 30/12/24.
//

#ifndef GIOCO_H
#define GIOCO_H

#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "grafica.h"
#include "misc.h"
#include "effetti.h"

// Macro rappresentanti gli input da tastiera che vengono usati nel gioco, come per giocare una carta, pescare etc.
#define COMANDO_OPZIONE_1 1 // Gioca una carta, ...
#define COMANDO_OPZIONE_2 2 // Pesca una carta, ...
#define COMANDO_OPZIONE_3 3 // Mostra le carte, ...
#define COMANDO_OPZIONE_4 4 // Le carte altrui, ...
#define COMANDO_ESCI 0 // Serve per uscire dal programma o tornare indietro
#define COMANDO_CONFERMA 13 // Invio
#define COMANDO_ANNULLA 27 // ESC

// Gioco e menù
void gioco(char *path);
void pescaCarta (Carta **mazzoGiocatore, Carta **mazzoPesca, Carta **mazzoScarti);
int avantiTurno(int turno, Giocatore **listaGiocatori, Carta **mazzoPesca, Carta **mazzoScarti);
void creaNuovaPartita (int *nGiocatori, Giocatore **listaGiocatori, Carta **mazzoPesca,
                        Carta **mazzoScarti, Carta **mazzoAulaStudio);

// Carte ed effetti
void giocaCarta (Giocatore *listaGiocatori, int nGiocatori);
void azioneCarta (Giocatore *listaGiocatori, int nGiocatori,
                  Giocatore *giocante, Carta *cartaGiocata, Effetto effetto,
                  Giocatore *giocatoriAffetti, int nAffetti,
                  Carta **mazzoPesca, Carta **mazzoScarti);
bool effettoTipoCarta (TipologiaCarta cartaGiocata, TipologiaCarta cartaAffetta);
int effettoTargetGiocatori(Giocatore **listaGiocatori, int nGiocatori, TargetGiocatori target);
void scartaEliminaCarta (Carta **mazzoOrigine, Carta *cartaGiocata, Carta **mazzoScarti);
void rubaPrendiCarta (Carta **mazzoInput,
                      Carta *cartaGiocata, Carta **mazzoDestinazione);

// Misc
void mostraStatusPartita (Giocatore *listaGiocatori, int nGiocatori, bool dettagli);
Carta *scegliCarta (Carta *mazzoScelta, TipologiaCarta tipoCartaGiocata);

#endif //GIOCO_H
