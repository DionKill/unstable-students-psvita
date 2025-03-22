//
// Created by diego on 30/12/24.
//

#include "gioco.h"

/** La funzione principale del gioco. Da qui viene gestito tutto.
 *
 */
void gioco (char *path) {
    // Crea la lista dei giocatori e li popola
    Giocatore *listaGiocatori = NULL;
    int nGiocatori;

    // Crea il mazzo di carte, popolato usando il file mazzo.txt
    Carta *mazzoPesca = NULL;

    // Crea il mazzo delle carte scartate (sacrtate) e dell'aula studio
    Carta *mazzoScarti = NULL; // Sacrate (per pochi)
    Carta *mazzoAulaStudio = NULL;

    creaNuovaPartita(&nGiocatori, &listaGiocatori, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio);
    //caricamento(&nGiocatori, &listaGiocatori, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio, path);

    // Gestione dei turni, e l'input scelto dal giocatore
    int turno = MIN, scelta;

    // TODO: Funzione che controlla quando vinci così la metto nella condizione del loop
    // TODO: risolvere che ogni cosa che fai ti fa pescare una carta (un altro loop)
    // Questo loop controlla le scelte del giocatore
    do {
        // Si salva sempre all'inizio del turno, prima di ogni cosa
        salvataggio(nGiocatori, listaGiocatori, mazzoPesca, mazzoScarti, mazzoAulaStudio, path);

        pulisciSchermo();

        // Si pesca la carta all'inizio del turno
        pescaCarta(&listaGiocatori->carteGiocatore, &mazzoPesca, &mazzoScarti);

        // Stampa l'header
        guiHeader(turno, nGiocatori, listaGiocatori->nome);

        // Stampa le scelte e prende la scelta da tastiera prima di entrare nello switch
        guiScegliAzione();
        scelta = inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_4);

        // Controlla la scelta e richiama le funzioni necessarie
        switch (scelta) {
            case COMANDO_OPZIONE_1:
                giocaCarta(listaGiocatori, nGiocatori, &mazzoPesca, &mazzoScarti, SUBITO);
                avantiTurno(&turno, &listaGiocatori, &mazzoPesca, &mazzoScarti);
            break;
            case COMANDO_OPZIONE_2:
                pescaCarta(&listaGiocatori->carteGiocatore, &mazzoPesca, &mazzoScarti);
                avantiTurno(&turno, &listaGiocatori, &mazzoPesca, &mazzoScarti);
            break;
            case COMANDO_OPZIONE_3:
                pulisciSchermo();
                guiHeader(turno, nGiocatori, listaGiocatori->nome);
                mostraStatusPartita(listaGiocatori, MIN, false);
            break;
            case COMANDO_OPZIONE_4:
                pulisciSchermo();
                guiHeader(turno, nGiocatori, listaGiocatori->nome);
                mostraStatusPartita(listaGiocatori->next, nGiocatori - 1, false);
            break;
            default: break; // Aggiunto solo perché CLion dava warning
        }
    } while (scelta != COMANDO_ESCI);
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
    printf("\n"
           BYEL "Nessun salvataggio trovato/selezionato"
           RESET ", creazione di una nuova partita..."
           "\n");
    // Crea i giocatori e restituisce l'intero che contiene il numero dei giocatori
    *nGiocatori = creaGiocatori(listaGiocatori);

    // Crea il mazzo di carte, e lo popola usando il file mazzo.txt
    leggiCarteDaFile(mazzoPesca);
    Carta *mazzoMatricole = dividiMazzoMatricole(mazzoPesca); // Crea il mazzo matricola partendo dal mazzo da pesca

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

/** Funzione che gestisce tutte le cose per andare avanti di un turno
 *
 * @param turno Il turno attuale
 * @param listaGiocatori La lista dei giocatori
 * @param mazzoPesca Il mazzo da cui pescare le carte
 * @param mazzoScarti Il mazzo degli scarti
 */
