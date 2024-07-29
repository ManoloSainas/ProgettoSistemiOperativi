// ● Codificare in C e con l’utilizzo delle ncurses un programma che:
// ○ Utilizzi un singolo processo
// ○ Mostri una pallina che si muove nelle due dimensioni della finestra del terminale
// ○ Nel momento in cui la pallina colpisce un bordo:
// ■ Rimbalza, senza uscire dalla schermata
// ■ Incrementa la propria velocità di un passo fisso, ma non oltre un massimo definito
// ■ Lo schermo lampeggia (funzione flash)
// ■ Il sistema emette un avviso sonoro (funzione beep)

#include <unistd.h>   // Per la funzione usleep
#include <ncurses.h>  // Per le funzioni ncurses
#include <stdlib.h>   // Per le funzioni srand e rand
#include <time.h>     // Per la funzione time

#define UDELAY 40000  // Ritardo in microsecondi
#define MAXSPEED 4    // Velocità massima della pallina

int main() {
    // Inizializzazione di ncurses
    initscr(); 
    noecho(); 
    curs_set(0); 
    nodelay(stdscr, true);

    // Inizializzazione delle variabili
    int maxy = LINES, maxx = COLS, newx = 0, newy = 0, dirx = 1, diry = 1;
    int y = maxy / 2, x = maxx / 2, speed = 1;
    srand(time(NULL));  // Inizializzazione del generatore di numeri casuali

    while (true) {
        // Lettura dinamica della dimensione della finestra
        getmaxyx(stdscr, maxy, maxx);
        
        // Pulizia dello schermo e disegno della pallina nella posizione corrente
        clear(); 
        mvaddch(y, x, 'o'); 
        refresh();
        
        // Ritardo per un breve periodo
        usleep(UDELAY);
        
        // Calcolo della nuova posizione
        newx = x + dirx; 
        newy = y + diry;  
        
        // Controllo della collisione con i bordi sinistro o destro
        if (newx >= maxx || newx < 0) {
            dirx *= -1;  // Inversione della direzione
            speed = (speed < MAXSPEED) ? speed + 1 : speed;  // Incremento della velocità
            flash();  // Lampeggio dello schermo
            beep();   // Emissione di un suono
        }
        x += dirx * speed;  // Aggiornamento della posizione x
        
        // Controllo della collisione con i bordi superiore o inferiore
        if (newy >= maxy || newy < 0) {
            diry *= -1;  // Inversione della direzione
            speed = (speed < MAXSPEED) ? speed + 1 : speed;  // Incremento della velocità
            flash();  // Lampeggio dello schermo
            beep();   // Emissione di un suono
        }
        y += diry * speed;  // Aggiornamento della posizione y
        
        // Uscita dal ciclo se viene premuto 'q'
        if (getch() == (int)'q') break;
    }
    
    // Fine della modalità ncurses
    endwin();
    return 0;
}

// Come compilare il programma:
// $ gcc -o esercizio1 esercizio1.c -lncurses