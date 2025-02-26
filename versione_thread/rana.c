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

    // Ensure controllo_rana is properly initialized
    bool controllo_rana = true;
    ranaGiocatore.controllo = &controllo_rana;

    keypad(gioco, TRUE);

    int num_spari = 0; // contatore per la gestione della quantità di spari

    while (controllo && *ranaGiocatore.controllo)
    {
        int c;

        // Lock the mutex before calling wgetch and modifying shared resources
        lock_mutex();
        c = wgetch(gioco);

        // Gestione movimento rana
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
            // Gestione sparo granate
            // case KEY_SPACE:
            //     if (num_spari <= MAXGRANATE - 2) // controllo per la quantità di spari
            //     {
            //         num_spari++;
            //         // creazione thread proiettile sinistro

            //         num_spari++;
            //         // creazione thread proiettile destro
            //         // pthread_t thread_proiettile_destro;
            //         // if (pthread_create(&thread_proiettile_destro, NULL, proiettile, &ranaGiocatore) != 0)
            //         // {
            //         //     perror("Errore creazione thread proiettile destro");
            //         // }
            //     }
            //     break;
        default:
            break;
        }

        // if (ranaGiocatore.y < maxy - 2 && ranaGiocatore.y > miny + 5)
        // {
        //     ranaGiocatore.direzione = flussi[16 - ranaGiocatore.y].direzione; // direzione del flusso
        //     ranaGiocatore.velocita = flussi[16 - ranaGiocatore.y].velocita;   // velocità del flusso
        // }

        // ranaGiocatore.proiettile = num_spari; // sfruttiamo la sezione inuttilizzata per debugging e controlli aggiuntivi

        // Scrittura nella lista thread delle informazioni della rana
        wait_produttore();
        lista_elementi[in] = ranaGiocatore;
        in = (in + 1) % DIM_BUFFER;
        signal_produttore();

        // Unlock the mutex after modifying shared resources
        unlock_mutex();
    }
    return NULL;
}