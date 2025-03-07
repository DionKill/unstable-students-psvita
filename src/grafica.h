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


// GUI
void guiSplashScreen ();
void guiHeader (int turno, int nGiocatori, char nomeGiocatore[]);
void guiStampaMazzo (Carta *mazzoCarte, bool dettagli);
void guiStampaCarta (Carta *carta, bool dettagli);
void guiStampaCarteGiocatore (Giocatore *giocatore, bool mostraMano);
void guiScegliAzione ();
void guiMostraStatoPartita ();

// Stringhe
void strAzione(char **str, Azione azione);
void strTipologiaCarta(char **str, TipologiaCarta tipologia);
void strTargetGiocatori(char **str, TargetGiocatori target);
void strQuando(char **str, Quando quando);
void strPuoEssereGiocato (char **str, bool puoEssereGiocato); // Disessere giocati
void strColoreGiocatore (char **str, int giocatore);

// Misc
void pulisciSchermo ();

#endif //GRAFICA_H
