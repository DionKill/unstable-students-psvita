//
// Created by diego on 28/12/24.
//

#include "strutture.h"
#include "grafica.h"

/*--- Gestione dei giocatori ---*/

/** Funzione che crea la lista di giocatori
 *
 * @return Ritorna la nuova lista di giocatori
 */
int creaGiocatori(Giocatore **listaGiocatori) {
    // Chiede quanti giocatori vuole che vengano creati
    printf("\n"
            "Quanti " BBLU "giocatori" RESET " giocheranno? [%d-%d]:", GIOCATORI_MIN, GIOCATORI_MAX);
    int nGiocatori = inserisciNumero(GIOCATORI_MIN, GIOCATORI_MAX);

    // Un doppio puntatore alla lista che verrà modificata
    Giocatore **tmp = listaGiocatori;

    int i = 0;
    // Chiede all'utente il nome finché non è valido
    while (i < nGiocatori) {

        *tmp = allocaGiocatore();
        printf ("\n"
                "Inserisci il nome del giocatore %s%d" RESET ":"
                CURSORE_INPUT
                "%s", strColoreGiocatore(i + 1), i + 1, strColoreGiocatore(i + 1)); // Colora il nome del giocatore

        scanf(" %" NOME_LENGTH_STR "[^\n]s", (*tmp)->nome);

        printf(RESET);
        if (strlen((*tmp)->nome) == 0)
            printf(RESET "\n"
                   "Inserire un valore corretto!");
        else {
            tmp = &(*tmp)->next; // Scorre nella lista
            i++;
        }

        flushInputBuffer();
    }

    rendiListaGiocatoriCircolare(*listaGiocatori); // Rende la lista dei giocatori circolare

    return nGiocatori;
}

/** Alloca lo spazio in memoria per un giocatore
 *
 * @return Ritorna un nuovo giocatore allocato
 */
Giocatore *allocaGiocatore () {
    // Alloca il giocatore, altrimenti esce, e imposta il prossimo nodo a NULL per evitare errori
    Giocatore *giocatore = NULL;
    giocatore = (Giocatore *) malloc(sizeof(Giocatore));

    if (giocatore == NULL) exit(EXIT_FAILURE);

    giocatore->next = NULL;

    return giocatore;
}

/** Inserisce la testa come prossimo nodo in coda, per poter ciclare all'infinito, creando una lista circolare
 *
 * @param listaGiocatori La lista dei giocatori
 * @return La lista dei giocatori ma che ha, come ultimo nodo, il primo della lista
 */
void rendiListaGiocatoriCircolare(Giocatore *listaGiocatori) {
    Giocatore *tmp = listaGiocatori; // Variabile temporanea
    while (listaGiocatori->next != NULL)
        listaGiocatori = listaGiocatori->next;
    listaGiocatori->next = tmp;
}

/*--- Gestione delle carte ---*/

/** Alloca spazio in memoria per una singola carta, altrimenti esce
 *
 * @return Ritorna la carta allocata
 */
Carta *allocaCarta () {
    Carta *carta = NULL;
    carta = (Carta *) malloc(sizeof(Carta));

    if (carta == NULL) exit(EXIT_FAILURE);

    carta->next = NULL;

    return carta;
}

/** Funzione ricorsiva che prende in ingresso un nodo Carta, alloca la successiva, e copia i contenuti nella successiva.
 *
 * @param carta Il nodo carta da copiare
 * @param nCopie Quante volte deve essere ancora copiato
 * @return Ritorna la carta
 */
Carta *copiaCarta (Carta *carta, int nCopie) {
    if (nCopie == 0)
        return NULL;

    // Crea un nodo temporaneo alias del prossimo nodo (il primo di nCopie dove bisogna copiare)
    Carta *tmp = allocaCarta();

    // Copia per dereferenza
    *tmp = *carta;

    // Copia ogni effetto per dereferenza, allocandolo prima
    if (tmp->nEffetti > 0) {
        // Alloca gli effetti
        tmp->effetto = (Effetto *) malloc(tmp->nEffetti * sizeof(Effetto)); // Alloca un array dinamico
        if (tmp->effetto == NULL) exit(EXIT_FAILURE);

        // Per ogni effetto, copia per dereferenza
        for (int i = 0; i < tmp->nEffetti; i++)
            tmp->effetto[i] = carta->effetto[i];
    }

    tmp->next = copiaCarta(tmp, nCopie - 1);

    return tmp;
}

