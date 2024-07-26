// Se si ha il dubbio su quale sia il codice intero associato ad un tasto è sempre possibile 
// attivare i tasti funzione e premerli per acquisirli, stampando poi il valore restituito

#include <ncurses.h>
int main() {
    int kcode;
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, 1);
    mvprintw(1, 1, "Premi un tasto qualunque (q per uscire)");
    while(1) {
        kcode = getch();
        deleteln();
        mvprintw(1, 1, "Codice del tasto premuto (q per uscire): %d", kcode); 
        if(kcode == (int)'q') break;
    }
    endwin();
}
