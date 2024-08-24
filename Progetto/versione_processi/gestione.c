#include "main.h"

/**
 * Procedura che azzera il campo status di uno oggetto in seguito
 * alla sua inattività oppure per la sua inizializzazione a inizio partita
 */
void resettaOggetto(oggetto *oggetto)
{
    oggetto->status = TERMINATO;
}

void initOggetto(oggetto *oggetto)
{
    oggetto->status = NON_ATTIVO;
}

void chiudiProcessi(oggetto *proiettiliRana, oggetto *rana, oggetto *piante, oggetto *proiettiliPianta, oggetto *coccodrilli)
{
    int i;

    // Check if the pointers are not null
    if (proiettiliRana == NULL || rana == NULL || piante == NULL || proiettiliPianta == NULL || coccodrilli == NULL)
    {
        fprintf(stderr, "Errore: puntatore nullo passato a chiudiProcessi\n");
        return;
    }

    // Terminate active projectile processes
    for (i = 0; i < NUM_PROIETTILI_RANA; i++)
    {
        if (proiettiliRana[i].status == ATTIVO)
        {
            if (kill(proiettiliRana[i].pid_oggetto, SIGKILL) == -1)
            {
                perror("Errore nel terminare il processo del proiettile");
            }
            proiettiliRana[i].status = TERMINATO;
        }
    }

    // Terminate the frog process if active
    if (rana->status == ATTIVO)
    {
        if (kill(rana->pid_oggetto, SIGKILL) == -1)
        {
            perror("Errore nel terminare il processo della rana");
        }
        rana->status = TERMINATO;
    }

    // Terminate active plant processes
    for (i = 0; i < NUM_PIANTE; i++)
    {
        if (piante[i].status == ATTIVO)
        {
            if (kill(piante[i].pid_oggetto, SIGKILL) == -1)
            {
                perror("Errore nel terminare il processo della pianta");
            }
            piante[i].status = TERMINATO;
        }
    }

    // Terminate actove bullets plants processes
    for (i = 0; i < NUM_PIANTE; i++)
    {
        if (proiettiliPianta[i].status == ATTIVO)
        {
            if (kill(piante[i].pid_oggetto, SIGKILL) == -1)
            {
                perror("Errore nel terminare il processo del proiettile della pianta");
            }
            proiettiliPianta[i].status = TERMINATO;
        }
    }

    // Terminate active crocodile processes
    for (i = 0; i < NUM_TOT_COCCODRILLI; i++)
    {
        if (coccodrilli[i].status == ATTIVO)
        {
            if (kill(coccodrilli[i].pid_oggetto, SIGKILL) == -1)
            {
                perror("Errore nel terminare il processo del coccodrillo");
            }
            coccodrilli[i].status = TERMINATO;
        }
    }
}