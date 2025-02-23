//
// Created by diego on 22/02/25.
//

#include "grafica.h"

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

/** Mostra il menù di scelta delle carte.
 *
 */
void scegliAzioneGui () {
    printf("Scegli la tua prossima azione: "
    "\t"
    "1. Gioca una carta"
    "\t"
    "2. Pesca una carta"
    "\t"
    "3. Salva ed Esci");
}

/** Pulisce lo schermo.
 * Usa le direttive di pre-processing per dedurre il comando da utilizzare.
 */
void pulisciSchermo () {
#ifdef _WIN32
    system("cls");
#elif __unix__
    system("clear");
#endif
}