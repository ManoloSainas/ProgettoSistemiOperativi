#include "frogger.h"

// x e y delle tane
posizioneTane posTane[NUM_TANE] = {
    {11, 6},
    {23, 6},
    {35, 6},
    {47, 6},
    {59, 6}};

void terminaGioco()
{
    wclear(gioco);   // pulisce la finestra di gioco
    wrefresh(gioco); // aggiorna la finestra di gioco
    delwin(gioco);   // elimina la finestra di gioco
    endwin();        // termina la finestra
}

// metodo per la gestione dei flussi del fiume
void gestioneFlussi(corrente *flussi, int *coccodrilli_flusso)
{
    // srand per generare numeri casuali
    srand(time(NULL));

    // serve per generare una velocità differente per ogni flusso
    int velocitaRandom;

    // inizializzazione flussi del fiume

    // flusso 0
    flussi[0].direzione = NESSUNA;
    flussi[NUM_FLUSSI_FIUME + 2].direzione = NESSUNA;
    flussi[NUM_FLUSSI_FIUME + 1].direzione = NESSUNA;
    flussi[0].velocita = 0;

    // generazione direzione dei flussi casuale
    if (rand() % 2 == 0)
        flussi[1].direzione = DESTRA;
    else
    {
        flussi[1].direzione = SINISTRA;
    }

    // gestione zone dove non ci sono flussi
    flussi[NUM_FLUSSI_FIUME + 2].velocita = 0;
    flussi[NUM_FLUSSI_FIUME + 1].velocita = 0;

    // flussi[1].velocita = 15000 + rand() % (100000) + 1; // Nel caso si voglia utilizzare una velocità casuale e non prefissata
    flussi[1].velocita = VELOCITA_FLUSSO;

    // gestione della direzione e della velocità dei restanti 7 flussi
    for (int i = 2; i <= NUM_FLUSSI_FIUME; i++)
    {
        if (i == 2)
        {
            velocitaRandom = 1 + rand() % ((i - 2) + 1); // velocità random per il primo flusso tra 1 e 2
        }
        else
        {
            velocitaRandom = 1 + rand() % (i - 2); // velocità random per i flussi successivi tra 1 e i-2
        }
        if (flussi[i - 1].direzione == DESTRA)
        {
            flussi[i].direzione = SINISTRA;
            flussi[i].velocita = VELOCITA_FLUSSO * velocitaRandom;
        }
        else
        {
            flussi[i].direzione = DESTRA;
            flussi[i].velocita = VELOCITA_FLUSSO * velocitaRandom;
        }
        // flussi[i].velocita = 15000 + rand() % (100000) + 1; // Nel caso si voglia utilizzare una velocità casuale e non prefissata
    }

    // inizializazione numero di coccodrilli per flusso
    for (int i = 0; i < NUM_FLUSSI_FIUME; i++)
    {
        coccodrilli_flusso[i] = NUM_COCCODRILLI_FLUSSO;
    }
}

// avvia il gioco e crea i vari processi
void avviaGioco(int vita, bool tana_status[], int punteggio)
{
    int tempoRimanente = TEMPO_TOTALE; // Inizializzazione del tempo rimanente

    int filedes[2], pipeRana[2];
    int pid_gioco;

    int coccodrilli_flusso[NUM_FLUSSI_FIUME] = {NUM_COCCODRILLI_FLUSSO}; // numero di coccodrilli per flusso
    corrente flussi[NUM_FLUSSI_FIUME + 3];

    // inizializzazione grafica
    graficaGioco(tana_status, punteggio, vita);

    // inizialiizzazione flussi del fiume
    gestioneFlussi(flussi, coccodrilli_flusso);

    // inizializzazione pipe
    inizializzazionePipe(filedes);
    inizializzazionePipe(pipeRana);

    // creazione processo rana
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
            for (int j = 1; j <= NUM_COCCODRILLI_FLUSSO; j++)
            { // creazione processi coccodrillo
                pid_gioco = fork();
                switch (pid_gioco)
                {
                case -1:
                    perror("Errore nella creazione del processo per il coccodrillo");
                    _exit(1);
                    break;
                case 0:
                    close(filedes[LETTURA]);

                    // spawn coccodrilli in modo casuale
                    srand(time(NULL) + i);
                    usleep((2500000 + rand() % 5000000 + 1500000) * j);

                    coccodrillo(filedes[SCRITTURA], i, j, flussi[i]);
                    _exit(0);
                    break;
                default:
                    break;
                }
            }
        }

        close(filedes[SCRITTURA]);
        close(pipeRana[LETTURA]);
        controlloGioco(filedes[LETTURA], pipeRana[SCRITTURA], vita, tana_status, tempoRimanente); // riceve le coordinate e controlla le collisioni

        break;
    }
}

