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
}

/** Stampa una riga che segnala alcune statistiche
 * TODO: cose che ha senso mostrare
 */
void guiHeader (int turno, char nomeGiocatore[]) {
    pulisciSchermo();
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

/** Dato un mazzo, stampa le carte in esso contenute con tutti i dettagli
 *
 * @param mazzoCarte Il mazzo di carte
 */
void guiStampaMazzo (Carta *mazzoCarte) {
    int i = 0;

    while (mazzoCarte != NULL) {
        // Stringa che verrà modificata per stampare il tipo come stringa
        printf("\n"
            "-----");
        guiStampaCarta(mazzoCarte);
        mazzoCarte = mazzoCarte->next;
        i++;
    }
    printf("\n"
    "-----");
    printf("\n" YELHB "TOTALE CARTE: %d" RESET "\n", i);
}

/** Stampa una singola carta
 *
 * @param carta La carta da stampare
 */
void guiStampaCarta (Carta *carta) {
    char *str = NULL;

    printf("\n"
        "Nome: %s", carta->nome);
    printf(" | "
        "Descrizione: %s", carta->descrizione);

    strTipologiaCarta(&str, carta->tipo);
    printf("\n"
        "Tipo: %s", str);

    printf("\n"
        "Numero di effetti: %d", carta->nEffetti);

    for (int i = 0; i < carta->nEffetti; i++) {
        strAzione(&str, carta->effetto[i].azione);
        printf("\n"
            "\t-<{ Azione: %s", str);

        strTargetGiocatori(&str, carta->effetto[i].targetGiocatori);
        printf("\t| "
            "Target: %s", str);

        strTipologiaCarta(&str, carta->effetto[i].tipo);
        printf("\t| "
            "Tipo: %s }>-", str);
    }

    strQuando(&str, carta->quandoEffetto);
    printf("\n"
        "Quando: %s", str);

    strPuoEssereGiocato(&str, carta->puoEssereGiocato);
    printf( " | "
        "Puo' essere giocato: %s" // Disessere giocati
        , str);
}

/** Mostra il menù di scelta delle carte.
 *
 */
void guiScegliAzione () {
    printf("\n"
        "Scegli la tua prossima azione:"
        "\n"
        "1. Gioca una carta"
        " | "
        "2. Pesca una carta"
        " | "
        "3. Controlla le tue carte"
        " | "
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

/** Funzione che restituisce una stringa che indica se la carta è o non è giocabile
 *
 * @param str La stringa che conterrà VERO o FALSO
 * @param puoEssereGiocato Bool che contiene se la carta può disessere giocata o no
 */
void strPuoEssereGiocato (char **str, bool puoEssereGiocato) {
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
