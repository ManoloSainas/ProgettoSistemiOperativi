
// ● Rilevamento collisione di una palla con il bordo schermo e rimbalzo su due assi

#include <unistd.h>
#include <ncurses.h>
#define UDELAY 40000

int main() {
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    int maxy = LINES, maxx = COLS, newx = 0, newy = 0, dirx = 1, diry = 1;
    int y = maxy/2, x = maxx/2;

    while(true) {
        getmaxyx(stdscr, maxy, maxx);  // Lettura dinamica dimensione finestra
        clear(); mvaddch(y, x, 'o'); refresh();
        usleep(UDELAY);
        
        newx = x + dirx; newy = y + diry;  
        if(newx >= maxx || newx < 0) dirx *= -1;
        x += dirx;
        if(newy >= maxy || newy < 0) diry *= -1;
        y += diry;
        if(getch() == (int)'q') break;
    }
    endwin();
}