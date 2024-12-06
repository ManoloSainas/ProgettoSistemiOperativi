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
        controlloGioco(filedes[LETTURA]);
        break;
    }

    terminaGioco();
}

void terminaGioco()
{
    delwin(gioco); // Delete the window
    endwin();      // End ncurses
}

void controlloGioco(int pipein)
{
    elementoGioco valoreLetto;
    elementoGioco rana;
    do
    {
        if (read(pipein, &valoreLetto, sizeof(valoreLetto)) > 0)
        {
            elementoGioco oggettoLetto;
            switch (valoreLetto.tipo)
            {
            case RANA:
                oggettoLetto = rana;
            }
            cancellaSprite(oggettoLetto);

            switch (valoreLetto.tipo)
            {
            case RANA:
                rana = valoreLetto;
                break;
            }

            stampaSprite(rana);

            wrefresh(gioco);
        }

    } while (wgetch(gioco) != 'x');

    chiudiProcessi(&rana);
}