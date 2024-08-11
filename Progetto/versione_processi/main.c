#include "main.h"

// Definizione delle variabili globali
int minx, miny;
int maxx, maxy;
WINDOW *gioco;

int main()
{

    inizializzazioneSchermo();

    avviaGioco();

    return 0;
}