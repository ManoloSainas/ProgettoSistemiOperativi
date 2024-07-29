
// ● Codificare in C e con l’utilizzo delle ncurses un programma che:
// ○ Utilizzi un singolo processo
// ○ Mostri un oggetto che si muove nelle due dimensioni della finestra del terminale
// ■ L’oggetto è definito dalla sprite di dimensione 1x5 seguente
// <-|->
// ○ Nel momento in cui l’oggetto colpisce un bordo:
// ■ Rimbalza, senza uscire dalla schermata
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#define UDELAY 40000
#define MAXSPEED 1
#define SPRITE_WIDTH 5

int main() {
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    int maxy = LINES, maxx = COLS, newx = 0, newy = 0, dirx = 1, diry = 1;
    int y = maxy/2, x = maxx/2, speed = 1;
    srand(time(NULL));

    while(true) {
        getmaxyx(stdscr, maxy, maxx);  // Lettura dinamica dimensione finestra
        clear(); 
        mvaddch(y, x, '<');
        mvaddch(y, x+1, '-');
        mvaddch(y, x+2, '|');
        mvaddch(y, x+3, '-');
        mvaddch(y, x+4, '>');
        refresh();
        usleep(UDELAY);
        
        newx = x + dirx; newy = y + diry;  
        if(newx >= maxx - SPRITE_WIDTH || newx < 0) {
            dirx *= -1;
            speed = (speed < MAXSPEED) ? speed + 1 : speed;
        }
        x += dirx * speed;
        if(newy >= maxy || newy < 0) {
            diry *= -1;
            speed = (speed < MAXSPEED) ? speed + 1 : speed;
        }
        y += diry * speed;
        if(getch() == (int)'q') break;
    }
    endwin();
    return 0;
}

// come compilare il programma:
// $ gcc -o esercizio3 esercizio3.c -lncurses

// come compilare il programma:
// $ gcc -o esercizio3 esercizio3.c -lncurses