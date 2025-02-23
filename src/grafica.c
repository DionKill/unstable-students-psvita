//
// Created by diego on 22/02/25.
//

#include "grafica.h"

// Genera un menu fantastico: https://patorjk.com/software/taag/#p=display&f=Big%20Money-ne&t=Unstable%0AStudents
// Ovviamente ho dovuto copiare una riga alla volta, aggiungere le virgole, i \, e i \n, tutto a mano...
void guiMenu () {
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

    printf("\n"
           "Premi INVIO per continuare"
           "\n");
}

/** Stampa una riga che segnala alcune statistiche
 * TODO: cose che ha senso mostrare
 */
void guiHeader (int turno, char nomeGiocatore[]) {
    printf(WHTB
        "\n"
        RESET
        "\t"
        CYN
        "Turno: "
        RESET
        "%d"
        "\t"
        YEL
        "Giocatore: "
        RESET
        "%s"
        "\n",
        turno, nomeGiocatore);
}

/** Mostra le carte di un giocatore.
 *
 * @param turno
 * @param giocatore Il giocatore di cui deve mostrare le carte
 */
void guiMostraCarte (int turno, Giocatore *giocatore) {
    guiHeader(turno, giocatore->nome);

    Carta *tmp = giocatore->carteGiocatore;
    guiStampaCarte(tmp);
    tmp = giocatore->carteAulaGiocatore;
    guiStampaCarte(tmp);
    tmp = giocatore->carteBonusMalusGiocatore;
    guiStampaCarte(tmp);
}

void guiStampaCarte (Carta *mazzoCarte) {
    int j = 0;

    while (mazzoCarte != NULL) {
        printf("\n"
        "-----");
        printf("\n"
        "Nome: %s", mazzoCarte->nome);
        printf("\n"
        "Descrizione: %s", mazzoCarte->descrizione);
        printf("\n"
        "Tipo: %d", mazzoCarte->tipo);
        printf("\n"
        "Numero di effetti: %d", mazzoCarte->nEffetti);

        for (j = 0; j < mazzoCarte->nEffetti; j++) {
            printf("\n"
            "\t- Azione: %d", mazzoCarte->effetto[j].azione);
            printf("\n"
            "\t- Target: %d", mazzoCarte->effetto[j].targetGiocatori);
            printf("\n"
            "\t- Tipo: %d", mazzoCarte->effetto[j].tipo);
        }

        printf("\n"
        "Quando: %d", mazzoCarte->quandoEffetto);
        printf("\n"
        "Disessere giocati: %d", mazzoCarte->puoEssereGiocato);
        printf("\n"
        "-----");

        mazzoCarte = mazzoCarte->next;
        j++;
    }
    printf("\n" REDB "TOTALE CARTE: %d\n" RESET, j);
}

/** Mostra il menù di scelta delle carte.
 *
 */
void guiScegliAzione () {
    printf("Scegli la tua prossima azione:"
    "\n"
    "1. Gioca una carta"
    "\t"
    "2. Pesca una carta"
    "\t"
    "3. Controlla le tue carte"
    "\t"
    "4. Salva ed Esci"
    "\n>: ");
}



/** Pulisce lo schermo.
 * Usa le direttive di pre-processing per dedurre il comando da utilizzare in base all'OS.
 */
void pulisciSchermo () {
#ifdef _WIN32
    system("cls"); // Windows a 32 o 64 bit, penso indipendente dall'architettura
#elif __unix__
    system("clear"); // Unix (GNU/Linux, MacOS, etc...)
#endif
}
