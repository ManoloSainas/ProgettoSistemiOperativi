#include "main.h"

// Definizione delle variabili globali
int minx, miny;
int maxx, maxy;
WINDOW *gioco;

// Inizializza un'array di posizioni delle piante inizializzato a -1 per tutte e tre le piante
int posizionePianteX[NUM_PIANTE] = {-1, -1, -1};

int main()
{

    inizializzazioneSchermo();

    avviaGioco();

    return 0;
}