//
// Created by diego on 22/02/25.
//

#include "grafica.h"

#include "gioco.h"

// Genera un menu fantastico: https://patorjk.com/software/taag/#p=display&f=Big%20Money-ne&t=Unstable%0AStudents
// Ovviamente ho dovuto copiare una riga alla volta, aggiungere le virgole, i \, e i \n, tutto a mano...
void guiSplashScreen () {
    pulisciSchermo();
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
 * @param turno Il turno della partita
 * @param nGiocatori Il numero totale dei giocatori (serve per il colore)
 * @param nomeGiocatore Il nome del giocatore
 */
void guiHeader (int turno, int nGiocatori, char *nomeGiocatore) {
    // Ottiene il colore
    char *strColore;
    strColoreGiocatore(&strColore, turno % nGiocatori);

    pulisciSchermo();
    printf(WHTB "\t\t\t\t\t\t\t\t\t\t" "\n" RESET
            "\t" CYN "Turno" RESET ": %d"
            "\t\t" YEL "Giocatore" RESET ": %s%s"
            RESET "\n",
            turno, strColore, nomeGiocatore);
}

/** Dato un mazzo, stampa le carte in esso contenute con tutti i dettagli
 *
 * @param mazzoCarte Il mazzo di carte
 * @param dettagli
 */
void guiStampaMazzo (Carta *mazzoCarte, bool dettagli) {
    // Conta anche il numero di carte, e stampa il numero di ogni carta (utile per giocare la carta)
    int i = 1;
    while (mazzoCarte != NULL) {
        printf("\n"
                YELB
                "Carta %d:"
                RESET, i);
        // Stringa che verrà modificata per stampare il tipo come stringa
        guiStampaCarta(mazzoCarte, true);
        mazzoCarte = mazzoCarte->next;
        i++;
    }
}

/** Stampa una singola carta
 *
 * @param carta La carta da stampare
 * @param dettagli Booleano che, se vero, mostra più dettagli sulla carta
 */
void guiStampaCarta (Carta *carta, bool dettagli) {
    char *strColore = NULL;

    printf("\n"
            BHBLU "Nome" RESET ": "
            UNDERLINE "%s" RESET,
            carta->nome);

    printf(" | "
            BHBLU "Descrizione"
            RESET ": %s",
            carta->descrizione);

    strTipologiaCarta(&strColore, carta->tipo);
    printf("\n"
            BYEL "Tipo"
            RESET ": %s"
            "\n", strColore);

    if (dettagli) {
        printf("Numero di effetti: %d", carta->nEffetti);

        for (int i = 0; i < carta->nEffetti; i++) {
            strAzione(&strColore, carta->effetto[i].azione);
            printf("\n"
                    "\t-<{ "
                    BHYEL "Azione"
                    RESET ": %s",
                    strColore);

            strTargetGiocatori(&strColore, carta->effetto[i].targetGiocatori);
            printf(" | "
                    BCYN "Target"
                    RESET ": %s",
                    strColore);

            strTipologiaCarta(&strColore, carta->effetto[i].tipo);
            printf(" | "
                    BRED "Tipo"
                    RESET ": %s"
                    " }>-", strColore);
        }

        strQuando(&strColore, carta->quandoEffetto);
        printf("\n"
                BMAG "Quando"
                RESET ": %s",
                strColore);

        strOpzionale(&strColore, carta->opzionale);
        printf( " | "
                BHWHT "Puo' essere giocato"
                RESET ": %s" // Disessere giocati
                "\n", strColore);
    }
}

void guiStampaCarteGiocatore (Giocatore *giocatore, bool mostraMano) {
    if (mostraMano) {
        printf("\n"
               HYEL "Carte in mano "
               RESET "al giocatore: "
               "%s", giocatore->nome);
        guiStampaMazzo(giocatore->carteGiocatore, false);
    }
    printf("\n"
            BRED "Carte in aula "
            RESET "al giocatore: "
            "%s", giocatore->nome);
    guiStampaMazzo(giocatore->carteAulaGiocatore, false);

    printf("\n"
            BRED "Carte bonus/malus "
            RESET "del giocatore: %s",
            giocatore->nome);
    guiStampaMazzo(giocatore->carteBonusMalusGiocatore, false);
}

/** Mostra il menù di scelta delle carte.
 *
 */
void guiScegliAzione () {
    printf("\n"
            BOLD
            "Scegli la tua prossima azione:"
            RESET "\n\t"
            "%d. " BBLU "Gioca" RESET " una carta"                           "\n\t"
            "%d. " BMAG "Pesca" RESET " una carta"                           "\n\t"
            "%d. " BGRN "Controlla" RESET " le tue carte..."                 "\n\t"
            "%d. " BGRN "Mostra le carte" RESET " degli altri giocatori..."  "\n\t"
            "%d. " BOLD "Salva ed Esci" RESET,
            COMANDO_OPZIONE_1,
            COMANDO_OPZIONE_2,
            COMANDO_OPZIONE_3,
            COMANDO_OPZIONE_4,
            COMANDO_ESCI
            );
}

/** Mostra il menù dello stato della partita.
 *
 */
void guiMostraStatoPartita () {
    printf("\n" "Scegli cosa vuoi fare:");
    printf("\n\t"
           "%d. Mostra le carte " BBLU "giocabili" RESET               "\n\t"
           "%d. Mostra le carte " BCYN "dell'aula" RESET               "\n\t"
           "%d. Mostra le carte " BRED "bonus/malus" RESET             "\n\t"
           "%d. " BOLD "Torna indietro" RESET,
           COMANDO_OPZIONE_1,
           COMANDO_OPZIONE_2,
           COMANDO_OPZIONE_3,
           COMANDO_ESCI
           );
}

/* Gestione delle stringhe */

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
    }
}

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
    }
}

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
 * @param opzionale Bool che contiene se la carta può disessere giocata o no
 */
void strOpzionale (char **str, bool opzionale) {
    if (opzionale) *str = STR_TRUE;
    else *str = STR_FALSE;
}

/** Modifica una stringa mettendoci dentro i colori per la printf
 *
 * @param str La stringa da modificare con la macro colorata
 * @param giocatore Il numero del giocatore a cui vi sarà dato un colore
 */
void strColoreGiocatore (char **str, int giocatore) {
    switch (giocatore) {
        case 1:
            *str = RED;
        break;
        case 2:
            *str = BLU;
        break;
        case 3:
            *str = GRN;
        break;
        case 4: // Questo sarebbe l'ultimo turno
            *str = YEL;
        break;
        default: *str = HWHT; // Viene usato solo in casi estremi, non dovrebbe mai comparire normalmente
    }
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
