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
    // Gestione dei turni, e l'input scelto dal giocatore
    int turno, scelta;

    // Crea il mazzo di carte, popolato usando il file mazzo.txt
    Carta *mazzoPesca = NULL;

    // Crea il mazzo delle carte scartate (sacrtate) e dell'aula studio
    Carta *mazzoScarti = NULL; // Sacrate (per pochi)
    Carta *mazzoAulaStudio = NULL;

    iniziaPartita(&listaGiocatori, &nGiocatori, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio, &turno, path);

    // TODO: Funzione che controlla quando vinci così la metto nella condizione del loop
    do {
        // Si salva sempre all'inizio del turno, prima di ogni cosa
        salvataggio(nGiocatori, listaGiocatori, mazzoPesca, mazzoScarti, mazzoAulaStudio, path, &turno);

        // Si pesca la carta all'inizio del turno
        pulisciSchermo();
        printf(LINEA_BIANCA
            BOLD "INIZIO DEL TURNO %d PER %s" RESET
            "\n", turno, listaGiocatori->nome);

        // Pesca la carta all'inizio del turno
        pescaCarta(&listaGiocatori->carteGiocatore, &mazzoPesca, &mazzoScarti);

        // Booleano che controlla se si il ciclo qua sotto deve continuare
        bool continua = true; // Se true allora continua a mostrare le opzioni, se false avanti di un turno

        // Esegue gli effetti di tutte le carte che hanno effetti all'inizio del turno
        effettiMazzo(listaGiocatori, nGiocatori, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio, INIZIO);

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
                                            &mazzoPesca, &mazzoScarti, &mazzoAulaStudio);
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
            avantiTurno(turno, &listaGiocatori, &mazzoScarti);
            effettiMazzo(listaGiocatori, nGiocatori, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio, FINE);
            turno++;
        }
    } while (scelta != COMANDO_ESCI);
}

/** Crea una nuova partita, allocando ogni cosa e caricando dal file
 *
 * @param listaGiocatori La lista dinamica che contiene tutti i giocatori
 * @param nGiocatori Numero di giocatori per la partita
 * @param mazzoPesca Il mazzo delle carte che vanno pescate
 * @param turno
 */
void creaNuovaPartita (Giocatore **listaGiocatori, int *nGiocatori, Carta **mazzoPesca, int *turno) {
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

    // Inizializza il turno
    *turno = MIN_1;
}

/** Funzione che gestisce tutte le cose per andare avanti di un turno
 *
 * @param turno Il turno attuale
 * @param listaGiocatori La lista dei giocatori
 * @param mazzoScarti Il mazzo degli scarti
 */
