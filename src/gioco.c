//
// Created by diego on 30/12/24.
//

#include "gioco.h"

/** La funzione principale del gioco. Da qui viene gestito tutto.
 *
 */
void gioco () {
    // TODO: salvataggio e funzione che popola le cose se è una partita nuova
    // Crea i giocatori e li popola
    Giocatore *giocatore = NULL;
    int nGiocatori = creaGiocatori(&giocatore);

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
    distribuisciCarte(nGiocatori, giocatore, &mazzoMatricole);
    distribuisciCarte(CARTE_DA_DISTRIBUIRE * nGiocatori, giocatore, &mazzoPesca);

    // Gestione dei turni
    int turno = 0;

    // TODO: Funzione che controlla quando vinci così la metto nella condizione del main
    // Questo loop controlla le scelte del giocatore. È abbastanza self-explanatory
    while (turno < 5) {
        guiHeader(turno, giocatore->nome);
        pescaCarta(&giocatore->carteGiocatore, &mazzoPesca);

        int scelta = scegliAzione();

        // Controlla la scelta e richiama le funzioni necessarie
        switch (scelta) {
            case COMANDO_GIOCA_CARTA:
                // TODO: gioca una carta

                //listaGiocatori = listaGiocatori->next; // Scorre la lista
                turno++;
            break;
            case COMANDO_PESCA_CARTA:
                pescaCarta(&giocatore->carteGiocatore, &mazzoPesca);
                //listaGiocatori = listaGiocatori->next; // Scorre la lista
                turno++;
            break;
            case COMANDO_MOSTRA_CARTE:
                // TODO: scegli quali di queste carte mostrare (o meglio ancora tutte e tre assieme affiancate)
                guiStampaMazzo(giocatore->carteGiocatore);
                guiStampaMazzo(giocatore->carteAulaGiocatore);
                guiStampaMazzo(giocatore->carteBonusMalusGiocatore);
            break;
            case COMANDO_ESCI:
                // TODO: tutta la roba del salvataggio lol
                return;
            default: break; // Aggiunto solo perché CLion dava warning
        }
        premiInvioPerContinuare();
    }
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

/** Una funzione che gioca una carta a scelta del giocatore
 *
 */
void giocaCarta (Giocatore *giocatore) {
    // Pulisce lo schermo e stampa il mazzo
    pulisciSchermo();
    guiStampaMazzo(giocatore->carteGiocatore);

    // L'utente inserisce una carta
    int scelta, nCarte = contaCarte(giocatore->carteGiocatore);
    do {
        printf("\n"
               "Scegli una carta:"
               CURSORE_INPUT);
        scanf("%d", &scelta);

        // Messaggio di errore se la carta scelta non è corretta
        if (scelta > nCarte)
            printf("\n"
                "Hai inserito un numero invalido!"
                CURSORE_INPUT);
    } while (scelta <= nCarte && scelta > 0);

    // Parte da uno nel contare
    for (int i = 0; i < scelta; ++i)
        giocatore = giocatore->next;
}

/** Gestisce gli effetti della carta, e in base a essi, cambia il seguito della partita
 * In pratica è la parte giocabile del gioco.
 *
 */
void gestioneEffetti () {

}

/** Piccola funzione che fa pescare una carta
 *
 */
void pescaCarta (Carta **mazzoGiocatore, Carta **mazzoPesca) {
    printf("Hai pescato:");
    spostaCarta(mazzoPesca, *mazzoPesca, mazzoGiocatore);
    guiStampaCarta(*mazzoPesca);
    premiInvioPerContinuare();
}
