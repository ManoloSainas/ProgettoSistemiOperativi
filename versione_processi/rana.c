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
    rana.x = RANA_X;
    rana.y = RANA_Y;
    rana.pid_oggetto = getpid();

    rana.velocita = 0;
    bool danno;
    clock_t start_m, stop_m, start_p, stop_p;
    long int x;
    double durata_f, durata_p;
    bool puoi_sparare = true;
    start_m = time(NULL);
    start_p = time(NULL);
    pid_t proiettilePid;
    posizione dati_p;

    while (1)
    {
        primoSparo = true;

        stop_p = clock();
        durata_p = (double)(stop_p - start_p) / CLOCKS_PER_SEC;

        int delay = 500000 - rana.velocita;
        if (delay < 0)
            delay = 0;

        if (durata_p > tempoRicarica && puoi_sparare)
        {
            beep();
            puoi_sparare = false;
        }
        stop_m = time(NULL);

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
            if (num_spari <= MAXGRANATE - 2)
            {
                switch (fork())
                {
                case -1:
                    mvwprintw(gioco, 3, maxx - 10, "spara sinistra fallito");
                    perror("Errore nell'esecuzione della fork.");
                    _exit(1);
                case 0:
                    // Processo proiettile
                    proiettile(pipeout, rana.y, rana.x, SPEED_GRANATE, DESTRA, 'r');
                    _exit(0);
                default:
                    num_spari++;
                    switch (fork())
                    {
                    case -1:
                        perror("Errore nell'esecuzione della fork.");
                        mvwprintw(gioco, 3, maxx - 10, "spara sinistra fallito");
                        _exit(1);
                    case 0:
                        // Processo proiettile
                        proiettile(pipeout, rana.y, rana.x, SPEED_GRANATE, SINISTRA, 'r');
                        _exit(0);
                    default:
                        num_spari++;
                        start_p = clock();
                        break;
                    }
                    break;
                }
            }
            break;
        default:

            int delay = 500000 - rana.velocita;
            if (delay < 0)
                delay = 0;

            if (rana.y < maxy - 2 && rana.y > miny + 6 && ch != KEY_UP && ch != KEY_DOWN)
            {
                if (difftime(stop_m, start_m) >= delay / (10 * 7))
                {
                    if (rana.direzione == DESTRA)
                    {
                        rana.x++;
                    }
                    if (rana.direzione == SINISTRA)
                    {
                        rana.x--;
                    }
                    start_m = time(NULL);
                }
            }
            break;
        }

        rana.direzione = flussi[16 - rana.y].direzione;
        rana.velocita = flussi[16 - rana.y].velocita;

        // Calcolo del ritardo basato sulla velocità

        rana.proiettile = num_spari;

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
                beep();
                chiudiProcessi(dati_p.pid);
                num_spari--;
            }
        }
    }

    _exit(1);
}