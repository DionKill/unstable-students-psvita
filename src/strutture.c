//
// Created by diego on 28/12/24.
//

#include "strutture.h"

#include "grafica.h"

/*--- Gestione dei giocatori ---*/

/** Inserisce la testa come prossimo nodo in coda, per poter ciclare all'infinito, creando una lista circolare
 *
 * @param listaGiocatori La lista dei giocatori
 * @return La lista dei giocatori ma che ha, come ultimo nodo, il primo della lista
 */
Giocatore *rendiListaGiocatoriCircolare (Giocatore *listaGiocatori) {
    Giocatore *tmp = listaGiocatori; // Variabile temporanea
    while (listaGiocatori->next != NULL)
        listaGiocatori = listaGiocatori->next;
    listaGiocatori->next = tmp;
    listaGiocatori = listaGiocatori->next;
    return listaGiocatori;
}

/** Una funzione migliorata per l'allocazione dei giocatori in memoria
 * Rispetto alla soluzione con un singolo puntatore, questa può scorrere in avanti senza dover tenere un puntatore
 * all'elemento precedente della lista, quindi è anche più semplice da implementare (ma comunque difficile da capire)
 *
 * @param giocatore Il giocatore da allocare
 */
void allocaGiocatoriBene (Giocatore **giocatore) {
    // Crea un doppio puntatore temporaneo al giocatore.
    Giocatore **tmp = giocatore;

    // Finché tmp esiste, va alla fine della lista
    while (*tmp != NULL)
        *tmp = (*tmp)->next;

    // Alloca lo spazio e nel peggiore dei casi esce con un errore
    *tmp = (Giocatore *) malloc(sizeof(Giocatore));
    if (*tmp == NULL) exit(EXIT_FAILURE);
}

/** Funzione che crea la lista di giocatori
 *
 * @return Ritorna la nuova lista di giocatori
 */
int creaGiocatori(Giocatore **listaGiocatori) {
    int nGiocatori;

    Giocatore **tmp = listaGiocatori;

    do {
        printf("\n"
            "Quanti giocatori giocheranno? [2-4]:"
            CURSORE_INPUT);

        scanf("%d", &nGiocatori);
        flushInputBuffer();

        if (nGiocatori < 2 || nGiocatori > 4 )
            printf("\n"
                "Il valore inserito non e' valido!");
    } while (nGiocatori < 2 || nGiocatori > 4);

    // Inizio richiesta di aiuto da parte degli americani
    char *str; // Stringa che contiene il colore del giocatore
    strColoreGiocatore(&str, nGiocatori);

    int i = 1;
    // Chiede all'utente il nome finché non è valido
    do {
        printf ("\n"
            "Inserisci il nome del giocatore %s%d:"
            RESET
            CURSORE_INPUT
            "%s", str, i, str); // Colora il nome del giocatore

        scanf(" %" NOME_LENGTH_STR "[^\n]s", (*tmp)->nome);

        printf(RESET);
        if (strlen((*tmp)->nome) == 0)
            printf("\n"
                   "Inserire un valore corretto!");
        else {
            tmp = &(*tmp)->next; // Scorre nella lista
            i++;
        }

        flushInputBuffer();
    } while (strlen((*tmp)->nome) == 0 && i < nGiocatori);

    *listaGiocatori = rendiListaGiocatoriCircolare(*listaGiocatori); // Rende la lista dei giocatori circolare

    return nGiocatori;
}

/*--- Gestione delle carte ---*/

/** Alloca spazio in memoria per una singola carta, altrimenti esce
 * @return Ritorna la carta allocata
 */
Carta *allocaCarta () {
    Carta *carta = NULL;
    carta = (Carta *) malloc(sizeof(Carta));

    if (carta == NULL) exit(EXIT_FAILURE);

    carta->next = NULL;

    return carta;
}

/** Cicla il mazzo e conta quante carte ci sono
 *
 * @param mazzo Il mazzo di cui bisogna contare le carte
 * @return Ritorna il numero di carte nel mazzo
 */
