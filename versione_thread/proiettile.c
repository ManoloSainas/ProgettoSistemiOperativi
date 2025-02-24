#include "frogger.h"

void *proiettile(void *info)
{
    info_elemento *letto = (info_elemento *)info;
    elementoGioco proiettile;
    bool controllo_proiettile = true;
    proiettile.controllo = &controllo_proiettile;
    proiettile.direzione = letto->direzione;
    proiettile.thread_oggetto = pthread_self();
    proiettile.y = letto->y;
    proiettile.velocita = letto->speed;
    int x = letto->x;
    double start_timer, fine_timer, durata_timer;
    durata_timer = proiettile.velocita / 1000000;
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

    start_timer = time(NULL);
    while (controllo && *proiettile.controllo)
    {
        fine_timer = time(NULL);
        if (difftime(fine_timer, start_timer) >= durata_timer)
        {

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
            wait_produttore();
            lista_elementi[in] = proiettile;
            in = (in + 1) % DIM_BUFFER;
            signal_produttore();
            start_timer = time(NULL);
        }
    }
    return NULL;
}