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

/** Dato un mazzo, stampa le carte in esso contenute con tutti i dettagli
 *
 * @param mazzoCarte Il mazzo di carte
 */
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

/** Modifica il parametro str con l'azione in entrata
 *
 * @param str La stringa da modificare
 * @param azione L'azione che dovrà essere cambiata in testo
 */
void strAzione(char* str, Azione azione) {
    switch (azione) {
        case GIOCA:
            strcpy(str, STR_GIOCA);
            break;
        case SCARTA:
            strcpy(str, STR_SCARTA);
            break;
        case ELIMINA:
            strcpy(str, STR_ELIMINA);
            break;
        case RUBA:
            strcpy(str, STR_RUBA);
            break;
        case PESCA:
            strcpy(str, STR_PESCA);
            break;
        case PRENDI:
            strcpy(str, STR_PRENDI);
            break;
        case BLOCCA:
            strcpy(str, STR_BLOCCA);
            break;
        case SCAMBIA:
            strcpy(str, STR_SCAMBIA);
            break;
        case MOSTRA:
            strcpy(str, STR_MOSTRA);
            break;
        case IMPEDIRE:
            strcpy(str, STR_IMPEDIRE);
            break;
        case INGEGNERE:
            strcpy(str, STR_INGEGNERE);
            break;
    }
}

/** Modifica il parametro str con la stringa della topologia
 *
 * @param str La stringa da modificare
 * @param tipologia La topologia da trasformare in testo
 */
void strTipologiaCarta(char* str, TipologiaCarta tipologia) {
    switch (tipologia) {
        case ALL:
            strcpy(str, STR_ALL);
            break;
        case STUDENTE:
            strcpy(str, STR_STUDENTE);
            break;
        case MATRICOLA:
            strcpy(str, STR_MATRICOLA);
            break;
        case STUDENTE_SEMPLICE:
            strcpy(str, STR_STUDENTE_SEMPLICE);
            break;
        case LAUREANDO:
            strcpy(str, STR_LAUREANDO);
            break;
        case BONUS:
            strcpy(str, STR_BONUS);
            break;
        case MALUS:
            strcpy(str, STR_MALUS);
            break;
        case MAGIA:
            strcpy(str, STR_MAGIA);
            break;
        case ISTANTANEA:
            strcpy(str, STR_ISTANTANEA);
            break;
    }
}

/** Da target a stringa
 *
 * @param str La stringa da modificare
 * @param target Il target da trasformare in testo
 */
void strTargetGiocatori(char* str, TargetGiocatori target) {
    switch (target) {
        case IO:
            strcpy(str, STR_IO);
            break;
        case TU:
            strcpy(str, STR_TU);
            break;
        case VOI:
            strcpy(str, STR_VOI);
            break;
        case TUTTI:
            strcpy(str, STR_TUTTI);
            break;
    }
}

/** Non ho più voglia di scrivere
 *
 * @param str La stringa da modificare
 * @param quando Croissant
 */
void strQuando(char* str, Quando quando) {
    switch (quando) {
        case SUBITO:
            strcpy(str, STR_SUBITO);
        break;
        case INIZIO:
            strcpy(str, STR_INIZIO);
        break;
        case FINE:
            strcpy(str, STR_FINE);
        break;
        case MAI:
            strcpy(str, STR_MAI);
        break;
        case SEMPRE:
            strcpy(str, STR_SEMPRE);
        break;
    }
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
