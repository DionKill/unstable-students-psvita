//
// Created by diego on 30/12/24.
//

#include "gioco.h"

/** La funzione principale del gioco. Da qui viene gestito tutto.
 *
 */
int gioco() {
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
        salvataggio(nGiocatori, listaGiocatori, mazzoPesca, mazzoScarti, mazzoAulaStudio, SALVATAGGIO);

        pulisciSchermo();
        guiHeader(turno, nGiocatori, listaGiocatori->nome);

        guiScegliAzione();
        int scelta = inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_4);

        // Controlla la scelta e richiama le funzioni necessarie
        switch (scelta) {
            case COMANDO_OPZIONE_1:
                giocaCarta(listaGiocatori, nGiocatori);
                turno = avantiTurno(turno, &listaGiocatori, &mazzoPesca, &mazzoScarti);
            break;
            case COMANDO_OPZIONE_2:
                pescaCarta(&listaGiocatori->carteGiocatore, &mazzoPesca, &mazzoScarti);
                turno = avantiTurno(turno, &listaGiocatori, &mazzoPesca, &mazzoScarti);
            break;
            case COMANDO_OPZIONE_3:
                pulisciSchermo();
                guiHeader(turno, nGiocatori, listaGiocatori->nome);
                mostraStatusPartita(listaGiocatori, nGiocatori, false);
            break;
            case COMANDO_OPZIONE_4:
                pulisciSchermo();
                guiHeader(turno, nGiocatori, listaGiocatori->nome);
                mostraStatusPartita(listaGiocatori->next, nGiocatori, false);
            break;
            case COMANDO_ESCI:
                return 0;
            default: break; // Aggiunto solo perché CLion dava warning
        }
    }
    return turno;
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

    // Dealloca il mazzo delle matricole, che non servirà più
    while (mazzoMatricole != NULL) {
        Carta *tmpMatricole = mazzoMatricole;
        mazzoMatricole = mazzoMatricole->next;
        free(tmpMatricole);
    }
}

int avantiTurno(int turno, Giocatore **listaGiocatori, Carta **mazzoPesca, Carta **mazzoScarti) {
    pescaCarta(&(*listaGiocatori)->carteGiocatore, mazzoPesca, mazzoScarti);

    *listaGiocatori = (*listaGiocatori)->next; // Scorre la lista
    return turno + 1;
}

/** Una funzione che gioca una carta scelta del giocatore
 * In pratica serve per disessere giocare le carte
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero dei giocatori nella partita
 */
void giocaCarta (Giocatore *listaGiocatori, int nGiocatori) {
    // Pulisce lo schermo e stampa il mazzo
    pulisciSchermo();
    guiStampaMazzo(listaGiocatori->carteGiocatore, true);

    // L'utente inserisce una carta
    int nCarte = contaCarte(listaGiocatori->carteGiocatore);
    int scelta = inserisciNumero(1, nCarte);

    // Puntatore temporaneo alle carte giocabili
    Carta *carta = listaGiocatori->carteGiocatore;

    // Parte da uno nel contare la carta selezionata
    for (int i = 0; i < scelta; ++i) {
        carta = carta->next;
    }

    // Ciclo che va avanti nEffetti volte
    for (int i = 0; i < carta->nEffetti; i++) {
        Giocatore *tmpGiocatoriAffetti = listaGiocatori;

    }
}

/** Una funzione che gestisce gli effetti della carta
 *
 */
void gestisciEffettiCarta (Giocatore **listaGiocatori, int nGiocatori,
                           Giocatore *giocante, Carta *cartaGiocata, Effetto effetto) {
    // Imposta i dati necessari per la lista dei giocatori che saranno affetti dagli effetti della carta
    Giocatore *giocatoriAffetti = *listaGiocatori;
    int nGiocatoriAffetti = effettoTargetGiocatori(&giocatoriAffetti, nGiocatori, effetto.targetGiocatori);
}

/** Gestisce gli effetti della carta
 *
 * Lista dei giocatori
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero che contiene quanti giocatori ci sono nella partita
 *
 * Giocatore che gioca la carta, e il suo effetto
 * @param giocante Il giocatore che sta giocando la carta
 * @param cartaGiocata La carta giocata dal giocatore
 * @param effetto L'effetto della carta
 *
 * I giocatori che saranno affetti dall'azione della carta
 * @param giocatoriAffetti La lista dei giocatori affetti dalla carta giocata
 * @param nAffetti Il numero dei giocatori affetti dalla carta giocata
 * @param mazzoPesca
 * @param mazzoScarti
 */
void azioneCarta (Giocatore *listaGiocatori, int nGiocatori,
                  Giocatore *giocante, Carta *cartaGiocata, Effetto effetto,
                  Giocatore *giocatoriAffetti, int nAffetti,
                  Carta **mazzoPesca, Carta **mazzoScarti) {
    // Switch che gestisce tutte le azioni che ha l'effetto della carta
    switch (effetto.azione) {
        case GIOCA:
            giocaCarta(giocante, nGiocatori);
        break;
        case SCARTA:
        case ELIMINA:
            // Sono uguali, cambia solo il mazzo che usano
            scartaEliminaCarta(giocante, cartaGiocata, mazzoScarti);
        break;
        case RUBA:
        case PRENDI:

        break;
        case PESCA:
            pescaCarta(&giocante->carteGiocatore, mazzoPesca, mazzoScarti);
        break;
        case SCAMBIA:
        break;
        // Qua sotto sono effetti che non possono essere trovati nelle carte giocabili
        default:
        break;
    }
}

