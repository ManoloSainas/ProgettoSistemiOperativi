#include <unistd.h>
#include <ncurses.h>
#define UDELAY 40000

int main() {
    // Inizializza la libreria ncurses, disabilita l'eco dei caratteri e nasconde il cursore
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    
    // Ottiene le dimensioni della finestra del terminale
    int maxy = LINES, maxx = COLS, newx = 0, dir = 1;
    int y = maxy/2, x = maxx/2;  // Posiziona la palla al centro dello schermo

    while(true) {
        // Aggiorna le dimensioni della finestra del terminale
        getmaxyx(stdscr, maxy, maxx);
        
        // Pulisce la finestra
        clear();
        
        // Disegna la palla nella posizione corrente
        mvaddch(y, x, 'o');
        
        // Aggiorna la finestra
        refresh();
        
        // Attende per un breve periodo
        usleep(UDELAY);
        
        // Calcola la nuova posizione della palla
        newx = x + dir;
        
        // Controlla se la palla ha colpito i bordi orizzontali
        if(newx >= maxx || newx < 0) dir *= -1;  // Inverte la direzione orizzontale
        
        // Aggiorna la posizione della palla
        x += dir;
        
        // Esce dal ciclo se viene premuto il tasto 'q'
        if(getch() == (int)'q') break;
    }
    
    // Termina la modalità ncurses
    endwin();
}