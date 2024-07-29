//  ● Caratteri che compaiono sullo schermo in posizioni casuali


#include <unistd.h> 
#include <ncurses.h> 
#include <time.h>
#include <stdlib.h>
#define NCHS 4
#define DELAY 1
int main() {
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    srand(time(NULL));
    int y = 0, x = 0, i;
    char chs[NCHS] = {'#', '$', '%', '?'};
    while(true) { 
        clear();
        for(i = 0; i < NCHS; i++) {
            y = rand() % LINES; x = rand() % COLS;
            mvaddch(y, x, chs[i]); 
        }
        refresh(); sleep(DELAY);
        if(getch() == (int)'q') break;
    }
    endwin();
}