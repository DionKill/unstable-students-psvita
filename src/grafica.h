//
// Created by diego on 22/02/25.
//

#ifndef GRAFICA_H
#define GRAFICA_H

#include <stdio.h>
#include <stdlib.h>
#include "strutture.h"
#include "misc.h"
#include "colori-ansi.h"
#include "vita.h"

// GUI
void guiSplashScreen ();
void guiHeader (int turno, int nGiocatori, char nomeGiocatore[]);
void guiStampaMazzo (Carta *mazzoCarte, bool dettagli);
void guiStampaCarta (Carta *carta, bool dettagli);
void guiScegliAzione ();
void guiMostraStatoPartita ();
void guiGiocaOpzionale (char *giocatore, char *carta);
void guiMostraGiocatori (Giocatore *listaGiocatori, int nGiocatori);
void guiVittoria (char *nome);

// Stringhe
const char *strAzione(Azione azione);
const char *strTipologiaCarta(TipologiaCarta tipologia);
const char *strTargetGiocatori(TargetGiocatori target);
const char *strQuando(Quando quando);
const char *strOpzionale(bool opzionale); // Disessere giocati
const char *strColoreGiocatore(int nGiocatore);

// Misc
void pulisciSchermo ();

#endif //GRAFICA_H
