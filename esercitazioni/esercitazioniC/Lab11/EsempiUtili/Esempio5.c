// Usiamo i tasti freccia per muovere un oggetto sullo schermo

#include <stdlib.h>
#include <curses.h>
#define OBJECT '#'
int main() {
    initscr(); noecho(); curs_set(0); cbreak();
    keypad(stdscr, 1);
    box(stdscr, ACS_VLINE, ACS_HLINE);
    int x = COLS/2, y = LINES/2, c;  // Inizio al centro
    mvaddch(y, x, OBJECT); refresh();
    while(1) {
        c = (int)getch();
        mvaddch(y, x, ' ');
        switch(c) {
            case KEY_UP:    if(y > 1)         y -= 1; break;
            case KEY_DOWN:  if(y < LINES - 2) y += 1; break;
            case KEY_LEFT:  if(x > 1)         x -= 1; break;
            case KEY_RIGHT: if(x < COLS - 2)  x += 1; break;
            case (int)'q':  endwin(); exit(0);
        }
        mvaddch(y, x, OBJECT); refresh();
    }
}
