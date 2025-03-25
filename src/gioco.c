//
// Created by diego on 30/12/24.
//

#include "gioco.h"

/* Gioco e menù */

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

    //creaNuovaPartita(&nGiocatori, &listaGiocatori, &mazzoPesca);
    caricamento(&nGiocatori, &listaGiocatori, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio, path);

    // Gestione dei turni, e l'input scelto dal giocatore
    int turno = MIN_1, scelta;

    // TODO: Funzione che controlla quando vinci così la metto nella condizione del loop
    do {
        // Si salva sempre all'inizio del turno, prima di ogni cosa
        salvataggio(nGiocatori, listaGiocatori, mazzoPesca, mazzoScarti, mazzoAulaStudio, path);

        // Si pesca la carta all'inizio del turno
        pulisciSchermo();
        printf("\n"
            BOLD "INIZIO DEL TURNO %d" RESET
            "\n", turno);

        pescaCarta(&listaGiocatori->carteGiocatore, &mazzoPesca, &mazzoScarti);

        pulisciSchermo();

        // Booleano che controlla se si il ciclo qua sotto deve continuare
        bool continua = true; // Se true allora continua a mostrare le opzioni, se false avanti di un turno

        // Questo loop controlla le scelte del giocatore
        do {
            // Stampa l'header
            guiHeader(turno, nGiocatori, listaGiocatori->nome);

            // Stampa le scelte e prende la scelta da tastiera prima di entrare nello switch
            guiScegliAzione();
            scelta = inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_4);

            // Controlla la scelta e richiama le funzioni necessarie
            switch (scelta) {
                case COMANDO_OPZIONE_1: // Gioca una carta
                    // Continua solo se il giocatore gioca una carta
                    continua = ! giocaCarta(listaGiocatori, nGiocatori,
                                            &mazzoPesca, &mazzoScarti, &mazzoAulaStudio, SUBITO);
                break;
                case COMANDO_OPZIONE_2: // Pesca una carta
                    pescaCarta(&listaGiocatori->carteGiocatore, &mazzoPesca, &mazzoScarti);
                    continua = false;
                break;
                case COMANDO_OPZIONE_3: // Mostra le carte del giocante
                    pulisciSchermo();
                    guiHeader(turno, nGiocatori, listaGiocatori->nome);
                    mostraStatusPartita(listaGiocatori, MIN_1, false);
                break;
                case COMANDO_OPZIONE_4: // Mostra le carte degli altri giocatori
                    pulisciSchermo();
                    guiHeader(turno, nGiocatori, listaGiocatori->nome);
                    mostraStatusPartita(listaGiocatori->next, nGiocatori - 1, false);
                break;
                default: break; // Aggiunto solo perché CLion dava warning
            }
        } while (scelta != COMANDO_ESCI && continua);

        // Va avanti di un turno
        if (scelta != COMANDO_ESCI) {
            avantiTurno(turno, &listaGiocatori, &mazzoPesca, &mazzoScarti);
            turno++;
        }

    } while (scelta != COMANDO_ESCI);
}

/** Funzione per il caricamento della partita
 *
 */
void iniziaPartita () {

}

/** Crea una nuova partita, allocando ogni cosa e caricando dal file
 *
 * @param nGiocatori Numero di giocatori per la partita
 * @param listaGiocatori La lista dinamica che contiene tutti i giocatori
 * @param mazzoPesca Il mazzo delle carte che vanno pescate
 */
