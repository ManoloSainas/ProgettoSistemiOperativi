#include "frogger.h"

void gestioneFlussi(corrente *flussi, int *coccodrilli_flusso)
{
    // inizializzazione flussi del fiume
    flussi[0].direzione = NESSUNA;
    flussi[NUM_FLUSSI_FIUME + 2].direzione = NESSUNA;
    flussi[NUM_FLUSSI_FIUME + 1].direzione = NESSUNA;
    flussi[0].velocita = 0;
    if (rand() % 2 == 0)
        flussi[1].direzione = DESTRA;
    else
    {
        flussi[1].direzione = SINISTRA;
    }

    flussi[NUM_FLUSSI_FIUME + 2].velocita = 0;
    flussi[NUM_FLUSSI_FIUME + 1].velocita = 0;
    flussi[1].velocita = rand() % (350000) + 100000;

    for (int i = 2; i <= NUM_FLUSSI_FIUME; i++)
    {
        if (flussi[i - 1].direzione == DESTRA)
        {
            flussi[i].direzione = SINISTRA;
        }
        else
        {
            flussi[i].direzione = DESTRA;
        }
        // flussi[i].velocita = rand() % (100) + 1;
        flussi[i].velocita = 1;
    }
    // inizializazione numero di coccodrilli per flusso
    for (int i = 0; i < NUM_FLUSSI_FIUME; i++)
    {
        coccodrilli_flusso[i] = rand() % (15 + 1 - 5) + 5;
    }
}

void avviaGioco(int vita, bool tana_status[], int punteggio)
{

    int filedes[2], pipeRana[2], pipeCocco[2];
    int pid_gioco;
    int coccodrilli_flusso[NUM_FLUSSI_FIUME], tot_coc = 0;
    corrente flussi[NUM_FLUSSI_FIUME + 3];

    for (int i = 0; i < NUM_FLUSSI_FIUME; i++)
    {
        tot_coc += coccodrilli_flusso[i];
    }

    graficaGioco(tana_status, punteggio, vita);
    gestioneFlussi(flussi, coccodrilli_flusso);
    inizializzazionePipe(filedes);
    inizializzazionePipe(pipeRana);
    inizializzazionePipe(pipeCocco);

    pid_gioco = fork();

    switch (pid_gioco)
    {
    case -1:
        perror("Errore nella creazione del processo per la rana");
        _exit(1);
        break;
    case 0:
        close(filedes[LETTURA]);
        close(pipeRana[SCRITTURA]);
        rana(filedes[SCRITTURA], pipeRana[LETTURA], flussi);
        _exit(0);
        break;
    default:
        for (int i = 1; i <= NUM_FLUSSI_FIUME; i++)
        {
            for (int j = 1; j <= MAXCOCCODRILLI / NUM_FLUSSI_FIUME; j++)
            {
                pid_gioco = fork();
                switch (pid_gioco)
                {
                case -1:
                    perror("Errore nella creazione del processo per il coccodrillo");
                    _exit(1);
                    break;
                case 0:
                    close(pipeCocco[SCRITTURA]);
                    close(filedes[LETTURA]);
                    srand(time(NULL) + i);
                    j == 1 ? usleep(3000000 - flussi[i].velocita * 100000 + (rand() % 6000000)) : usleep((3000000 - flussi[i].velocita + rand() % 5000000 + 2000000) * j);
                    coccodrillo(filedes[SCRITTURA], pipeCocco[LETTURA], i, j, flussi[i]);
                    _exit(0);
                    break;
                default:
                    break;
                }
            }
        }

        close(filedes[SCRITTURA]);
        close(pipeRana[LETTURA]);
        close(pipeCocco[LETTURA]);
        controlloGioco(filedes[LETTURA], pipeRana[SCRITTURA], pipeCocco[SCRITTURA], vita, tana_status);

        break;
    }

    //  terminaGioco();
}

void terminaGioco()
{
    wclear(gioco);
    wrefresh(gioco);
    delwin(gioco); // Delete the window
    endwin();      // End ncurses
}

