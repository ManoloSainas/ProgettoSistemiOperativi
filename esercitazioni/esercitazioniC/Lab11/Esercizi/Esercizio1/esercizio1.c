
// ● Codificare in C con l’utilizzo delle ncurses un programma che si occupa di riempire la 
// schermata del terminale con un carattere prescelto, stampando un carattere la volta
// ○ Il programma deve essere implementato con un singolo processo
// ○ Le stampe devono essere intervallate da una pausa di 500 microsecondi
// ○ La posizione di stampa deve essere generata casualmente ad ogni intervallo, 
// scegliendo tra una di quelle all’interno dell’area di stampa
// ■ Non importa se la posizione generata è stata scelta in precedenza
// ○ La pressione di qualsiasi tasto da parte dell’utente deve interrompere il programma
// ■ Si consiglia di impostare il comportamento non-bloccante degli input

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main() {
    initscr(); // Inizializza la libreria ncurses e imposta lo schermo.
    cbreak(); // Disabilita il buffering di linea, rendendo i caratteri di input immediatamente disponibili al programma.
    noecho(); // Disabilita l'eco dei caratteri digitati sullo schermo.
    nodelay(stdscr, TRUE); // Rende getch() non bloccante, quindi restituisce ERR se non ci sono input disponibili.
    curs_set(0); // Nasconde il cursore.
    timeout(0); // Imposta getch() per essere non bloccante con un timeout di 0 millisecondi.
    srand(time(NULL)); // Inizializza il generatore di numeri casuali con l'ora corrente.
    while (1) { // Inizia un ciclo infinito.
        int x = rand() % COLS; // Genera una coordinata x casuale all'interno della larghezza dello schermo.
        int y = rand() % LINES; // Genera una coordinata y casuale all'interno dell'altezza dello schermo.
        mvaddch(y, x, '*'); // Sposta il cursore alla posizione generata e aggiunge il carattere '*'.
        refresh(); // Aggiorna lo schermo.
        usleep(500000); // Pausa di 500 millisecondi.
        if (getch() != ERR) // Se un tasto è stato premuto.
            break; // Interrompi il ciclo.
    }
    endwin(); // Termina la modalità ncurses.
    return 0; // Termina il programma.
}