void creaNuovaPartita (int *nGiocatori, Giocatore **listaGiocatori, Carta **mazzoPesca) {
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
void avantiTurno(int turno, Giocatore **listaGiocatori, Carta **mazzoPesca, Carta **mazzoScarti) {
    pulisciSchermo();
    printf("\n"
           BOLD "FINE DEL TURNO %d per %s" RESET, turno, (*listaGiocatori)->nome);

    // Se il giocatore ha più di 5 carte giocabili, dovrà scartare (sacrtare) una carta o più carte
    while (contaCarte((*listaGiocatori)->carteGiocatore) > MAX_CARTE_MAZZO_GIOCATORE) {
        printf("\n"
               "Hai troppe " BBLU "carte in mano" RESET "! Devi scartarne qualcuna!");

        // Crea una carta temporanea al mazzo, che verrà anche usata per scartare le carte se ne ha troppe
        Carta *tmp = (*listaGiocatori)->carteGiocatore;
        int scelta = inserisciNumero(MIN_1, contaCarte(tmp));

        for (int i = 0; i < scelta; i++)
            tmp = tmp->next;

        scartaEliminaCarta(&(*listaGiocatori)->carteGiocatore, tmp, mazzoScarti);
    }

    pulisciSchermo();
    printf("\n"
           "Fine del turno %d per %s."
           "\n", turno, (*listaGiocatori)->nome);
    premiInvioPerContinuare();

    //*listaGiocatori = (*listaGiocatori)->next; // Scorre la lista al prossimo giocatore
}

/** Un menù che chiede al giocatore che cosa vuole vedere riguardo la partita corrente
 *
 * @param listaGiocatori La lista dei giocatori
 * @param nGiocatori Il numero totale dei giocatori
 * @param dettagli Se vedere o meno le carte in dettaglio
 */
void mostraStatusPartita (Giocatore *listaGiocatori, int nGiocatori, bool dettagli) {
    int input; // L'input inserito da tastiera

    // Ciclo per rimanere dentro il menù finché non preme 0
    do {
        pulisciSchermo();

        // Pulisce lo schermo e stampa le opzioni
        guiMostraStatoPartita();

        // Richiesta dell'input al giocatore
        input = inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_4);
        Giocatore *giocatoriAffetti = listaGiocatori;

        switch (input) {
            case COMANDO_OPZIONE_1:
                for (int i = 0; i < nGiocatori; i++) {
                    pulisciSchermo();
                    guiStampaMazzo(giocatoriAffetti->carteGiocatore, dettagli);

                    printf("\n\t"
                        BOLD "Mazzo di %s" RESET,
                        giocatoriAffetti->nome);
                    premiInvioPerContinuare();

                    giocatoriAffetti = giocatoriAffetti->next;
                }
            break;
            case COMANDO_OPZIONE_2:
                for (int i = 0; i < nGiocatori; i++) {
                    pulisciSchermo();
                    guiStampaMazzo(giocatoriAffetti->carteAulaGiocatore, dettagli);

                    printf("\n\t"
                        BOLD "Mazzo di %s" RESET,
                        giocatoriAffetti->nome);
                    premiInvioPerContinuare();

                    giocatoriAffetti = giocatoriAffetti->next;
                }
            break;
            case COMANDO_OPZIONE_3:
                for (int i = 0; i < nGiocatori; i++) {
                    pulisciSchermo();
                    guiStampaMazzo(giocatoriAffetti->carteBonusMalusGiocatore, dettagli);

                    printf("\n\t"
                        BOLD "Mazzo di %s" RESET,
                        giocatoriAffetti->nome);
                    premiInvioPerContinuare();

                    giocatoriAffetti = giocatoriAffetti->next;
                }
            break;
            default: break;
        }
    } while (input != COMANDO_ESCI);
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
        int scelta = inserisciNumero(MIN_1, size);

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
    int scelta = inserisciNumero(MIN_1, nGiocatori);

    // Scorre fino a quel giocatore
    for (int i = 0; i < scelta; i++)
        giocatoreScelto = giocatoreScelto->next;

    printf("\n"
        "Hai scelto %s", giocatoreScelto->nome);

    // Ritorna il giocatore
    return giocatoreScelto;
}

/* Effetti */

/** Una funzione che gioca una carta scelta del giocatore
 * In pratica serve per giocare una carta durante la "Fase Fase Azione"
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero dei giocatori nella partita
 * @param mazzoPesca Il mazzo da cui vengono pescate le carte
 * @param mazzoScarti Il mazzo in cui vengono scartate le carte
 * @param mazzoAulaStudio Il mazzo dove vanno scartate le matricole
 * @param quando Quando la carta può essere giocata
 */
