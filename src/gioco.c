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
                giocaCarta(listaGiocatori, nGiocatori);
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
void giocaCarta (Giocatore *listaGiocatori, int nGiocatori) {
    // Pulisce lo schermo e stampa il mazzo
    pulisciSchermo();
    guiStampaMazzo(listaGiocatori->carteGiocatore, false);

    // L'utente inserisce una carta
    int nCarte = contaCarte(listaGiocatori->carteGiocatore);
    int scelta = inserisciNumero(1, nCarte);

    // Puntatore temporaneo alle carte giocabili
    Carta *carta = listaGiocatori->carteGiocatore;

    // Parte da uno nel contare la carta selezionata
    for (int i = 0; i < scelta; ++i) {
        carta = carta->next;
    }

    // Struttura che contiene tutti i dati per poter effettuare la giocata
    GiocatoriAffetti giocatoriAffetti = {
        listaGiocatori,
        nGiocatori,

        listaGiocatori,
        carta
    };

    // Ciclo che va avanti nEffetti volte
    for (int i = 0; i < carta->nEffetti; i++) {
        Giocatore *tmpGiocatoriAffetti = listaGiocatori;

        // Questi parametri andranno sempre cambiati, perciò vengono dichiarati qua dentro.
        giocatoriAffetti.indexEffetto = i;
        giocatoriAffetti.giocatoriAffetti = tmpGiocatoriAffetti;
        giocatoriAffetti.nGiocatoriAffetti = effettoTargetGiocatori(&tmpGiocatoriAffetti, nGiocatori, carta->effetto[i].targetGiocatori);

        //effettoAzioneCarta
    }

}

/** Gestisce gli eventi dell'effetto azione della carta
 *
 * @param giocatoriAffetti Una struttura che contiene puntatori ai giocatori, carta giocata e giocatori affetti
 * @param mazzoPesca Il mazzo dove pescare le carte
 * @param mazzoScarti Il mazzo dove vengono scartate le carte
 */
void effettoAzioneCarta (GiocatoriAffetti giocatoriAffetti, Carta **mazzoPesca, Carta **mazzoScarti) {
    // Crea una variabile per... Evitare di dover scrivere tutto quello
    Azione azione = giocatoriAffetti.cartaGiocata->effetto[giocatoriAffetti.indexEffetto].azione;
    int nGiocatori = giocatoriAffetti.nGiocatori;

    switch (azione) {
        case GIOCA:
            giocaCarta(giocatoriAffetti.giocante, nGiocatori);
        break;
        case SCARTA: {
            printf("\n" "Dovrai scartare una carta. Scegli quale.");

            int nCarte = contaCarte(giocatoriAffetti.giocante->carteGiocatore);
            int scelta = inserisciNumero(1, nCarte);

            Carta *tmp = giocatoriAffetti.giocante->carteGiocatore;
            for (int i = 1; i < scelta; i++) {
                tmp = tmp->next;
            }

            spostaCarta(&giocatoriAffetti.giocante->carteGiocatore, tmp, mazzoScarti);
        }
        break;
        case ELIMINA: {
            printf("\n" "Dovrai scartare una carta. Scegli quale.");

            int nCarte = contaCarte(giocatoriAffetti.giocante->carteGiocatore);
            int scelta = inserisciNumero(1, nCarte);

            Carta *tmp = giocatoriAffetti.giocante->carteGiocatore;
            for (int i = 1; i < scelta; i++) {
                tmp = tmp->next;
            }

            spostaCarta(&giocatoriAffetti.giocante->carteGiocatore, tmp, mazzoScarti);
        }
        break;
        case RUBA:

        break;
        case PESCA:
            pescaCarta(&giocatoriAffetti.giocante->carteGiocatore, mazzoPesca);
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
}

/** Restituisce la lista di giocatori a cui vanno applicati gli effetti.
 * Perché fare questo al posto di creare una nuova lista?
 * Perché per farlo va allocata memoria, e non ha senso dato che alla fine si lavora sempre sugli stessi puntatori
 * di mazzi di carte dei giocatori (in breve: sarebbe più complicato del necessario).
 *
 * @param listaGiocatori La lista dei giocatori originale, modificata in base al target
 * @param nGiocatori Il numero dei giocatori totali della partita
 * @param target L'effetto è applicato a questo/i giocatore/i
 */
int effettoTargetGiocatori(Giocatore **listaGiocatori, int nGiocatori, TargetGiocatori target) {
    // La lunghezza della lista
    int nTarget = nGiocatori;

    // Questo switch controlla i target, e modifica la variabile di ritorno, insieme alla lista
    switch (target) {
        case IO:
            nTarget = 1;
        break;
        case TU:
            // Scorre in avanti la lista e nGiocatori - 1 per far scegliere tutti tranne se stessi
            *listaGiocatori = (*listaGiocatori)->next;
            int scelta = inserisciNumero(1, nGiocatori - 1);

            // Scorre fino al giocatore scelto
            for (int i = 0; i < scelta; i++)
                *listaGiocatori = (*listaGiocatori)->next;

            nTarget = nGiocatori - 1;
        break;
        case VOI:
            *listaGiocatori = (*listaGiocatori)->next;
            nTarget = nGiocatori - 1;
        break;
        case TUTTI:
            nTarget = nGiocatori;
    }
    return nTarget;
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
