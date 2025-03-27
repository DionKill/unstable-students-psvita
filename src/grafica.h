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

// A essere sincero non so se sono necessarie, comunque le ho fatte giusto per comodità
// Azione
#define STR_GIOCA "GIOCA"
#define STR_SCARTA "SCARTA"
#define STR_ELIMINA "ELIMINA"
#define STR_RUBA "RUBA"
#define STR_PESCA "PESCA"
#define STR_PRENDI "PRENDI"
#define STR_BLOCCA "BLOCCA"
#define STR_SCAMBIA "SCAMBIA"
#define STR_MOSTRA "MOSTRA"
#define STR_IMPEDIRE "IMPEDIRE"
#define STR_INGEGNERE "INGEGNERE"
// Tipologia carta
#define STR_ALL "ALL"
#define STR_STUDENTE "STUDENTE"
#define STR_MATRICOLA "MATRICOLA"
#define STR_STUDENTE_SEMPLICE "STUDENTE SEMPLICE"
#define STR_LAUREANDO "LAUREANDO"
#define STR_BONUS "BONUS"
#define STR_MALUS "MALUS"
#define STR_MAGIA "MAGIA"
#define STR_ISTANTANEA "ISTANTANEA"
// Target giocatori
#define STR_IO "IO"
#define STR_TU "TU"
#define STR_VOI "VOI"
#define STR_TUTTI "TUTTI"
// Quando
#define STR_SUBITO "SUBITO"
#define STR_INIZIO "INIZIO"
#define STR_FINE "FINE"
#define STR_MAI "MAI"
#define STR_SEMPRE "SEMPRE"
// Può essere giocato (conosciuto anche come disessere giocati)
#define STR_TRUE "VERO"
#define STR_FALSE "FALSO"

// GUI
void guiSplashScreen ();
void guiHeader (int turno, int nGiocatori, char nomeGiocatore[]);
void guiStampaMazzo (Carta *mazzoCarte, bool dettagli);
void guiStampaCarta (Carta *carta, bool dettagli);
void guiStampaCarteGiocatore (Giocatore *giocatore, bool mostraMano);
void guiScegliAzione ();
void guiMostraStatoPartita ();
void guiMostraGiocatori (Giocatore *listaGiocatori, int nGiocatori);

// Stringhe
void strAzione (Azione azione);
void strTipologiaCarta (TipologiaCarta tipologia);
void strTargetGiocatori (TargetGiocatori target);
void strQuando (Quando quando);
void strOpzionale (bool opzionale); // Disessere giocati
void strColoreGiocatore (char **str, int giocatore);

// Misc
void pulisciSchermo ();

#endif //GRAFICA_H
