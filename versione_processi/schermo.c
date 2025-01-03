#include "frogger.h"

void inizializzazioneSchermo()
{
    initscr();     // inizializzazione schermo
    noecho();      // elimina visualizazzione tasti premuti
    curs_set(0);   // nasconde cursore
    start_color(); // inizializzazione colori

    // Settaggio delle coordinate relative la grandezza dello schermo di gioco
    miny = 1;
    minx = 1;

    gioco = newwin(maxy, maxx, miny, minx);
    nodelay(gioco, true);
    box(gioco, ACS_VLINE, ACS_HLINE); // Disegna il contorno

    // Pulizia dello schermo
    clear();
    refresh();

    // Inizializzazione dei colori
    init_pair(COLORE_STANDARD, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLORE_CANCELLAZIONE, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLORE_ROSSO, COLOR_RED, COLOR_BLACK);
    init_pair(COLORE_CIANO, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLORE_BLU, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLORE_VERDE, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLORE_GIALLO, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLORE_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);

    init_pair(COLORE_RANA_ARGINE, COLOR_RED, COLOR_MAGENTA);
    init_pair(COLORE_RANA_COCCODRILLO, COLOR_RED, COLOR_BLUE);
    init_pair(COLORE_RANA_TANA, COLOR_RED, COLOR_GREEN);

    init_pair(COLORE_COCCODRILLO, COLOR_GREEN, COLOR_BLUE);
    init_pair(COLORE_COCCODRILLO_SPARO, COLOR_RED, COLOR_BLUE);
    init_pair(SFONDO_MARCIAPIEDE, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(SFONDO_ACQUA, COLOR_BLACK, COLOR_BLUE);
    init_pair(SFONDO_ERBA, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(SFONDO_TANE, COLOR_BLACK, COLOR_YELLOW);
    init_pair(COLORE_TANE, COLOR_BLACK, COLOR_WHITE);
}
