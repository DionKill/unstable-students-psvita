//
// Created by diego on 30/12/24.
//

#include "gioco.h"

#include "debug.h"
#include "file.h"
#include "strutture.h"

// Genera un menu fantastico: https://patorjk.com/software/taag/#p=display&f=Big%20Money-ne&t=Unstable%0AStudents
// Ovviamente ho dovuto copiare una riga alla volta, aggiungere le virgole, i \, e i \n, tutto a mano...
void menu () {
    printf ("\n"
    MAG
    "-------------------------------------------------------------------------------"       "\n"
    RESET
    "\n"
    " /$$   /$$                       /$$               /$$       /$$"                      "\n"
    "| $$  | $$                      | $$              | $$      | $$"                      "\n"
    "| $$  | $$ /$$$$$$$   /$$$$$$$ /$$$$$$    /$$$$$$ | $$$$$$$ | $$  /$$$$$$"             "\n"
    "| $$  | $$| $$__  $$ /$$_____/|_  $$_/   |____  $$| $$__  $$| $$ /$$__  $$"            "\n"
    "| $$  | $$| $$  \\ $$|  $$$$$$   | $$      /$$$$$$$| $$  \\ $$| $$| $$$$$$$$"          "\n"
    "| $$  | $$| $$  | $$ \\____  $$  | $$ /$$ /$$__  $$| $$  | $$| $$| $$_____/"           "\n"
    "|  $$$$$$/| $$  | $$ /$$$$$$$/  |  $$$$/|  $$$$$$$| $$$$$$$/| $$|  $$$$$$$"            "\n"
    " \\______/ |__/  |__/|_______/    \\___/   \\_______/|_______/ |__/ \\_______/"        "\n"
    "\n"
    "  /$$$$$$   /$$                     /$$                       /$$"                     "\n"
    " /$$__  $$ | $$                    | $$                      | $$"                     "\n"
    "| $$  \\__//$$$$$$   /$$   /$$  /$$$$$$$  /$$$$$$  /$$$$$$$  /$$$$$$   /$$$$$$$"       "\n"
    "|  $$$$$$|_  $$_/  | $$  | $$ /$$__  $$ /$$__  $$| $$__  $$|_  $$_/  /$$_____/"        "\n"
    " \\____  $$ | $$    | $$  | $$| $$  | $$| $$$$$$$$| $$  \\ $$  | $$   |  $$$$$$"       "\n"
    " /$$  \\ $$ | $$ /$$| $$  | $$| $$  | $$| $$_____/| $$  | $$  | $$ /$$\\____  $$"      "\n"
    "|  $$$$$$/ |  $$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$| $$  | $$  |  $$$$//$$$$$$$/"        "\n"
    " \\______/   \\___/   \\______/  \\_______/ \\_______/|__/  |__/   \\___/ |_______/"   "\n"
    "\n"
    MAG
    "-------------------------------------------------------------------------------"       "\n"
    RESET
    );
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
    *listaGiocatori = aggiungiGiocatori(*listaGiocatori, nGiocatori);

    *listaGiocatori = rendiListaGiocatoriCircolare(*listaGiocatori); // Rende la lista dei giocatori circolare

    return nGiocatori;
}

/** Distribuisce le carte ai giocatori, prendendole dal mazzo già mescolato da pesca.
 *
 * @param cntCarte Il numero di giocatori (perché la lista è circolare -_-)
 * @param listaGiocatori La lista di giocatori a cui dare le carte
 * @param mazzoPesca Il mazzo da cui attingere le carte
 */
