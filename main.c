//Nome: Diego Pireddu (d.pireddu18@studenti.unica.it)
//Matricola: 60/61/66754
//Tipologia progetto: BASE

#include <time.h>

#include "strutture.h"
#include "gioco.h"
#include "file.h"
//#include "debug.h" // Solo per testing personale, da rimuovere

int main(void) {
    srand(time(NULL));

    menu();

    Carta *c = NULL;

    c = leggiCarteDaFile();
    //stampaCarte(c);
    c = shuffleCarte(c);
    //stampaCarte(c);

    liberaMemoria(c); // Da migliorare ovviamente, non è finito

    return 0;
}
