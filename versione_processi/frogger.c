#include "frogger.h"

// Definizione delle variabili globali
int minx, miny;
int maxx, maxy;
WINDOW *gioco;

int main()
{
    srand(time(NULL));

    inizializzazioneSchermo();

    avviaGioco();

    return 0;
}
