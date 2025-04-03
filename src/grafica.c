//
// Created by diego on 22/02/25.
//

#include "grafica.h"

#include "gioco.h"

/** Genera un menu fantastico.
 * Sito usato per generare la bozza:
 * https://patorjk.com/software/taag/#p=display&f=Big%20Money-ne&t=Unstable%0AStudents
 *
 * Ovviamente ho dovuto copiare una riga alla volta, aggiungere le virgole, i \, e i \n, tutto a mano...
 * È stato a dir poco estenuante...
 */
void guiSplashScreen () {
    pulisciSchermo();
    printf(LINEA_BIANCA "\n"
            HRED "  /$$   /$$"  RED "          "  YEL "          "  GRN "   /$$    "   CYN "          "   HBLU " /$$      "     BLU " /$$"                           "\n"
            HRED " | $$  | $$"  RED "          "  YEL "          "  GRN "  | $$    "   CYN "          "   HBLU "| $$      "     BLU "| $$"                           "\n"
            HRED " | $$  | $$"  RED " /$$$$$$$ "  YEL "  /$$$$$$$"  GRN " /$$$$$$   "  CYN " /$$$$$$ "    HBLU "| $$$$$$$ "     BLU "| $$"        MAG "  /$$$$$$"    "\n"
            HRED " | $$  | $$"  RED "| $$__  $$ " YEL "/$$_____/"   GRN "|_  $$_/   "  CYN "|____  $$"    HBLU "| $$__  $$"     BLU "| $$"        MAG " /$$__  $$"   "\n"
            HRED " | $$  | $$"  RED "| $$  \\ $$" YEL "|  $$$$$$"   GRN "   | $$     " CYN " /$$$$$$$"    HBLU "| $$  \\ $$"    BLU "| $$"        MAG "| $$$$$$$$"   "\n"
            HRED " | $$  | $$"  RED "| $$  | $$ " YEL "\\____  $$"  GRN "  | $$ /$$ "  CYN "/$$__  $$"    HBLU "| $$  | $$"     BLU "| $$"        MAG "| $$_____/"   "\n"
            HRED " |  $$$$$$/"  RED "| $$  | $$ " YEL "/$$$$$$$/ "  GRN " |  $$$$/ "   CYN "| $$$$$$$"    HBLU "| $$$$$$$/"     BLU "| $$"        MAG "|  $$$$$$$"   "\n"
            HRED "  \\______/"  RED " |__/  |__/" YEL "|_______/ "  GRN "   \\___/ "   CYN "  \\_______/" HBLU "|_______/ "     BLU "|__/"        MAG " \\_______/"  "\n"
                                                                                                                                                                     "\n"
            HRED "   /$$$$$$ "  RED "  /$$      " YEL "          "  GRN "     /$$    " CYN "         "    HBLU "        "      BLU "/$$"                              "\n"
            HRED "  /$$__  $$"  RED " | $$      " YEL "          "  GRN "    | $$   "  CYN "         "    HBLU "         "     BLU "| $$"                            "\n"
            HRED " | $$  \\__/" RED "/$$$$$$   "  YEL "/$$   /$$ "  GRN " /$$$$$$$  "  CYN "/$$$$$$  "    HBLU "/$$$$$$$ "     BLU "/$$$$$$"       MAG "  /$$$$$$$"  "\n"
            HRED " |  $$$$$$"   RED "|_  $$_/  "  YEL "| $$  | $$ " GRN "/$$__  $$ "   CYN "/$$__  $$"    HBLU "| $$__  $$ "   BLU "|_$$_/ "       MAG "/$$_____/"   "\n"
            HRED "  \\____  $$" RED " | $$    "   YEL "| $$  | $$"  GRN " |$$  | $$ "  CYN "|$$$$$$$$"    HBLU "| $$  \\ $$ "  BLU "| $$  "        MAG "|  $$$$$$|"  "\n"
            HRED "  /$$  \\ $$" RED " | $$ /$$"   YEL "| $$  | $$"  GRN " |$$  | $$ "  CYN "|$$_____/"    HBLU "| $$  | $$ "   BLU "| $$/$$"       MAG "\\____ $$|"  "\n"
            HRED " |  $$$$$$/ " RED "|  $$$$/"    YEL "|  $$$$$$/"  GRN " |$$$$$$$ "   CYN " |$$$$$$$"    HBLU " | $$  | $$ "  BLU "|  $$$$"       MAG "/$$$$$$$/"   "\n"
            HRED "  \\______/ " RED "  \\___/   " YEL "\\______/"   GRN "  \\_______/" CYN " \\_______/"  HBLU "|__/  |__/  "  BLU "\\___/" MAG "|_______/"          "\n"
            "\n" LINEA_BIANCA
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
    printf(LINEA_BIANCA
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
               BHGRN "Carta %d" RESET ":" RESET, i);
        // Stringa che verrà modificata per stampare il tipo come stringa
        guiStampaCarta(mazzoCarte, dettagli);
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
    printf("\n"
            BHBLU "Nome" RESET ": "
            UNDERLINE "%s" RESET,
            carta->nome);

    printf(" | "
           BYEL "Tipo" RESET ": ");
    strTipologiaCarta(carta->tipo);

    printf("\n"
           BHBLU "Descrizione" RESET ": %s",
           carta->descrizione);

    if (dettagli) {
        for (int i = 0; i < carta->nEffetti; i++) {
            // Lo stampa solo la prima volta
            if (i == MIN_0)
                printf("\n"
                      BRED "Numero di effetti: " RESET
                           "%d", carta->nEffetti);
            printf("\n"
                    "\t-<{ "
                    BHYEL "Azione" RESET ": ");
            strAzione(carta->effetto[i].azione);

            printf(" | "
                    BCYN "Target" RESET ": ");
            strTargetGiocatori(carta->effetto[i].targetGiocatori);

            printf(" | "
                    BRED "Tipo" RESET ": ");
            strTipologiaCarta(carta->effetto[i].tipo);
            printf(" }>-");
        }
    }
    printf("\n"
        BMAG "Quando" RESET ": ");
    strQuando(carta->quandoEffetto);

    // Disessere giocati
    printf( " | "
            BCYN "Opzionale" RESET ": ");
    strOpzionale(carta->opzionale);

    printf("\n");
}

/** Mostra il menù di scelta delle carte.
 *
 */
void guiScegliAzione () {
    printf("\n"
            BOLD
            "Scegli la tua prossima azione:"
            RESET "\n\t"
            HRED "[%d]" RESET ". " BBLU "Gioca" RESET " una carta"              "\n\t"
            HRED "[%d]" RESET ". " BMAG "Pesca" RESET " una carta"              "\n\t"
            "[%d]. " BGRN "Controlla" RESET " le tue carte..."                  "\n\t"
            "[%d]. " BGRN "Mostra le carte" RESET " degli altri giocatori..."   "\n\t"
            "[%d]. " BOLD "Salva ed Esci" RESET                                 "\n\n"
            BHRED "NB: rosso" RESET HYEL " va avanti di un turno." RESET        "\n",
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
    printf("\n" BOLD "Scegli quali mazzi vuoi vedere:");
    printf("\n\t"
           "[%d]. Mostra le carte " BBLU "giocabili" RESET              "\n\t"
           "[%d]. Mostra le carte " BCYN "dell'aula" RESET              "\n\t"
           "[%d]. Mostra le carte " BRED "bonus/malus" RESET            "\n\t"
           "[%d]. " BOLD "Torna indietro" RESET,
           COMANDO_OPZIONE_1,
           COMANDO_OPZIONE_2,
           COMANDO_OPZIONE_3,
           COMANDO_ESCI
           );
}

void guiGiocaOpzionale (char *nomeCarta) {
    printf("\n" LINEA_BIANCA
       "La carta " BLU "%s" RESET " ha effetti " BCYN "opzionali" RESET "." "\n\n"
       BOLD "Vuoi abilitarli?" RESET                                        "\n"
       "[%d] " GRN "Si" RESET                                               "\n"
       "[%d] " RED "No" RESET,
      nomeCarta,  COMANDO_OPZIONE_1, COMANDO_OPZIONE_2);
}

/** Funzione che viene chiamata per mostrare i giocatori affetti da una carta
 *
 * @param listaGiocatori La lista dei giocatori
 * @param nGiocatori Il numero dei giocatori
 */
void guiMostraGiocatori (Giocatore *listaGiocatori, int nGiocatori) {
    printf("\n"
           BOLD "GIOCATORI:" RESET);
    for (int i = 0; i < nGiocatori; i++) {
        // Colori per i nomi dei giocatori. Non volendo passare il turno per il calcolo del colore, esso sarà errato
        char *strColore;
        strColoreGiocatore(&strColore, i + 1);

        printf("\n"
               "[%d]. %s%s" RESET, i + 1, strColore, listaGiocatori->nome);

        // Scorre la lista in avanti
        listaGiocatori = listaGiocatori->next;
    }
}

/* Gestione delle stringhe */

/** Modifica il parametro str con l'azione in entrata
 *
 * @param azione L'azione che dovrà essere cambiata in testo
 */
void strAzione (Azione azione) {
    switch (azione) {
        case GIOCA:
            printf("%s", STR_GIOCA);
        break;
        case SCARTA:
            printf("%s", STR_SCARTA);
        break;
        case ELIMINA:
            printf("%s", STR_ELIMINA);
        break;
        case RUBA:
            printf("%s", STR_RUBA);
        break;
        case PESCA:
            printf("%s", STR_PESCA);
        break;
        case PRENDI:
            printf("%s", STR_PRENDI);
        break;
        case BLOCCA:
            printf("%s", STR_BLOCCA);
        break;
        case SCAMBIA:
            printf("%s", STR_SCAMBIA);
        break;
        case MOSTRA:
            printf("%s", STR_MOSTRA);
        break;
        case IMPEDIRE:
            printf("%s", STR_IMPEDIRE);
        break;
        case INGEGNERE:
            printf("%s", STR_INGEGNERE);
        break;
    }
}

/** Modifica il parametro str con la stringa della topologia
 *
 * @param tipologia La topologia da trasformare in testo
 */

/** Stampa la stringa della tipologia della carta */
void strTipologiaCarta (TipologiaCarta tipologia) {
    switch (tipologia) {
        case ALL:
            printf("%s", STR_ALL);
        break;
        case STUDENTE:
            printf("%s", STR_STUDENTE);
        break;
        case MATRICOLA:
            printf("%s", STR_MATRICOLA);
        break;
        case STUDENTE_SEMPLICE:
            printf("%s", STR_STUDENTE_SEMPLICE);
        break;
        case LAUREANDO:
            printf("%s", STR_LAUREANDO);
        break;
        case BONUS:
            printf("%s", STR_BONUS);
        break;
        case MALUS:
            printf("%s", STR_MALUS);
        break;
        case MAGIA:
            printf("%s", STR_MAGIA);
        break;
        case ISTANTANEA:
            printf("%s", STR_ISTANTANEA);
        break;
    }
}

/** Da target a stringa
 *
 * @param target Il target da trasformare in testo
 */
void strTargetGiocatori (TargetGiocatori target) {
    switch (target) {
        case IO:
            printf("%s", STR_IO);
        break;
        case TU:
            printf("%s", STR_TU);
        break;
        case VOI:
            printf("%s", STR_VOI);
        break;
        case TUTTI:
            printf("%s", STR_TUTTI);
        break;
    }
}

/** Non ho più voglia di scrivere
 *
 * @param quando Croissant
 */
void strQuando (Quando quando) {
    switch (quando) {
        case SUBITO:
            printf("%s", STR_SUBITO);
        break;
        case INIZIO:
            printf("%s", STR_INIZIO);
        break;
        case FINE:
            printf("%s", STR_FINE);
        break;
        case MAI:
            printf("%s", STR_MAI);
        break;
        case SEMPRE:
            printf("%s", STR_SEMPRE);
        break;
    }
}

/** Funzione che restituisce una stringa che indica se la carta è o non è giocabile
 *
 * @param opzionale Bool che contiene se la carta può disessere giocata o no
 */
void strOpzionale (bool opzionale) {
    if (opzionale) printf("%s", STR_TRUE);
    else printf("%s", STR_FALSE);
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
        case 4:
        case 0: // Questo sarebbe l'ultimo turno
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
