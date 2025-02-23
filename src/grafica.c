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
        // Stringa che verrà modificata per stampare il tipo come stringa
        char *str = NULL;

        printf("\n"
        "-----");
        printf("\n"
        "Nome: %s", mazzoCarte->nome);
        printf("\n"
        "Descrizione: %s", mazzoCarte->descrizione);

        strTipologiaCarta(&str, mazzoCarte->tipo);
        printf("\n"
        "Tipo: %s", str);

        printf("\n"
        "Numero di effetti: %d", mazzoCarte->nEffetti);

        for (j = 0; j < mazzoCarte->nEffetti; j++) {
            strAzione(&str, mazzoCarte->effetto[j].azione);
            printf("\n"
            "\t- Azione: %s", str);

            strTargetGiocatori(&str, mazzoCarte->effetto[j].targetGiocatori);
            printf("\n"
            "\t- Target: %s", str);

            strTipologiaCarta(&str, mazzoCarte->effetto[j].tipo);
            printf("\n"
            "\t- Tipo: %s", str);
        }

        strQuando(&str, mazzoCarte->quandoEffetto);
        printf("\n"
        "Quando: %s", str);

        strDisessereGiocati(&str, mazzoCarte->puoEssereGiocato);
        printf( "\n"
                YELB
                "Disessere giocati: %s"
                RESET, str);
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
void strAzione(char **str, Azione azione) {
    switch (azione) {
        case GIOCA:
            *str = STR_GIOCA;
        break;
        case SCARTA:
            *str = STR_SCARTA;
        break;
        case ELIMINA:
            *str = STR_ELIMINA;
        break;
        case RUBA:
            *str = STR_RUBA;
        break;
        case PESCA:
            *str = STR_PESCA;
        break;
        case PRENDI:
            *str = STR_PRENDI;
        break;
        case BLOCCA:
            *str = STR_BLOCCA;
        break;
        case SCAMBIA:
            *str = STR_SCAMBIA;
        break;
        case MOSTRA:
            *str = STR_MOSTRA;
        break;
        case IMPEDIRE:
            *str = STR_IMPEDIRE;
        break;
        case INGEGNERE:
            *str = STR_INGEGNERE;
        break;
    }
}

/** Modifica il parametro str con la stringa della topologia
 *
 * @param str La stringa da modificare
 * @param tipologia La topologia da trasformare in testo
 */
void strTipologiaCarta(char **str, TipologiaCarta tipologia) {
    switch (tipologia) {
        case ALL:
            *str = STR_ALL;
        break;
        case STUDENTE:
            *str = STR_STUDENTE;
        break;
        case MATRICOLA:
            *str = STR_MATRICOLA;
        break;
        case STUDENTE_SEMPLICE:
            *str = STR_STUDENTE_SEMPLICE;
        break;
        case LAUREANDO:
            *str = STR_LAUREANDO;
        break;
        case BONUS:
            *str = STR_BONUS;
        break;
        case MALUS:
            *str = STR_MALUS;
        break;
        case MAGIA:
            *str = STR_MAGIA;
        break;
        case ISTANTANEA:
            *str = STR_ISTANTANEA;
        break;
    }}

/** Da target a stringa
 *
 * @param str La stringa da modificare
 * @param target Il target da trasformare in testo
 */
void strTargetGiocatori(char **str, TargetGiocatori target) {
    switch (target) {
        case IO:
            *str = STR_IO;
        break;
        case TU:
            *str = STR_TU;
        break;
        case VOI:
            *str = STR_VOI;
        break;
        case TUTTI:
            *str = STR_TUTTI;
        break;
    }}

/** Non ho più voglia di scrivere
 *
 * @param str La stringa da modificare
 * @param quando Croissant
 */
void strQuando(char **str, Quando quando) {
    switch (quando) {
        case SUBITO:
            *str = STR_SUBITO;
        break;
        case INIZIO:
            *str = STR_INIZIO;
        break;
        case FINE:
            *str = STR_FINE;
        break;
        case MAI:
            *str = STR_MAI;
        break;
        case SEMPRE:
            *str = STR_SEMPRE;
        break;
    }
}

/** Funzione che restituisce una stringa che contiene la disessere giocabilità vera o falsa
 *
 * @param str La stringa che conterrà VERO o FALSO
 * @param puoEssereGiocato Disessere giocati
 */
void strDisessereGiocati (char **str, bool puoEssereGiocato) {
    if (puoEssereGiocato) *str = STR_TRUE;
    else *str = STR_FALSE;
}

/** Pulisce lo schermo.
 * Usa le direttive di pre-processing per dedurre il comando da utilizzare in base all'OS.
 */
void pulisciSchermo () {
#ifdef _WIN32
    system("cls"); // Windows a 32 o 64 bit, indipendente dall'architettura (sapevate che c'è windows nt 4.0 per ppc?)
#elif __unix__
    system("clear"); // Unix (GNU/Linux, MacOS, etc...)
#endif
}
