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
void guiSplashScreen() {
    pulisciSchermo();
    printf(LINEA_BIANCA
           HRED "  /$$   /$$"  RED "          "  YEL "          "  GRN "   /$$    "   CYN "          "   HBLU " /$$      "     BLU " /$$"                      "\n"
           HRED " | $$  | $$"  RED "          "  YEL "          "  GRN "  | $$    "   CYN "          "   HBLU "| $$      "     BLU "| $$"                      "\n"
           HRED " | $$  | $$"  RED " /$$$$$$$ "  YEL "  /$$$$$$$"  GRN " /$$$$$$   "  CYN " /$$$$$$ "    HBLU "| $$$$$$$ "     BLU "| $$"    MAG "  /$$$$$$"   "\n"
           HRED " | $$  | $$"  RED "| $$__  $$ " YEL "/$$_____/"   GRN "|_  $$_/   "  CYN "|____  $$"    HBLU "| $$__  $$"     BLU "| $$"    MAG " /$$__  $$"  "\n"
           HRED " | $$  | $$"  RED "| $$  \\ $$" YEL "|  $$$$$$"   GRN "   | $$     " CYN " /$$$$$$$"    HBLU "| $$  \\ $$"    BLU "| $$"    MAG "| $$$$$$$$"  "\n"
           HRED " | $$  | $$"  RED "| $$  | $$ " YEL "\\____  $$"  GRN "  | $$ /$$ "  CYN "/$$__  $$"    HBLU "| $$  | $$"     BLU "| $$"    MAG "| $$_____/"  "\n"
           HRED " |  $$$$$$/"  RED "| $$  | $$ " YEL "/$$$$$$$/ "  GRN " |  $$$$/ "   CYN "| $$$$$$$"    HBLU "| $$$$$$$/"     BLU "| $$"    MAG "|  $$$$$$$"  "\n"
           HRED "  \\______/"  RED " |__/  |__/" YEL "|_______/ "  GRN "   \\___/ "   CYN "  \\_______/" HBLU "|_______/ "     BLU "|__/"    MAG " \\_______/" "\n"
                                                                                                                                                               "\n"
           HRED "   /$$$$$$ "  RED "  /$$      " YEL "          "  GRN "     /$$    " CYN "         "    HBLU "        "      BLU "/$$"                        "\n"
           HRED "  /$$__  $$"  RED " | $$      " YEL "          "  GRN "    | $$   "  CYN "         "    HBLU "         "     BLU "| $$"                       "\n"
           HRED " | $$  \\__/" RED "/$$$$$$   "  YEL "/$$   /$$ "  GRN " /$$$$$$$  "  CYN "/$$$$$$  "    HBLU "/$$$$$$$ "     BLU "/$$$$$$"  MAG "  /$$$$$$$"  "\n"
           HRED " |  $$$$$$"   RED "|_  $$_/  "  YEL "| $$  | $$ " GRN "/$$__  $$ "   CYN "/$$__  $$"    HBLU "| $$__  $$ "   BLU "|_$$_/ "  MAG "/$$_____/"   "\n"
           HRED "  \\____  $$" RED " | $$    "   YEL "| $$  | $$"  GRN " |$$  | $$ "  CYN "|$$$$$$$$"    HBLU "| $$  \\ $$ "  BLU "| $$  "   MAG "|  $$$$$$|"  "\n"
           HRED "  /$$  \\ $$" RED " | $$ /$$"   YEL "| $$  | $$"  GRN " |$$  | $$ "  CYN "|$$_____/"    HBLU "| $$  | $$ "   BLU "| $$/$$"  MAG "\\____ $$|"  "\n"
           HRED " |  $$$$$$/ " RED "|  $$$$/"    YEL "|  $$$$$$/"  GRN " |$$$$$$$ "   CYN " |$$$$$$$"    HBLU " | $$  | $$ "  BLU "|  $$$$"  MAG "/$$$$$$$/"   "\n"
           HRED "  \\______/ " RED "  \\___/   " YEL "\\______/"   GRN "  \\_______/" CYN " \\_______/"  HBLU "|__/  |__/  "  BLU "\\___/"   MAG "|_______/"   "\n"
           "\n" LINEA_BIANCA
    );
}

/** Stampa una riga che segnala alcune statistiche
 * @param turno Il turno della partita
 * @param nGiocatori Il numero totale dei giocatori (serve per il colore)
 * @param nomeGiocatore Il nome del giocatore
 */
void guiHeader(int turno, int nGiocatori, char *nomeGiocatore) {
    pulisciSchermo();
    printf(LINEA_BIANCA
           "\t" CYN "Turno" RESET ": %d"
           "\t\t" YEL "Giocatore" RESET ": %s%s"
           RESET "\n",
           turno, strColoreGiocatore(turno % nGiocatori), nomeGiocatore);
}

/** Dato un mazzo, stampa le carte in esso contenute con tutti i dettagli
 *
 * @param mazzoCarte Il mazzo di carte
 * @param dettagli
 */