void controlloGioco(int pipein, int pipeRana, int pipeCocco, int vita, bool tana_status[])
{
    /*
    elementoGioco granRana;
    granRana.direzione= DESTRA;
    granRana.x=5;
    granRana.y=10;
    granRana.tipo=GRANATA;*/

    posizione pos_r, pos_c[MAXCOCCODRILLI], pos_granate[MAXGRANATE], pos_proiettili[MAXPROIETTILI];
    int score = 0;
    elementoGioco valoreLetto;
    elementoGioco rana, coccodrillo, granata, proiettile;
    bool danno, esiste;
    int countG = 0, countP = 0;
    pos_r.y = 16;
    pos_r.x = 36;
    pid_t primo_cocco;
    for (int i = 0; i < MAXCOCCODRILLI; i++)
    {
        pos_c[i].pid = -500;
    }
    for (int i = 0; i < MAXGRANATE; i++)
    {
        pos_granate[i].pid = -500;
    }
    for (int i = 0; i < MAXPROIETTILI; i++)
    {
        pos_proiettili[i].pid = -500;
    }

    do
    {

        danno = false;
        for (int i = 0; i < MAXCOCCODRILLI; i++)
        {
            if (pos_r.y == pos_c[i].y && pos_c[i].pid != -500)
            {
                if (pos_c[i].direzione == SINISTRA)
                {
                    if (pos_r.x >= pos_c[i].x - 1 && pos_r.x <= pos_c[i].x + 3)
                    {
                        danno = true;
                        break;
                    }
                }
                else
                {
                    if (pos_r.x < pos_c[i].x + 1 && pos_r.x > pos_c[i].x - 5)
                    {
                        danno = true;
                        break;
                    }
                }
            }
        }
        if (pos_r.y == maxy - 2 || pos_r.y == maxy - 11 || pos_r.y == maxy - 12)
        {
            danno = true;
        }

        if (read(pipein, &valoreLetto, sizeof(valoreLetto)) > 0)
        {
            elementoGioco oggettoLetto;
            switch (valoreLetto.tipo)
            {
            case RANA:
                oggettoLetto = rana;
                break;
            case COCCODRILLO:
                oggettoLetto = coccodrillo;
                break;
            case GRANATA:
                oggettoLetto = granata;
                break;
            case PROIETTILE_COCCODRILLO:
                oggettoLetto = proiettile;
            }
            cancellaSprite(oggettoLetto);

            switch (valoreLetto.tipo)
            {
            case RANA:
                rana = valoreLetto;
                pos_r.x = rana.x;
                pos_r.y = rana.y;

                break;
            case COCCODRILLO:
                coccodrillo = valoreLetto;

                for (int i = 0; i < MAXCOCCODRILLI; i++)
                {
                    if (pos_c[i].pid == coccodrillo.pid_oggetto)
                    {
                        pos_c[i].x = coccodrillo.x;
                        pos_c[i].y = coccodrillo.y;
                        break;
                    }
                    if (pos_c[i].pid == -500)
                    {
                        pos_c[i].pid = coccodrillo.pid_oggetto;
                        pos_c[i].x = coccodrillo.x;
                        pos_c[i].y = coccodrillo.y;
                        pos_c[i].direzione = coccodrillo.direzione;
                        pos_c[i].proiettile = coccodrillo.proiettile;
                        break;
                    }
                }
                break;
            case GRANATA:
                granata = valoreLetto;
                esiste = false;
                for (int i = 0; i < MAXGRANATE; i++)
                {
                    if (pos_granate[i].pid == granata.pid_oggetto)
                    {

                        pos_granate[i].x = granata.x;
                        pos_granate[i].y = granata.y;
                        esiste = true;
                        if ((pos_granate[i].direzione == DESTRA && pos_granate[i].x >= maxx) || (pos_granate[i].direzione == SINISTRA && pos_granate[i].x <= 1))
                        {
                            if (write(pipeRana, &pos_granate[i], sizeof(posizione)) == -1)
                                ;
                            countG--;
                            pos_granate[i].pid = -500;
                        }
                        break;
                    }
                }
                if (!esiste)
                {
                    for (int i = 0; i < MAXGRANATE; i++)
                    {
                        if (pos_granate[i].pid == -500)
                        {
                            if (countG > MAXGRANATE)
                            {
                                write(pipeRana, &pos_granate[i], sizeof(posizione));
                            }
                            else
                            {

                                countG++;
                                pos_granate[i].pid = granata.pid_oggetto;
                                pos_granate[i].x = granata.x;
                                pos_granate[i].y = granata.y;
                                pos_granate[i].direzione = granata.direzione;
                                break;
                            }
                        }
                    }
                }
                break;

            case PROIETTILE_COCCODRILLO:
                proiettile = valoreLetto;
                esiste = false;
                for (int i = 0; i < MAXPROIETTILI; i++)
                {
                    if (pos_proiettili[i].pid == proiettile.pid_oggetto)
                    {
                        pos_proiettili[i].x = proiettile.x;
                        pos_proiettili[i].y = proiettile.y;
                        if ((pos_proiettili[i].direzione == DESTRA && pos_proiettili[i].x >= maxx) || (pos_proiettili[i].direzione == SINISTRA && pos_proiettili[i].x <= 1))
                        {
                            for (int y = 0; y < MAXCOCCODRILLI; y++)
                            {
                                if (pos_c[y].proiettile == pos_proiettili[i].pid)
                                {
                                    kill(pos_c[y].pid, SIGUSR1);
                                    waitpid(pos_c[y].pid, NULL, 0);
                                    countP--;
                                    pos_proiettili[i].pid = -500;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
                if (!esiste)
                {
                    for (int i = 0; i < MAXPROIETTILI; i++)
                    {
                        if (pos_proiettili[i].pid == -500 && countP < MAXPROIETTILI)
                        {
                            if (countG > MAXCOCCODRILLI)
                            {
                                for (int y = 0; y < MAXCOCCODRILLI; y++)
                                {
                                    if (pos_c[y].proiettile == pos_proiettili[i].pid)
                                    {
                                        kill(pos_c[y].pid, SIGUSR1);
                                        countP--;
                                        pos_proiettili[i].pid = -500;
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                countP++;
                                pos_proiettili[i].pid = proiettile.pid_oggetto;
                                pos_proiettili[i].x = proiettile.x;
                                pos_proiettili[i].y = proiettile.y;
                                pos_proiettili[i].direzione = proiettile.direzione;
                                break;
                            }
                        }
                    }
                }
                break;

            default:
                break;
            }
        }

        stampaSprite(coccodrillo);
        stampaSprite(rana);
        stampaSprite(granata);
        stampaSprite(proiettile);

        wrefresh(gioco);

        if (pos_r.y == 6)
        {
            if (pos_r.x != 11 && pos_r.x != 23 && pos_r.x != 35 && pos_r.x != 47 && pos_r.x != 59)
            {
                for (int i = 0; i < MAXCOCCODRILLI; i++)
                {
                    kill(pos_c[i].pid, SIGUSR1);
                    chiudiProcessi(pos_c[i].pid);
                }
                for (int i = 0; i < MAXGRANATE; i++)
                {
                    write(pipeRana, &pos_granate[i], sizeof(posizione));
                }

                chiudiProcessi(rana.pid_oggetto);
                exit(6);
            }
            else
            {

                if (pos_r.x == 11)
                {
                    if (tana_status[0])
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(1);
                    }
                    else
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(6);
                    }
                }

                if (pos_r.x == 23)
                {
                    if (tana_status[1])
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(2);
                    }
                    else
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(6);
                    }
                }

                if (pos_r.x == 35)
                {
                    if (tana_status[2])
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(3);
                    }
                    else
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(6);
                    }
                }

                if (pos_r.x == 47)
                {
                    if (tana_status[3])
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(4);
                    }
                    else
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(6);
                    }
                }

                if (pos_r.x == 59)
                {
                    if (tana_status[4])
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(5);
                    }
                    else
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            kill(pos_c[i].pid, SIGUSR1);
                            chiudiProcessi(pos_c[i].pid);
                        }
                        for (int i = 0; i < MAXGRANATE; i++)
                        {
                            write(pipeRana, &pos_granate[i], sizeof(posizione));
                        }

                        chiudiProcessi(rana.pid_oggetto);
                        exit(6);
                    }
                }
            }
        }

        if (!danno)
        {
            for (int i = 0; i < MAXCOCCODRILLI; i++)
            {
                kill(pos_c[i].pid, SIGUSR1);
                chiudiProcessi(pos_c[i].pid);
            }
            for (int i = 0; i < MAXGRANATE; i++)
            {
                write(pipeRana, &pos_granate[i], sizeof(posizione));
            }

            chiudiProcessi(rana.pid_oggetto);
            exit(6);
        }
    } while (true);
}

// bool collisioneTane(int ranaX, int ranaY)
// {
//     for (int i = 0; i < 5; i++)
//     {
//         if (ranaX >= posTane[i].x && ranaX <= posTane[i].x2 && ranaY == posTane[i].y)
//         {
//             tane[i] == false ? tane[i] = true : viteRana--;
//             return true;
//         }
//     }
//     return false;
// }
