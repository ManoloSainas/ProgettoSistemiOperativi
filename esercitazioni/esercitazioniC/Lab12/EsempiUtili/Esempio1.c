
// ● Spostamento di un carattere a intervalli regolari, con sola scelta della direzione


#include <ncurses.h>
#include <unistd.h>
#define UDELAY 250000

int main() {
    // Inizializza la libreria ncurses, disabilita l'eco dei caratteri e nasconde il cursore
    initscr(); noecho(); curs_set(0);
    
    // Abilita la modalità cbreak, disabilita il blocco dell'input e abilita l'uso delle frecce direzionali
    cbreak(); nodelay(stdscr, true); keypad(stdscr, true);
    
    // Imposta la posizione iniziale del carattere al centro dello schermo
    int y = LINES/2, x = COLS/2, dir = -1, c;
    
    while(true) {
        // Legge l'input dell'utente
        c = getch();
        
        // Se l'input è una freccia direzionale, aggiorna la direzione
        if(258 <= c && c <= 261) dir = c % 258;  // Solo frecce direzionali
        
        // Aggiorna la posizione del carattere in base alla direzione
        switch(dir) {
            case 0: y+=1; break;  // dir == 0 -> c == 258 -> Giù
            case 1: y-=1; break;  // dir == 1 -> c == 259 -> Su
            case 2: x-=2; break;  // dir == 2 -> c == 260 -> Sinistra
            case 3: x+=2; break;  // dir == 3 -> c == 261 -> Destra
        }
        
        // Pulisce lo schermo e disegna il carattere nella nuova posizione
        clear(); mvaddch(y, x, '#'); refresh();
        
        // Attende per un breve periodo
        usleep(UDELAY);
        
        // Termina il ciclo se il carattere esce dai bordi dello schermo
        if(y < 0 || y >= LINES || x < 0 || x >= COLS) break;
    }
    
    // Termina la modalità ncurses
    endwin();
}