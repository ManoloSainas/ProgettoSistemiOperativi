
// ● Spostamento di un carattere a intervalli regolari, con sola scelta della direzione

#include <ncurses.h>
#include <unistd.h>
#define UDELAY 250000
int main() {
    initscr(); noecho(); curs_set(0);
    cbreak(); nodelay(stdscr, true); keypad(stdscr, true);
    int y = LINES/2, x = COLS/2, dir = -1, c;
    while(true) {
        c = getch();
        if(258 <= c && c <= 261) dir = c % 258;  // Solo frecce direzionali
        switch(dir) {
            case 0: y+=1; break;  // dir == 0 -> c == 258 -> Su
            case 1: y-=1; break;  // dir == 1 -> c == 259 -> Su
            case 2: x-=2; break;  // dir == 2 -> c == 260 -> Su
            case 3: x+=2; break;  // dir == 3 -> c == 261 -> Su
        }
        clear(); mvaddch(y, x, '#'); refresh();
        usleep(UDELAY);
        if(y < 0 || y >= LINES || x < 0 || x >= COLS) break;
    }
    endwin();
}
