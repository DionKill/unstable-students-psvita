//
// Created by diego on 30/12/24.
//

#include "gioco.h"

/* Gioco e menù */

/** La funzione principale del gioco. Da qui viene gestito tutto.
 *
 */
void gioco(char *path) {
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
        effettiMazzo(listaGiocatori, nGiocatori, INIZIO, &mazzoPesca, &mazzoScarti, &mazzoAulaStudio);

        // Questo loop controlla le scelte del giocatore
        do {
            // Stampa header
            guiHeader(turno, nGiocatori, listaGiocatori->nome);

            // Stampa le scelte e prende la scelta da tastiera prima di entrare nello switch
            guiScegliAzione();
            scelta = inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_4);

            // Controlla la scelta e richiama le funzioni necessarie
            switch (scelta) {
                case COMANDO_OPZIONE_1: // Gioca una carta
                    // Continua solo se il giocatore gioca una carta
                    continua = !giocaCarta(listaGiocatori, nGiocatori,
                                           &mazzoPesca, &mazzoScarti, &mazzoAulaStudio);
                    break;
                case COMANDO_OPZIONE_2: // Pesca una carta
                    pescaCarta(&listaGiocatori->carteGiocatore, &mazzoPesca, &mazzoScarti);
                    continua = false;
                    break;
                case COMANDO_OPZIONE_3: // Mostra le carte del giocante
                    pulisciSchermo();
                    guiHeader(turno, nGiocatori, listaGiocatori->nome);
                    mostraStatusPartita(listaGiocatori, listaGiocatori, MIN_1);
                    break;
                case COMANDO_OPZIONE_4: // Mostra le carte degli altri giocatori
                    pulisciSchermo();
                    guiHeader(turno, nGiocatori, listaGiocatori->nome);
                    mostraStatusPartita(listaGiocatori, listaGiocatori->next, nGiocatori - 1);
                    break;
                default: break; // Aggiunto solo perché CLion dava warning
            }
        } while (scelta != COMANDO_ESCI && continua);

        // Va avanti di un turno
        if (scelta != COMANDO_ESCI) {
            avantiTurno(turno, &listaGiocatori, &mazzoScarti);
            turno++;
        }
    } while (scelta != COMANDO_ESCI && !areYaWinningSon(listaGiocatori));
}

/** Crea una nuova partita, allocando ogni cosa e caricando dal file
 *
 * @param listaGiocatori La lista dinamica che contiene tutti i giocatori
 * @param nGiocatori Numero di giocatori per la partita
 * @param mazzoPesca Il mazzo delle carte che vanno pescate
 * @param turno
 */