/** Divide il mazzo in un mazzo normale e uno matricola.
 * Si basa sul fatto che le carte matricola sono sempre all'inizio della lista, dopo averle lette dal file.
 *
 * @param mazzo Il mazzo da cui vengono estratte le carte matricola. Viene anch'esso modificato.
 * @return Ritorna un nuovo mazzo contenente solo matricole.
 */
Carta *dividiMazzoMatricole (Carta **mazzo) {
    Carta *mazzoMatricole = NULL;
    Carta **tmp = mazzo;

    // Finché ci sono matricole scorre avanti e le sposta in un nuovo mazzo
    while (*tmp != NULL) {
        if ((*tmp)->tipo == MATRICOLA)
            spostaCarta(mazzo, *tmp, &mazzoMatricole);
        else tmp = &(*tmp)->next;
    }

    return mazzoMatricole;
}

/** Cerca una carta in base al "filtro" dato in parametro
 * Non viene cercato il target.
 *
 * @param mazzo Il mazzo in cui verrà cercata la carta
 * @param azione L'azione da cercare
 * @param tipo Il tipo della carta
 * @param quando Il quando della carta da cercare
 * @return Ritorna la carta se trovata, altrimenti NULL
 */
Carta *cercaCarta (Carta *mazzo, Azione azione, TipologiaCarta tipo, Quando quando) {
    // Se il mazzo è vuoto, esce subito
    if (mazzo == NULL) return mazzo;

    // Puntatore temporaneo al mazzo (non necessario, ma usato per leggibilità)
    Carta *cartaDaCercare = mazzo;

    // Controlla tutto il mazzo e se trova una carta con quelle specifiche la ritorna
    while (cartaDaCercare != NULL) {
        if (cartaDaCercare->quandoEffetto == quando)
            for (int i = 0; i < cartaDaCercare->nEffetti; i++)
                if (cartaDaCercare->effetto[i].azione == azione && cartaDaCercare->tipo == tipo)
                    return cartaDaCercare;
        cartaDaCercare = cartaDaCercare->next;
    }

    // Ritorna NULL se non la trova
    return cartaDaCercare;
}

/** Funzione che sposta la carta appena giocata nell'apposito mazzo
 *
 * @param giocatore Il giocatore a cui va messa la carta nel mazzo giusto
 * @param carta La carta da spostare
 * @param mazzoScarti
 */
Carta **mazzoGiocatoreGiusto(Giocatore *giocatore, Carta *carta, Carta **mazzoScarti) {
    // Se la carta è STUDENTE allora la sposta nel mazzo Aula del giocatore
    if (isStudente(carta->tipo)) // Niente matricole, già fatto a inizio gioco
        return &giocatore->carteAulaGiocatore;

    // Altrimenti, se è una carta BONUS-MALUS la sposta nel mazzo delle bonus-malus
    if (isBonusMalus(carta->tipo))
        return &giocatore->carteBonusMalusGiocatore;

    // Altrimenti, per esclusione, è una carta giocabile, e niente finisce negli scarti
    return mazzoScarti;
}

/** Cicla il mazzo e conta quante carte ci sono
 *
 * @param mazzo Il mazzo di cui bisogna contare le carte
 * @return Ritorna il numero di carte nel mazzo
 */
int contaCarte (Carta *mazzo) {
    int i = 0; // Contatore

    // Scorre finché il mazzo non è NULL e aumenta di uno il contatore
    while (mazzo != NULL) {
        i++;
        mazzo = mazzo->next;
    }
    return i; // Restituisce il valore
}

/** Conta le carte di un certo tipo in un mazzo
 *
 * @param mazzo Il mazzo dove vanno cercate le carte
 * @param filtro La tipologia della carta da cercare
 * @return Ritorna il numero delle carte di quel tipo trovate nel mazzo
 */
int contaCarteFiltro (Carta *mazzo, TipologiaCarta filtro) {
    int i = 0; // Contatore

    // Scorre finché il mazzo non è NULL e aumenta di uno il contatore
    while (mazzo != NULL) {
        if (effettoTipoCarta(mazzo->tipo, filtro))
            i++;
        mazzo = mazzo->next;
    }
    return i; // Restituisce il valore

}

/** Dati i due mazzi, sposta una carta da un mazzo e la mette in testa all'altro.
 *
 * @param mazzoInput Il mazzo da cui dovrà essere spostata la carta.
 * @param cartaInput La carta da spostare (può anche essere il mazzoInput).
 * @param mazzoOutput Il mazzo in cui testa verrà posta la nuova carta.
 */
