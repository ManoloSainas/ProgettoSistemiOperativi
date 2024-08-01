// regole di gioco:
// ● Scrivere in C con l’utilizzo delle ncurses un programma multiprocesso che 
// implementi il gioco “guardia e ladro”
// ● Il gioco si compone di due oggetti sullo schermo:
// ○ La guardia (carattere #), controllata dall’utente tramite le frecce direzionali, inizia la 
// partita dall’angolo inferiore destro dello schermo
// ○ Il ladro (carattere $), controllato dalla logica di gioco e che si muove in maniera 
// casuale, inizia la partita dall’angolo superiore sinistro dello schermo
// ● Il gioco termina quando la guardia “cattura” il ladro
// ○ Ovvero quando il carattere # controllato dall’utente si sovrappone al carattere $

// dettagli implementativi:
// ● Il programma richiesto deve far uso di 3 processi distinti:
// ○ Un primo processo figlio acquisisce l’input utente e gestisce la posizione della 
// guardia, comunicandola al padre ogni volta che la aggiorna
// ○ Un secondo processo figlio gestisce la posizione del ladro, generando uno 
// spostamento casuale e comunicando la posizione aggiornata al padre
// ○ Il processo padre riceve le comunicazioni sulle posizioni degli oggetti di gioco, 
// stampa gli oggetti nelle posizioni aggiornate e controlla la terminazione
// ● Definire delle funzioni dedicate per la logica del padre e dei processi figlio
// ● Utilizzare le pipe per le comunicazioni tra processi padre/figlio
// ● Gli eventi di gioco devono essere adeguatamente temporizzati

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define GUARD '#'
#define THIEF '$'

void guard_process(int write_pipe);
void thief_process(int write_pipe);
void parent_process(int read_pipe_guard, int read_pipe_thief);

int main() {
    int pipe_guard[2], pipe_thief[2];
    pid_t pid_guard, pid_thief;

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(30); // Reduce timeout to 30ms for more responsive input
    curs_set(0); // Hide the cursor

    // Create pipes
    pipe(pipe_guard);
    pipe(pipe_thief);

    // Fork guard process
    if ((pid_guard = fork()) == 0) {
        close(pipe_guard[0]);
        guard_process(pipe_guard[1]);
        exit(0);
    }

    // Fork thief process
    if ((pid_thief = fork()) == 0) {
        close(pipe_thief[0]);
        thief_process(pipe_thief[1]);
        exit(0);
    }

    // Parent process
    close(pipe_guard[1]);
    close(pipe_thief[1]);
    parent_process(pipe_guard[0], pipe_thief[0]);

    // Wait for child processes to finish
    wait(NULL);
    wait(NULL);

    // End ncurses
    curs_set(1); // Show the cursor again
    endwin();
    return 0;
}

void guard_process(int write_pipe) {
    int x = COLS - 1, y = LINES - 1;
    char buffer[2];

    while (1) {
        int ch = getch();
        switch (ch) {
            case KEY_UP:    y = (y > 0) ? y - 1 : y; break;
            case KEY_DOWN:  y = (y < LINES - 1) ? y + 1 : y; break;
            case KEY_LEFT:  x = (x > 0) ? x - 1 : x; break;
            case KEY_RIGHT: x = (x < COLS - 1) ? x + 1 : x; break;
        }
        buffer[0] = x;
        buffer[1] = y;
        write(write_pipe, buffer, 2);
    }
}

void thief_process(int write_pipe) {
    int x = 0, y = 0;
    char buffer[2];
    srand(time(NULL));

    while (1) {
        int direction = rand() % 4;
        switch (direction) {
            case 0: y = (y > 0) ? y - 1 : y; break;
            case 1: y = (y < LINES - 1) ? y + 1 : y; break;
            case 2: x = (x > 0) ? x - 1 : x; break;
            case 3: x = (x < COLS - 1) ? x + 1 : x; break;
        }
        buffer[0] = x;
        buffer[1] = y;
        write(write_pipe, buffer, 2);
        usleep(200000); // Sleep for 200ms to slow down the thief
    }
}

void parent_process(int read_pipe_guard, int read_pipe_thief) {
    int guard_x = COLS - 1, guard_y = LINES - 1;
    int thief_x = 0, thief_y = 0;
    char buffer[2];

    while (1) {
        // Read guard position
        if (read(read_pipe_guard, buffer, 2) > 0) {
            guard_x = buffer[0];
            guard_y = buffer[1];
        }

        // Read thief position
        if (read(read_pipe_thief, buffer, 2) > 0) {
            thief_x = buffer[0];
            thief_y = buffer[1];
        }

        // Clear screen and draw objects
        clear();
        mvaddch(guard_y, guard_x, GUARD);
        mvaddch(thief_y, thief_x, THIEF);
        refresh();

        // Check for game termination
        if (guard_x == thief_x && guard_y == thief_y) {
            mvprintw(LINES / 2, COLS / 2 - 5, "You caught the thief!");
            refresh();
            sleep(2);
            break;
        }
    }
}