void creaNuovaPartita(Giocatore **listaGiocatori, int *nGiocatori, Carta **mazzoPesca, int *turno) {
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
void avantiTurno(int turno, Giocatore **listaGiocatori, Carta **mazzoScarti) {
    // Se il giocatore ha più di 5 carte giocabili, dovrà scartare (sacrtare) una carta o più carte
    while (contaCarte((*listaGiocatori)->carteGiocatore) > MAX_CARTE_MAZZO_GIOCATORE) {
        pulisciSchermo();

        guiStampaMazzo((*listaGiocatori)->carteGiocatore, false);
        printf("\n"
            "%s, hai troppe " BBLU "carte in mano" RESET "! Devi scartarne qualcuna!", (*listaGiocatori)->nome);

        // Carta da scartare, può essere di qualsiasi tipo dato che è nel mazzo del giocatore
        Carta *cartaDaScartare = scegliCarta((*listaGiocatori)->carteGiocatore, ALL);

        // Essendo sempre lo stesso mazzo, è più facile usare la funzione qui sotto rispetto ad azioneScartaElimina
        spostaCarta(&(*listaGiocatori)->carteGiocatore, cartaDaScartare, mazzoScarti);
    }
    pulisciSchermo();
    printf(LINEA_BIANCA
           "FINE DEL TURNO %d PER: %s"
           "\n", turno, (*listaGiocatori)->nome);
    premiInvioPerContinuare();

    // Scorre la lista al prossimo giocatore
    *listaGiocatori = (*listaGiocatori)->next;
}

/** Un menù che chiede al giocatore che cosa vuole vedere riguardo la partita corrente
 *
 * @param giocante
 * @param listaGiocatori La lista dei giocatori
 * @param nGiocatori Il numero totale dei giocatori
 */
void mostraStatusPartita(Giocatore *giocante, Giocatore *listaGiocatori, int nGiocatori) {
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
                mostraMazzoGiocabile(giocante, listaGiocatori, nGiocatori);
                break;
            case COMANDO_OPZIONE_2:
                for (int i = 0; i < nGiocatori; i++) {
                    pulisciSchermo();
                    guiStampaMazzo(giocatoriAffetti->carteAulaGiocatore, true);

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
                    guiStampaMazzo(giocatoriAffetti->carteBonusMalusGiocatore, true);

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

/** Mostra la mano di un giocatore
 *
 * @param giocante Il giocatore che effettua l'azione di vedere le carte giocabili
 * @param giocatoriAffetti La lista di tutti i giocatori affetti
 * @param nGiocatoriAffetti Il numero di giocatori presenti nella lista
 */
void mostraMazzoGiocabile(Giocatore *giocante, Giocatore *giocatoriAffetti, int nGiocatoriAffetti) {
    pulisciSchermo();

    // Se il giocante e la lista sono uguali, significa che è il giocante che deve vedere le sue carte
    if (giocante == giocatoriAffetti)
        guiStampaMazzo(giocatoriAffetti->carteGiocatore, true);

        // Altrimenti, si controlla se l'effetto mostra è presente nei malus, ed è SEMPRE attivo
    else if (cercaCarta(giocatoriAffetti->carteBonusMalusGiocatore, MOSTRA, MALUS, SEMPRE) != NULL) {
        // Ciclo che continua per tutti i giocatori affetti
        for (int i = 0; i < nGiocatoriAffetti; i++) {
            // Stampa le carte
            guiStampaMazzo(giocatoriAffetti->carteGiocatore, true);
            printf("\n\t"
                   BOLD "Mazzo di %s" RESET, giocatoriAffetti->nome);

            giocatoriAffetti = giocatoriAffetti->next;
        }
    } else {
        printf("\n"
            BRED "Non puoi vedere" RESET " le " BLU "carte giocabili" RESET " di un altro giocatore, "
            "a meno che non abbiano una carta con " MAG "effetto MOSTRA" RESET "!"
            "\n");
    }
    premiInvioPerContinuare();
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
bool giocaCarta(Giocatore *listaGiocatori, int nGiocatori,
                Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio) {
    // Il totale delle carte in mano
    int nCarte = contaCarte(listaGiocatori->carteGiocatore);
    // Un booleano che serve per sapere se ha giocato o meno la carta, se non ha giocato riporta al menù precedente
    bool haGiocatoCarta = false;

    // Pulisce lo schermo e stampa il mazzo
    pulisciSchermo();
    guiStampaMazzo(listaGiocatori->carteGiocatore, false);

    // Se il mazzo contiene carte allora può giocarle
    if (nCarte > MIN_0) {
        printf("\n" LINEA_BIANCA
               "Scegli una " BLU "carta" RESET " da giocare [%d-%d] (%d torna indietro):", MIN_1, nCarte, MIN_0);

        // L'utente inserisce il numero di una carta
        int scelta = inserisciNumero(MIN_0, nCarte);

        // Se la carta scelta è diversa da ESCI
        if (scelta != MIN_0) {
            // Puntatore temporaneo alle carte giocabili
            Carta *cartaGiocata = listaGiocatori->carteGiocatore;

            // Parte da uno nel contare la carta selezionata, e scorre fino a quella
            for (int i = MIN_1; i < scelta; i++)
                cartaGiocata = cartaGiocata->next;

            printf("\n"
                BHMAG "HAI SCELTO LA CARTA:" RESET);
            guiStampaCarta(cartaGiocata, false);
            premiInvioPerContinuare();

            // In caso la carta è MALUS, e il momento è SUBITO, va spostata nel mazzo di un altro giocatore
            if (cartaGiocata->tipo == MALUS) {
                Giocatore *giocatoreScelto = scegliGiocatore(listaGiocatori, nGiocatori);
                bool contrastato = effettiContrastanti(listaGiocatori, giocatoreScelto, cartaGiocata->tipo);

                if (!contrastato)
                    spostaCarta(&listaGiocatori->carteGiocatore, cartaGiocata,
                                &giocatoreScelto->carteBonusMalusGiocatore);
            } else {
                Carta *mazzoTmp = NULL;
                spostaCarta(&listaGiocatori->carteGiocatore, cartaGiocata, &mazzoTmp);

                gestisciEffettiCarta(listaGiocatori, nGiocatori, cartaGiocata, mazzoPesca,
                                     mazzoScarti, mazzoAulaStudio, SUBITO);

                Carta **mazzoOutput = mazzoGiocatoreGiusto(listaGiocatori, cartaGiocata, mazzoScarti);
                spostaCarta(&mazzoTmp, cartaGiocata, mazzoOutput);
            }
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

/** Controllo della vittoria
 *
 * @param giocatore Il giocatore da controllare
 * @return Ritorna true se il giocatore ha vinto, altrimenti false
 */
bool areYaWinningSon(Giocatore *giocatore) {
    Carta *c = cercaCarta(giocatore->carteBonusMalusGiocatore, INGEGNERE, MALUS, SEMPRE);
    int nCarte = contaCarte(giocatore->carteAulaGiocatore);

    if (nCarte >= CARTE_STUDENTE_VITTORIA && c != NULL) {
        pulisciSchermo();
        guiVittoria(giocatore->nome);
        premiInvioPerContinuare();
        return true;
    }
    return false;
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
 * @param momento Quando gli effetti possono essere applicati
 */
void gestisciEffettiCarta(Giocatore *listaGiocatori, int nGiocatori, Carta *cartaGiocata,
                          Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio, Quando momento) {
    // Cicla per il numero di effetti
    for (int i = 0; i < cartaGiocata->nEffetti; i++) {
        Effetto *effetto = &cartaGiocata->effetto[i]; // Creato per aumentare la leggibilità
        Giocatore *giocatoriAffetti = listaGiocatori;
        int nGiocatoriAffetti = effettoTargetGiocatori(&giocatoriAffetti, nGiocatori, effetto->targetGiocatori);

        // Se gli effetti sono giocabili, esegue le azioni di ogni effetto
        if (isGiocabile(listaGiocatori, giocatoriAffetti, cartaGiocata, momento))
            for (int j = 0; j < nGiocatoriAffetti; j++) {
                azioneCarta(listaGiocatori, nGiocatori, giocatoriAffetti, cartaGiocata,
                           effetto, mazzoPesca, mazzoScarti, mazzoAulaStudio);
                giocatoriAffetti = giocatoriAffetti->next;
            }
    }
}

/** Richiama la funzione degli effetti della carta in base al Quando come parametro
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero dei giocatori
 * @param quando Quando l'effetto deve essere giocato
 * @param mazzoPesca Il mazzo da dove vengono pescate le carte
 * @param mazzoScarti Il mazzo degli scarti
 * @param mazzoAulaStudio Il mazzo dove vanno le matricole dopo che vengono scartate
 */
void effettiMazzo(Giocatore *listaGiocatori, int nGiocatori,
                  Quando quando, Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio) {
    Carta *tmp = listaGiocatori->carteAulaGiocatore;

    while (tmp != NULL) {
        gestisciEffettiCarta(listaGiocatori, nGiocatori, tmp,
                             mazzoPesca, mazzoScarti, mazzoAulaStudio, quando);
        tmp = tmp->next;
    }

    tmp = listaGiocatori->carteBonusMalusGiocatore;

    while (tmp != NULL) {
        gestisciEffettiCarta(listaGiocatori, P1, tmp,
                             mazzoPesca, mazzoScarti, mazzoAulaStudio, quando);
        tmp = tmp->next;
    }
}

/* Azioni */

/** Gestisce gli effetti della carta
 *
 * Lista dei giocatori
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero che contiene quanti giocatori ci sono nella partita
 * @param cartaGiocata La carta giocata dal giocatore
 * @param effetto L'effetto della carta giocata
 * @param giocatoreAffetto La lista dei giocatori affetti dalla carta giocata
 * @param mazzoPesca Il mazzo da dove vengono pescate le carte
 * @param mazzoScarti Il mazzo dove vengono scartate, o per meglio dire, "sacrtate", le carte
 * @param mazzoAulaStudio Il mazzo dove vanno le carte matricola
 */
void azioneCarta(Giocatore *listaGiocatori, int nGiocatori, Giocatore *giocatoreAffetto,
                 Carta *cartaGiocata, Effetto *effetto,
                 Carta **mazzoPesca, Carta **mazzoScarti, Carta **mazzoAulaStudio) {
    // Puntatore a tipo Carta che verrà usata dentro lo switch case (principalmente per i mazzi)
    Carta *tmp;
    bool contrastato = effettiContrastanti(listaGiocatori, giocatoreAffetto, cartaGiocata->tipo);

    // Switch che gestisce tutte le azioni che ha l'effetto della carta
    switch (effetto->azione) {
        case GIOCA:
            giocaCarta(listaGiocatori, nGiocatori, mazzoPesca, mazzoScarti, mazzoAulaStudio);
            break;
        // Le seguenti 4 azioni sono sostanzialmente le stesse, ma per comodità le divido in due blocchi
        case SCARTA: // Sacrtare le carte (per pochi)
        case ELIMINA:
            if (!contrastato)
                azioneScartaElimina(giocatoreAffetto, cartaGiocata, effetto, mazzoScarti, mazzoAulaStudio);
            break;
        case PRENDI:
        case RUBA:
            if (!contrastato)
                azioneRubaPrendi(listaGiocatori, giocatoreAffetto, effetto);
            break;
        case PESCA:
            pescaCarta(&giocatoreAffetto->carteGiocatore, mazzoPesca, mazzoScarti);
            break;
        case SCAMBIA:
            if (!contrastato) {
                tmp = giocatoreAffetto->carteGiocatore;
                giocatoreAffetto->carteGiocatore = listaGiocatori->carteGiocatore;
                listaGiocatori->carteGiocatore = tmp;
            }
            break;
        // Qua sotto sono effetti che non possono essere trovati nelle carte giocabili, perciò non hanno effetto
        default:
            printf("\n"
                BYEL "Hai appena buttato una carta... Gli effetti non erano applicabili!" RESET
                "\n");
            premiInvioPerContinuare();
        break;
    }
}

/** Gestisce le azioni di scartare una carta
 *
 * @param giocatoreAffetto Il giocatore da cui verrà scartata una carta
 * @param cartaGiocata La carta giocata dal giocatore, serve come filtro per evitare che questa venga eliminata
 * @param effetto L'effetto della carta giocata
 * @param mazzoScarti Il mazzo in cui verrà scartata la carta
 * @param mazzoAulaStudio Qui vanno scartate solo le matricole (se la carta è una matricola)
 */
void azioneScartaElimina(Giocatore *giocatoreAffetto, Carta *cartaGiocata, Effetto *effetto, Carta **mazzoScarti,
                         Carta **mazzoAulaStudio) {
    // Crea un puntatore temporaneo al mazzo da cui verrà scartata la carta
    Carta **mazzoInput = NULL;
    if (effetto->azione == SCARTA)
        mazzoInput = &giocatoreAffetto->carteGiocatore;
    // Azione ELIMINA
    else {
        if (isStudente(effetto->tipo))
            mazzoInput = &giocatoreAffetto->carteAulaGiocatore;
        else if (isBonusMalus(effetto->tipo))
            mazzoInput = &giocatoreAffetto->carteBonusMalusGiocatore;
        else mazzoInput = scegliMazzo(&giocatoreAffetto->carteAulaGiocatore,
                                      &giocatoreAffetto->carteBonusMalusGiocatore);
    }

    // Se il mazzo ha almeno una carta, allora può procedere
    int nCarta = contaCarteFiltro(*mazzoInput, cartaGiocata->tipo);

    if (nCarta > 0) {
        // Stampe per il giocatore
        guiStampaMazzo(*mazzoInput, false);
        printf("\n" LINEA_BIANCA
               "Scegli una " BLU "carta" RESET " da " BRED "scartare da " RESET BLU "%s" RESET " [%d-%d]",
               giocatoreAffetto->nome, MIN_1, nCarta);

        // Puntatore alla carta da scartare
        Carta *cartaScelta = scegliCarta(*mazzoInput, effetto->tipo);

        // Puntatore al mazzo dove verrà buttata la carta
        Carta **mazzoOutput = NULL;

        // Se la carta è una matricola, per forza va scartata nell'aula studio, perché non può essere recuperata
        if (cartaScelta->tipo == MATRICOLA) mazzoOutput = mazzoAulaStudio;
        else mazzoOutput = mazzoScarti;

        // Scarta la carta
        spostaCarta(mazzoInput, cartaScelta, mazzoOutput);
    } else {
        // Se non c'è nessuna carta, stampa un messaggio di avviso ma va avanti comunque, affari di chi butta la carta
        printf("\n"
               YEL "Non è possibile scartare/prendere nessuna carta da " RESET BLU "%s." RESET
               "\n", giocatoreAffetto->nome);
        premiInvioPerContinuare();
    }
}

/** Gestisce l'azione di ruba o prende la carta
 *
 * @param giocante Il giocatore che gioca la carta
 * @param giocatoreAffetto Il giocatore a cui verrà rubata la carta
 * @param effetto L'effetto della carta giocata
 */
void azioneRubaPrendi(Giocatore *giocante, Giocatore *giocatoreAffetto, Effetto *effetto) {
    // Puntatore temporaneo a un mazzo del giocatore affetto in base all'effetto della carta giocata
    Carta **mazzoInput = NULL;

    if (effetto->azione == PRENDI)
        mazzoInput = &giocatoreAffetto->carteGiocatore;
    // Azione RUBA
    else {
        if (isStudente(effetto->tipo))
            mazzoInput = &giocatoreAffetto->carteAulaGiocatore;
        else if (isBonusMalus(effetto->tipo))
            mazzoInput = &giocatoreAffetto->carteBonusMalusGiocatore;
        else
            mazzoInput = scegliMazzo(&giocatoreAffetto->carteAulaGiocatore,
                                     &giocatoreAffetto->carteBonusMalusGiocatore);
    }

    int nCarte = contaCarte(*mazzoInput);
    if (nCarte > 0) {
        // Stampe
        guiStampaMazzo(*mazzoInput, false);
        printf("\n"
               "Scegli una " BLU "carta" RESET " da " YEL "rubare da " BLU "%s " RESET "[%d, %d]",
               giocatoreAffetto->nome, MIN_1, nCarte);

        // La carta scelta dal giocante, che verrà rubata
        Carta *cartaScelta = scegliCarta(*mazzoInput,
                                         effetto->tipo);

        // Il mazzo delle carte dove verrà posta la carta rubata
        Carta **mazzoOutput = NULL;
        if (effetto->azione == PRENDI)
            mazzoOutput = &giocante->carteGiocatore;
        else if (effetto->azione == RUBA) {
            if (isStudente(effetto->tipo))
                mazzoOutput = &giocante->carteAulaGiocatore;
            else if (isBonusMalus(effetto->tipo))
                mazzoOutput = &giocante->carteBonusMalusGiocatore;
        }

        // Sposta la carta da un mazzo all'altro (quindi la "ruba")
        spostaCarta(mazzoInput, cartaScelta, mazzoOutput);
    } else {
        printf("\n"
               YEL "Non è possibile rubare/scartare nessuna carta da %s." RESET
               "\n", giocatoreAffetto->nome);
        premiInvioPerContinuare();
    }
}

/** Funzione che fa pescare una carta dal mazzo della pesca, altrimenti usa quello degli scarti
 *
 */
void pescaCarta(Carta **mazzoGiocatore, Carta **mazzoPesca, Carta **mazzoScarti) {
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
        "Hai pescato:" "\n");
    guiStampaCarta(*mazzoPesca, false);

    spostaCarta(mazzoPesca, *mazzoPesca, mazzoGiocatore);

    premiInvioPerContinuare();
}
