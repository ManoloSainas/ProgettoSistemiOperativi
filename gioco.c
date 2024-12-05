#include "frogger.h"

// Function to initialize a pipe
void inizializzazionePipe(int filedes[])
{
    if (pipe(filedes) == -1)
    {
        perror("Errore nella creazione della pipe");
        _exit(1);
    }
}

void avviaGioco()
{

    int filedes[2];
    int pid_gioco;

    inizializzazionePipe(filedes);

    pid_gioco = fork();
    switch (pid_gioco)
    {
    case -1:
        perror("Errore nella creazione del processo per la rana");
        _exit(1);
        break;
    case 0:
        close(filedes[LETTURA]);
        rana(filedes[SCRITTURA]);
        _exit(0);
        break;
    default:
        close(filedes[SCRITTURA]);
        if (wgetch(gioco) == 'x')
        {
            delwin(gioco);
            endwin();
            break;
        }
        break;
    }
}