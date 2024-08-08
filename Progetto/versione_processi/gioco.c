#include "main.h"

void gioco(int pipe_fd) {
    int x, y;
    int vite = NUMERO_VITE;
    int tempo = TEMPO_GIOCO;

    initscr(); // Inizializza ncurses per il processo di gioco
    noecho();
    curs_set(FALSE);

    inizializza_colori();
    
    // Leggi le coordinate iniziali dalla pipe
    if (read(pipe_fd, &x, sizeof(int)) <= 0) return;
    if (read(pipe_fd, &y, sizeof(int)) <= 0) return;

    


    // Disegna la barra inferiore con le vite e il tempo
    disegna_barra_inferiore(vite, tempo);

    // Disegna il marciapiede di partenza
    disegna_marciapiede();


    // Disegna la rana nella posizione iniziale
    disegna_rana(x, y);
    refresh();

    while (1) {
        // Aggiorna il tempo
        tempo -= 1;

        // Pulisci lo schermo
        clear();

        // Leggi le coordinate aggiornate dalla pipe
        if (read(pipe_fd, &x, sizeof(int)) <= 0) break;
        if (read(pipe_fd, &y, sizeof(int)) <= 0) break;

       

       
        // Aggiorna la barra inferiore con le vite e il tempo
        disegna_barra_inferiore(vite, tempo);

        disegna_marciapiede();

         // Disegna la rana alle coordinate ricevute
        disegna_rana(x, y);


        // Aggiorna lo schermo
        refresh();

     
    }

    endwin(); // Chiudi correttamente ncurses
}