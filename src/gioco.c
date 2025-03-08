//
// Created by diego on 30/12/24.
//

#include "gioco.h"

/** La funzione principale del gioco. Da qui viene gestito tutto.
 *
 */
void gioco () {
    // Crea la lista dei giocatori e li popola
    Giocatore *listaGiocatori = NULL;
    int nGiocatori;

    // Crea il mazzo di carte, popolato usando il file mazzo.txt
    Carta *mazzoPesca = NULL;

    // Crea il mazzo delle carte scartate (sacrtate) e dell'aula studio
    Carta *mazzoScarti = NULL; // Sacrate (per pochi)
    Carta *mazzoAulaStudio = NULL;

    // Mazzo che contiene le matricole che verranno distribuite a tutti i giocatori
    Carta *mazzoMatricole = NULL;

    //creaNuovaPartita(&nGiocatori, &listaGiocatori, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio);
    caricamento(&nGiocatori, &listaGiocatori, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio, SALVATAGGIO);

    // Gestione dei turni
    int turno = 1;

    // TODO: Funzione che controlla quando vinci così la metto nella condizione del main
    // Questo loop controlla le scelte del giocatore. È abbastanza self-explanatory
    while (turno < 50) {
        pulisciSchermo();
        guiHeader(turno, nGiocatori, listaGiocatori->nome);

        guiScegliAzione();
        int scelta = inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_4);

        // Controlla la scelta e richiama le funzioni necessarie
        switch (scelta) {
            case COMANDO_OPZIONE_1:
                // TODO: gioca una carta
                giocaCarta(listaGiocatori);
                turno = avantiTurno(turno, &listaGiocatori, &mazzoPesca);
            break;
            case COMANDO_OPZIONE_2:
                pescaCarta(&listaGiocatori->carteGiocatore, &mazzoPesca);
                turno = avantiTurno(turno, &listaGiocatori, &mazzoPesca);
            break;
            case COMANDO_OPZIONE_3:
                pulisciSchermo();
                guiHeader(turno, nGiocatori, listaGiocatori->nome);

                mostraStatusPartita(listaGiocatori, nGiocatori);

            break;
            case COMANDO_ESCI:
                // TODO: tutta la roba del salvataggio lol
                return;
            default: break; // Aggiunto solo perché CLion dava warning
        }
        salvataggio(nGiocatori, listaGiocatori, mazzoPesca, mazzoScarti, mazzoAulaStudio, SALVATAGGIO);
    }
}

/** Crea una nuova partita, allocando ogni cosa e caricando dal file
 *
 * @param nGiocatori Numero di giocatori per la partita
 * @param listaGiocatori La lista dinamica che contiene tutti i giocatori
 * @param mazzoPesca Il mazzo delle carte che vanno pescate
 * @param mazzoScarti Il mazzo degli scarti
 * @param mazzoAulaStudio Il mazzo delle carte STUDENTE che vengono buttate qui quando giocate
 */
void creaNuovaPartita (int *nGiocatori, Giocatore **listaGiocatori, Carta **mazzoPesca,
                       Carta **mazzoScarti, Carta **mazzoAulaStudio) {
    // Crea i giocatori e restituisce l'intero che contiene il numero dei giocatori
    *nGiocatori = creaGiocatori(listaGiocatori);

    // Crea il mazzo di carte, e lo popola usando il file mazzo.txt
    leggiCarteDaFile(mazzoPesca);
    Carta *mazzoMatricole = dividiMazzoMatricola(mazzoPesca); // Crea il mazzo matricola partendo dal mazzo da pesca

    // Mischia i due mazzi appena creati
    shuffleCarte(mazzoPesca);
    shuffleCarte(&mazzoMatricole);

    // Distribuisce le carte a ogni giocatore
    distribuisciCarte(*nGiocatori, *listaGiocatori, &mazzoMatricole); // Distribuisce le matricole
    distribuisciCarte(CARTE_DA_DISTRIBUIRE * *nGiocatori, *listaGiocatori, mazzoPesca); // Distribuisce il resto

    // Rimuove il mazzo delle matricole, che non servirà più
    free(mazzoMatricole);
}

int avantiTurno(int turno, Giocatore **listaGiocatori, Carta **mazzoPesca) {
    pescaCarta(&(*listaGiocatori)->carteGiocatore, mazzoPesca);

    *listaGiocatori = (*listaGiocatori)->next; // Scorre la lista
    return turno + 1;
}

/** Una funzione che gioca una carta scelta del giocatore
 *
 */
void giocaCarta (Giocatore *giocatore) {
    // Pulisce lo schermo e stampa il mazzo
    pulisciSchermo();
    guiStampaMazzo(giocatore->carteGiocatore, false);

    // L'utente inserisce una carta
    int nCarte = contaCarte(giocatore->carteGiocatore);
    int scelta = inserisciNumero(1, nCarte);

    gestioneEffetti(giocatore, scelta);
}