/** Restituisce vero se la tipologia della carta come parametro è uguale a quella della carta
 *
 * @param cartaGiocata Il tipo della carta che viene giocata in questo momento
 * @param cartaAffetta Il tipo della carta che può essere affetta
 * @return
 */
bool effettoTipoCarta (TipologiaCarta cartaGiocata, TipologiaCarta cartaAffetta) {
    bool ris = false;

    switch (cartaGiocata) {
        case ALL:
            ris = true;
        break;
        case STUDENTE:
            if (cartaAffetta == MATRICOLA || cartaAffetta == STUDENTE_SEMPLICE || cartaAffetta == LAUREANDO)
                ris = true;
        break;
        case BONUS:
        case MALUS:
            if (cartaAffetta == BONUS || cartaAffetta == MALUS)
                ris = true;
        break;
        default:
            if (cartaGiocata == cartaAffetta)
                ris = true;
        break;
    }
    return ris;
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

void scartaEliminaCarta (Giocatore *giocatore, Carta *cartaGiocata, Carta **mazzoScarti) {
    printf("\n"
           "Scegli la carta da scartare dal tuo mazzo %s"
           "\n");

    // Il mazzo di appoggio da cui dovrà essere rimossa la carta scelta dal giocatore
    Carta *mazzoDestinazione = mazzoDelGiocatoreGiustoInBaseAlTipoDellaCarta(cartaGiocata, giocatore);

    int nCarte = contaCarte(mazzoDestinazione);
    Carta *tmp;

    // Ciclo che continua se la carta da scartare è di un tipo compatibile con la carta scelta
    do {
        int scelta = inserisciNumero(1, nCarte);

        // Temporaneo per scorrere il mazzo fino al punto necessario
        tmp = mazzoDestinazione;
        for (int i = 1; i < scelta; i++) {
            tmp = tmp->next;
        }
    } while ( ! effettoTipoCarta(cartaGiocata->tipo, tmp->tipo) ); // Quando da true, esce

    spostaCarta(&giocatore->carteGiocatore, cartaGiocata, &mazzoDestinazione);
}

void rubaPrendiCarta (Giocatore *giocante, Carta *cartaGiocata,
                      Giocatore *giocatoriAffetti, int nGiocatoriAffetti) {
    printf("\n"
           "Ruba una carta dall'avversario!"
           "\n");

    Carta *tmp;

    for (int i = 0; i < nGiocatoriAffetti; i++) {
        Carta *mazzoDestinazione = mazzoDelGiocatoreGiustoInBaseAlTipoDellaCarta(cartaGiocata, giocatoriAffetti);
        int nCarte = contaCarte(mazzoDestinazione);

        do {
            int scelta = inserisciNumero(1, nCarte);

            // Temporaneo per scorrere il mazzo fino al punto necessario
            tmp = mazzoDestinazione;
            for (int j = 1; j < scelta; j++) {
                tmp = tmp->next;
            }
        } while ( ! effettoTipoCarta(cartaGiocata->tipo, tmp->tipo) ); // Quando da true, esce
    }
}

/** Funzione che fa pescare una carta dal mazzo della pesca, altrimenti usa quello degli scarti
 *
 */
void pescaCarta (Carta **mazzoGiocatore, Carta **mazzoPesca, Carta **mazzoScarti) {
    // Se il mazzo da pesca è vuoto viene scambiato con quello degli scarti (geniale)
    if (*mazzoPesca == NULL) {
        printf("\n"
               "Il mazzo della pesca è vuoto, verrà scambiato con quello degli scarti."
               "\n");
        *mazzoPesca = *mazzoScarti;
        *mazzoScarti = NULL;
        // Mischia il mazzo della pesca appena ottenuto
        shuffleCarte(mazzoPesca);
    }

    printf("\n"
           "Hai pescato:"
           "\n");
    spostaCarta(mazzoPesca, *mazzoPesca, mazzoGiocatore);
    guiStampaCarta(*mazzoPesca, false);
    premiInvioPerContinuare();
    pulisciSchermo();
}

/** Un menù che chiede al giocatore che cosa vuole vedere riguardo la partita corrente
 *
 * @param listaGiocatori La lista dei giocatori
 * @param nGiocatori Il numero totale dei giocatori
 * @param dettagli Se vedere o meno le carte in dettaglio
 */
void mostraStatusPartita (Giocatore *listaGiocatori, int nGiocatori, bool dettagli) {
    // Pulisce lo schermo e stampa le opzioni
    guiMostraStatoPartita();

    // Richiesta dell'input al giocatore
    int input = inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_4);
    switch (input) {
        case COMANDO_OPZIONE_1:
            for (int i = 0; i < nGiocatori; i++) {
                pulisciSchermo();
                guiStampaMazzo(listaGiocatori->carteGiocatore, dettagli);
                premiInvioPerContinuare();
                listaGiocatori = listaGiocatori->next;
            }
        break;
        case COMANDO_OPZIONE_2:
            for (int i = 0; i < nGiocatori; i++) {
                pulisciSchermo();
                guiStampaMazzo(listaGiocatori->carteAulaGiocatore, dettagli);
                premiInvioPerContinuare();
                listaGiocatori = listaGiocatori->next;
            }
        break;
        case COMANDO_OPZIONE_3:
            for (int i = 0; i < nGiocatori; i++) {
                pulisciSchermo();
                guiStampaMazzo(listaGiocatori->carteBonusMalusGiocatore, dettagli);
                premiInvioPerContinuare();
                listaGiocatori = listaGiocatori->next;
            }
        break;
        default: break;
    }
}
