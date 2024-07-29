// ● Codificare in C e con l’utilizzo delle ncurses un programma che:
// ○ Utilizzi un singolo processo
// ○ Mostri due palline che si muovono nelle due dimensioni della finestra del terminale
// ○ Nel momento in cui una pallina colpisce un bordo:
// ■ Rimbalza, senza uscire dalla schermata
// ○ Nel momento in cui le due palline si scontrano tra loro:
// ■ Rimbalzano e continuano a muoversi nella schermata
// ■ Lo schermo lampeggia (funzione flash)

#include <unistd.h>   // Per la funzione usleep
#include <ncurses.h>  // Per le funzioni ncurses
#include <stdlib.h>   // Per le funzioni srand e rand
#include <time.h>     // Per la funzione time

#define UDELAY 40000  // Ritardo in microsecondi
#define MAXSPEED 3    // Velocità massima delle palline

int main() {
    // Inizializzazione di ncurses
    initscr(); 
    noecho(); 
    curs_set(0); 
    nodelay(stdscr, true);

    // Inizializzazione delle variabili
    int maxy = LINES, maxx = COLS;
    int y1 = maxy / 2, x1 = maxx / 4, y2 = maxy / 2, x2 = 3 * maxx / 4;
    int dirx1 = 1, diry1 = 1, dirx2 = -1, diry2 = -1;
    int speed1 = 1, speed2 = 1;
    srand(time(NULL));  // Inizializzazione del generatore di numeri casuali

    while (true) {
        // Lettura dinamica della dimensione della finestra
        getmaxyx(stdscr, maxy, maxx);
        
        // Pulizia dello schermo e disegno delle palline nelle posizioni correnti
        clear();
        mvaddch(y1, x1, 'o');
        mvaddch(y2, x2, 'x');
        refresh();
        
        // Ritardo per un breve periodo
        usleep(UDELAY);

        // Aggiornamento della posizione della pallina 1
        int newx1 = x1 + dirx1 * speed1;
        int newy1 = y1 + diry1 * speed1;
        if (newx1 >= maxx || newx1 < 0) {
            dirx1 *= -1;  // Inversione della direzione
            speed1 = (speed1 < MAXSPEED) ? speed1 + 1 : speed1;  // Incremento della velocità
        }
        if (newy1 >= maxy || newy1 < 0) {
            diry1 *= -1;  // Inversione della direzione
            speed1 = (speed1 < MAXSPEED) ? speed1 + 1 : speed1;  // Incremento della velocità
        }
        x1 += dirx1 * speed1;
        y1 += diry1 * speed1;

        // Aggiornamento della posizione della pallina 2
        int newx2 = x2 + dirx2 * speed2;
        int newy2 = y2 + diry2 * speed2;
        if (newx2 >= maxx || newx2 < 0) {
            dirx2 *= -1;  // Inversione della direzione
            speed2 = (speed2 < MAXSPEED) ? speed2 + 1 : speed2;  // Incremento della velocità
        }
        if (newy2 >= maxy || newy2 < 0) {
            diry2 *= -1;  // Inversione della direzione
            speed2 = (speed2 < MAXSPEED) ? speed2 + 1 : speed2;  // Incremento della velocità
        }
        x2 += dirx2 * speed2;
        y2 += diry2 * speed2;

        // Controllo della collisione tra le due palline
        if (x1 == x2 && y1 == y2) {
            dirx1 *= -1;
            diry1 *= -1;
            dirx2 *= -1;
            diry2 *= -1;
            flash();  // Lampeggio dello schermo
        }

        // Uscita dal ciclo se viene premuto 'q'
        if (getch() == 'q') break;
    }

    // Fine della modalità ncurses
    endwin();
    return 0;
}

// Come compilare il programma:
// $ gcc -o esercizio2 esercizio2.c -lncurses