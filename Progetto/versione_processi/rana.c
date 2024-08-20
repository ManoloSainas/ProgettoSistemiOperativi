#include "main.h"

int indexProiettile = 0;

void rana(int pipeout)
{
    struct timeval start, end;
    int tempoTrascorso, tempoRicarica = RICARICA_PROIETTILI * maxx; // Adatta la velocità di ricarica dei proiettili a seconda della dimensione dello schermo

    tempoTrascorso = tempoRicarica + 1; // si aggiunge uno perchè altrimenti il primo proiettile non verrebbe sparato

    int pid_rana;
    bool primoSparo = false; // True se sono stati sparati i primi proiettili
    keypad(gioco, TRUE);

    oggetto oggetto_rana;
    oggetto_rana.tipo = RANA;
    oggetto_rana.x = maxx / 2;
    oggetto_rana.y = maxy - 1;
    oggetto_rana.pid_oggetto = getpid();
    oggetto_rana.status = ATTIVO;
    oggetto_rana.proiettili = TRUE;
    oggetto_rana.index = 0;
    oggetto_rana.direzioneFiume = NESSUNA_DIREZIONE;
    oggetto_rana.tipologiaCoccodrillo = NESSUNA_TIPOLOGIA;

    // write(pipeout, &oggetto_rana, sizeof(oggetto));

    while (1)
    {
        if (primoSparo)
        {
            gettimeofday(&end, NULL);
            tempoTrascorso = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);

            // Comunica se si può sparare la prossima coppia di proiettili
            if (tempoTrascorso > tempoRicarica)
            {
                oggetto_rana.proiettili = true;
                write(pipeout, &oggetto_rana, sizeof(oggetto));
            }
        }

        wtimeout(gioco, 0); // Non aspetta l'input dell'utente
        int inputUtente = wgetch(gioco);

        switch (inputUtente)
        {
        case KEY_UP:
            if (oggetto_rana.y > miny)
                oggetto_rana.y -= SPOSTAMENTO_RANA;
            break;
        case KEY_DOWN:
            if (oggetto_rana.y < maxy - 1)
                oggetto_rana.y += SPOSTAMENTO_RANA;
            break;
        case KEY_LEFT:
            if (oggetto_rana.x > minx)
                oggetto_rana.x -= SPOSTAMENTO_RANA;
            break;
        case KEY_RIGHT:
            if (oggetto_rana.x < maxx - COLONNE_SPRITE_RANA)
                oggetto_rana.x += SPOSTAMENTO_RANA;
            break;

        case KEY_SPACE:
            if (tempoTrascorso > tempoRicarica)
            {
                if (indexProiettile > NUM_PROIETTILI_RANA)
                    indexProiettile = 0;

                pid_rana = fork();

                switch (pid_rana)
                {
                case -1:
                    perror("Errore nell'esecuzione della fork.");
                    _exit(1);
                case 0:
                    // Processo proiettile
                    proiettileRana(pipeout, oggetto_rana.y, oggetto_rana.x);
                    break;
                default:
                    oggetto_rana.proiettili = false;
                    indexProiettile++;

                    gettimeofday(&start, NULL);
                    primoSparo = true;

                    tempoTrascorso = 0;
                    break;
                }
            }
        }

        // Scrittura nella pipe delle informazioni della rana
        write(pipeout, &oggetto_rana, sizeof(oggetto));
    }

    _exit(1);
}

void proiettileRana(int pipeout, int pos_ranay, int pos_ranax)
{
    oggetto oggetto_proiettile_rana;

    // Inizializzazione dei dati del proiettile
    oggetto_proiettile_rana.tipo = PROIETTILE_RANA;
    oggetto_proiettile_rana.x = pos_ranax;
    oggetto_proiettile_rana.y = pos_ranay - RIGHE_SPRITE_RANA;
    oggetto_proiettile_rana.y -= SPOSTAMENTO_X_PROIETTILI_RANA;
    oggetto_proiettile_rana.index = indexProiettile;
    oggetto_proiettile_rana.pid_oggetto = getpid();
    oggetto_proiettile_rana.status = ATTIVO;
    oggetto_proiettile_rana.proiettili = FALSE;
    oggetto_proiettile_rana.direzioneFiume = NESSUNA_DIREZIONE;
    oggetto_proiettile_rana.tipologiaCoccodrillo = NESSUNA_TIPOLOGIA;

    // Scrittura nella pipe delle informazioni del proiettile
    write(pipeout, &oggetto_proiettile_rana, sizeof(oggetto));

    while (1)
    {
        oggetto_proiettile_rana.y -= SPOSTAMENTO_Y_PROIETTILI_RANA;

        usleep(SPEED_PROIETTILI);

        write(pipeout, &oggetto_proiettile_rana, sizeof(oggetto));
    }
}