#include "frogger.h"
/*
prende in input, una pipe in scrittura, una in lettura e i flussi di corrente
*/
void *rana(void *arg)
{

    elementoGioco ranaGiocatore;

    bool controllo_rana = true;
    ranaGiocatore.controllo = &controllo_rana;

    keypad(gioco, TRUE);

    int num_spari = 0; // contatore per la gestione della quantità di spari

    // inizializzazione dell'elemento rana
    ranaGiocatore.tipo = RANA;
    ranaGiocatore.x = RANA_X;
    ranaGiocatore.y = RANA_Y;
    ranaGiocatore.thread_oggetto = pthread_self();
    ranaGiocatore.velocita = 0;
    while (controllo && *(ranaGiocatore.controllo))
    {
        // Gestione movimento rana
        switch (wgetch(gioco))
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
        case KEY_SPACE:

            if (num_spari <= MAXGRANATE - 2) // controllo per la quantità di spari
            {
                num_spari++;
                // creaziobe thread proiettile sinistro
                num_spari++;
                // creazione thread proiettile destro
            }
        }

        if (ranaGiocatore.y < maxy - 2 && ranaGiocatore.y > miny + 5)
        {
            ranaGiocatore.direzione = flussi[16 - ranaGiocatore.y].direzione; // direzione del flusso
            ranaGiocatore.velocita = flussi[16 - ranaGiocatore.y].velocita;   // velocità del flusso
        }

        ranaGiocatore.proiettile = num_spari; // sfruttiamo la sezione inuttilizzata per debugging e controlli aggiuntivi

        // Scrittura nella lista thread delle informazioni della rana
        wait_produttore();
        lista_elementi[in] = ranaGiocatore;
        in = (in + 1) % DIM_BUFFER;
        signal_produttore();
    }
    return NULL;
}