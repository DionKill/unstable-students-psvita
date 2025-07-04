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
#include "vita.h"

// Gioco e menù
void gioco (char *path);
void creaNuovaPartita (Giocatore **listaGiocatori, int *nGiocatori, Carta **mazzoPesca, int *turno);
void avantiTurno (int turno, Giocatore **listaGiocatori, Carta **mazzoScarti);
void mostraStatusPartita (Giocatore *giocante, Giocatore *listaGiocatori, int nGiocatori);
void mostraMazzoGiocabile (Giocatore* giocante, Giocatore *giocatoriAffetti, int nGiocatoriAffetti);

// Effetti
bool giocaCarta (Giocatore *listaGiocatori, int nGiocatori,
                 Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio);
bool controlloVittoria (Giocatore *giocatore);

bool gestisciEffettiCarta (Giocatore *listaGiocatori, int nGiocatori, Carta *cartaGiocata,
                          Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio, Quando momento);
void effettiQuando (Giocatore *listaGiocatori, int nGiocatori,
                   Quando quando, Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio);
void effettiMazzoCarta (Giocatore *listaGiocatori, int nGiocatori, Quando quando,
                          Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio, Carta *mazzo);
// Azioni
void azioneCarta (Giocatore *listaGiocatori, int nGiocatori, Giocatore *giocatoreAffetto, Carta *cartaGiocata,
                  Effetto *effetto, Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio);
void azioneScartaElimina (Giocatore *giocatoreAffetto, Effetto *effetto, Carta **mazzoScarti, Carta **mazzoAulaStudio);
void azioneRubaPrendi (Giocatore *giocante, Giocatore *giocatoreAffetto, Effetto *effetto);
void pescaCarta (Carta **mazzoGiocatore, Carta **mazzoPesca, Carta **mazzoScarti, char *giocatore);

#endif //GIOCO_H
