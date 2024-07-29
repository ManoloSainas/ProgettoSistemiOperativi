#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#define UDELAY 40000
#define MAXSPEED 1
#define SPRITE_WIDTH 5

int main() {
    // Inizializza la libreria ncurses, disabilita l'eco dei caratteri e nasconde il cursore
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    
    // Ottiene le dimensioni della finestra del terminale
    int maxy = LINES, maxx = COLS, newx = 0, newy = 0, dirx = 1, diry = 1;
    int y = maxy/2, x = maxx/2, speed = 1;
    srand(time(NULL));

    while(true) {
        // Aggiorna le dimensioni della finestra del terminale
        getmaxyx(stdscr, maxy, maxx);
        
        // Pulisce la finestra
        clear();
        
        // Disegna la sprite nella posizione corrente
        mvaddch(y, x, '<');
        mvaddch(y, x+1, '-');
        mvaddch(y, x+2, '|');
        mvaddch(y, x+3, '-');
        mvaddch(y, x+4, '>');
        
        // Aggiorna la finestra
        refresh();
        
        // Attende per un breve periodo
        usleep(UDELAY);
        
        // Calcola la nuova posizione della sprite
        newx = x + dirx; newy = y + diry;
        
        // Controlla se la sprite ha colpito i bordi orizzontali
        if(newx >= maxx - SPRITE_WIDTH || newx < 0) {
            dirx *= -1;  // Inverte la direzione orizzontale
            speed = (speed < MAXSPEED) ? speed + 1 : speed;  // Aumenta la velocità se non ha raggiunto il massimo
        }
        x += dirx * speed;
        
        // Controlla se la sprite ha colpito i bordi verticali
        if(newy >= maxy || newy < 0) {
            diry *= -1;  // Inverte la direzione verticale
            speed = (speed < MAXSPEED) ? speed + 1 : speed;  // Aumenta la velocità se non ha raggiunto il massimo
        }
        y += diry * speed;
        
        // Esce dal ciclo se viene premuto il tasto 'q'
        if(getch() == (int)'q') break;
    }
    
    // Termina la modalità ncurses
    endwin();
    return 0;
}

// come compilare il programma:
// $ gcc -o esercizio3 esercizio3.c -lncurses