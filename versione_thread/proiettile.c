#include "frogger.h"

void *proiettile(void *info)
{

    info_elemento *letto=(info_elemento*)info;
    elementoGioco proiettile;
    proiettile.direzione = letto->direzione;
    proiettile.thread_oggetto = pthread_self();
    proiettile.y = letto->y;
    proiettile.velocita = letto->speed;
    int x=letto->x;
    // In base al tipo di proiettile, determina il comportamento("c" -> proiettile coccodrillo, "r" -> granata rana)
    switch (letto->tipo)
    {
    case 'c':
        proiettile.tipo = PROIETTILE_COCCODRILLO;

        if (proiettile.direzione == DESTRA)
        {
            proiettile.x = x + COLONNE_SPRITE_COCCODRILLO - 1;
        }
        if (proiettile.direzione == SINISTRA)
        {
            proiettile.x = x - 2;
        }

        break;

    case 'r':
        proiettile.tipo = GRANATA;
        if (proiettile.direzione == DESTRA)
        {
            proiettile.x = x + 2;
        }
        if (proiettile.direzione == SINISTRA)
        {
            proiettile.x = x - 1;
        }
        break;

    default:
        break;
    }


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
       //inserire nella lista thread
    }
    return(1);
}