#include "frogger.h"

void proiettile(int pipeout, int y, int x, int velocita, DirezioneFlusso direzione, char tipo)
{
    elementoGioco proiettile;
    proiettile.direzione = direzione;
    proiettile.pid_oggetto = getpid();
    proiettile.y = y;
    proiettile.proiettile = getppid(); // PID del processo padre, serve ad identificare principalmente il coccodrillo
    proiettile.velocita = velocita;

    // In base al tipo di proiettile, determina il comportamento("c" -> proiettile coccodrillo, "r" -> granata rana)
    switch (tipo)
    {
    case 'c':
        proiettile.tipo = PROIETTILE_COCCODRILLO;

        if (direzione == DESTRA)
        {
            proiettile.x = x + COLONNE_SPRITE_COCCODRILLO - 1;
        }
        if (direzione == SINISTRA)
        {
            proiettile.x = x - 2;
        }

        break;

    case 'r':
        proiettile.tipo = GRANATA;
        if (direzione == DESTRA)
        {
            proiettile.x = x + 2;
        }
        if (direzione == SINISTRA)
        {
            proiettile.x = x - 1;
        }
        break;

    default:
        break;
    }

    // Invia il proiettile a controlloGioco
    write(pipeout, &proiettile, sizeof(elementoGioco));

    while (1)
    {
        usleep(proiettile.velocita); // Aggiorna la posizione del proiettile ogni tot millisecondi

        switch (proiettile.direzione)
        {
        case DESTRA:
            proiettile.x += SPOSTAMENTO_PROIETTILE;
            break;
        case SINISTRA:
            proiettile.x -= SPOSTAMENTO_PROIETTILE;
            break;
        default:
            break;
        }
        // Invia il proiettile a controlloGioco
        write(pipeout, &proiettile, sizeof(elementoGioco));
    }
    _exit(1);
}