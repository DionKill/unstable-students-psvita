//
// Created by diego on 30/12/24.
//

#include "gioco.h"

/** La funzione principale del gioco. Da qui viene gestito tutto.
 *
 */
void gioco () {
    premiInvioPerContinuare();
    pulisciSchermo();

    // Crea i giocatori e li popola
    Giocatore *listaGiocatori = NULL;
    int nGiocatori = creaGiocatori(&listaGiocatori);

    // Crea il mazzo di carte, e lo popola usando il file mazzo.txt
    Carta *mazzoPesca = NULL;
    leggiCarteDaFile(&mazzoPesca);

    // Crea il mazzo delle carte scartate (sacrtate) e dell'aula studio
    Carta *mazzoScarti = NULL; // Sacrate (per pochi)
    Carta *mazzoAulaStudio = NULL;

    // Divide il mazzo da pesca originale, creando il mazzo delle matricole
    Carta *mazzoMatricole = NULL;
    mazzoMatricole = dividiMazzoMatricola(&mazzoPesca); // Crea il mazzo matricola partendo dal mazzo da pesca

    // Mischia i due mazzi appena creati
    shuffleCarte(&mazzoPesca);
    shuffleCarte(&mazzoMatricole);

    // Distribuisce le carte a ogni giocatore, dal mazzo delle matricole e quello di pesca
    distribuisciCarte(nGiocatori, listaGiocatori, &mazzoMatricole);
    distribuisciCarte(N_CARTE_PER_GIOCATORE * nGiocatori, listaGiocatori, &mazzoPesca);

    // Gestione dei turni
    int turno = 0;

    // TODO: Funzione che controlla quando vinci così la metto nella condizione del main
    while (turno < 5) {
        guiHeader(turno, listaGiocatori->nome);

        int scelta = scegliAzione();

        switch (scelta) {
            case COMANDO_GIOCA_CARTA:
                // TODO: gioca una carta
                turno++;
            break;
            case COMANDO_PESCA_CARTA:
                spostaCartaNelMazzoGiocatoreGiusto(listaGiocatori, &mazzoPesca);
                guiStampaCarta(mazzoPesca);
                turno++;
            break;
            case COMANDO_MOSTRA_CARTE:
                // TODO: scegli quali di queste carte mostrare (o meglio ancora tutte e tre assieme affiancate)
                guiStampaMazzo(listaGiocatori->carteGiocatore);
                guiStampaMazzo(listaGiocatori->carteAulaGiocatore);
                guiStampaMazzo(listaGiocatori->carteBonusMalusGiocatore);
                turno++;
            break;
            case COMANDO_ESCI:
                // TODO: tutta la roba del salvataggio lol
                return;
            default: break;
        }
        premiInvioPerContinuare();
    }
}

/** Funzione ricorsiva per allocare la lista di Giocatori in memoria.
 *
 * @param listaGiocatori Il giocatore attuale
 * @param nGiocatori Il quantitativo di giocatori
 * @return Ritorna la lista
 */
Giocatore *allocaGiocatori (Giocatore *listaGiocatori, int nGiocatori) {
    if (nGiocatori == 0)
        return listaGiocatori;

    // Crea un nuovo giocatore e lo alloca
    listaGiocatori = (Giocatore *) malloc(sizeof(Giocatore));
    if (listaGiocatori == NULL) exit(EXIT_FAILURE); // Esce se non puo' allocare memoria

    // Continua il codice, mettendo subito il prossimo nodo a NULL
    listaGiocatori->next = NULL;

    // Chiede all'utente il nome finché non è valido, il numero viene calcolato (+4 byte di memoria risparmiati)
    do {
        printf ("\n"
                "Inserisci il nome del giocatore %d: ", (nGiocatori - 5) * -1); // Spoiler non funziona bene
        scanf(" %" NOME_LENGTH_STR "[^\n]s", listaGiocatori->nome);
        flushInputBuffer();
    } while (strlen(listaGiocatori->nome) < 0);

    // Si richiama da solo finché non ha finito di aggiungere i giocatori
    listaGiocatori->next = allocaGiocatori(listaGiocatori->next, nGiocatori - 1);

    // Ritorna la lista
    return listaGiocatori;
}

/** Funzione che crea la lista di giocatori
 *
 * @return Ritorna la nuova lista di giocatori
 */
int creaGiocatori(Giocatore **listaGiocatori) {
    int nGiocatori;

    printf("Quanti giocatori giocheranno?");
    do {
        printf("\n"
        "[2-4]: ");
        scanf("%d", &nGiocatori);
        if (nGiocatori < 2 || nGiocatori > 4 )
            printf("\n"
                "Il valore inserito non e' valido!");
    flushInputBuffer();
    } while (nGiocatori < 2 || nGiocatori > 4);

    // Alloca lo spazio in memoria e li aggiunge in una lista di tipo Giocatore
    *listaGiocatori = allocaGiocatori(*listaGiocatori, nGiocatori);

    *listaGiocatori = rendiListaGiocatoriCircolare(*listaGiocatori); // Rende la lista dei giocatori circolare

    return nGiocatori;
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
        switch ((*mazzoPesca)->tipo) {
            case MATRICOLA:
            case STUDENTE_SEMPLICE:
            case LAUREANDO:
                (*mazzoPesca)->next = listaGiocatori->carteAulaGiocatore;
                listaGiocatori->carteAulaGiocatore = *mazzoPesca;
            break;

            case BONUS:
            case MALUS:
                (*mazzoPesca)->next = listaGiocatori->carteBonusMalusGiocatore;
                listaGiocatori->carteBonusMalusGiocatore = *mazzoPesca;
            break;

            default:
                (*mazzoPesca)->next = listaGiocatori->carteGiocatore;
                listaGiocatori->carteGiocatore = *mazzoPesca;
            break;
        }

        // Scorre avanti le due liste
        *mazzoPesca = tmpMazzoPesca;
        listaGiocatori = listaGiocatori->next;
    }
}

/** Una funzione che gioca una carta a scelta del giocatore
 *
 */
void giocaCarta () {

}

/** Funzione ineccepibile che fornisce una scelta di cose che può fare il giocatore e ritorna l'opzione scelta.
 *
 * @return La scelta del giocatore.
 */
int scegliAzione () {
    // Stampa il menù di scelta
    guiScegliAzione();

    int scelta;
    scanf("%d", &scelta);
    flushInputBuffer();

    return scelta;
}

void premiInvioPerContinuare () {
    printf("\n"
           "Premi INVIO per continuare"
           "\n");
    flushInputBuffer();
}

// TODO: scegli che mazzo vedere

/** Funzione che libera l'input buffer.
 * Con questa non vengono aggiunti gli invii e caratteri speciali durante l'inserimento di testo.
 */
void flushInputBuffer () {
    while (getchar() != '\n');
}
