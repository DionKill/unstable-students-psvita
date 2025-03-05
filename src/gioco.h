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

// Macro rappresentanti gli input da tastiera che vengono usati nel gioco, come per giocare una carta, pescare etc.
#define COMANDO_OPZIONE_1 1 // Gioca una carta, ...
#define COMANDO_OPZIONE_2 2 // Pesca una carta, ...
#define COMANDO_OPZIONE_3 3 // Mostra le carte, ...
#define COMANDO_OPZIONE_4 4 // Le carte altrui, ...
#define COMANDO_ESCI 0 // Serve per uscire dal programma o tornare indietro
#define COMANDO_CONFERMA 13 // Invio
#define COMANDO_ANNULLA 27 // ESC

void gioco ();
void giocaCarta (Giocatore *giocatore);
void gestioneEffetti ();
void pescaCarta (Carta **mazzoGiocatore, Carta **mazzoPesca);

int avantiTurno(int turno, Giocatore **listaGiocatori, Carta **mazzoPesca);
int scegliAzione ();
void creaNuovaPartita (int *nGiocatori, Giocatore **listaGiocatori, Carta **mazzoPesca,
                        Carta **mazzoScarti, Carta **mazzoAulaStudio);
void mostraStatusPartita (Giocatore *listaGiocatori);

#endif //GIOCO_H
