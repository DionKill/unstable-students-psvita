//
// Created by diego on 28/12/24.
//

#ifndef STRUTTURE_H
#define STRUTTURE_H

#include <stdbool.h>

#define NOME_GIOCATORE_LENGTH 32 //32 char utili
#define NOME_CARTA_LENGTH 31 //32 char in totale, compreso il terminatore di stringa (perché?)
#define DESC_CARTA_LENGTH 255 //256 char totali per la descrizione della carta

typedef enum {ALL, STUDENTE, MATRICOLA, STUDENTE_SEMPLICE, LAUREANDO, BONUS, MALUS, MAGIA, ISTANTANEA} TipologiaCarta;
typedef enum {GIOCA, SCARTA, ELIMINA, RUBA, PESCA, PRENDI, BLOCCA, SCAMBIA, MOSTRA, IMPEDIRE, INGEGNERE} Azione;
typedef enum {SUBITO, INIZIO, FINE, MAI, SEMPRE} Quando;
typedef enum {IO, TU, VOI, TUTTI} TargetGiocatori;

typedef struct giocatore {
    char nome[NOME_GIOCATORE_LENGTH + 1];
    //lista carte giocatore
    //lista carte aula giocatore
    //lista carte bonus/malus giocatore
    struct giocatore *next;
} Giocatore;

typedef struct carta {
    char nome[NOME_CARTA_LENGTH + 1];
    char desc[DESC_CARTA_LENGTH + 1];
    TipologiaCarta tipo;
    int nEffetti;
    struct effetto *effetto;
    Quando quandoEffetto;
    bool puoEssereGiocato;
    struct carta *next;
} Carta;

typedef struct effetto {
    Azione azione;
    TipologiaCarta tipo;
    TargetGiocatori targetGiocatori;
} Effetto;

#endif //STRUTTURE_H
