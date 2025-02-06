//
// Created by diego on 30/12/24.
//

#include "gioco.h"
#include "strutture.h"

// Genera un menu fantastico: https://patorjk.com/software/taag/#p=display&f=Big%20Money-ne&t=Unstable%0AStudents
// Ovviamente ho dovuto copiare una riga alla volta, aggiungere le virgole, i \, e i \n, tutto a mano
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

/** La funzione principale del gioco. Da qui viene gestito tutto.
 *
 */
void gioco () {
    Giocatore *listaGiocatori = NULL;
    creaGiocatori(&listaGiocatori);

}

/** Funzione che crea la lista di giocatori
 *
 * @return Ritorna la nuova lista di giocatori
 */
void creaGiocatori (Giocatore **listaGiocatori) {
    int nGiocatori;

    printf("Quanti giocatori giocheranno?");
    do {
        printf("\n"
        "[2-4]: ");
        scanf("%d", &nGiocatori);
        if (nGiocatori < 2 || nGiocatori > 4 )
            printf("\n"
                "Il valore inserito non e' valido!");
    } while (nGiocatori < 2 || nGiocatori > 4);

    // Alloca lo spazio in memoria e li aggiunge in una lista di tipo Giocatore
    *listaGiocatori = aggiungiGiocatori(*listaGiocatori, nGiocatori);

    *listaGiocatori = rendiListaGiocatoriCircolare(*listaGiocatori); // Rende la lista dei giocatori circolare
}