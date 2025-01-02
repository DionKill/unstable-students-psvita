//
// Created by diego on 28/12/24.
//

#ifndef STRUTTURE_H
#define STRUTTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NOME_LENGTH 31 //32 char utili
#define NOME_LENGTH_STR "31"
#define DESC_CARTA_LENGTH 255 //256 char totali per la descrizione della carta

typedef enum {ALL, STUDENTE, MATRICOLA, STUDENTE_SEMPLICE, LAUREANDO, BONUS, MALUS, MAGIA, ISTANTANEA} TipologiaCarta;
typedef enum {GIOCA, SCARTA, ELIMINA, RUBA, PESCA, PRENDI, BLOCCA, SCAMBIA, MOSTRA, IMPEDIRE, INGEGNERE} Azione;
typedef enum {SUBITO, INIZIO, FINE, MAI, SEMPRE} Quando;
typedef enum {IO, TU, VOI, TUTTI} TargetGiocatori;

typedef struct giocatore {
    char nome[NOME_LENGTH + 1];
    struct carta *carteGiocatore;
    struct carta *carteAulaGiocatore;
    struct carta *carteBonusMalusGiocatore;
    struct giocatore *next;
} Giocatore;

typedef struct carta {
    char nome[NOME_LENGTH + 1];
    char desc[DESC_CARTA_LENGTH + 1];
    TipologiaCarta tipo;
    int nEffetti;
    struct effetto *effetto;
    Quando quandoEffetto;
    bool puoEssereGiocato; // Disessere giocati (per pochi)
    struct carta *next;
} Carta;

typedef struct effetto {
    Azione azione;
    TipologiaCarta tipo;
    TargetGiocatori targetGiocatori;
} Effetto;

Giocatore  *inizializzaListaGiocatori (int nGiocatori);
Giocatore *aggiungiGiocatori (Giocatore *nodo, int nGiocatori);
Giocatore *inserisciTestaInCoda (Giocatore *listaGiocatori);

#endif //STRUTTURE_H