void avantiTurno(int *turno, Giocatore **listaGiocatori, Carta **mazzoPesca, Carta **mazzoScarti) {
    // Se il giocatore ha più di 5 carte giocabili, dovrà scartare (sacrtare) una carta
    if (contaCarte((*listaGiocatori)->carteGiocatore) > MAX_CARTE_MAZZO_GIOCATORE)
        scartaEliminaCarta(&(*listaGiocatori)->carteGiocatore, NULL, mazzoScarti);

    pulisciSchermo();
    printf("\n"
           "Fine del turno %d per %s."
           "\n", *turno, (*listaGiocatori)->nome);
    premiInvioPerContinuare();

    *listaGiocatori = (*listaGiocatori)->next; // Scorre la lista al prossimo giocatore
    turno++;
}

/** Una funzione che gioca una carta scelta del giocatore
 * In pratica serve per giocare una carta durante la "Fase Fase Azione"
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero dei giocatori nella partita
 * @param mazzoPesca Il mazzo da cui vengono pescate le carte
 * @param mazzoScarti Il mazzo in cui vengono scartate le carte
 * @param quando Quando la carta può essere giocata
 */
void giocaCarta (Giocatore *listaGiocatori, int nGiocatori,
                 Carta **mazzoPesca, Carta **mazzoScarti, Quando quando) {
    // Pulisce lo schermo e stampa il mazzo
    pulisciSchermo();
    guiStampaMazzo(listaGiocatori->carteGiocatore, true);

    printf("\n"
           "--------------------------------------------" "\n"
           "Scegli una " BLU "carta" RESET " da giocare:");

    // L'utente inserisce una carta
    int nCarte = contaCarte(listaGiocatori->carteGiocatore);
    int scelta = inserisciNumero(MIN, nCarte);

    // Puntatore temporaneo alle carte giocabili
    Carta *cartaGiocata = listaGiocatori->carteGiocatore;

    // Parte da uno nel contare la carta selezionata, e scorre fino a quella
    for (int i = MIN; i < scelta; i++)
        cartaGiocata = cartaGiocata->next;

    gestisciEffettiCarta(listaGiocatori, nGiocatori, cartaGiocata, mazzoPesca, mazzoScarti, quando);
    // TODO: scartare la carta in automatico quando l'effetto è stato giocato (se viene giocato)
    while (contaCarte(listaGiocatori->carteGiocatore) > MAX_CARTE_MAZZO_GIOCATORE) {
        scartaEliminaCarta(&listaGiocatori->carteGiocatore, cartaGiocata, mazzoScarti);
    }
}

/** Gestisce gli effetti di una carta.
 *
 * Gli effetti vengono gestiti in questo ordine:
 * QUANDO, TARGET, AZIONE
 * Questo modo è ordinato e permette l'esecuzione degli effetti per ogni specifica carta
 *
 * Se il QUANDO è "vero", allora la carta può essere giocata durante l'esecuzione
 * Se TARGET è TU, allora bisognerà chiedere al giocatore chi sarà il giocatore affetto
 * AZIONE effettua l'azione "attiva" della carta
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero totale dei giocatori
 * @param cartaGiocata La carta giocata dal giocatore
 * @param mazzoPesca Il mazzo da cui pescare le carte
 * @param mazzoScarti Il mazzo in cui scartare (sacrtare, per pochi insomma) le carte
 * @param quando Quando la carta può essere giocata, se non combacia con la carta giocata allora salta la carta
 */
void gestisciEffettiCarta (Giocatore *listaGiocatori, int nGiocatori,
                           Carta *cartaGiocata,
                           Carta **mazzoPesca, Carta **mazzoScarti, Quando quando) {
    /* QUANDO */
    // Se i QUANDO combaciano, allora la carta è di un tipo giocabile in questo momento
    if (cartaGiocata->quandoEffetto == quando) {

        /* TARGET */
        // Imposta i dati necessari per la lista dei giocatori che saranno affetti dagli effetti della carta
        // Se è TU, si sceglie il giocatore, altrimenti la funzione imposta in automatico i valori
        Giocatore *giocatoriAffetti = listaGiocatori;
        int nGiocatoriAffetti = effettoTargetGiocatori(&giocatoriAffetti, nGiocatori, cartaGiocata->tipo);

        /* AZIONE */
        // Continua finché gli effetti non sono finiti
        for (int i = 0; i < cartaGiocata->nEffetti; i++) {
            azioneCarta(listaGiocatori, nGiocatori, cartaGiocata, &cartaGiocata->effetto[i], giocatoriAffetti,
                        nGiocatoriAffetti, mazzoPesca,
                        mazzoScarti);
        }
    }
}

