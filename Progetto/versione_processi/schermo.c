#include "main.h"

void inizializzazioneSchermo()
{

    initscr();     // Inizializzazione dello schermo
    noecho();      // Eliminata visualizzazione tasti premuti
    curs_set(0);   // Nasconde il cursore
    start_color(); // Necessario per utilizzare i colori

    getmaxyx(stdscr, maxy, maxx); // Rileva la dimensione del terminale

    // Settaggio delle coordinate relative la grandezza dello schermo di gioco
    miny = maxy / 8;
    minx = maxx / 8;
    maxy = maxy - maxy / 4;
    maxx = maxx - maxx / 4;

    // Dichiarazione di un nuovo schermo relativo al gioco, i limiti dell'area di
    // gioco sono indicati grazie alla presenza di un contorno
    gioco = newwin(maxy, maxx, miny, minx);

    box(gioco, ACS_VLINE, ACS_HLINE); // Disegna il contorno

    // Settaggio delle coordinate dell'area di gioco per la generazione degli
    // oggetti e per le collisioni Vista la presenza del bordo l'area di gioco
    // viene ristretta di 1 carattere per evitare collisioni con esso
    miny = 1;
    minx = 1;
    maxy -= 2; // Diminuito di due perchè verranno scritte le informazioni
    maxx -= 1;

    // Inizializzazione dei colori
    init_pair(COLORE_STANDARD, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLORE_CANCELLAZIONE, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLORE_ROSSO, COLOR_RED, COLOR_BLACK);
    init_pair(COLORE_CIANO, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLORE_BLU, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLORE_VERDE, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLORE_GIALLO, COLOR_YELLOW, COLOR_BLACK);
}