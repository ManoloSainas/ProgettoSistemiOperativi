#include "frogger.h"

// Definizione delle variabili globali
int minx, miny;
int maxx, maxy;
WINDOW *gioco;

int main()
{

    inizializzazioneSchermo();

    while (true)
    {
        mvwaddstr(gioco, 1, 1, "FROGGER 2 LA VENDETTa");
        wrefresh(gioco);

        if (wgetch(gioco) == 'x')
        {
            delwin(gioco);
            endwin();
            break;
        }
    }

    return 0;
}
