
// ● Codificare in C e con l’utilizzo delle ncurses un programma che:
// ○ Utilizzi un singolo processo
// ○ Mostri due palline che si muovono nelle due dimensioni della finestra del terminale
// ○ Nel momento in cui una pallina colpisce un bordo:
// ■ Rimbalza, senza uscire dalla schermata
// ○ Nel momento in cui le due palline si scontrano tra loro:
// ■ Rimbalzano e continuano a muoversi nella schermata
// ■ Lo schermo lampeggia (funzione flash)
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define UDELAY 40000
#define MAXSPEED 3

int main() {
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    int maxy = LINES, maxx = COLS;
    int y1 = maxy / 2, x1 = maxx / 4, y2 = maxy / 2, x2 = 3 * maxx / 4;
    int dirx1 = 1, diry1 = 1, dirx2 = -1, diry2 = -1;
    int speed1 = 1, speed2 = 1;
    srand(time(NULL));

    while (true) {
        getmaxyx(stdscr, maxy, maxx);  // Dynamic window size
        clear();
        mvaddch(y1, x1, 'o');
        mvaddch(y2, x2, 'x');
        refresh();
        usleep(UDELAY);

        // Update ball 1 position
        int newx1 = x1 + dirx1 * speed1;
        int newy1 = y1 + diry1 * speed1;
        if (newx1 >= maxx || newx1 < 0) {
            dirx1 *= -1;
            speed1 = (speed1 < MAXSPEED) ? speed1 + 1 : speed1;
        }
        if (newy1 >= maxy || newy1 < 0) {
            diry1 *= -1;
            speed1 = (speed1 < MAXSPEED) ? speed1 + 1 : speed1;
        }
        x1 += dirx1 * speed1;
        y1 += diry1 * speed1;

        // Update ball 2 position
        int newx2 = x2 + dirx2 * speed2;
        int newy2 = y2 + diry2 * speed2;
        if (newx2 >= maxx || newx2 < 0) {
            dirx2 *= -1;
            speed2 = (speed2 < MAXSPEED) ? speed2 + 1 : speed2;
        }
        if (newy2 >= maxy || newy2 < 0) {
            diry2 *= -1;
            speed2 = (speed2 < MAXSPEED) ? speed2 + 1 : speed2;
        }
        x2 += dirx2 * speed2;
        y2 += diry2 * speed2;

        // Check for collision between balls
        if (x1 == x2 && y1 == y2) {
            dirx1 *= -1;
            diry1 *= -1;
            dirx2 *= -1;
            diry2 *= -1;
            flash();
        }

        if (getch() == 'q') break;
    }
    endwin();
    return 0;
}
// come compilare il programma:
// $ gcc -o esercizio2 esercizio2.c -lncurses