/** Gestisce gli effetti della carta
 *
 * Lista dei giocatori
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero che contiene quanti giocatori ci sono nella partita
 *
 * Giocatore che gioca la carta, e il suo effetto
 * @param cartaGiocata La carta giocata dal giocatore
 * @param effetto L'effetto della carta
 *
 * I giocatori che saranno affetti dall'azione della carta
 * @param giocatoriAffetti La lista dei giocatori affetti dalla carta giocata
 * @param nAffetti Il numero dei giocatori affetti dalla carta giocata
 * @param mazzoPesca Il mazzo da dove vengono pescate le carte
 * @param mazzoScarti Il mazzo dove vengono scartate, o per meglio dire, "sacrtate", le carte
 */
void azioneCarta (Giocatore *listaGiocatori, int nGiocatori,
                  Carta *cartaGiocata, Effetto *effetto,
                  Giocatore *giocatoriAffetti, int nAffetti,
                  Carta **mazzoPesca, Carta **mazzoScarti) {
    // Puntatore a tipo Carta che verrà usata dentro lo switch case (principalmente per i mazzi)
    Carta *tmp;

    // Switch che gestisce tutte le azioni che ha l'effetto della carta
    switch (effetto->azione) {
        case GIOCA:
            //giocaCarta(giocatoriAffetti, nGiocatori);
        break;
        // SCARTA ed ELIMINA fanno eliminare una carta da un mazzo dei giocatori affetti
        case SCARTA: // Sacrtare le carte (per pochi)
            tmp = giocatoriAffetti->carteGiocatore;
        case ELIMINA:
            if (effettoTipoCarta(cartaGiocata->tipo, BONUS))
                tmp = giocatoriAffetti->carteBonusMalusGiocatore;
            else tmp = giocatoriAffetti->carteAulaGiocatore;
            scartaEliminaCarta(&tmp, cartaGiocata, mazzoScarti);
        break;
        // RUBA e PRENDI fanno rubare una carta da uno dei mazzi dei giocatori affetti
        case PRENDI:
            tmp = giocatoriAffetti->carteGiocatore;
        break;
        case RUBA:
            if (effettoTipoCarta(cartaGiocata->tipo, BONUS))
                tmp = giocatoriAffetti->carteBonusMalusGiocatore;
            else tmp = giocatoriAffetti->carteAulaGiocatore;
            scartaEliminaCarta(&tmp, cartaGiocata, mazzoScarti);
        case PESCA: // Fa pescare una carta ai giocatori affetti
            pescaCarta(&giocatoriAffetti->carteGiocatore, mazzoPesca, mazzoScarti);
        break;
        // Scambia la propria mano con quella di un altro giocatore
        case SCAMBIA:
            tmp = giocatoriAffetti->carteGiocatore;
            giocatoriAffetti->carteGiocatore = listaGiocatori->carteGiocatore;
            listaGiocatori->carteGiocatore = tmp;
        break;
        // Qua sotto sono effetti che non possono essere trovati nelle carte giocabili, perciò non hanno effetto
        default:
        break;
    }
}

/** Restituisce vero se la tipologia della carta come parametro è uguale a quella della carta
 *
 * @param cartaGiocata Il tipo della carta che viene giocata in questo momento
 * @param cartaAffetta Il tipo della carta che può essere affetta
 * @return Ritorna true le tipologie combaciano, altrimenti false
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
int effettoTargetGiocatori (Giocatore **listaGiocatori, int nGiocatori, TargetGiocatori target) {
    // La lunghezza della lista
    int nTarget = nGiocatori;

    // Questo switch controlla i target, e modifica la variabile di ritorno, insieme alla lista
    switch (target) {
        case IO:
            nTarget = MIN;
        break;
        case TU:
            // Scorre in avanti la lista e nGiocatori - 1 per far scegliere tutti tranne se stessi
            *listaGiocatori = (*listaGiocatori)->next;
            printf("Scegli giocatore:");
            int scelta = inserisciNumero(MIN, nGiocatori - 1);

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

/** Funzione che gestisce gli effetti di SCARTA ed ELIMINA
 *
 */
