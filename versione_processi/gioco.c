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
    flussi[1].velocita = 1;

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
                    usleep((3000000 - flussi[i].velocita + rand() % 5000000 + 2000000) * j);
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
    posizione pos_r, pos_c[MAXCOCCODRILLI], pos_granate[MAXGRANATE], pos_proiettili[MAXCOCCODRILLI];
    int score = 0;
    elementoGioco valoreLetto;
    elementoGioco rana, coccodrillo, granata, proiettile;
    elementoGioco tempG, tempP;
    bool danno, esiste;
    int countG = 0, countP = 0;
    pos_r.y = 16;
    pos_r.x = 36;
    pid_t primo_cocco;
    for (int i = 0; i < MAXCOCCODRILLI; i++)
    {
        pos_c[i].pid = INVALID_PID;
        pos_c[i].x = -1;
        pos_c[i].y = -1;
        pos_proiettili[i].pid = INVALID_PID;
        pos_proiettili[i].x = -1;
        pos_proiettili[i].y = -1;
    }
    for (int i = 0; i < MAXGRANATE; i++)
    {
        pos_granate[i].pid = INVALID_PID;
        pos_granate[i].x = -1;
        pos_granate[i].y = -1;
    }

    do
    {

        granata.pid_oggetto=INVALID_PID;
        danno = true;
        for (int i = 0; i < MAXCOCCODRILLI; i++)
        {
            if (pos_r.y == pos_c[i].y && pos_c[i].pid != INVALID_PID)
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

        if (pos_r.x == 0 || pos_r.x == maxx - 2)
        {
            danno = false;
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
                        pos_c[i].proiettile = coccodrillo.proiettile;
                        break;
                    }
                    if (pos_c[i].pid == INVALID_PID)
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
                    if (pos_granate[i].pid == granata.pid_oggetto && pos_granate[i].pid != INVALID_PID )
                    {

                        pos_granate[i].x = granata.x;
                        pos_granate[i].y = granata.y;
                        esiste = true;
                        if ((pos_granate[i].direzione == DESTRA && pos_granate[i].x >= maxx) || (pos_granate[i].direzione == SINISTRA && pos_granate[i].x < 0))
                        {
                            
                            if (write(pipeRana, &pos_granate[i], sizeof(posizione)) == -1)
                                ;
                            countG--;
                            pos_granate[i].pid = INVALID_PID;
                            pos_granate[i].x = -1;
                            pos_granate[i].y = -1;
                        }
                        break;
                    }
                }
                if (!esiste)
                {
                    for (int i = 0; i < MAXGRANATE; i++)
                    {
                        if (pos_granate[i].pid == INVALID_PID)
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
                break;

            case PROIETTILE_COCCODRILLO:
                proiettile = valoreLetto;
                esiste = false;
                for (int i = 0; i < MAXCOCCODRILLI; i++)
                {
                    if (pos_proiettili[i].pid == proiettile.pid_oggetto && pos_proiettili[i].pid != INVALID_PID)
                    {
                        pos_proiettili[i].pid = proiettile.pid_oggetto;
                        pos_proiettili[i].x = proiettile.x;
                        pos_proiettili[i].y = proiettile.y;
                        esiste = true;
                        if ((pos_proiettili[i].direzione == DESTRA && pos_proiettili[i].x >= maxx) || (pos_proiettili[i].direzione == SINISTRA && pos_proiettili[i].x < 1))
                        {

                            for (int y = 0; y < MAXCOCCODRILLI; y++)
                            {
                                if (pos_c[y].proiettile == pos_proiettili[i].pid)
                                {
                                    kill(pos_c[y].pid, SIGUSR1);
                                    countP--;
                                    pos_c[y].pid = INVALID_PID;
                                    pos_proiettili[i].pid = INVALID_PID;
                                    pos_proiettili[i].x = -1;
                                    pos_proiettili[i].y = -1;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    if (!esiste)
                    {

                        if (pos_proiettili[i].pid == INVALID_PID)
                        {
                            if (countP < MAXCOCCODRILLI)
                            {
                                countP++;
                                pos_proiettili[i].pid = proiettile.pid_oggetto;
                                pos_proiettili[i].x = proiettile.x;
                                pos_proiettili[i].y = proiettile.y;
                                pos_proiettili[i].direzione = proiettile.direzione;
                                break;
                            }
                            else
                            {
                                 for (int y = 0; y < MAXCOCCODRILLI; y++)
                                {
                                    if (pos_c[y].proiettile == proiettile.pid_oggetto){
                                        kill(pos_c[y].pid, SIGUSR1);}
                                }
                                pos_proiettili[i].pid = INVALID_PID;
                                pos_proiettili[i].x = -1;
                                pos_proiettili[i].y = -1;

                               
                            }
                        }
                    }
                }
            default:
                break;
            }
        }

        /*Gestione collisione proiettili con granate */
        for (int i = 0; i < MAXGRANATE; i++)
        {
            if (pos_granate[i].pid != INVALID_PID)
            {
                for (int y = 0; y < MAXCOCCODRILLI; y++)
                {
                    if (pos_proiettili[y].pid != INVALID_PID)
                    { 
                    if (pos_granate[i].x > 0 && pos_proiettili[y].x > 0)
                    {
                        if (pos_granate[i].y == pos_proiettili[y].y && pos_granate[i].x == pos_proiettili[y].x)
                        {
                            // Memorizzazione delle informazioni della granata e del proiettile
                            tempG.x = pos_granate[i].x;
                            tempG.y = pos_granate[i].y;
                            tempG.tipo = GRANATA;
                            tempG.pid_oggetto = pos_granate[i].pid;

                            tempP.x = pos_proiettili[y].x;
                            tempP.y = pos_proiettili[y].y;
                            tempP.tipo = PROIETTILE_COCCODRILLO;
                            tempP.pid_oggetto = pos_proiettili[y].pid;

                            // Resetta la posizione e il PID del proiettile
                            pos_proiettili[y].x = -1;
                            pos_proiettili[y].y = -1;

                            countP--;
                            countG--;
                            beep();
                            // Notifica tramite pipe
                            if (write(pipeRana, &pos_granate[i], sizeof(posizione)) == -1)
                                perror("ERRORE PIPE RANA GRANATA");
                            // Resetta la posizione e il PID della granata
                            pos_granate[i].x = -1;
                            pos_granate[i].y = -1;
                            pos_granate[i].pid = INVALID_PID;
                            // Controlla se il proiettile appartiene a un coccodrillo e invia segnale
                            for (int j = 0; j < MAXCOCCODRILLI; j++)
                            {
                                if (pos_c[j].proiettile == pos_proiettili[y].pid)
                                {
                                    if (kill(pos_c[j].pid, 0) == 0){
                                        kill(pos_c[j].pid, SIGUSR1);}
                                    pos_c[j].proiettile = INVALID_PID;
                                    pos_proiettili[y].pid = INVALID_PID;
                                    cancellaProiettile(tempG);
                                    cancellaProiettile(tempP);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }}
            }
        }

        stampaSprite(coccodrillo);
        stampaSprite(rana);
        if (granata.pid_oggetto > 0)
            stampaSprite(granata);
        if (proiettile.pid_oggetto > 0)
            stampaSprite(proiettile);
        if (granata.pid_oggetto > 0)
        {
            cancellaProiettile(tempG);
            tempG.pid_oggetto = INVALID_PID;
        }
        if (proiettile.pid_oggetto < 0)
        {
            cancellaProiettile(tempP);
            tempP.pid_oggetto = INVALID_PID;
        }

        wrefresh(gioco);

        // controllo interazione tane
        if (pos_r.y == 6)
        {
            chiusuraFineManche(pos_c, pos_granate, pipeRana, rana.pid_oggetto);

            if (pos_r.x != 11 && pos_r.x != 23 && pos_r.x != 35 && pos_r.x != 47 && pos_r.x != 59)
            {

                exit(6);
            }

            if (pos_r.x == 11)
            {
                if (tana_status[0])
                {
                    exit(1);
                }
                else
                {
                    exit(6);
                }
            }

            if (pos_r.x == 23)
            {
                if (tana_status[1])
                {
                    exit(2);
                }
                else
                {
                    exit(6);
                }
            }

            if (pos_r.x == 35)
            {
                if (tana_status[2])
                {
                    exit(3);
                }
                else
                {
                    exit(6);
                }
            }

            if (pos_r.x == 47)
            {
                if (tana_status[3])
                {
                    exit(4);
                }
                else
                {
                    exit(6);
                }
            }

            if (pos_r.x == 59)
            {
                if (tana_status[4])
                {
                    exit(5);
                }
                else
                {
                    exit(6);
                }
            }
        }
        // controllo collisione rana proiettile
        for (int i = 0; i < MAXCOCCODRILLI; i++)
        {

            if (pos_proiettili[i].y == pos_r.y && (pos_proiettili[i].x == pos_r.x || pos_proiettili[i].x == pos_r.x + 1))
            {
                if (pos_proiettili[i].pid != INVALID_PID)
                {
                    danno = false;
                    break;
                }
            }
        }
        // controllo gestione del danno
        if (!danno)
        {
            chiusuraFineManche(pos_c, pos_granate, pipeRana, rana.pid_oggetto);
            exit(6);
        }
    } while (true);
}

void chiusuraFineManche(posizione pos_c[], posizione pos_granate[], int pipeRana, pid_t pid_rana)
{

    for (int i = 0; i < MAXCOCCODRILLI; i++)
    {
        kill(pos_c[i].pid, SIGUSR1);
        chiudiProcessi(pos_c[i].pid);
    }
    for (int i = 0; i < MAXGRANATE; i++)
    {
        if (write(pipeRana, &pos_granate[i], sizeof(posizione)) == -1)
        {
            perror("Errore nella scrittura sulla pipe");
            _exit(6);
        }
    }
    chiudiProcessi(pid_rana);
}