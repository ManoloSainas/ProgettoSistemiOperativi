#include "frogger.h"

int inizializzazioneSchermo()
{
    initscr();     // inizializzazione schermo
    noecho();      // elimina visualizazzione tasti premuti
    curs_set(0);   // nasconde cursore
    start_color(); // inizializzazione colori

    getmaxyx(stdscr, maxy, maxx); // Rileva la dimensione del terminale

    // Settaggio delle coordinate relative la grandezza dello schermo di gioco
    miny = 5;
    minx = 10;
    maxy = 18;
    maxx = 72;

    gioco = newwin(maxy, maxx, miny, minx);

    box(gioco, ACS_VLINE, ACS_HLINE); // Disegna il contorno
    return 0;
}