
// ● Codificare in C e con l’utilizzo delle ncurses un programma che:
// ○ Utilizzi un singolo processo
// ○ Mostri una pallina che si muove nelle due dimensioni della finestra del terminale
// ○ Nel momento in cui la pallina colpisce un bordo:
// ■ Rimbalza, senza uscire dalla schermata
// ■ Incrementa la propria velocità di un passo fisso, ma non oltre un massimo definito
// ■ Lo schermo lampeggia (funzione flash)
// ■ Il sistema emette un avviso sonoro (funzione beep)

#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#define UDELAY 40000
#define MAXSPEED 4

int main() {
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    int maxy = LINES, maxx = COLS, newx = 0, newy = 0, dirx = 1, diry = 1;
    int y = maxy/2, x = maxx/2, speed = 1;
    srand(time(NULL));

    while(true) {
        getmaxyx(stdscr, maxy, maxx);  // Lettura dinamica dimensione finestra
        clear(); mvaddch(y, x, 'o'); refresh();
        usleep(UDELAY);
        
        newx = x + dirx; newy = y + diry;  
        if(newx >= maxx || newx < 0) {
            dirx *= -1;
            speed = (speed < MAXSPEED) ? speed + 1 : speed;
            flash(); beep();
        }
        x += dirx * speed;
        if(newy >= maxy || newy < 0) {
            diry *= -1;
            speed = (speed < MAXSPEED) ? speed + 1 : speed;
            flash(); beep();
        }
        y += diry * speed;
        if(getch() == (int)'q') break;
    }
    endwin();
}

// come compilare il programma:
// $ gcc -o esercizio1 esercizio1.c -lncurses