bool giocaCarta(Giocatore *listaGiocatori, int nGiocatori,
                Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio, Quando quando) {
    // Il totale delle carte in mano
    int nCarte = contaCarte(listaGiocatori->carteGiocatore);
    // Un booleano che serve per sapere se ha giocato o meno la carta, se non ha giocato riporta al menù precedente
    bool haGiocatoCarta = false;

    // Pulisce lo schermo e stampa il mazzo
    pulisciSchermo();
    guiStampaMazzo(listaGiocatori->carteGiocatore, true);

    // Se il mazzo contiene carte allora può giocarle
    if (nCarte > MIN_0) {
        printf("\n"
              "-----------------------------------------------------" "\n"
              "Scegli una " BLU "carta" RESET " da giocare [%d-%d] (%d torna indietro):", MIN_1, nCarte, MIN_0);

        // L'utente inserisce il numero di una carta
        int scelta = inserisciNumero(MIN_0, nCarte);

        // Se la carta scelta è "0"
        if (scelta != MIN_0) {
            // Puntatore temporaneo alle carte giocabili
            Carta *cartaGiocata = listaGiocatori->carteGiocatore;

            // Parte da uno nel contare la carta selezionata, e scorre fino a quella
            for (int i = MIN_1; i < scelta; i++)
                cartaGiocata = cartaGiocata->next;

            gestisciEffettiCarta(listaGiocatori, nGiocatori, cartaGiocata, mazzoPesca, mazzoScarti, mazzoAulaStudio, quando);
            // TODO: scartare la carta in automatico quando l'effetto è stato giocato (se viene giocato)
            while (contaCarte(listaGiocatori->carteGiocatore) > MAX_CARTE_MAZZO_GIOCATORE) {
                scartaEliminaCarta(&listaGiocatori->carteGiocatore, cartaGiocata, mazzoScarti);
            }
            haGiocatoCarta = true;
        }
    } else {
        printf("\n"
              "Non hai carte da giocare!"
              "\n");
        premiInvioPerContinuare();
    }
    return haGiocatoCarta;
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
 * @param mazzoAulaStudio Il mazzo dove vanno scartate le matricole
 * @param quando Quando la carta può essere giocata, se non combacia con la carta giocata allora salta la carta
 */
void gestisciEffettiCarta (Giocatore *listaGiocatori, int nGiocatori,
                           Carta *cartaGiocata,
                           Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio, Quando quando) {
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
                        mazzoScarti, mazzoAulaStudio);
        }
    }
}

/** Restituisce vero se la tipologia della carta come parametro è uguale a quella della carta
 *
 * @param tipoCartaGiocata Il tipo della carta che viene giocata in questo momento
 * @param tipoCartaAffetta Il tipo della carta che può essere affetta
 * @return Ritorna true le tipologie combaciano, altrimenti false
 */