void spostaCarta (Carta **mazzoInput, Carta *cartaInput, Carta **mazzoOutput) {
    // Se una di queste due carte è nulla, non c'è niente da spostare, perciò esce
    if (*mazzoInput != NULL && cartaInput != NULL) {
        // Una carta che scorre fino a quella precedente di quella che serve spostare
        Carta *scorriLista = *mazzoInput;

        // Se il mazzo di input e la carta da spostare sono diversi, allora scorre fino quella precedente che va spostata
        if (cartaInput != *mazzoInput) {
            while (scorriLista->next != cartaInput)
                scorriLista = scorriLista->next;

            // Unisce la carta prima e dopo quella che va spostata (sostanzialmente la rimuove dal mazzoInput)
            scorriLista->next = scorriLista->next->next;

            // Crea una carta temporanea al mazzo di output corrente
            Carta *tmp = *mazzoOutput;
            *mazzoOutput = cartaInput;
            cartaInput->next = tmp;
        }

        // Entra qui solo se mazzoInput e cartaInput sono identiche
        else {
            // Usa scorriLista come temporaneo per contenere il valore attuale del mazzoInput
            // Il mazzoInput scorre in avanti di uno, dato che la sua carta è contenuta in scorriLista
            *mazzoInput = (*mazzoInput)->next;

            // Il prossimo elemento di scorriLista viene impostato al mazzoOutput
            scorriLista->next = *mazzoOutput;

            /* Il mazzoOutput viene impostato a scorriLista, che contiene la carta che è stata appena aggiunta seguita
             * dalle precedenti contenute nel mazzoOutput */
            *mazzoOutput = scorriLista;
        }
    }
}

/** Funzione che sposta tutti gli elementi della lista originale a una nuova lista, in modo casuale.
 *
 * L'algoritmo è fin troppo complesso, articolato, intricato, macchinoso, contorto, tortuoso,
 * c'erano altri metodi, è lento, ma lo volevo fare così senza passare per un array, quindi...
 *
 * @param mazzoOriginale Il mazzo che andrà modificato
 */
void shuffleCarte (Carta **mazzoOriginale) {
    Carta *mazzoRandomizzato = NULL;

    //Se il mazzo originale è nullo, esce
    if (*mazzoOriginale == NULL) return;

    Carta *pre = NULL; // La carta precedente a quella che dobbiamo cambiare, così è possibile rimuoverla dalla lista
    int lunghezza = contaCarte(*mazzoOriginale); // La lunghezza della lista

    // Finché nel *mazzoOriginale ci sono ancora carte, continua
    while (*mazzoOriginale != NULL) {
        int rnd = rand() % lunghezza;
        Carta *tmp = *mazzoOriginale; // Carta temporanea che scorrerà fino a quella da spostare al nuovo mazzo
        pre = NULL;

        // Scorre fino all'elemento da spostare
        for (int i = 0; i < rnd; i++) {
            pre = tmp;
            tmp = tmp->next;
        }

        // Rimuove l'elemento dal *mazzoOriginale, collegando il nodo precedente a quello successivo di tmp
        if (pre != NULL) {
            pre->next = tmp->next;
        } else {
            *mazzoOriginale = tmp->next;
        }

        // Aggiunge la carta alla testa del mazzo (così non bisogna scorrere fino alla fine ogni volta)
        tmp->next = mazzoRandomizzato;
        mazzoRandomizzato = tmp;

        lunghezza--;
    }
    *mazzoOriginale = mazzoRandomizzato;
}

/** Distribuisce le carte ai giocatori, prendendole dal mazzo già mescolato da pesca.
 *
 * @param cntCarte Il numero di giocatori
 * @param listaGiocatori La lista di giocatori a cui dare le carte
 * @param mazzoPesca Il mazzo da cui attingere le carte
 */
void distribuisciCarte (int cntCarte, Giocatore *listaGiocatori, Carta **mazzoPesca) {
    // Un ciclo che continua finché ci sono carte da pescare (scorre nella lista circolare dei giocatori)
    for (int i = 0; i < cntCarte; i++) {
        // Copia della testa per non perdere il next del mazzoPesca quando scorrerà (lo vediamo immediatamente)
        Carta *tmpMazzoPesca = (*mazzoPesca)->next;

        // Mette come next della carta in testa al mazzo da pesca, la testa del mazzo del giocatore
        // La nuova carta (con tutte le carte già presenti al next) viene messa come nuova testa del mazzo
        if ((*mazzoPesca)->tipo == MATRICOLA) {
            (*mazzoPesca)->next = listaGiocatori->carteAulaGiocatore;
            listaGiocatori->carteAulaGiocatore = *mazzoPesca;
        } else {
            (*mazzoPesca)->next = listaGiocatori->carteGiocatore;
            listaGiocatori->carteGiocatore = *mazzoPesca;
        }
        // Scorre avanti le due liste
        *mazzoPesca = tmpMazzoPesca;
        listaGiocatori = listaGiocatori->next;
    }
}

