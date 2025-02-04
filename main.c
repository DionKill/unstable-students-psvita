//Nome: Diego Pireddu (d.pireddu18@studenti.unica.it)
//Matricola: 60/61/66754
//Tipologia progetto: BASE

#include <time.h>

#include "strutture.h"
#include "gioco.h"
#include "file.h"
#include "debug.h" // Solo per testing personale, da rimuovere

// Versione non definitiva, in fase di sviluppo
int main(void) {
    srand(time(NULL));

    menu();

    Carta *mazzo = NULL;
    mazzo = leggiCarteDaFile(); // Qui il totale delle carte è 138

    Carta *mazzoMatricole = dividiMazzoMatricola(&mazzo); // Rimuove sei carte, qui ci sono 6 carte
    shuffleCarte(&mazzoMatricole); // Mischia
    stampaCarte(mazzoMatricole); // Stampa le 6 carte Matricola

    shuffleCarte(&mazzo); // Lo shuffle va fatto DOPO che i due mazzi son stati divisi, altrimenti non funziona!
    stampaCarte(mazzo); // Stampa le 132 carte rimaste dopo aver levato 6 matricole

    liberaMemoria(mazzo); // Da migliorare ovviamente, non è finito
    return 0;
}
