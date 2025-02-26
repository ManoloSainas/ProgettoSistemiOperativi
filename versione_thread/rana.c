#include "frogger.h"

void *rana(void *arg)
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

    pthread_t granataSinistra, granataDestra;

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
            // case KEY_SPACE:

            //     if (num_spari < MAXGRANATE - 2)
            //     {
            //         info_elemento granataDestra_info, granataSinistra_info;
            //         granataDestra_info.x = ranaGiocatore.x;
            //         granataDestra_info.y = ranaGiocatore.y;
            //         granataDestra_info.speed = SPEED_PROIETTILI;
            //         granataDestra_info.tipo = 'r';
            //         granataDestra_info.direzione = DESTRA;

            //         granataSinistra_info.x = ranaGiocatore.x;
            //         granataSinistra_info.y = ranaGiocatore.y;
            //         granataSinistra_info.speed = SPEED_PROIETTILI;
            //         granataSinistra_info.tipo = 'r';
            //         granataSinistra_info.direzione = SINISTRA;

            //         if (pthread_create(&granataDestra, NULL, &proiettile, &granataDestra_info) == 0)
            //         {
            //             num_spari++;
            //         }
            //         else
            //         {
            //             perror("errore creazione thread");
            //         }

            //         if (pthread_create(&granataSinistra, NULL, &proiettile, &granataSinistra_info) == 0)
            //         {
            //             num_spari++;
            //         }
            //         else
            //         {
            //             perror("errore creazione thread");
            //         }
            //     }

            break;
        default:
            break;
        }

        wait_produttore();
        lista_elementi[in] = ranaGiocatore;
        in = (in + 1) % DIM_BUFFER;
        signal_produttore();

        usleep(50000);
    }
    return NULL;
}
