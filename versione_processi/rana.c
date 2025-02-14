#include "frogger.h"
elementoGioco ranaGiocatore;

void rana(int pipeout, int pipein, corrente flussi[])
{
    fcntl(pipein, F_SETFL, O_NONBLOCK); // pipe in scrittura della rana non bloccante
    // gestione tempo sparo

    signal(SIGUSR1, handler_rana);

    int tempoTrascorso, tempoRicarica = 1; // Adatta la velocità di ricarica dei proiettili a seconda della dimensione dello schermo

    tempoTrascorso = tempoRicarica + 1; // si aggiunge uno perchè altrimenti il primo proiettile non verrebbe sparato

    bool primoSparo = false; // true se sono state sparate le granate
    keypad(gioco, TRUE);

    int num_spari = 0;
    ranaGiocatore.tipo = RANA;
    ranaGiocatore.x = RANA_X;
    ranaGiocatore.y = RANA_Y;
    ranaGiocatore.pid_oggetto = getpid();

    ranaGiocatore.velocita = 0;
    bool danno;
    clock_t start_m, stop_m, start_p, stop_p;
    long int x;
    double durata_f, durata_p;
    bool puoi_sparare = true;
    start_m = clock();
    start_p = clock();
    pid_t proiettilePid;
    posizione dati_p;

    while (1)
    {
        primoSparo = true;

        stop_p = clock();
        durata_p = (double)(stop_p - start_p) / CLOCKS_PER_SEC;

        if (durata_p > tempoRicarica && puoi_sparare)
        {

            puoi_sparare = false;
        }

        // Calcolo del ritardo basato sulla velocità
        int delay = 500000 - ranaGiocatore.velocita;
        if (delay < 0)
            delay = 0;

        int ch = wgetch(gioco);
        switch (ch)
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
        case KEY_SPACE:

            if (num_spari <= MAXGRANATE - 2)
            {
                num_spari++;
                switch (fork())
                {
                case -1:
                    mvwprintw(gioco, 3, maxx - 10, "spara sinistra fallito");

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
                        switch (fork())
                        {
                        case -1:
                            perror("Errore nell'esecuzione della fork sparo destra");
                            mvwprintw(gioco, 3, maxx - 10, "spara sinistra fallito");
                            _exit(1);
                        case 0:
                            // Processo proiettile
                            proiettile(pipeout, ranaGiocatore.y, ranaGiocatore.x, SPEED_GRANATE, SINISTRA, 'r');

                            break;
                        default:

                            start_p = clock();

                            break;
                        }
                        break;
                    }
                    break;
                }
            }
        }

        ranaGiocatore.direzione = flussi[16 - ranaGiocatore.y].direzione;
        ranaGiocatore.velocita = flussi[16 - ranaGiocatore.y].velocita;

        // if (rana.y < maxy - 2 && rana.y > miny + 6 && ch != KEY_UP && ch != KEY_DOWN && ch != KEY_SPACE)
        // {
        //     usleep(delay);
        //     if (rana.direzione == DESTRA)
        //     {
        //         rana.x++;
        //     }
        //     if (rana.direzione == SINISTRA)
        //     {
        //         rana.x--;
        //     }
        // }

        ranaGiocatore.proiettile = num_spari;
        // Scrittura nella pipe delle informazioni della rana
        if (write(pipeout, &ranaGiocatore, sizeof(elementoGioco)) == -1)
        {
            perror("Errore nella scrittura sulla pipe rana->main");
            _exit(1);
        }
        if (read(pipein, &dati_p, sizeof(posizione)) > 0)
        {
            if (kill(dati_p.pid, 0) == 0)
            {
                chiudiProcessi(dati_p.pid);
                num_spari--;
            }
        }
    }

    _exit(1);
}

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