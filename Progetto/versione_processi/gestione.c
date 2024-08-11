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

void chiudiProcessi(oggetto *proiettileRana, oggetto *rana)
{
    int i;

    for (i = 0; i < NUM_PROIETTILI_RANA; i++)
    {
        if (proiettileRana[i].status == ATTIVO)
        {
            kill(proiettileRana[i].pid_oggetto, SIGKILL);
            proiettileRana[i].status = TERMINATO;
        }
    }

    if (rana->status == ATTIVO)
    {
        kill(rana->pid_oggetto, SIGKILL);
        rana->status = TERMINATO;
    }
}