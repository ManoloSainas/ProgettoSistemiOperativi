#include <unistd.h>
#include <ncurses.h>
#define UDELAY 40000

int main() {
    // Inizializza la libreria ncurses, disabilita l'eco dei caratteri e nasconde il cursore
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    
    // Ottiene le dimensioni della finestra del terminale
    int maxy = LINES, maxx = COLS, newx = 0, newy = 0, dirx = 1, diry = 1;
    int y = maxy/2, x = maxx/2;  // Posiziona la palla al centro dello schermo

    while(true) {
        // Aggiorna le dimensioni della finestra del terminale
        getmaxyx(stdscr, maxy, maxx);  // Lettura dinamica dimensione finestra
        
        // Pulisce la finestra e disegna la palla nella posizione corrente
        clear(); mvaddch(y, x, 'o'); refresh();
        
        // Attende per un breve periodo
        usleep(UDELAY);
        
        // Calcola la nuova posizione della palla
        newx = x + dirx; newy = y + diry;
        
        // Controlla se la palla ha colpito i bordi orizzontali
        if(newx >= maxx || newx < 0) dirx *= -1;  // Inverte la direzione orizzontale
        x += dirx;
        
        // Controlla se la palla ha colpito i bordi verticali
        if(newy >= maxy || newy < 0) diry *= -1;  // Inverte la direzione verticale
        y += diry;
        
        // Esce dal ciclo se viene premuto il tasto 'q'
        if(getch() == (int)'q') break;
    }
    
    // Termina la modalità ncurses
    endwin();
}