int contaCarte (Carta *mazzo) {
    int i = 0;
    while (mazzo != NULL) {
        mazzo = mazzo->next;
        i++;
    }
    return i;
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

    // Copia i dati un parametro alla volta
    strcpy(tmp->nome, carta->nome);
    strcpy(tmp->descrizione, carta->descrizione);
    tmp->tipo = carta->tipo;
    tmp->nEffetti = carta->nEffetti;

    // Copia i parametri degli effetti
    if (tmp->nEffetti > 0) {
        tmp->effetto = (Effetto *) malloc(tmp->nEffetti * sizeof(Effetto)); // Alloca un array dinamico

        if (tmp->effetto == NULL) exit(EXIT_FAILURE);

        // For che legge gli effetti e li mette nell'array dinamico
        for (int i = 0; i < tmp->nEffetti; i++) {
            tmp->effetto[i].azione = carta->effetto[i].azione;
            tmp->effetto[i].tipo = carta->effetto[i].tipo;
            tmp->effetto[i].targetGiocatori = carta->effetto[i].targetGiocatori;
        }
    }

    tmp->quandoEffetto = carta->quandoEffetto;
    tmp->puoEssereGiocato = carta->puoEssereGiocato; // Disessere giocati

    tmp->next = copiaCarta(tmp, nCopie - 1);

    return tmp;
}

/** Divide il mazzo in un mazzo normale e uno matricola.
 * Si basa sul fatto che le carte matricola sono sempre all'inizio della lista, dopo averle lette dal file.
 *
 * @param mazzo Il mazzo da cui vengono estratte le carte matricola. Viene anch'esso modificato.
 * @return Ritorna un nuovo mazzo contenente solo matricole.
 */
Carta *dividiMazzoMatricola (Carta **mazzo) {
    if (*mazzo == NULL) return NULL;

    Carta *mazzoMatricole = NULL;

    // Finché ci sono matricole scorre avanti e le sposta in un nuovo mazzo
    while ((*mazzo)->tipo == MATRICOLA)
        spostaCarta(mazzo, *mazzo, &mazzoMatricole);

    return mazzoMatricole;
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

/** Dati i due mazzi, sposta una carta da un mazzo e la mette in testa all'altro.
 *
 * @param mazzoInput Il mazzo da cui dovrà essere spostata la carta.
 * @param cartaInput La carta da spostare (può anche essere il mazzoInput).
 * @param mazzoOutput Il mazzo in cui testa verrà posta la nuova carta.
 */
void spostaCarta (Carta **mazzoInput, Carta *cartaInput, Carta **mazzoOutput) {
    // Se una di queste due carte è nulla, non c'è niente da spostare, perciò esce
    if (*mazzoInput == NULL || cartaInput == NULL)
        return;

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

/** Funzione che sposta la carta desiderata nel corretto mazzo del giocatore,
 * ad esempio le carte bonus nel mazzo bonusmalus... etc...
 * @param giocatore Il giocatore a cui va messa la carta nel mazzo giusto
 * @param carta La carta da spostare
 */
void spostaCartaNelMazzoGiocatoreGiusto (Giocatore *giocatore, Carta **carta) {
    if ((*carta)->tipo == MATRICOLA || (*carta)->tipo == STUDENTE_SEMPLICE || (*carta)->tipo == LAUREANDO)
        spostaCarta(carta, *carta, &giocatore->carteAulaGiocatore);

    else if ((*carta)->tipo == BONUS || (*carta)->tipo == MALUS)
        spostaCarta(carta, *carta, &giocatore->carteBonusMalusGiocatore);

    else spostaCarta(carta, *carta, &giocatore->carteGiocatore);
}

/** Distribuisce le carte ai giocatori, prendendole dal mazzo già mescolato da pesca.
 * TODO: serve a qualcosa?????
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

/** Libera la memoria usata dal programma prima dell'uscita
 * Spoiler: le dipendenze circolari mi costringono a mettere la funzione qui, perciò così sarà.
 * TODO: completare la funzione
 *
 * @param testaMazzo Il mazzo da liberare
 */
void liberaMemoria (Carta *testaMazzo) {
    while (testaMazzo != NULL) {
        Carta *tmp = testaMazzo;
        testaMazzo = testaMazzo->next;

        free(tmp->effetto);
        free(tmp);
    }
}
