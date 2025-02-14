#include "frogger.h"
elementoGioco ranaGiocatore;
/*
prende in input, una pipe in scrittura, una in lettura e i flussi di corrente
*/
void *rana(corrente flussi[])
{

    signal(SIGUSR1, handler_rana); // gestione segnale per lo spostamento del coccodrillo con rana sopra

    keypad(gioco, TRUE);

    int num_spari = 0; // contatore per la gestione della quantità di spari

    // inizializzazione dell'elemento rana
    ranaGiocatore.tipo = RANA;
    ranaGiocatore.x = RANA_X;
    ranaGiocatore.y = RANA_Y;
    ranaGiocatore.thread_oggetto = pthread_self();
    ranaGiocatore.velocita = 0;

    pid_t dati_p; // variabile che viene riscritta nella pipe di lettura per la gestione delle collisioni delle granate

    while (1)
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
                // creaziobe processo proiettile sinistro
                switch (fork())
                {
                case -1:
                    perror("Errore nell'esecuzione della fork sparo sinistra.");
                    _exit(1);
                case 0:
                    // Processo proiettile
                    proiettile(pipeout, ranaGiocatore.y, ranaGiocatore.x, SPEED_GRANATE, DESTRA, 'r');
                    break;
                default:
                    if (num_spari < MAXGRANATE)
                    {
                        num_spari++;
                        // creazione processo proiettile destro
                        switch (fork())
                        {
                        case -1:
                            perror("Errore nell'esecuzione della fork sparo destra");
                            _exit(1);
                        case 0:
                            // Processo proiettile
                            proiettile(pipeout, ranaGiocatore.y, ranaGiocatore.x, SPEED_GRANATE, SINISTRA, 'r');
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                    break;
                }
            }
        }

        ranaGiocatore.direzione = flussi[16 - ranaGiocatore.y].direzione; // direzione del flusso
        ranaGiocatore.velocita = flussi[16 - ranaGiocatore.y].velocita;   // velocità del flusso

        ranaGiocatore.proiettile = num_spari; // sfruttiamo la sezione inuttilizzata per debugging e controlli aggiuntivi

        // Scrittura nella pipe delle informazioni della rana
        if (write(pipeout, &ranaGiocatore, sizeof(elementoGioco)) == -1)
        {
            perror("Errore nella scrittura sulla pipe rana->main");

            _exit(1);
        }
        // Lettura dalla pipe dei dati, serve per cancellare le granate (collisioni o fine manche)
        if (read(pipein, &dati_p, sizeof(pid_t)) > 0)
        {
            if (kill(dati_p, 0) == 0)
            {
                chiudiProcessi(dati_p);
                num_spari--;
            }
        }
    }

    _exit(1);
}

// movimento automatico della rana che si trova sopra un coccodrillo
void handler_rana(int sig)
{
    if (sig == SIGUSR1)
    {
        if (ranaGiocatore.direzione == DESTRA)
            ranaGiocatore.x++;
        if (ranaGiocatore.direzione == SINISTRA)
            ranaGiocatore.x--;
    }
}