// processo che riceve le coordinate e controlla le collisioni
void controlloGioco(int pipein, int pipeRana, int vita, bool tana_status[], int tempoRimanente)
{
    time_t inizioTempo = time(NULL); // Inizializzazione del tempo di inizio

    // elementi per la gestione delle granate e dei proiettili dopo la collisione
    elementoGioco granata_eg, proiettile_eg;

    // liste e variabili per salvare le posizioni dei vari elementi, t_posg è per salvare la granata per l'eliminazione dopo la collisione
    posizione pos_r, pos_c[MAXCOCCODRILLI], pos_granate[MAXGRANATE], pos_proiettili[MAXCOCCODRILLI], t_posg;

    elementoGioco valoreLetto;                            // elemento letto dalla pipe
    elementoGioco rana, coccodrillo, granata, proiettile; // elementi del gioco

    int score = 0;
    pid_t coccodrillo_rana;     // identificatore del coccodrillo cavalcato dalla rana
    bool danno, esiste;         // flag gestione collisione e esistenza negli array, danno->false danno ricevuto, danno->true niente danno
    int countG = 0, countP = 0; // contatori per gestione corretta del numero di processi e debugging

    // inizializzazione posizione rana
    pos_r.y = RANA_Y;
    pos_r.x = RANA_X;

    // inizializzazione elementi del gioco
    rana.pid_oggetto = INVALID_PID;
    coccodrillo.pid_oggetto = INVALID_PID;
    granata.pid_oggetto = INVALID_PID;
    proiettile.pid_oggetto = INVALID_PID;

    // reset di granata_eg e di proiettile_eg
    granata_eg.pid_oggetto = INVALID_PID;
    proiettile_eg.pid_oggetto = INVALID_PID;

    // inizializzazione liste di posizione
    // INVALID_PID è un identificativo che serve per distinguere un oggetto attivo da uno eliminato o vuoto
    for (int i = 0; i < MAXCOCCODRILLI; i++) // ogni coccodrillo è associata un proiettile quindi maxproiettili=maxcoccodrilli
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
        // Aggiorna il timer e lo stampa
        time_t tempoAttuale = time(NULL);
        if (difftime(tempoAttuale, inizioTempo) >= 1) // se un secondo è passato
        {
            tempoRimanente--;           // Decrementa il timer
            inizioTempo = tempoAttuale; // aggiorna inizio tempo
        }

        // Stampa il timer
        wattron(gioco, COLOR_PAIR(COLORE_ROSSO));
        mvwprintw(gioco, 1, maxx / 2 - 2, "%2d", tempoRimanente);
        wattroff(gioco, COLOR_PAIR(COLORE_ROSSO));

        // se finisce il tempo il giocatore perde una vita e finisce la manche
        if (tempoRimanente == 0)
        {
            chiusuraFineManche(pos_c, pos_granate, pos_proiettili, pipeRana, rana.pid_oggetto, pipein);
            exit(6);
        }

        danno = false; // danno con l'acqua -> false

        // controllo collisione acqua
        coccodrillo_rana = INVALID_PID;
        for (int i = 0; i < MAXCOCCODRILLI; i++)
        {
            if (pos_r.y == pos_c[i].y && pos_c[i].pid != INVALID_PID)
            {
                if (pos_c[i].direzione == SINISTRA)
                {
                    if (pos_r.x >= pos_c[i].x - 1 && pos_r.x <= pos_c[i].x + 3)
                    {
                        coccodrillo_rana = pos_c[i].pid; // aggiornamento del coccodrillo cavalcato
                        danno = true;
                        break;
                    }
                }
                else
                {
                    if (pos_r.x < pos_c[i].x + 1 && pos_r.x > pos_c[i].x - 5)
                    {
                        coccodrillo_rana = pos_c[i].pid; // aggiornamento del coccodrillo cavalcato
                        danno = true;
                        break;
                    }
                }
            }
        }

        // zone in cui la rana può stare senza ricevere danno
        if (pos_r.y == maxy - 2 || pos_r.y == maxy - 11 || pos_r.y == maxy - 12)
        {
            danno = true;
        }

        // collisioni con le pareti del campo
        if (pos_r.x == 0 || pos_r.x == maxx - 2)
        {
            danno = false;
        }

        // legge il valore dalla pipe
        if (read(pipein, &valoreLetto, sizeof(valoreLetto)) > 0)
        {
            // cancella gli sprite
            cancellaSprite(rana);
            cancellaSprite(coccodrillo);
            cancellaSprite(granata);
            cancellaSprite(proiettile);

            // aggiorna le posizioni e inserimento negli array di posizione
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
                        // se il coccodrillo è cavalcato dalla rana
                        if (pos_c[i].pid == coccodrillo_rana)
                        {
                            kill(rana.pid_oggetto, SIGUSR1); // invio segnale alla rana per spostarsi quando il coccodrillo si muove
                        }

                        pos_c[i].x = coccodrillo.x;
                        pos_c[i].y = coccodrillo.y;
                        pos_c[i].proiettile = coccodrillo.proiettile;

                        // collisione coccodrilli con le pareti
                        if ((pos_c[i].x == maxx + 2 && pos_c[i].direzione == DESTRA) || (pos_c[i].x == minx - 4 && pos_c[i].direzione == SINISTRA))
                        {
                            kill(pos_c[i].pid, SIGUSR2); // segnale per indicare che il coccodrillo deve rispawnare in un' altra posizione
                        }

                        break;
                    }
                    /*
                    inizializzazione del coccodrillo in una posizione valida, essendo che ogni processo rimane fino alla fine
                    ogni posizione verrà occupata e non c'è bisogno di fare controlli aggiuntivi
                    */
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
                esiste = false;
                granata = valoreLetto;

                // controllo e aggiornamento delle granate
                for (int i = 0; i < MAXGRANATE; i++)
                {
                    if (pos_granate[i].pid == granata.pid_oggetto)
                    {
                        pos_granate[i].x = granata.x;
                        pos_granate[i].direzione = granata.direzione;

                        // collisione delle granate con i bordi
                        if ((pos_granate[i].x == maxx && pos_granate[i].direzione == DESTRA) || (pos_granate[i].x == 0 && pos_granate[i].direzione == SINISTRA))
                        {
                            if (pos_granate[i].pid != INVALID_PID) // eliminazione granata inviando alla rana un messaggio su pipe
                            {
                                if (write(pipeRana, &pos_granate[i].pid, sizeof(pid_t)) == -1)
                                {
                                    perror("Errore nella scrittura sulla pipe");
                                    _exit(6);
                                }
                                // la posizione della granata eliminata diventa valida e il count granate viene decrementato
                                pos_granate[i].pid = INVALID_PID;
                                countG--;
                            }
                        }

                        esiste = true; // la granata è presente nell'array
                        break;
                    }
                }
                // se nell'array non trova una granata con lo stesso pid la inserisce nell'array
                if (!esiste)
                {
                    for (int i = 0; i < MAXGRANATE; i++)
                    {
                        if (countG < MAXGRANATE)
                        {
                            if (pos_granate[i].pid == INVALID_PID)
                            {
                                countG++; // aumenta il count delle granate presenti
                                pos_granate[i].pid = granata.pid_oggetto;
                                pos_granate[i].x = granata.x;
                                pos_granate[i].y = granata.y;
                                break;
                            }
                        }
                        else
                        { // se per caso dovessero generarsi dei processi in più del dovuto vengono eliminati
                            t_posg.pid = granata.pid_oggetto;
                            if (t_posg.pid != INVALID_PID)
                            {
                                if (write(pipeRana, &t_posg.pid, sizeof(pid_t)) == -1)
                                {
                                    perror("Errore nella scrittura sulla pipe");
                                    _exit(6);
                                }
                            }

                            break;
                        }
                    }
                }
                break;

            case PROIETTILE_COCCODRILLO:
                proiettile = valoreLetto;
                esiste = false;
                // controllo e aggiornamento del proiettile in caso sia gia presente nell'array
                for (int i = 0; i < MAXCOCCODRILLI; i++)
                {
                    if (pos_proiettili[i].pid == proiettile.pid_oggetto)
                    {
                        pos_proiettili[i].x = proiettile.x;

                        // controllo collisioni tra proiettile e muro
                        if (((proiettile.x > maxx) && (proiettile.direzione == DESTRA)) || ((proiettile.x < 0) && (proiettile.direzione == SINISTRA)))
                        {
                            kill(pos_proiettili[i].proiettile, SIGUSR1); // invio un segnale a processo coccodrillo che distruggerà il proiettile

                            // la posizione del proiettile nell'array diventa valida e il count dei proiettili viene decrementato
                            pos_proiettili[i].pid = INVALID_PID;
                            countP--;
                        }

                        // il proiettile è presente nell'array
                        esiste = true;
                        break;
                    }
                }
                // caso in cui il proiettile non è presente nell'array, viene aggiunto nell'array
                if (!esiste)
                {
                    if (countP < MAXCOCCODRILLI) // controllo se si sta superando il limite, uccide il processo oltre il limite
                    {
                        for (int i = 0; i < MAXCOCCODRILLI; i++)
                        {
                            if (pos_proiettili[i].pid == INVALID_PID)
                            {
                                countP++; // aumenta il count dei proiettili presenti
                                pos_proiettili[i].pid = proiettile.pid_oggetto;
                                pos_proiettili[i].proiettile = proiettile.proiettile;
                                pos_proiettili[i].x = proiettile.x;
                                pos_proiettili[i].y = proiettile.y;
                                break;
                            }
                        }
                    }
                    else
                    {
                        kill(proiettile.proiettile, SIGUSR1); // se per qualche motivo il numero di proiettili è superiore al massimo, uccide il processo
                    }
                }
                break;
            default:
                break;
            }
        }

        /*Gestione collisione proiettili con granate */
        for (int i = 0; i < MAXGRANATE; i++)
        {
            if (pos_granate[i].pid != INVALID_PID)
            {
                for (int j = 0; j < MAXCOCCODRILLI; j++)
                {
                    if (pos_proiettili[j].pid != INVALID_PID)
                    {
                        if ((pos_granate[i].y == pos_proiettili[j].y) && (pos_granate[i].x == pos_proiettili[j].x))
                        {
                            // info della granata da eliminare
                            granata_eg.pid_oggetto = pos_granate[i].pid;
                            granata_eg.x = pos_granate[i].x;
                            granata_eg.y = pos_granate[i].y;
                            t_posg = pos_granate[i];

                            // info del proiettile da eliminare
                            proiettile_eg.pid_oggetto = pos_proiettili[j].pid;
                            proiettile_eg.x = pos_proiettili[j].x;
                            proiettile_eg.y = pos_proiettili[j].y;

                            beep();

                            // cancella graficamente i proiettili e le granate
                            cancellaProiettile(granata_eg);
                            cancellaProiettile(proiettile_eg);
                            wrefresh(gioco);

                            // reinizializzazione del pid delle posizioni per permet
                            pos_granate[i].pid = INVALID_PID;
                            pos_proiettili[j].pid = INVALID_PID;
                            granata_eg.pid_oggetto = INVALID_PID;
                            proiettile_eg.pid_oggetto = INVALID_PID;
                            proiettile.pid_oggetto = INVALID_PID;
                            granata.pid_oggetto = INVALID_PID;

                            // decremento del contatore delle granate e dei proiettili
                            countP--;
                            countG--;

                            // viene inviato un segnale al coccodrillo per eliminare il proiettile dopo la collisione
                            if (pos_proiettili[j].proiettile != INVALID_PID)
                            {
                                if (kill(pos_proiettili[j].proiettile, SIGUSR1) == -1)
                                {
                                    perror("Errore nell'inviare il segnale SIGUSR1 al processo proiettile");
                                }
                            }
                            // viene inviato un messaggio attraverso la pipe della rana per eliminare la granata dopo la collisione
                            if (t_posg.pid != INVALID_PID)
                            {
                                if (write(pipeRana, &t_posg.pid, sizeof(pid_t)) == -1)
                                {
                                    perror("Errore nella scrittura sulla pipe");
                                    _exit(6);
                                }
                                waitpid(t_posg.pid, NULL, 0);
                            }
                            break;
                        }
                    }
                }
            }
        }

        // stampa delle sprite
        if (coccodrillo.pid_oggetto > 0)
            stampaSprite(coccodrillo);

        if (rana.pid_oggetto > 0)
            stampaSprite(rana);

        if (granata.pid_oggetto > 0)
            stampaSprite(granata);

        if (proiettile.pid_oggetto > 0)
            stampaSprite(proiettile);

        // stampe di debug

        // stampa dei pid delle granate e del proiettile (impostando un coccodrillo a schermo)
        // mvwprintw(gioco, 2, 3, "pid_proiettile:  %6d", pos_proiettili[0].pid);
        // mvwprintw(gioco, 3, 3, "pid_granata s?:  %6d", pos_granate[0].pid);
        // mvwprintw(gioco, 4, 3, "pid_granata d?:  %6d", pos_granate[1].pid);

        // stampa il numero di granate e proiettili presenti
        // mvwprintw(gioco, 2, 3, "numG:  %2d", countG);
        // mvwprintw(gioco, 3, 3, "numP:  %2d", countP);

        // stampa il numero di proiettili della rana (il valore che è presente dentro rana.c)
        // mvwprintw(gioco, 3, 3, "numProiettiliRana:  %2d", rana.proiettile);
        wrefresh(gioco);

        // controllo interazione tane

        // se la rana è nella y delle tane
        if (pos_r.y == posTane[0].y)
        {
            chiusuraFineManche(pos_c, pos_granate, pos_proiettili, pipeRana, rana.pid_oggetto, pipein);

            // controllo se la rana ha preso danno nell'erba intorno alle tane
            if (pos_r.x != posTane[0].x && pos_r.x != posTane[1].x && pos_r.x != posTane[2].x && pos_r.x != posTane[3].x && pos_r.x != posTane[4].x)
            {
                exit(6);
            }

            // controllo se la rana è in una delle tane, se la tana è libera esce con un determianto stato altrimenti prende danno ed esce con un altro stato

            if (pos_r.x == posTane[0].x)
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

            if (pos_r.x == posTane[1].x)
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

            if (pos_r.x == posTane[2].x)
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

            if (pos_r.x == posTane[3].x)
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

            if (pos_r.x == posTane[4].x)
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

        // controllo collisione tra rana e proiettile
        for (int i = 0; i < MAXCOCCODRILLI; i++)
        {
            if ((pos_proiettili[i].y == pos_r.y) && ((pos_proiettili[i].x == pos_r.x) || (pos_proiettili[i].x == pos_r.x + 1)))
            {
                if (pos_proiettili[i].pid != INVALID_PID)
                {
                    danno = false;
                    break;
                }
            }
        }

        // controllo gestione del danno, se ha preso danno chiude la manche ed esce con un determinato stato
        if (!danno)
        {
            chiusuraFineManche(pos_c, pos_granate, pos_proiettili, pipeRana, rana.pid_oggetto, pipein);
            exit(6);
        }

    } while (true);
}

// funzione per la chiusura dei processi dopo una manche
void chiusuraFineManche(posizione pos_c[], posizione pos_granate[], posizione pos_proiettili[], int pipeRana, pid_t pid_rana, int pipein)
{
    for (int i = 0; i < MAXCOCCODRILLI; i++)
    {
        if (pos_c[i].pid != INVALID_PID)
        {
            kill(pos_c[i].pid, SIGUSR1);
            chiudiProcessi(pos_c[i].pid);
        }

        if (pos_proiettili[i].pid != INVALID_PID)
        {
            pos_proiettili[i].pid = INVALID_PID;
        }
    }
    for (int i = 0; i < MAXGRANATE; i++)
    {
        if (pos_granate[i].pid != INVALID_PID)
        {
            if (write(pipeRana, &pos_granate[i].pid, sizeof(pid_t)) == -1)
            {
                perror("Errore nella scrittura sulla pipe");
                _exit(6);
            }
        }
    }

    if (pid_rana != INVALID_PID)
    {
        chiudiProcessi(pid_rana);
    }
    close(pipeRana);
    close(pipein);
}