#include "frogger.h"

void *
rana(void *arg)
{
    elementoGioco ranaGiocatore;
    info_elemento *info_rana = (info_elemento *)arg;
    ranaGiocatore.tipo = RANA;
    ranaGiocatore.x = info_rana->x;
    ranaGiocatore.y = info_rana->y;
    ranaGiocatore.thread_oggetto = pthread_self();
    ranaGiocatore.velocita = info_rana->speed;
    ranaGiocatore.direzione = info_rana->direzione;
    ranaGiocatore.proiettile = INVALID_THREAD;

    bool controllo_rana = true;
    ranaGiocatore.controllo = &controllo_rana;

    // Impostiamo nodelay per evitare che wgetch blocchi
    nodelay(gioco, TRUE);
    keypad(gioco, TRUE);

    int num_spari = 0; // contatore per la gestione della quantità di spari

    while (controllo && *ranaGiocatore.controllo)
    {
        int c;

        // Usiamo un mutex per proteggere l'accesso a wgetch()
        lock_mutex();
        c = wgetch(gioco);
        unlock_mutex();

        switch (c)
        {
        case KEY_UP:
            if (ranaGiocatore.y > miny + 5)
                ranaGiocatore.y -= SPOSTAMENTO_RANA;
            break;
        case KEY_DOWN:
            if (ranaGiocatore.y < maxy - 2)
                ranaGiocatore.y += SPOSTAMENTO_RANA;
            break;
        case KEY_LEFT:
            if (ranaGiocatore.x > minx)
                ranaGiocatore.x -= SPOSTAMENTO_RANA;
            break;
        case KEY_RIGHT:
            if (ranaGiocatore.x < maxx - 3)
                ranaGiocatore.x += SPOSTAMENTO_RANA;
            break;
        default:
            break;
        }

        wait_produttore();
        lista_elementi[in] = ranaGiocatore;
        in = (in + 1) % DIM_BUFFER;
        signal_produttore();

        // Evitiamo di usare la CPU al 100%
        usleep(50000);
    }
    return NULL;
}