void distribuisciCarte (int cntCarte, Giocatore *listaGiocatori, Carta **mazzoPesca) {
    // Un ciclo che continua finché ci sono carte da pescare (scorre nella lista circolare dei giocatori)
    for (int i = 0; i < cntCarte; i++) {
        // Copia della testa per non perdere il next del mazzoPesca quando scorrerà (lo vediamo immediatamente)
        Carta *tmpMazzoPesca = (*mazzoPesca)->next;
        switch ((*mazzoPesca)->tipo) {
            case MATRICOLA:
            case STUDENTE_SEMPLICE:
            case LAUREANDO:
                // Mette come next della carta in testa al mazzo da pesca, la testa del mazzo delle carte del giocatore
                (*mazzoPesca)->next = listaGiocatori->carteAulaGiocatore;
                // La nuova carta (con tutte le carte già presenti al next) viene messa come nuova testa del mazzo
                listaGiocatori->carteAulaGiocatore = *mazzoPesca;
                break;

            case BONUS:
            case MALUS:
                // Mette come next della carta in testa al mazzo da pesca, la testa del mazzo delle carte del giocatore
                (*mazzoPesca)->next = listaGiocatori->carteBonusMalusGiocatore;
                // La nuova carta (con tutte le carte già presenti al next) viene messa come nuova testa del mazzo
                listaGiocatori->carteBonusMalusGiocatore = *mazzoPesca;
                break;

            default:
                // Mette come next della carta in testa al mazzo da pesca, la testa del mazzo delle carte del giocatore
                (*mazzoPesca)->next = listaGiocatori->carteGiocatore;
                // La nuova carta (con tutte le carte già presenti al next) viene messa come nuova testa del mazzo
                listaGiocatori->carteGiocatore = *mazzoPesca;
                break;
        }

        // Scorre avanti le due liste
        *mazzoPesca = tmpMazzoPesca;
        listaGiocatori = listaGiocatori->next;
    }
}

/** La funzione principale del gioco. Da qui viene gestito tutto.
 *
 */
void gioco () {
    // Crea i giocatori e li popola
    Giocatore *listaGiocatori = NULL;
    int nGiocatori = creaGiocatori(&listaGiocatori);

    // Crea il mazzo di carte, e lo popola usando il file mazzo.txt
    Carta *mazzoPesca = NULL;
    leggiCarteDaFile(&mazzoPesca);

    // Crea il mazzo delle carte scartate e dell'aula studio
    Carta *mazzoScarti = NULL; // Sacrate (per pochi)
    Carta *mazzoAulaStudio = NULL;

    // Divide il mazzo da pesca originale, creando il mazzo delle matricole
    Carta *mazzoMatricole = NULL;
    mazzoMatricole = dividiMazzoMatricola(&mazzoPesca); // Crea il mazzo matricola partendo dal mazzo da pesca

    // Mischia i due mazzi appena creati
    shuffleCarte(&mazzoPesca);
    shuffleCarte(&mazzoMatricole);

    // Distribuisce le carte a ogni giocatore, dal mazzo di pesca
    stampaCarte(mazzoPesca);
    distribuisciCarte(N_CARTE_PER_GIOCATORE * nGiocatori, listaGiocatori, &mazzoPesca);

    // Gestione dei turni
    int turno = 0;
    // TODO: Funzione che controlla quando vinci così la metto nella condizione del main
    while (turno < 5) {
        int scelta = scegliAzione();
        
        switch (scelta) {
            case 1:
                // TODO: gioca una carta
                    break;
            case 2:
                // TODO: pesca una carta
                    break;
            case 3:
                // TODO: tutta la roba del salvataggio lol
                    break;
        }
        turno++;
    }
}

/** Funzione ineccepibile che fornisce una scelta di cose che può fare il giocatore e ritorna l'opzione scelta.
 *
 * @return La scelta del giocatore.
 */
int scegliAzione () {
    printf("Scegli la tua prossima azione: "
        "\t"
        "1. Gioca una carta"
        "\t"
        "2. Pesca una carta"
        "\t"
        "3. Salva ed Esci"
        "\t");

    int scelta;
    scanf("%d", &scelta);

    return scelta;
}

/** Funzione che libera l'input buffer.
 * Con questa non vengono aggiunti gli invii e caratteri speciali durante l'inserimento di testo.
 */
void flushInputBuffer () {
    while (getchar() != '\n');
}