void guiStampaMazzo(Carta *mazzoCarte, bool dettagli) {
    // Conta anche il numero di carte, e stampa il numero di ogni carta (utile per giocare la carta)
    int i = 1;

    printf(LINEA_BIANCA);

    while (mazzoCarte != NULL) {
        printf(BHGRN "Carta %d" RESET ":" RESET, i);
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
void guiStampaCarta(Carta *carta, bool dettagli) {
    printf("\n"
           BHBLU "Nome" RESET ": "
           UNDERLINE "%s" RESET,
           carta->nome);

    printf(" | "
           BYEL "Tipo" RESET ": %s", strTipologiaCarta(carta->tipo));

    printf("\n"
           BHBLU "Descrizione" RESET ": %s", carta->descrizione);

    if (dettagli) {
        for (int i = 0; i < carta->nEffetti; i++) {
            // Lo stampa solo la prima volta
            if (i == MIN_0) {
                printf("\n"
                       BMAG "Quando" RESET ": %s", strQuando(carta->quandoEffetto));

                // Disessere giocati
                printf(" | "
                       BCYN "Opzionale" RESET ": %s", strOpzionale(carta->opzionale));
                printf("\n"
                       BRED "Numero di effetti: " RESET "%d", carta->nEffetti);
            }

            printf("\n"
                   "\t-<{ " BHYEL "Azione" RESET ": %s", strAzione(carta->effetto[i].azione));

            printf(" | "
                   BCYN "Target" RESET ": %s", strTargetGiocatori(carta->effetto[i].targetGiocatori));

            printf(" | "
                   BRED "Tipo" RESET ": %s }>-", strTipologiaCarta(carta->effetto[i].tipo));
        }
    }
    printf("\n\n");
}

/** Mostra il menù di scelta delle carte.
 *
 */
void guiScegliAzione() {
    printf("\n"
           BOLD
           "Scegli la tua prossima azione:"
           RESET "\n\t"
           HRED "[%d]" RESET ". " BBLU "Gioca" RESET " una carta" "\n\t"
           HRED "[%d]" RESET ". " BMAG "Pesca" RESET " una carta" "\n\t"
           "[%d]. " BGRN "Controlla" RESET " le tue carte..." "\n\t"
           "[%d]. " BGRN "Mostra le carte" RESET " degli altri giocatori..." "\n\t"
           "[%d]. " BOLD "Salva ed Esci" RESET "\n\n"
           BHRED "NB: rosso" RESET HYEL " va avanti di un turno." RESET "\n",
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
void guiMostraStatoPartita() {
    printf(LINEA_BIANCA
           BOLD "Scegli quale mazzo vuoi vedere:" RESET);
    printf("\n\t"
           "[%d]. Mostra le carte " BBLU "giocabili" RESET "\n\t"
           "[%d]. Mostra le carte " BCYN "dell'aula" RESET "\n\t"
           "[%d]. Mostra le carte " BRED "bonus/malus" RESET "\n\t"
           "[%d]. " BOLD "Torna indietro" RESET,
           COMANDO_OPZIONE_1,
           COMANDO_OPZIONE_2,
           COMANDO_OPZIONE_3,
           COMANDO_ESCI
    );
}

void guiGiocaOpzionale(char *giocatore, char *carta) {
    printf("\n" LINEA_BIANCA
           "%s, la carta " BLU "%s" RESET " ha effetti " BCYN "opzionali" RESET "." "\n\n"
           BOLD "Vuoi abilitarli?" RESET "\n"
           "[%d] " GRN "Si" RESET "\n"
           "[%d] " RED "No" RESET, giocatore, carta, COMANDO_OPZIONE_1, COMANDO_ESCI);
}

/** Funzione che viene chiamata per mostrare i giocatori affetti da una carta
 *
 * @param listaGiocatori La lista dei giocatori
 * @param nGiocatori Il numero dei giocatori
 */
void guiMostraGiocatori(Giocatore *listaGiocatori, int nGiocatori) {
    printf("\n"
        BOLD "GIOCATORI:" RESET);
    for (int i = 0; i < nGiocatori; i++) {
        printf("\n"
               "[%d]. %s%s" RESET, i + 1, strColoreGiocatore(i + 1), listaGiocatori->nome);

        // Scorre la lista in avanti
        listaGiocatori = listaGiocatori->next;
    }
}

/** Menù della vittoria
 *
 * @param nome Il nome del giocatore vittorioso
 */
void guiVittoria(char *nome) {
    printf("\n" LINEA_BIANCA);
    printf(BHYEL "HA VINTO %s"
           "\n" RESET, nome);

    printf(".------..------..------..------..------..------..------..------." "\n"
        "|U.--. ||N.--. ||S.--. ||T.--. ||A.--. ||B.--. ||L.--. ||E.--. |" "\n"
        "| (\\/) || :(): || :/\\: || :/\\: || (\\/) || :(): || :/\\: || (\\/) |" "\n"
        "| :\\/: || ()() || :\\/: || (__) || :\\/: || ()() || (__) || :\\/: |" "\n"
        "| '--'U|| '--'N|| '--'S|| '--'T|| '--'A|| '--'B|| '--'L|| '--'E|" "\n"
        "`------'`------'`------'`------'`------'`------'`------'`------'" "\n"
        ".------..------..------..------..------..------..------..------." "\n"
        "|S.--. ||T.--. ||U.--. ||D.--. ||E.--. ||N.--. ||T.--. ||S.--. |" "\n"
        "| :/\\: || :/\\: || (\\/) || :/\\: || (\\/) || :(): || :/\\: || :/\\: |" "\n"
        "| :\\/: || (__) || :\\/: || (__) || :\\/: || ()() || (__) || :\\/: |" "\n"
        "| '--'S|| '--'T|| '--'U|| '--'D|| '--'E|| '--'N|| '--'T|| '--'S|" "\n"
        "`------'`------'`------'`------'`------'`------'`------'`------'" "\n");

    printf("\n" BHMAG "CONGRATULAZIONI!" RESET);
}

/* Gestione delle stringhe
 * Omessi i break perché... Beh tanto ritorna subito...
 *
 * Non sono sicuro di quanto sia corretto questo approccio,
 * ma fare allocazione dinamica per delle stringhe mi sembra un tantino eccessivo.
 */

/** Modifica il parametro str con l'azione in entrata
 *
 * @param azione L'azione che dovrà essere cambiata in testo
 */
const char *strAzione(Azione azione) {
    switch (azione) {
        case GIOCA: return "GIOCA";
        case SCARTA: return "SCARTA";
        case ELIMINA: return "ELIMINA";
        case RUBA: return "RUBA";
        case PESCA: return "PESCA";
        case PRENDI: return "PRENDI";
        case BLOCCA: return "BLOCCA";
        case SCAMBIA: return "SCAMBIA";
        case MOSTRA: return "MOSTRA";
        case IMPEDIRE: return "IMPEDIRE";
        case INGEGNERE: return "INGEGNERE";
        default: return "";
    }
}

/** Modifica il parametro str con la stringa della topologia
 *
 * @param tipologia La topologia da trasformare in testo
 */
const char *strTipologiaCarta(TipologiaCarta tipologia) {
    switch (tipologia) {
        case ALL: return "ALL";
        case STUDENTE: return "STUDENTE";
        case MATRICOLA: return "MATRICOLA";
        case STUDENTE_SEMPLICE: return "STUDENTE SEMPLICE";
        case LAUREANDO: return "LAUREANDO";
        case BONUS: return "BONUS";
        case MALUS: return "MALUS";
        case MAGIA: return "MAGIA";
        case ISTANTANEA: return "ISTANTANEA";
        default: return "";
    }
}

/** Da target a stringa
 *
 * @param target Il target da trasformare in testo
 */
const char *strTargetGiocatori(TargetGiocatori target) {
    switch (target) {
        case IO: return "IO";
        case TU: return "TU";
        case VOI: return "VOI";
        case TUTTI: return "TUTTI";
        default: return "";
    }
}

/** Da enum Quando a stringa
 *
 * @param quando Il momento in cui viene eseguito l'effetto
 */
const char *strQuando(Quando quando) {
    switch (quando) {
        case SUBITO: return "SUBITO";
        case INIZIO: return "INIZIO";
        case FINE: return "FINE";
        case MAI: return "MAI";
        case SEMPRE: return "SEMPRE";
        default: return "";
    }
}

/** Funzione che restituisce una stringa che indica se la carta è o non è giocabile
 *
 * @param opzionale Bool che contiene se la carta può disessere giocata o no
 */
const char *strOpzionale(bool opzionale) {
    return opzionale ? "VERO" : "FALSO"; // Operatore ternario
}

/** Modifica una stringa mettendoci dentro i colori per la printf
 *
 * @param nGiocatore Il numero del giocatore a cui vi sarà dato un colore
 */
const char *strColoreGiocatore(int nGiocatore) {
    switch (nGiocatore) {
        case P1: return RED;
        case P2: return BLU;
        case P3: return GRN;
        case P4:
        case P0: // Questo sarebbe il resto dell'ultimo turno
            return YEL;
        default: return HWHT; // Viene usato solo in casi estremi, non dovrebbe mai comparire normalmente
    }
}

/** Pulisce lo schermo.
 * Usa le direttive di pre-processing per dedurre il comando da utilizzare in base all'OS.
 */
void pulisciSchermo() {
#ifdef _WIN32
    system("cls"); // Windows a 32 o 64 bit, indipendente dall'architettura (sapevate che c'è windows nt 4.0 per ppc?)
#elif __unix__
    system("clear"); // Unix (GNU/Linux, MacOS, etc...)
#endif
}