void scartaEliminaCarta (Carta **mazzoOrigine, Carta *cartaGiocata, Carta **mazzoScarti) {
    if (contaCarte(*mazzoOrigine) >= MIN) {
        guiStampaMazzo(*mazzoOrigine, false);
        printf("\n"
       "Scegli la carta da scartare dal tuo mazzo %s");

        Carta *cartaDaRimuovere = scegliCarta(*mazzoOrigine, cartaGiocata->tipo);
        spostaCarta(mazzoOrigine, cartaDaRimuovere, mazzoScarti);
    } else {
        printf("\n"
               BYEL "Non puoi scartare nessuna carta!");
    }
}

/** Gestione degli effetti RUBA e PRENDI
 *
 * @param mazzoInput Il mazzo da cui andrà presa la carta
 * @param cartaGiocata La carta giocata dal giocatore
 * @param mazzoDestinazione Il mazzo dove andrà spostata la nuova carta
 */
void rubaPrendiCarta (Carta **mazzoInput, Carta *cartaGiocata, Carta **mazzoDestinazione) {
    printf("\n"
           "Ruba una carta all'avversario!"
           "\n");

    guiStampaMazzo(*mazzoDestinazione, false);
    Carta *cartaSelezionata = scegliCarta(*mazzoInput, cartaGiocata->tipo);

    spostaCarta(mazzoInput, cartaSelezionata, mazzoDestinazione);
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
                printf("\n"
                       "Carte di %s", listaGiocatori->nome);
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

/** Il giocatore sceglie una carta dal mazzo in base al tipo necessario
 *
 * @param mazzoScelta Il mazzo da cui va scelta la tipologia della carta
 * @param tipoCartaGiocata Il filtro del tipo della carta
 */
Carta *scegliCarta (Carta *mazzoScelta, TipologiaCarta tipoCartaGiocata) {
    int size = contaCarte(mazzoScelta);

    Carta *cartaScelta = mazzoScelta;

    // Un while che continua finché il giocatore non sceglie una carta valida
    do {
        int scelta = inserisciNumero(MIN, size);

        // Scorre fino alla carta
        for (int i = 0; i < scelta; i++) {
            cartaScelta = cartaScelta->next;
        }
        // Controlla se la carta inserita è valida, se si continua il ciclo
    } while (! effettoTipoCarta(tipoCartaGiocata, cartaScelta->tipo));

    return cartaScelta;
}

/** Sceglie un giocatore tra tutti i giocatori (anche se stessi)
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero totale dei giocatori
 */
Giocatore *scegliGiocatore(Giocatore *listaGiocatori, int nGiocatori) {
    // Puntatore temporaneo alla lista dei giocatori
    Giocatore *giocatoreScelto = listaGiocatori;

    printf("\n"
        "Scegli il giocatore:"
        CURSORE_INPUT);

    // Scorre tutti i giocatori per far vedere i nomi
    for (int i = 0; i < nGiocatori; i++) {
        printf("%d: %s", i + 1, giocatoreScelto->nome);
        giocatoreScelto = giocatoreScelto->next;
    }

    // "Resetta" il puntatore temporaneo
    giocatoreScelto = listaGiocatori;
    // Inserisce il numero che corrisponde all'indice del giocatore
    int scelta = inserisciNumero(MIN, nGiocatori);

    // Scorre fino a quel giocatore
    for (int i = 0; i < scelta; i++)
        giocatoreScelto = giocatoreScelto->next;

    printf("\n"
        "Hai scelto %s", giocatoreScelto->nome);

    // Ritorna il giocatore
    return giocatoreScelto;
}
