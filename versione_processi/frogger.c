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
        mvwaddstr(gioco, 1, 1, "----------xx----------xx----------xx----------xx----------xx----------"); // Per visualizzare la distanza tra le tane
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
