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
    bool puoEssereGiocato; // Disessere giocati (per pochi)
    struct carta *next;
} Carta;

typedef struct effetto {
    Azione azione;
    TipologiaCarta tipo;
    TargetGiocatori targetGiocatori;
} Effetto;

/* Strutture aggiuntive */

// Struttura che serve per gestire gli effetti delle carte sui giocatori
typedef struct giocatoriAffetti {
    // La lista di tutti i giocatori, con la sua lunghezza
    struct giocatore *listaGiocatori;
    int nGiocatori;

    // Il giocatore che sta giocando, con la sua carta e l'indice dell'effetto da applicare
    struct giocatore *giocante;
    struct carta *cartaGiocata;
    int indexEffetto;

    // La lista che contiene il o i giocatori che subiranno gli effetti delle carte
    struct giocatore *giocatoriAffetti;
    int nGiocatoriAffetti;
} GiocatoriAffetti;

// Gestione giocatori (alcune di queste includono inserimento di testo, ma era inevitabile)
int creaGiocatori (Giocatore **listaGiocatori);
Giocatore *allocaGiocatore ();
void rendiListaGiocatoriCircolare (Giocatore *listaGiocatori);

// Gestione carte
Carta *allocaCarta ();
int contaCarte (Carta *mazzo);
Carta *copiaCarta (Carta *carta, int nCopie);
Carta *dividiMazzoMatricola (Carta **mazzo);
void shuffleCarte (Carta **mazzoOriginale);
void spostaCarta (Carta **mazzoInput, Carta *cartaInput, Carta **mazzoOutput);
void distribuisciCarte (int cntCarte, Giocatore *listaGiocatori, Carta **mazzoPesca);

// Miscellanee
void liberaMemoria (Carta *testaMazzo);

#endif //STRUTTURE_H
