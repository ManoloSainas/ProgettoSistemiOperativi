
// ● Rilevamento collisione di una palla con il bordo schermo (scalabile dinamicamente)

#include <unistd.h>
#include <ncurses.h>
#define UDELAY 40000

int main() {
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    int maxy = LINES, maxx = COLS, newx = 0, dir = 1;
    int y = maxy/2, x = maxx/2;

    while(true) {
        getmaxyx(stdscr, maxy, maxx);  // Lettura dinamica dimensione finestra
        clear();
        mvaddch(y, x, 'o');
        refresh();
        usleep(UDELAY);
        newx = x + dir;  
        if(newx >= maxx || newx < 0) dir *= -1;
        x += dir;
        if(getch() == (int)'q') break;
    }
    endwin();
}