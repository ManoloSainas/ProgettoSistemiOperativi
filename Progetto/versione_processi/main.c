#include "main.h"

int main() {
    initscr(); // Inizializza ncurses per il processo di gioco
    noecho();
    curs_set(FALSE);

    // file descriptor per la pipe gioco
    int gioco_fds[2];
    // pid della rana
    pid_t pid_rana;

    if (pipe(gioco_fds) == -1) {
        perror("Errore nella creazione della pipe!");
        exit(1);
    }

    avvia_gioco();
    inizializza_colori();

    // creazione del processo rana
    pid_rana = fork();

    if (pid_rana < 0) {
        perror("Errore nella creazione del processo rana!");
        exit(2);
    } else if (pid_rana == 0) {
        // processo rana
        close(gioco_fds[0]); // chiude il lato di lettura della pipe
        rana(gioco_fds[1]);
    } else {
        // processo padre
        close(gioco_fds[1]); // chiude il lato di scrittura della pipe
        gioco(gioco_fds[0]);
        wait(NULL);
        kill(pid_rana, SIGTERM); // Invia un segnale di terminazione controllato
    }

    endwin();
    _exit(0);
}