/* Gestione degli effetti */

/** Restituisce se la carta è uno studente
 *
 * @param tipo Il tipo della carta giocata
 * @return Ritorna true se è uno studente, false altrimenti
 */
bool isStudente (TipologiaCarta tipo) {
    return tipo == STUDENTE || tipo == MATRICOLA || tipo == STUDENTE_SEMPLICE || tipo == LAUREANDO;
}

/** Restituisce se la carta è una bonus o malus o no
 *
 * @param tipo Il tipo della carta
 * @return Ritorna true se è bonus o malus, altrimenti false
 */
bool isBonusMalus (TipologiaCarta tipo) {
    return tipo == BONUS || tipo == MALUS;
}

/** Restituisce se la carta è giocabile oppure no in base all'opzionale
 *
 * @param giocante
 * @param giocatoriAffetti
 * @param carta La carta di cui controllare se gli effetti sono applicabili
 * @param momento Il momento attuale di cui controllare gli effetti della partita (SUBITO, INIZIO...)
 * @return Se il quando della carta e il momento attuale sono uguali,
 * e la carta è opzionale (o comunque giocabile) e non è contrastata restituisce true
 */
bool isGiocabile (Giocatore *giocante, Giocatore *giocatoriAffetti, Carta *carta, Quando momento) {
    // Create per leggibilità, andare a capo in un return per risparmiare due byte e nessuna riga non ha senso
    bool contrastato = effettiContrastanti(giocante, giocatoriAffetti, carta->tipo);
    bool opzionale = effettiOpzionali(giocante->nome, carta);

    return carta->quandoEffetto == momento && opzionale && !contrastato;
}

/** Controlla se la carta contiene effetti che possono contrastare la carta giocata
 *
 * @param giocante Il giocatore che gioca la carta
 * @param giocatoreAffetto Il giocatore su cui verrà giocata la carta
 * @param tipoCartaGiocata La carta che viene giocata
 * @return True se esistono, altrimenti false
 */
bool effettiContrastanti (Giocatore *giocante, Giocatore *giocatoreAffetto, TipologiaCarta tipoCartaGiocata) {
    // BLOCCA
    if (giocante != giocatoreAffetto) {
        Carta *carta = cercaCarta(giocatoreAffetto->carteGiocatore, BLOCCA, ISTANTANEA, SUBITO);
        bool ris = effettiOpzionali(giocatoreAffetto->nome, carta);
        if (ris) {
            if (carta != NULL) {
                printf("\n" LINEA_BIANCA
                      BHRED "MAI" RESET "!"      "\n"
                      "%s ha " RED "bloccato" RESET " gli " CYN "effetti" RESET " della " BLU "carta" RESET "!",
                      giocatoreAffetto->nome);
                premiInvioPerContinuare();
            }
        }
        return ris;
    }

    // IMPEDIRE e gestione se nessuno dei precedenti
    Carta *carta = cercaCarta(giocante->carteBonusMalusGiocatore, IMPEDIRE, tipoCartaGiocata, SEMPRE);
    return carta != NULL;
}

/** Gestisce se gli effetti sono opzionali.
 * Se non sono opzionali, si attivano in automatico.
 * Se sono opzionali, viene richiesto se si vuole attivarli.
 *
 * @param giocatore
 * @param carta La carta di cui si vogliono controllare gli effetti opzionali
 * @return Ritorna true se gli effetti possono essere giocati (a prescindere da quanti sono), altrimenti false
 */
bool effettiOpzionali (char *giocatore, Carta *carta) {
    if (!carta->opzionale && carta->tipo != ISTANTANEA) return true;

    guiGiocaOpzionale(giocatore, carta->nome);

    return (bool) inserisciNumero(COMANDO_ESCI, COMANDO_OPZIONE_1);
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
            nTarget = P1;
        break;
        case TU:
            nTarget = nGiocatori - 1;
            *listaGiocatori = scegliGiocatore((*listaGiocatori)->next, nTarget);
        break;
        case VOI:
            *listaGiocatori = (*listaGiocatori)->next;
            nTarget = nGiocatori - 1;
        break;
        case TUTTI:
            nTarget = nGiocatori;
        break;
    }
    return nTarget;
}

/** Sceglie un giocatore tra tutti i giocatori (anche se stessi)
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero totale dei giocatori
 */
