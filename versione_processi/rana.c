#include "frogger.h"

void rana(int pipeout, int pipein, corrente flussi[])
{
    fcntl(pipein, F_SETFL, O_NONBLOCK); // pipe in scrittura della rana non bloccante
    // gestione tempo sparo

    int tempoTrascorso, tempoRicarica = 1; // Adatta la velocità di ricarica dei proiettili a seconda della dimensione dello schermo

    tempoTrascorso = tempoRicarica + 1; // si aggiunge uno perchè altrimenti il primo proiettile non verrebbe sparato

    bool primoSparo = false; // true se sono state sparate le granate
    keypad(gioco, TRUE);

    elementoGioco rana;
    int num_spari = 0;
    rana.tipo = RANA;
    rana.x = 36;
    rana.y = 16;
    rana.pid_oggetto = getpid();

    rana.velocita = 0;
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
            beep();
            puoi_sparare = false;
        }

        int ch = wgetch(gioco);
        switch (ch)
        {
        case KEY_UP:
            if (rana.y > miny + 5)
                rana.y -= SPOSTAMENTO_RANA;
            break;
        case KEY_DOWN:
            if (rana.y < maxy - 2)
                rana.y += SPOSTAMENTO_RANA;
            break;
        case KEY_LEFT:
            if (rana.x > minx)
                rana.x -= SPOSTAMENTO_RANA;
            break;
        case KEY_RIGHT:
            if (rana.x < maxx - 3)
                rana.x += SPOSTAMENTO_RANA;
            break;
        case KEY_SPACE:

            if (num_spari < MAXGRANATE)
            {
                num_spari++;
                switch (fork())
                {
                case -1:
                    mvwprintw(gioco, 3, maxx - 10, "spara sinistra fallito");

                    perror("Errore nell'esecuzione della fork.");

                    _exit(1);
                case 0:
                    // Processo proiettile
                    // mvwprintw(gioco, 5, 3, "spara destra");
                    // wrefresh(gioco);
                    proiettile(pipeout, rana.y, rana.x, 100000, DESTRA, 'r');

                    break;
                default:
                    if (num_spari < MAXGRANATE)
                    {
                        num_spari++;
                        switch (fork())
                        {
                        case -1:
                            perror("Errore nell'esecuzione della fork.");
                            mvwprintw(gioco, 3, maxx - 10, "spara sinistra fallito");
                            _exit(1);
                        case 0:
                            // Processo proiettile
                            // mvwprintw(gioco, 3, maxx - 10, "spara sinistra");
                            // wrefresh(gioco);
                            proiettile(pipeout, rana.y, rana.x, 100000, SINISTRA, 'r');

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

        // // LA RANA CADE DAL COCCODRILLO PER COLPA DELL' IF CHE C'È QUI SOTTO
        // durata_f = (double)(stop_m - start_m) / CLOCKS_PER_SEC;
        // stop_m = clock(); // stop timer movimento
        rana.direzione = flussi[16 - rana.y].direzione;
        rana.velocita = flussi[16 - rana.y].velocita;
        // if (durata_f * 100000 >= (double)(500000 - rana.velocita))
        // {

        // Calcolo del ritardo basato sulla velocità
        int delay = 500000 - rana.velocita;
        if (delay < 0)
            delay = 0;

        if (rana.y < maxy - 2 && rana.y > miny + 6 && ch != KEY_UP && ch != KEY_DOWN)
        {
            usleep(delay);
            if (rana.direzione == DESTRA)
            {
                rana.x++; // nonostante non ci siano le collisioni con l'acqua, la rana se va sopra perde una vita
            }
            if (rana.direzione == SINISTRA)
            {
                rana.x--; // nonostante non ci siano le collisioni con l'acqua, la rana se va sopra perde una vita
            }
        }

        // start_m = clock();
        // }

        // Scrittura nella pipe delle informazioni della rana
        if (write(pipeout, &rana, sizeof(elementoGioco)) == -1)
        {
            perror("Errore nella scrittura sulla pipe");
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