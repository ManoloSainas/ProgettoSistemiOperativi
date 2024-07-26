//  Acquisiamo un carattere e usiamolo per stampare una croce (X) a pieno schermo

#include <curses.h>
int main() {
    initscr();  // Inizializza ncurses e schermo
    curs_set(2);
    cbreak();
 
    mvaddstr(1, 1, "Inserisci un carattere a piacere:");
    char c = mvgetch(2,2);
    int size = ((LINES < COLS) ? LINES : COLS) - 2;
    mvprintw(3, 1, "Dimensione croce: %d\n Premi un tasto per stamparla ", size);
    getch();
 
    erase();
    for (int i=0; i<=size; i++) {
        mvaddch(i, i, c);
        mvaddch(i, size-i, c);
    }
    mvaddstr(LINES-1, 1, "Premi un tasto per terminare ");
    getch();
 
    endwin();   // Ripristina il normale utilizzo del terminale
}
