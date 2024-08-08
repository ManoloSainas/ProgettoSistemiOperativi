#include "main.h"

void rana(int pipe_fd) {
    initscr(); // Inizializza ncurses per il processo rana
    noecho();
    curs_set(FALSE);

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x); // Ottieni le dimensioni attuali della finestra terminale

    int x = (max_x - LARGHEZZA_RANA) / 2; // Posizione iniziale X (centro in basso)
    int y = max_y - ALTEZZA_RANA - 2;         // Posizione iniziale Y (in basso)
    int ch;

    // Invio immediato della posizione iniziale della rana
    write(pipe_fd, &x, sizeof(int));
    write(pipe_fd, &y, sizeof(int));

    while ((ch = getch()) != 'q') {
        if (is_term_resized(max_y, max_x)) {
            resize_term(0, 0);
            getmaxyx(stdscr, max_y, max_x);
        }

        // Aggiorna la posizione della rana basata sull'input dell'utente
        switch (ch) {
            case SU:
                if (y - SALTO_RANA_SU >= 0) {
                    y -= SALTO_RANA_SU;
                }
                break;
            case GIU:
                if (y + SALTO_RANA_GIU < max_y-2) {
                    y += SALTO_RANA_GIU;
                }
                break;
            case SINISTRA:
                if (x - SALTO_RANA_SINISTRA >= 6) {
                    x -= SALTO_RANA_SINISTRA;
                }
                break;
            case DESTRA:
                if (x + SALTO_RANA_DESTRA < max_x - LARGHEZZA_RANA -6) {
                    x += SALTO_RANA_DESTRA;
                }
                break;
            default:
                break;
        }

        // Scrivi le coordinate aggiornate sulla pipe
        write(pipe_fd, &x, sizeof(int));
        write(pipe_fd, &y, sizeof(int));
    }

    endwin();
}
