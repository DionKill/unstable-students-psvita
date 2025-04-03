//
// Created by diego on 28/12/24.
//

#ifndef STRUTTURE_H
#define STRUTTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#define NOME_LENGTH 31 //32 char totali
#define NOME_LENGTH_STR "31"
#define DESC_CARTA_LENGTH 255 //256 char totali per la descrizione della carta
#define CARTE_DA_DISTRIBUIRE 5

#define GIOCATORI_MIN 2
#define GIOCATORI_MAX 4

typedef enum {GIOCA, SCARTA, ELIMINA, RUBA, PESCA, PRENDI, BLOCCA, SCAMBIA, MOSTRA, IMPEDIRE, INGEGNERE} Azione;
typedef enum {ALL, STUDENTE, MATRICOLA, STUDENTE_SEMPLICE, LAUREANDO, BONUS, MALUS, MAGIA, ISTANTANEA} TipologiaCarta;
typedef enum {IO, TU, VOI, TUTTI} TargetGiocatori;
typedef enum {SUBITO, INIZIO, FINE, MAI, SEMPRE} Quando;

typedef struct giocatore {
    char nome[NOME_LENGTH + 1];
    struct carta *carteGiocatore;
    struct carta *carteAulaGiocatore;
    struct carta *carteBonusMalusGiocatore;
    struct giocatore *next;
} Giocatore;

typedef struct carta {
    char nome[NOME_LENGTH + 1];
    char descrizione[DESC_CARTA_LENGTH + 1];
    TipologiaCarta tipo;
    int nEffetti;
    struct effetto *effetto;
    Quando quandoEffetto;
    bool opzionale; // Disessere giocati (per pochi)
    struct carta *next;
} Carta;

typedef struct effetto {
    Azione azione;
    TipologiaCarta tipo;
    TargetGiocatori targetGiocatori;
} Effetto;

// Gestione giocatori (alcune di queste includono inserimento di testo, ma era inevitabile)
Giocatore *allocaGiocatore ();
int creaGiocatori (Giocatore **listaGiocatori);
void rendiListaGiocatoriCircolare (Giocatore *listaGiocatori);

// Gestione carte
Carta *allocaCarta ();
Carta *copiaCarta (Carta *carta, int nCopie);
Carta *dividiMazzoMatricole (Carta **mazzo);
Carta *cercaCarta(Carta *mazzo, Azione azione, Quando quando, TargetGiocatori target);
Carta **mazzoGiocatoreGiusto(Giocatore *giocatore, Carta *carta, Carta **mazzoScarti);
int contaCarte (Carta *mazzo);
void spostaCarta (Carta **mazzoInput, Carta *cartaInput, Carta **mazzoOutput);
void shuffleCarte (Carta **mazzoOriginale);
void distribuisciCarte (int cntCarte, Giocatore *listaGiocatori, Carta **mazzoPesca);

// Gestione effetti
bool esisteAzioneNelMazzo (Carta *mazzo, Azione azione);
bool isStudente (TipologiaCarta tipo);
bool isBonusMalus (TipologiaCarta tipo);

// Miscellanee
void liberaMemoria (Carta *testaMazzo);

#endif //STRUTTURE_H