/** Gestisce gli effetti delle carte.
 * @param listaGiocatori Il giocatore attuale
 * @param nCarta Il numero della carta da utilizzare (per non passare la carta direttamente)
*/
void gestioneEffetti (Giocatore *listaGiocatori, int nCarta) {
    // Puntatore temporaneo alle carte giocabili
    Carta *tmp = listaGiocatori->carteGiocatore;
    // Parte da uno nel contare la carta da selezionare
    for (int i = 0; i < nCarta; ++i) {
        tmp = tmp->next;
    }

    // Una variabile che contiene il contenuto di nEffetti, perché l'accesso è troppo frequente per lasciarlo a tmp
    int nEffetti = tmp->nEffetti;

    for (int i = 0; i < nEffetti; i++) {
        // Gestione azioni

    }
}

/** Gestisce gli eventi dell'effetto azione della carta
 *
 * @param giocatore Il giocatore interessato
 * @param nGiocatori
 * @param azione L'azione della carta
 */
void effettoAzioneCarta (Giocatore *giocatore, int nGiocatori, Azione azione) {
    // Crea un giocatore temporaneo per gestire gli effetti
    Giocatore *giocatoreTmp = NULL;

    switch (azione) {
        case GIOCA:
            giocaCarta(giocatore);
        break;
        case SCARTA:
            printf("\n" "Dovrai scartare una carta. Scegli quale.");
        break;
        case ELIMINA:

        break;
        case RUBA:
        break;
        case PESCA:
        break;
        case PRENDI:
        break;
        case BLOCCA:
        break;
        case SCAMBIA:
        break;
        case MOSTRA:
        break;
        case IMPEDIRE:
        break;
        case INGEGNERE:
        break;
    }
    while (giocatoreTmp != NULL);
}

/** Chiama l'effetto della
 *
 * @param target Il o i giocatori a cui verranno applicati gli effetti
 */
void effettoTargetCarta(Giocatore *giocatore, int nGiocatori, Carta *carta, Effetto *effetto) {
    // Il target dei giocatori
    switch (effetto->targetGiocatori) {
        case IO:
            effettoAzioneCarta(giocatore, nGiocatori, effetto->azione);
        break;
        case TU:
            // Scorre in avanti la lista e nGiocatori - 1 per far scegliere tutti tranne se stessi
            giocatore = giocatore->next;
            int scelta = inserisciNumero(1, nGiocatori - 1);

            // Scorre fino al giocatore scelto
            for (int i = 0; i < scelta; i++)
                giocatore = giocatore->next;
            effettoAzioneCarta(giocatore, nGiocatori, effetto->azione);
        break;
        case VOI:
            giocatore = giocatore->next;
            effettoAzioneCarta(giocatore, nGiocatori - 1, effetto->azione);
        break;
        case TUTTI:
            effettoAzioneCarta(giocatore, nGiocatori, effetto->azione);
    }
}

/** Un menù che chiede al giocatore che cosa vuole vedere riguardo la partita corrente
 *
 * @param listaGiocatori La lista dei giocatori
 */
void mostraStatusPartita (Giocatore *listaGiocatori, int nGiocatori) {
    // Pulisce lo schermo e stampa le opzioni
    guiMostraStatoPartita();

    // Richiesta dell'input al giocatore
    int input = inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_4);
    switch (input) {
        case COMANDO_OPZIONE_1:
            pulisciSchermo();
            guiStampaMazzo(listaGiocatori->carteGiocatore, false);
            premiInvioPerContinuare();
        break;
        case COMANDO_OPZIONE_2:
            pulisciSchermo();
            guiStampaMazzo(listaGiocatori->carteAulaGiocatore, false);
            premiInvioPerContinuare();
        break;
        case COMANDO_OPZIONE_3:
            pulisciSchermo();
            guiStampaMazzo(listaGiocatori->carteBonusMalusGiocatore, false);
            premiInvioPerContinuare();
        break;
        case COMANDO_OPZIONE_4:
            for (int i = 0; i < nGiocatori - 1; i++) {
                listaGiocatori = listaGiocatori->next;
                if (false) // TODO: se il giocatore contiene una carta che gli fa mostrare tutto o non so bene come funzioni
                    guiStampaCarteGiocatore(listaGiocatori, true);
                else guiStampaCarteGiocatore(listaGiocatori, false);
            }
            premiInvioPerContinuare();
        break;
        default: break;
    }
}

/** Piccola funzione che fa pescare una carta
 *
 */
void pescaCarta (Carta **mazzoGiocatore, Carta **mazzoPesca) {
    printf("\n"
           "Hai pescato:"
           "\n");
    spostaCarta(mazzoPesca, *mazzoPesca, mazzoGiocatore);
    guiStampaCarta(*mazzoPesca, false);
    premiInvioPerContinuare();
    pulisciSchermo();
}