void avantiTurno (int turno, Giocatore **listaGiocatori, Carta **mazzoScarti) {
    // Stampe al giocatore
    pulisciSchermo();
    printf(LINEA_BIANCA
           "FINE DEL TURNO %d PER: %s"
           "\n", turno, (*listaGiocatori)->nome);
    premiInvioPerContinuare();

    // Se il giocatore ha più di 5 carte giocabili, dovrà scartare (sacrtare) una carta o più carte
    while (contaCarte((*listaGiocatori)->carteGiocatore) > MAX_CARTE_MAZZO_GIOCATORE) {
        guiStampaMazzo((*listaGiocatori)->carteGiocatore, false);
        printf("\n"
               "Hai troppe " BBLU "carte in mano" RESET "! Devi scartarne qualcuna!");

        // Carta da scartare, può essere di qualsiasi tipo dato che è nel mazzo del giocatore
        Carta *cartaDaScartare = scegliCarta((*listaGiocatori)->carteGiocatore, ALL);

        // Essendo sempre lo stesso mazzo, è più facile usare la funzione qui sotto rispetto a azioneScartaElimina
        spostaCarta(&(*listaGiocatori)->carteGiocatore, cartaDaScartare, mazzoScarti);
    }
    // Scorre la lista al prossimo giocatore
    *listaGiocatori = (*listaGiocatori)->next;
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
 * @param mazzoScelto Il mazzo da cui va scelta la tipologia della carta
 * @param tipoCartaGiocata Il filtro del tipo della carta
 */
Carta *scegliCarta (Carta *mazzoScelto, TipologiaCarta tipoCartaGiocata) {
    int size = contaCarte(mazzoScelto);

    Carta *cartaScelta;

    // Un while che continua finché il giocatore non sceglie una carta valida
    do {
        cartaScelta = mazzoScelto;
        int scelta = inserisciNumero(MIN_1, size);

        // Scorre fino alla carta
        for (int i = 1; i < scelta; i++)
            cartaScelta = cartaScelta->next;

        // Controlla se la carta inserita è valida, se si continua il ciclo
    } while (!effettoTipoCarta(tipoCartaGiocata, cartaScelta->tipo));

    printf("\n"
           YEL "Hai scelto la carta: " RESET BOLD "%s" RESET
           "\n", cartaScelta->nome);
    premiInvioPerContinuare();

    return cartaScelta;
}

/** Sceglie un mazzo a cui applicare gli effetti
 *
 * @param mazzoAulaStudio Mazzo dell'aula studio
 * @param mazzoBonusMalus Mazzo delle carte bonus e malus
 * @return Ritorna la carta scelta
 */
Carta *scegliMazzo (Carta *mazzoAulaStudio, Carta *mazzoBonusMalus) {
    printf("\n"
           BOLD"SCEGLI IL MAZZO (non puoi cambiarlo dopo)" RESET    "\n"
           "1. Mazzo Aula Studio"                                   "\n"
           "2. Mazzo Bonus-Malus");

    // Intero che permette di scegliere uno dei due mazzi
    int scelta = inserisciNumero(MIN_1, MAX_MAZZI);

    if (scelta == 1)
        return mazzoAulaStudio;
    return mazzoBonusMalus;
}

/** Sceglie un giocatore tra tutti i giocatori (anche se stessi)
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero totale dei giocatori
 */
Giocatore *scegliGiocatore (Giocatore *listaGiocatori, int nGiocatori) {
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
 */
bool giocaCarta (Giocatore *listaGiocatori, int nGiocatori,
                Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio) {
    // Il totale delle carte in mano
    int nCarte = contaCarte(listaGiocatori->carteGiocatore);
    // Un booleano che serve per sapere se ha giocato o meno la carta, se non ha giocato riporta al menù precedente
    bool haGiocatoCarta = false;

    // Pulisce lo schermo e stampa il mazzo
    pulisciSchermo();
    guiStampaMazzo(listaGiocatori->carteGiocatore, true);

    // Se il mazzo contiene carte allora può giocarle
    if (nCarte > MIN_0) {
        printf("\n" LINEA_BIANCA
              "Scegli una " BLU "carta" RESET " da giocare [%d-%d] (%d torna indietro):", MIN_1, nCarte, MIN_0);

        // L'utente inserisce il numero di una carta
        int scelta = inserisciNumero(MIN_0, nCarte);

        // TODO: la carta è possibile giocarla?
        // Se la carta scelta è diversa da ESCI
        if (scelta != MIN_0) {
            // Puntatore temporaneo alle carte giocabili
            Carta *cartaGiocata = listaGiocatori->carteGiocatore;

            // Parte da uno nel contare la carta selezionata, e scorre fino a quella
            for (int i = MIN_1; i < scelta; i++)
                cartaGiocata = cartaGiocata->next;

            // Sposta la carta appena giocata in un mazzo temporaneo
            Carta *mazzoTmp = NULL;
            spostaCarta(&listaGiocatori->carteGiocatore, cartaGiocata, &mazzoTmp);

            // Gestisce tutti gli effetti di una carta
            for (int i = 0; i < cartaGiocata->nEffetti; i++)
                gestisciEffettoCarta(listaGiocatori, nGiocatori, cartaGiocata, mazzoPesca, mazzoScarti, mazzoAulaStudio,
                                     &cartaGiocata->effetto[i]);

            // Sposta la carta giocata nel mazzo giusto, es: negli scarti
            mazzoGiocatoreGiusto(listaGiocatori, cartaGiocata, *mazzoScarti);

            haGiocatoCarta = true;
        }
    } else {
        printf("\n"
              YEL "Non hai carte da giocare!" RESET
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
 * @param effetto L'effetto della carta
 */
void gestisciEffettoCarta (Giocatore *listaGiocatori, int nGiocatori, Carta *cartaGiocata,
                           Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio, Effetto *effetto) {
    /* QUANDO */
    // Se i QUANDO combaciano, allora la carta è di un tipo giocabile in questo momento
    if (cartaGiocata->quandoEffetto == cartaGiocata->quandoEffetto) {
        pulisciSchermo();
        printf(LINEA_BIANCA
                BOLD "Effetti della " RESET BLU "carta" RESET " " UNDERLINE "%s" RESET ", quando: " BMAG,
                cartaGiocata->nome);
        strQuando(cartaGiocata->quandoEffetto);
        printf(RESET "\n");

        /* TARGET */
        // Imposta i dati necessari per la lista dei giocatori che saranno affetti dagli effetti della carta
        // Se è TU, si sceglie il giocatore, altrimenti la funzione imposta in automatico i valori
        Giocatore *giocatoriAffetti = listaGiocatori;
        int nGiocatoriAffetti = effettoTargetGiocatori(&giocatoriAffetti, nGiocatori, effetto->targetGiocatori);

        /* AZIONE */
        // Gestisce un effetto di una carta
        azioneCarta(listaGiocatori, nGiocatori, cartaGiocata, effetto, giocatoriAffetti,
                    nGiocatoriAffetti, mazzoPesca,
                    mazzoScarti, mazzoAulaStudio);
    }
}

/** Richiama la funzione degli effetti della carta in base al Quando come parametro
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero dei giocatori
 * @param mazzoPesca Il mazzo da dove vengono pescate le carte
 * @param mazzoScarti Il mazzo degli scarti
 * @param mazzoAulaStudio Il mazzo dove vanno le matricole dopo che vengono scartate
 * @param quando Quando l'effetto deve essere giocato
 */
void effettiMazzo (Giocatore *listaGiocatori, int nGiocatori,
                   Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio,
                   Quando quando) {
    Carta *tmp = listaGiocatori->carteAulaGiocatore;
    while (tmp != NULL) {
        for (int i = 0; i < tmp->nEffetti; i++)
            if (tmp->quandoEffetto == quando)
                gestisciEffettoCarta(listaGiocatori, nGiocatori, tmp,
                                     mazzoPesca, mazzoScarti, mazzoAulaStudio,
                                     &tmp->effetto[i]);
        tmp = tmp->next;
    }

    tmp = listaGiocatori->carteBonusMalusGiocatore;
    while (tmp != NULL) {
        for (int i = 0; i < tmp->nEffetti; i++)
            if (tmp->quandoEffetto == quando)
                gestisciEffettoCarta(listaGiocatori, nGiocatori, tmp,
                                     mazzoPesca, mazzoScarti, mazzoAulaStudio,
                                     &tmp->effetto[i]);
        tmp = tmp->next;
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

            printf("\n"
                   "Inserisci il giocatore a cui applicare gli effetti.");
            guiMostraGiocatori(*listaGiocatori, nGiocatori - 1);

            int scelta = inserisciNumero(1, nGiocatori - 1);

            // Scorre fino al giocatore scelto
            for (int i = 0; i < scelta - 1; i++)
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
        // Le seguenti 4 azioni sono sostanzialmente le stesse, ma per comodità le divido in due blocchi
        case SCARTA: // Sacrtare le carte (per pochi)
        case ELIMINA:
            azioneScartaElimina(giocatoriAffetti, effetto, mazzoScarti, mazzoAulaStudio);
        break;
        case PRENDI:
        case RUBA:
            azioneRubaPrendi(listaGiocatori, giocatoriAffetti, effetto);
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

/** Gestisce le azioni di scartare una carta
 *
 * @param giocatoreAffetto Il giocatore da cui verrà scartata una carta
 * @param effettoCartaGiocata L'effetto della carta giocata
 * @param mazzoScarti Il mazzo in cui verrà scartata la carta
 * @param mazzoAulaStudio Qui vanno scartate solo le matricole (se la carta è una matricola)
 */
void azioneScartaElimina (Giocatore *giocatoreAffetto, Effetto *effettoCartaGiocata, Carta **mazzoScarti, Carta **mazzoAulaStudio) {

    // Crea un puntatore temporaneo al mazzo da cui verrà scartata la carta
    Carta *mazzoGiocatoreInput = NULL;
    if (effettoCartaGiocata->azione == SCARTA)
        giocatoreAffetto->carteGiocatore;
    else if (effettoCartaGiocata->azione == ELIMINA) {
        if (isStudente(effettoCartaGiocata->tipo))
            mazzoGiocatoreInput = giocatoreAffetto->carteAulaGiocatore;
        else if (isBonusMalus(effettoCartaGiocata->tipo))
            mazzoGiocatoreInput = giocatoreAffetto->carteBonusMalusGiocatore;
        else mazzoGiocatoreInput = scegliMazzo(giocatoreAffetto->carteAulaGiocatore,
                                               giocatoreAffetto->carteBonusMalusGiocatore);
    }

    // Se il mazzo ha almeno una carta, allora può procedere
    int nCarta = contaCarte(mazzoGiocatoreInput);

    if (nCarta > 0) {
        // Stampe per il giocatore
        guiStampaMazzo(mazzoGiocatoreInput, false);
        printf("\n" LINEA_BIANCA
            "Scegli una " BLU "carta" RESET " da " BRED "scartare" RESET " [%d-%d]",
            MIN_1, nCarta);

        // Puntatore alla carta da scartare
        Carta *cartaScelta = scegliCarta(mazzoGiocatoreInput, effettoCartaGiocata->tipo);

        // Puntatore al mazzo dove verrà buttata la carta
        Carta *mazzoOutput = NULL;

        // Se la carta è una matricola, per forza va scartata nell'aula studio, perché non può essere recuperata
        if (cartaScelta->tipo == MATRICOLA) mazzoOutput = *mazzoAulaStudio;
        else mazzoOutput = *mazzoScarti;

        // Scarta la carta
        spostaCarta(&mazzoGiocatoreInput, cartaScelta, &mazzoOutput);
    } else {
        // Se non c'è nessuna carta, stampa un messaggio di avviso ma va avanti comunque, affari di chi butta la carta
        printf("\n"
              YEL "Non è possibile scartare/prendere nessuna carta." RESET
              "\n");
        premiInvioPerContinuare();
    }
}

/** Gestisce l'azione di ruba o prende la carta
 *
 * @param giocante Il giocatore che gioca la carta
 * @param giocatoreAffetto Il giocatore a cui verrà rubata la carta
 * @param effettoCartaGiocata L'effetto della carta giocata
 */
void azioneRubaPrendi (Giocatore *giocante, Giocatore *giocatoreAffetto, Effetto *effettoCartaGiocata) {
    // Puntatore temporaneo a un mazzo del giocatore affetto in base all'effetto della carta giocata
    Carta *mazzoGiocatoreInput = NULL;

    if (effettoCartaGiocata->azione == PRENDI)
        mazzoGiocatoreInput = giocatoreAffetto->carteGiocatore;
    else if (effettoCartaGiocata->azione == RUBA) {
        if (isStudente(effettoCartaGiocata->tipo))
            mazzoGiocatoreInput = giocatoreAffetto->carteAulaGiocatore;
        else if (isBonusMalus(effettoCartaGiocata->tipo))
            mazzoGiocatoreInput = giocatoreAffetto->carteBonusMalusGiocatore;
        else mazzoGiocatoreInput = scegliMazzo(giocatoreAffetto->carteAulaGiocatore,
                                               giocatoreAffetto->carteBonusMalusGiocatore);
    }

    int nCarte = contaCarte(mazzoGiocatoreInput);
    if (nCarte > 0) {
        // Stampe
        guiStampaMazzo(mazzoGiocatoreInput, false);
        printf("\n"
               "Scegli una " BLU "carta" RESET " da " YEL "rubare " RESET "[%d, %d]", MIN_1, nCarte);

        // La carta scelta dal giocante, che verrà rubata
        Carta *cartaScelta = scegliCarta(mazzoGiocatoreInput, effettoCartaGiocata->tipo);

        // Il mazzo delle carte dove verrà posta la carta rubata
        Carta *mazzoGiocatoreOutput = NULL;
        if (effettoCartaGiocata->azione == PRENDI)
            mazzoGiocatoreOutput = giocante->carteGiocatore;
        else if (effettoCartaGiocata->azione == RUBA) {
            if (isStudente(effettoCartaGiocata->tipo))
                mazzoGiocatoreOutput = giocante->carteAulaGiocatore;
            else if (isBonusMalus(effettoCartaGiocata->tipo))
                mazzoGiocatoreOutput = giocante->carteBonusMalusGiocatore;
        }

        // Sposta la carta da un mazzo all'altro (quindi la "ruba")
        spostaCarta(&mazzoGiocatoreInput, cartaScelta, &mazzoGiocatoreOutput);
    } else {
        printf("\n"
                YEL "Non è possibile rubare/scartare nessuna carta." RESET
                "\n");
        premiInvioPerContinuare();
    }
}

/** Funzione che fa pescare una carta dal mazzo della pesca, altrimenti usa quello degli scarti
 *
 */
void pescaCarta (Carta **mazzoGiocatore, Carta **mazzoPesca, Carta **mazzoScarti) {
    // Se il mazzo da pesca è vuoto viene scambiato con quello degli scarti (geniale)
    if (*mazzoPesca == NULL) {
        printf("\n"
            BYEL "Il mazzo della pesca è vuoto, verrà scambiato con quello degli scarti." RESET
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
