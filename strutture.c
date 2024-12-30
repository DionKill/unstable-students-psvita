//
// Created by diego on 28/12/24.
//

#include "strutture.h"

// Genera un menù fantastico: https://patorjk.com/software/taag/#p=display&f=Big%20Money-ne&t=Unstable%0AStudents
void menu () {
    printf ("\n"
    "-------------------------------------------------------------------------------"       "\n"
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
    "-------------------------------------------------------------------------------"       "\n"
    );
}

Giocatore  *allocaListaGiocatori (int nGiocatori) {
    Giocatore *lista = NULL;
    Giocatore *nodo = NULL;

    for (int i = 0; i < nGiocatori; i++) {
        nodo = (Giocatore *) malloc(sizeof (Giocatore));
        nodo->next = NULL;

        if (lista == NULL)
            lista = nodo;
        else lista->next = nodo;
    }
    return nodo;
}

Giocatore *inserisciGiocatoreInCoda (Giocatore *testa) {
    Giocatore *nuovoNodo = (Giocatore *) malloc(sizeof(Giocatore));
    Giocatore *temp = testa;

    if (nuovoNodo == NULL)
        exit(EXIT_FAILURE);

    while (testa->next != NULL)
        testa = testa->next;

    testa->next = nuovoNodo;

    return temp;
}