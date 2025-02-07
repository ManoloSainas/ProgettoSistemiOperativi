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

void chiudiProcessi(pid_t pid)
{

    // Verifica che il PID sia valido
    if (pid != INVALID_PID)
    {

        if (pid <= 0)
        {
            perror("Errore: PID non valido o processo inesistente");
            return;
        }

        // Invia il segnale SIGKILL per terminare il processo
        if (kill(pid, SIGKILL) == -1)
        {
            perror("Errore nell'inviare il segnale SIGKILL al processo");
            return;
        }

        // Aspetta la terminazione del processo
        if (waitpid(pid, NULL, 0) == -1)
        {
            perror("Errore nell'attendere la terminazione del processo");
        }
    }
}