bool effettoTipoCarta (TipologiaCarta tipoCartaGiocata, TipologiaCarta tipoCartaAffetta) {
    bool ris = false;

    switch (tipoCartaGiocata) {
        case ALL:
            ris = true;
        break;
        case STUDENTE:
            ris = isStudente(tipoCartaAffetta);
        break;
        case BONUS:
        case MALUS:
            ris = isBonusMalus(tipoCartaAffetta);
        break;
        default:
            ris = tipoCartaGiocata == tipoCartaAffetta;
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
            nTarget = MIN_1;
        break;
        case TU:
            // Scorre in avanti la lista e nGiocatori - 1 per far scegliere tutti tranne se stessi
            *listaGiocatori = (*listaGiocatori)->next;
            printf("Scegli giocatore:");
            int scelta = inserisciNumero(MIN_1, nGiocatori - 1);

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

/* Azioni */

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
 * @param mazzoAulaStudio
 */
void azioneCarta (Giocatore *listaGiocatori, int nGiocatori,
                  Carta *cartaGiocata, Effetto *effetto,
                  Giocatore *giocatoriAffetti, int nAffetti,
                  Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio) {
    // Puntatore a tipo Carta che verrà usata dentro lo switch case (principalmente per i mazzi)
    Carta *tmp;

    // Switch che gestisce tutte le azioni che ha l'effetto della carta
    switch (effetto->azione) {
        case GIOCA:
            //giocaCarta(giocatoriAffetti, nGiocatori);
        break;
        // SCARTA ed ELIMINA fanno eliminare una carta da un mazzo dei giocatori affetti
        case SCARTA: // Sacrtare le carte (per pochi)
        case ELIMINA:
            tmp = mazzoGiocatorePerAzione(giocatoriAffetti, cartaGiocata->tipo, effetto->azione);

            // Se la carta è di tipo matricola, andrà eliminata e messa nel mazzo delle matricole
            if (effettoTipoCarta(cartaGiocata->tipo, MATRICOLA))
                scartaEliminaCarta(&tmp, cartaGiocata, mazzoAulaStudio);
            // Altrimenti va nel mazzo degli scarti
            else scartaEliminaCarta(&tmp, cartaGiocata, mazzoScarti);
        break;
        // RUBA e PRENDI fanno rubare una carta da uno dei mazzi dei giocatori affetti
        case PRENDI:
        case RUBA:
            tmp = mazzoGiocatorePerAzione(giocatoriAffetti, cartaGiocata->tipo, effetto->azione);

            rubaPrendiCarta(&giocatoriAffetti->carteGiocatore, cartaGiocata, &tmp);
        break;
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
    guiStampaCarta(*mazzoPesca, false);

    spostaCarta(mazzoPesca, *mazzoPesca, mazzoGiocatore);

    premiInvioPerContinuare();
}

/** Funzione che gestisce gli effetti di SCARTA ed ELIMINA
 * @param mazzoOrigine Il mazzo dove risiede la carta da eliminare
 * @param cartaGiocata La carta da eliminare
 * @param mazzoScarti Il mazzo dove andrà scartata (o sacrtata) la carta
 */
void scartaEliminaCarta (Carta **mazzoOrigine, Carta *cartaGiocata, Carta **mazzoScarti) {
    if (contaCarte(*mazzoOrigine) >= MIN_1) {
        int nCarte = contaCarte(*mazzoOrigine);

        pulisciSchermo();
        guiStampaMazzo(*mazzoOrigine, false);
        printf("\n"
               "-----------------------------------------------------" "\n"
               "Scegli una " BLU "carta" RESET " da scartare [%d-%d] (%d torna indietro):", MIN_1, nCarte, MIN_0);

        Carta *cartaDaRimuovere = scegliCarta(*mazzoOrigine, cartaGiocata->tipo);
        spostaCarta(mazzoOrigine, cartaDaRimuovere, mazzoScarti);
    } else {
        printf("\n"
               BYEL "Non puoi scartare nessuna carta!");
        premiInvioPerContinuare();
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

/** Funzione che restituisce il mazzo appropriato in base all'azione
 *
 * @param giocatoreAffetto Il giocatore di cui verrà restituito il mazzo appropriato per l'azione
 * @param tipoCartaGiocata Il tipo della carta giocata
 * @param azione L'azione della carta
 * @return Restituisce il mazzo in cui andrà effettuato l'effetto
 */
Carta *mazzoGiocatorePerAzione (Giocatore *giocatoreAffetto, TipologiaCarta tipoCartaGiocata, Azione azione) {
    Carta *tmp = NULL;

    // Se l'azione è SCARTA o PRENDI, il mazzo affetto è sempre quello della mano
    if (azione == SCARTA || azione == PRENDI)
        tmp = giocatoreAffetto->carteGiocatore;
    // Altrimenti, se è ELIMINA o RUBA, può essere uno dei due mazzi in base al tipo
    else if (azione == ELIMINA || azione == RUBA) {
        // Se l'effetto è BONUS o MALUS, sarà quello delle bonus-malus
        if (effettoTipoCarta(tipoCartaGiocata, BONUS))
            tmp = giocatoreAffetto->carteBonusMalusGiocatore;
        // Altrimenti, per esclusione, è l'aula
        else tmp = giocatoreAffetto->carteAulaGiocatore;
    }

    // Restituisce il temporaneo
    return tmp;
}

/** Funzione che restituisce un mazzo di carte in base al tipo della carta messa come parametro
 *
 * @param mazzoScarti Il mazzo dove vanno scartate la maggior parte delle carte
 * @param mazzoAulaStudio Il mazzo in cui vanno scartate le carte
 * @param tipo Il tipo della carta giocata
 * @return Ritorna il mazzo in base al tipo della carta
 */
Carta *mazzoScartiPerTipoCarta (Carta *mazzoScarti, Carta *mazzoAulaStudio, TipologiaCarta tipo) {
    Carta *mazzo = NULL;

    if (effettoTipoCarta(tipo, MATRICOLA))
        mazzo = mazzoAulaStudio;
    else mazzo = mazzoScarti;

    return mazzo;
}
