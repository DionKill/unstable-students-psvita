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

// Macro rappresentanti gli input da tastiera che vengono usati nel gioco e altro
#define COMANDO_OPZIONE_1 1 // Gioca una carta, ...
#define COMANDO_OPZIONE_2 2 // Pesca una carta, ...
#define COMANDO_OPZIONE_3 3 // Mostra le carte, ...
#define COMANDO_OPZIONE_4 4 // Le carte altrui, ...
#define COMANDO_ESCI 0 // Serve per uscire dal programma o tornare indietro
#define COMANDO_CONFERMA 13 // Invio
#define COMANDO_ANNULLA 27 // ESC
#define MIN_0 0 // Il minimo per diverse cose: numero di turni, il minimo da cercare...
#define MIN_1 1 // Il minimo per diverse cose: numero di turni, il minimo da cercare...

#define MAX_MAZZI 2
#define MAX_CARTE_MAZZO_GIOCATORE 5

// Gioco e menù
void gioco (char *path);
void creaNuovaPartita (Giocatore **listaGiocatori, int *nGiocatori, Carta **mazzoPesca, int *turno);
void avantiTurno (int turno, Giocatore **listaGiocatori, Carta **mazzoScarti);
void mostraStatusPartita (Giocatore *listaGiocatori, int nGiocatori, bool dettagli);
Carta *scegliCarta (Carta *mazzoScelto, TipologiaCarta tipoCartaGiocata);
Carta *scegliMazzo (Carta *mazzoAulaStudio, Carta *mazzoBonusMalus);
Giocatore *scegliGiocatore (Giocatore *listaGiocatori, int nGiocatori);

// Effetti
bool giocaCarta (Giocatore *listaGiocatori, int nGiocatori, Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio);
void gestisciEffettoCarta (Giocatore *listaGiocatori, int nGiocatori,
                           Carta *cartaGiocata,
                           Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio, Effetto *effetto);

void effettiMazzo (Giocatore *listaGiocatori, int nGiocatori,
                   Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio,
                   Quando quando);
bool effettoTipoCarta (TipologiaCarta tipoCartaGiocata, TipologiaCarta tipoCartaAffetta);
int effettoTargetGiocatori (Giocatore **listaGiocatori, int nGiocatori, TargetGiocatori target);

// Azioni
void azioneCarta (Giocatore *listaGiocatori, int nGiocatori,
                  Carta *cartaGiocata, Effetto *effetto,
                  Giocatore *giocatoriAffetti, int nAffetti,
                  Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio);
void azioneScartaElimina (Giocatore *giocatoreAffetto, Effetto *effettoCartaGiocata, Carta **mazzoScarti, Carta **mazzoAulaStudio);
void azioneRubaPrendi (Giocatore *giocante, Giocatore *giocatoreAffetto, Effetto *effettoCartaGiocata);
void pescaCarta (Carta **mazzoGiocatore, Carta **mazzoPesca, Carta **mazzoScarti);

#endif //GIOCO_H