Giocatore *scegliGiocatore (Giocatore *listaGiocatori, int nGiocatori) {
    printf("\n"
           BOLD "Inserisci il giocatore a cui applicare gli effetti." RESET);
    guiMostraGiocatori(listaGiocatori, nGiocatori);

    int scelta = inserisciNumero(1, nGiocatori);

    // Scorre fino a quel giocatore
    for (int i = 0; i < scelta - 1; i++)
        listaGiocatori = listaGiocatori->next;

    // Ritorna il giocatore
    return listaGiocatori;
}

/** Il giocatore sceglie una carta dal mazzo in base al tipo necessario
 *
 * @param mazzoScelto Il mazzo da cui va scelta la tipologia della carta
 * @param tipoCartaGiocata Il filtro del tipo della carta
 */
Carta *scegliCarta (Carta *mazzoScelto, TipologiaCarta tipoCartaGiocata) {
    int size = contaCarte(mazzoScelto);
    bool tipoValido; // Se i tipi combaciano (o è ALL) è valida
    Carta *cartaScelta;

    // Un while che continua finché il giocatore non sceglie una carta valida
    do {
        cartaScelta = mazzoScelto;
        int scelta = inserisciNumero(MIN_1, size);

        // Scorre fino alla carta
        for (int i = 1; i < scelta; i++)
            cartaScelta = cartaScelta->next;

        tipoValido = effettoTipoCarta(tipoCartaGiocata, cartaScelta->tipo);

        // Controlla se la carta inserita è valida, se si continua il ciclo
    } while (!tipoValido);

    printf("\n"
           BHYEL "HAI SCELTO LA CARTA:" RESET);
    guiStampaCarta(cartaScelta, false);
    premiInvioPerContinuare();

    return cartaScelta;
}

/** Sceglie un mazzo a cui applicare gli effetti
 *
 * @param mazzoAulaStudio Mazzo dell'aula studio
 * @param mazzoBonusMalus Mazzo delle carte bonus e malus
 * @return Ritorna la carta scelta
 */
Carta **scegliMazzo(Carta **mazzoAulaStudio, Carta **mazzoBonusMalus) {
    printf("\n"
           BOLD"SCEGLI IL MAZZO (non puoi cambiarlo)" RESET     "\n"
           "[%d]. Mazzo Aula Studio"                            "\n"
           "[%d]. Mazzo Bonus-Malus",
           COMANDO_OPZIONE_1, COMANDO_OPZIONE_2);

    // Intero che permette di scegliere uno dei due mazzi
    int scelta = inserisciNumero(MIN_1, MAX_MAZZI);

    if (scelta == 1)
        return mazzoAulaStudio;
    return mazzoBonusMalus;
}

/* Miscellanee */

/** Libera la memoria usata dal programma prima dell'uscita.
 * Non influisce sul salvataggio.
 *
 * @param listaGiocatori La lista di tutti i giocatori
 * @param nGiocatori Il numero dei giocatori
 * @param mazzoPesca Il mazzo da dove vengono pescate le carte
 * @param mazzoScarti Il mazzo dove vengono scartate le carte
 * @param mazzoAulaStudio Il mazzo dove vanno le matricole
 */
void liberaMemoria (Giocatore *listaGiocatori, int nGiocatori, Carta *mazzoPesca,
                    Carta *mazzoScarti, Carta *mazzoAulaStudio) {
    // Crea un doppio puntatore temporaneo che andrà a liberare la memoria di ogni giocatore
    for (int i = 0; i < nGiocatori; i++) {
        // Crea un giocatore temporaneo per pulire la memoria
        Giocatore *spazzino = listaGiocatori->next;

        // Libera tutti i mazzi
        liberaMemoriaMazzo(listaGiocatori->carteGiocatore);
        liberaMemoriaMazzo(listaGiocatori->carteAulaGiocatore);
        liberaMemoriaMazzo(listaGiocatori->carteBonusMalusGiocatore);

        // Libera il resto della struttura
        free(listaGiocatori);

        // Va al prossimo giocatore
        listaGiocatori = spazzino;
    }

    // Libera la memoria degli altri mazzi
    liberaMemoriaMazzo(mazzoPesca);
    liberaMemoriaMazzo(mazzoScarti);
    liberaMemoriaMazzo(mazzoAulaStudio);
}

/** Libera memoria per un mazzo
 *
 * @param mazzo Il mazzo da liberare
 */
void liberaMemoriaMazzo (Carta *mazzo) {
    int nCarte = contaCarte(mazzo);

    for (int i = 0; i < nCarte; i++) {
        Carta *tmp = mazzo->next;

        // Dealloca tutti gli effetti
        if (mazzo->effetto != NULL)
            free(mazzo->effetto);

        free(mazzo);
        mazzo = tmp;
    }
}
