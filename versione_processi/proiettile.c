#include "frogger.h"

void proiettile(int pipeout, int y, int x, int velocita, DirezioneFlusso direzione, char tipo)
{
    elementoGioco proiettile;
    proiettile.direzione = direzione;
    proiettile.pid_oggetto = getpid();
    proiettile.y = y;

    switch (tipo)
    {
    case 'c':
        proiettile.tipo = PROIETTILE_COCCODRILLO;
        if (direzione == DESTRA)
        {
            proiettile.x = x + COLONNE_SPRITE_COCCODRILLO;
        }
        if (direzione == SINISTRA)
        {
            proiettile.x = x - 1;
        }
        proiettile.velocita = 500000 - (velocita * 250000);

        // Evita di far spawnare il proiettile dentro il coccodrillo
        if (proiettile.x >= x && proiettile.x < x + COLONNE_SPRITE_COCCODRILLO)
        {
            proiettile.x = (direzione == DESTRA) ? x + COLONNE_SPRITE_COCCODRILLO : x - COLONNE_SPRITE_COCCODRILLO;
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
        proiettile.velocita = velocita;
        break;

    default:
        break;
    }

    write(pipeout, &proiettile, sizeof(elementoGioco));

    while (1)
    {
        usleep(proiettile.velocita);
        switch (proiettile.direzione)
        {
        case DESTRA:
            proiettile.x++;
            break;
        case SINISTRA:
            proiettile.x--;
            break;
        default:
            break;
        }

        write(pipeout, &proiettile, sizeof(elementoGioco));
    }
    _exit(1);
}