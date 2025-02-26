//
// Created by diego on 22/02/25.
//

#ifndef GRAFICA_H
#define GRAFICA_H

#include <stdio.h>
#include <stdlib.h>
#include "colori-ansi.h"
#include "strutture.h"

// GUI
void guiMenu ();
void guiScegliAzione ();
void guiStampaMazzo (Carta *mazzoCarte);
void guiStampaCarta (Carta *carta);
void guiHeader (int turno, char nomeGiocatore[]);

// Da enum a stringa
void strAzione(char **str, Azione azione);
void strTipologiaCarta(char **str, TipologiaCarta tipologia);
void strTargetGiocatori(char **str, TargetGiocatori target);
void strQuando(char **str, Quando quando);
void strPuoEssereGiocato (char **str, bool puoEssereGiocato); // Disessere giocati

// Misc
void pulisciSchermo ();

#endif //GRAFICA_H
