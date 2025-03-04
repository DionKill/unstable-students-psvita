//Nome: Diego Pireddu (d.pireddu18@studenti.unica.it)
//Matricola: 60/61/66754
//Tipologia progetto: BASE

#include <time.h>
#include "strutture.h"
#include "gioco.h"

// Versione non definitiva, in fase di sviluppo

/** Funzione principale del programma.
 *
 * @param argc Numero di argomenti con cui viene chiamato il programma (minimo 1, perché c'è sempre il nome dell'eseg.)
 * @param argv La matrice che contiene i parametri passati come stringa.
 * @return Esce dal programma.
 */
int main(int argc, char **argv) {
    srand(time(NULL));

    guiSplashScreen();
    // Chiede di continuare dopo aver mostrato il menù
    premiInvioPerContinuare();
    pulisciSchermo();

    gioco();

    return 0;
}
