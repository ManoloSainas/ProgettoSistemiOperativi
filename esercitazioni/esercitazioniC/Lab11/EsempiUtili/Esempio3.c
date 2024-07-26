// Creiamo e delimitiamo una nuova finestra

#include <ncurses.h>
int main() {
    WINDOW *w1;

    initscr();
    curs_set(0);
    noecho();

    // Nuova finestra: 10 linee, 20 colonne, offset verticale di 1 e orizzontale di 2
    w1 = newwin(9, 20, 1, 2);
    
    // Aggiunge il bordo
    box(w1, ACS_VLINE, ACS_HLINE);
    
    // Stampa al centro della finestra
    mvwaddstr(w1, 4, 3, "Premi un tasto");
    
    // Attesa bloccante con refresh annesso
    wgetch(w1);

    endwin();
}
