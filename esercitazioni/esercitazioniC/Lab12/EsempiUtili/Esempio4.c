//  ● Caratteri che compaiono sullo schermo in posizioni casuali

#include <unistd.h> 
#include <ncurses.h> 
#include <time.h>
#include <stdlib.h>
#define NCHS 4
#define DELAY 1

int main() {
    // Inizializza la libreria ncurses, disabilita l'eco dei caratteri e nasconde il cursore
    initscr(); noecho(); curs_set(0); nodelay(stdscr, true);
    
    // Inizializza il generatore di numeri casuali con il tempo corrente
    srand(time(NULL));
    
    int y = 0, x = 0, i;
    char chs[NCHS] = {'#', '$', '%', '?'};  // Array di caratteri da visualizzare
    
    while(true) { 
        // Pulisce lo schermo
        clear();
        
        // Genera posizioni casuali per ciascun carattere e li visualizza
        for(i = 0; i < NCHS; i++) {
            y = rand() % LINES;  // Genera una posizione verticale casuale
            x = rand() % COLS;   // Genera una posizione orizzontale casuale
            mvaddch(y, x, chs[i]);  // Visualizza il carattere alla posizione generata
        }
        
        // Aggiorna lo schermo
        refresh();
        
        // Attende per un breve periodo
        sleep(DELAY);
        
        // Esce dal ciclo se viene premuto il tasto 'q'
        if(getch() == (int)'q') break;
    }
    
    // Termina la modalità ncurses
    endwin();
}