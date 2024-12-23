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

void chiudiProcessi(elementoGioco *elemento)
{
    if (elemento == NULL)
    {
        fprintf(stderr, "Errore: puntatore nullo passato a chiudiProcessi\n");
        return;
    }
    switch (elemento->tipo)
    {
    case RANA:
        if (kill(elemento->pid_oggetto, SIGKILL) == -1)
        {
            perror("Errore nel terminare il processo della rana");
        }
        waitpid(elemento->pid_oggetto, NULL, 0);
        break;
    case COCCODRILLO:
        if (kill(elemento->pid_oggetto, SIGKILL) == -1)
        {
            perror("Errore nel terminare il processo coccodrillo");
        }
        waitpid(elemento->pid_oggetto, NULL, 0);
        break;
    case GRANATA_DESTRA_RANA:
        if (kill(elemento->pid_oggetto, SIGKILL) == -1)
        {
            perror("Errore nel terminare il processo granata destra");
        }
        waitpid(elemento->pid_oggetto, NULL, 0);
        break;
    case GRANATA_SINISTRA_RANA:
        if (kill(elemento->pid_oggetto, SIGKILL) == -1)
        {
            perror("Errore nel terminare il processo granata sinistra");
        }
        waitpid(elemento->pid_oggetto, NULL, 0);
        break;
    default:
        break;
    }
}