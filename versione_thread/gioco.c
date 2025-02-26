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
void avviaGioco(bool tana_status[], int punteggio, int vita)
{
    int tempoRimanente = TEMPO_TOTALE; // Inizializzazione del tempo rimanente

    int coccodrilli_flusso[NUM_FLUSSI_FIUME] = {NUM_COCCODRILLI_FLUSSO}; // numero di coccodrilli per flusso

    // inizializzazione grafica
    graficaGioco(tana_status, punteggio, vita);

    // inizialiizzazione flussi del fiume
    gestioneFlussi(flussi, coccodrilli_flusso);

    pthread_t rana_id, coccodrilli[MAXCOCCODRILLI];
    info_elemento infococco[MAXCOCCODRILLI];

    void *info_void;
    pthread_create(&rana_id, NULL, &rana, NULL);

    // Prima facciamo funzionare rana, granate rana, tane e manche -> poi attiviamo anche coccodrilli e proiettili

    // int count = 0;
    // for (int i = 1; i <= NUM_FLUSSI_FIUME; i++)
    // {
    //     for (int j = 1; j <= NUM_COCCODRILLI_FLUSSO; j++)
    //     {

    //         infococco[count].direzione = flussi[i - 1].direzione;
    //         infococco[count].speed = flussi[i - 1].velocita;
    //         infococco[count].y = maxy - i - 2;
    //         int x_cocco = (infococco[count].direzione == DESTRA) ? (minx - 1) : maxx - 1;
    //         infococco[count].x = j; // info cocco usato per  dare al coccodrillo informazioni sull'attesa
    //         info_void = &infococco[count];
    //         // thread coccodrilli
    //         pthread_create(&coccodrilli[count], NULL, &coccodrillo, info_void);
    //         count++;
    //     }
    // }
}

// processo che riceve le coordinate e controlla le collisioni
int controlloGioco(int vita, bool tana_status[], int tempoRimanente, int punteggio)
{
    controllo = true;
    avviaGioco(tana_status, punteggio, vita);
    time_t inizioTempo = time(NULL); // Inizializzazione del tempo di inizio

    // elementi per la gestione delle granate e dei proiettili dopo la collisione
    elementoGioco granata_eg, proiettile_eg;

    // liste e variabili per salvare le posizioni dei vari elementi, t_posg è per salvare la granata per l'eliminazione dopo la collisione
    posizione pos_r, pos_c[MAXCOCCODRILLI], pos_granate[MAXGRANATE], pos_proiettili[MAXCOCCODRILLI], t_posg;

    elementoGioco valoreLetto;                            // elemento letto dalla pipe
    elementoGioco rana, coccodrillo, granata, proiettile; // elementi del gioco

    int score = 0;
    pthread_t coccodrillo_rana; // identificatore del coccodrillo cavalcato dalla rana
    bool danno, esiste;         // flag gestione collisione e esistenza negli array, danno->false danno ricevuto, danno->true niente danno
    int countG = 0, countP = 0; // contatori per gestione corretta del numero di processi e debugging

    // inizializzazione posizione rana
    pos_r.y = RANA_Y;
    pos_r.x = RANA_X;

    // inizializzazione elementi del gioco
    rana.thread_oggetto = INVALID_THREAD;
    coccodrillo.thread_oggetto = INVALID_THREAD;
    granata.thread_oggetto = INVALID_THREAD;
    proiettile.thread_oggetto = INVALID_THREAD;

    // reset di granata_eg e di proiettile_eg
    granata_eg.thread_oggetto = INVALID_THREAD;
    proiettile_eg.thread_oggetto = INVALID_THREAD;

    // inizializzazione liste di posizione
    // INVALID_THREAD è un identificativo che serve per distinguere un oggetto attivo da uno eliminato o vuoto
    for (int i = 0; i < MAXCOCCODRILLI; i++) // ogni coccodrillo è associata un proiettile quindi maxproiettili=maxcoccodrilli
    {
        pos_c[i].thread_id = INVALID_THREAD;
        pos_c[i].x = -1;
        pos_c[i].y = -1;
        pos_proiettili[i].thread_id = INVALID_THREAD;
        pos_proiettili[i].x = -1;
        pos_proiettili[i].y = -1;
    }
    for (int i = 0; i < MAXGRANATE; i++)
    {
        pos_granate[i].thread_id = INVALID_THREAD;
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
            chiusuraFineManche(pos_c, pos_proiettili, pos_granate, rana.thread_oggetto);
            return 6;
        }

        danno = true; // danno con l'acqua -> false

        // controllo collisione acqua
        coccodrillo_rana = INVALID_THREAD;
        for (int i = 0; i < MAXCOCCODRILLI; i++)
        {
            if (pos_r.y == pos_c[i].y && pos_c[i].thread_id != INVALID_THREAD)
            {
                if (pos_c[i].direzione == SINISTRA)
                {
                    if (pos_r.x >= pos_c[i].x - 1 && pos_r.x <= pos_c[i].x + 3)
                    {
                        coccodrillo_rana = pos_c[i].thread_id; // aggiornamento del coccodrillo cavalcato
                        danno = true;
                        break;
                    }
                }
                else
                {
                    if (pos_r.x < pos_c[i].x + 1 && pos_r.x > pos_c[i].x - 5)
                    {
                        coccodrillo_rana = pos_c[i].thread_id; // aggiornamento del coccodrillo cavalcato
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
        // mettere controllo thread

        {
            // cancella gli sprite
            cancellaSprite(rana);
            cancellaSprite(coccodrillo);
            cancellaSprite(granata);
            cancellaSprite(proiettile);

            wait_consumatore();
            valoreLetto = lista_elementi[out];
            out = (out + 1) % DIM_BUFFER;
            signal_consumatore();
            // aggiorna le posizioni e inserimento negli array di posizione
            switch (valoreLetto.tipo)
            {
            case RANA:
                rana = valoreLetto;
                pos_r.x = rana.x;
                pos_r.y = rana.y;
                pos_r.controllo = rana.controllo;
                break;
            case COCCODRILLO:
                coccodrillo = valoreLetto;

                for (int i = 0; i < MAXCOCCODRILLI; i++)
                {
                    if (pos_c[i].thread_id == coccodrillo.thread_oggetto)
                    {
                        // se il coccodrillo è cavalcato dalla rana
                        if (pos_c[i].thread_id == coccodrillo_rana)
                        {
                            // inserire spostamento rana
                        }

                        pos_c[i].x = coccodrillo.x;
                        pos_c[i].proiettile = coccodrillo.proiettile;
                        pos_c[i].controllo = coccodrillo.controllo;
                        // collisione coccodrilli con le pareti
                        if ((pos_c[i].x == maxx + 2 && pos_c[i].direzione == DESTRA) || (pos_c[i].x == minx - 4 && pos_c[i].direzione == SINISTRA))
                        {
                            // mettere la x come puntatore?
                        }

                        break;
                    }
                    /*
                    inizializzazione del coccodrillo in una posizione valida, essendo che ogni processo rimane fino alla fine
                    ogni posizione verrà occupata e non c'è bisogno di fare controlli aggiuntivi
                    */
                    if (pos_c[i].thread_id == INVALID_THREAD)
                    {
                        pos_c[i].thread_id = coccodrillo.thread_oggetto;
                        pos_c[i].x = coccodrillo.x;
                        pos_c[i].y = coccodrillo.y;
                        pos_c[i].direzione = coccodrillo.direzione;
                        pos_c[i].proiettile = coccodrillo.proiettile;
                        pos_c[i].controllo = coccodrillo.controllo;
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
                    if (pos_granate[i].thread_id == granata.thread_oggetto)
                    {
                        pos_granate[i].x = granata.x;
                        pos_granate[i].direzione = granata.direzione;

                        // collisione delle granate con i bordi
                        if ((pos_granate[i].x == maxx && pos_granate[i].direzione == DESTRA) || (pos_granate[i].x == 0 && pos_granate[i].direzione == SINISTRA))
                        {
                            if (pos_granate[i].thread_id != INVALID_THREAD) // eliminazione granata inviando alla rana un messaggio su pipe
                            {

                                *pos_granate[i].controllo = false;
                                pthread_cancel(pos_granate[i].thread_id);
                                pthread_join(pos_granate[i].thread_id, NULL);
                                // la posizione della granata eliminata diventa valida e il count granate viene decrementato
                                pos_granate[i].thread_id = INVALID_THREAD;
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
                            if (pos_granate[i].thread_id == INVALID_THREAD)
                            {
                                countG++; // aumenta il count delle granate presenti
                                pos_granate[i].thread_id = granata.thread_oggetto;
                                pos_granate[i].x = granata.x;
                                pos_granate[i].y = granata.y;
                                pos_granate[i].controllo = granata.controllo;
                                break;
                            }
                        }
                        else
                        { // se per caso dovessero generarsi dei processi in più del dovuto vengono eliminati
                            t_posg.thread_id = granata.thread_oggetto;
                            if (t_posg.thread_id != INVALID_THREAD)
                            {
                                pthread_cancel(t_posg.thread_id);
                                pthread_join(t_posg.thread_id, NULL);
                                pos_granate[i].thread_id = INVALID_THREAD;
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
                    if (pos_proiettili[i].thread_id == proiettile.thread_oggetto)
                    {
                        pos_proiettili[i].x = proiettile.x;

                        // controllo collisioni tra proiettile e muro
                        if (((proiettile.x > maxx) && (proiettile.direzione == DESTRA)) || ((proiettile.x < 0) && (proiettile.direzione == SINISTRA)))
                        {
                            // inserire killthread
                            *pos_proiettili[i].controllo = false;
                            pthread_cancel(pos_proiettili[i].thread_id);
                            pthread_join(pos_proiettili[i].thread_id, NULL);
                            pos_proiettili[i].thread_id = INVALID_THREAD;
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
                            if (pos_proiettili[i].thread_id == INVALID_THREAD)
                            {
                                countP++; // aumenta il count dei proiettili presenti
                                pos_proiettili[i].thread_id = proiettile.thread_oggetto;
                                pos_proiettili[i].proiettile = proiettile.proiettile;
                                pos_proiettili[i].x = proiettile.x;
                                pos_proiettili[i].y = proiettile.y;
                                pos_proiettili[i].controllo = proiettile.controllo;
                                break;
                            }
                        }
                    }
                    else
                    {
                        // inserire kill thread proiettile
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
            if (pos_granate[i].thread_id != INVALID_THREAD)
            {
                for (int j = 0; j < MAXCOCCODRILLI; j++)
                {
                    if (pos_proiettili[j].thread_id != INVALID_THREAD)
                    {
                        if ((pos_granate[i].y == pos_proiettili[j].y) && (pos_granate[i].x == pos_proiettili[j].x))
                        {
                            // info della granata da eliminare
                            granata_eg.thread_oggetto = pos_granate[i].thread_id;
                            granata_eg.x = pos_granate[i].x;
                            granata_eg.y = pos_granate[i].y;
                            t_posg = pos_granate[i];

                            // info del proiettile da eliminare
                            proiettile_eg.thread_oggetto = pos_proiettili[j].thread_id;
                            proiettile_eg.x = pos_proiettili[j].x;
                            proiettile_eg.y = pos_proiettili[j].y;

                            beep();

                            // cancella graficamente i proiettili e le granate
                            cancellaProiettile(granata_eg);
                            cancellaProiettile(proiettile_eg);
                            wrefresh(gioco);

                            // decremento del contatore delle granate e dei proiettili
                            countP--;
                            countG--;

                            // viene inviato un segnale al coccodrillo per eliminare il proiettile dopo la collisione
                            if (pos_proiettili[j].proiettile != INVALID_THREAD)
                            {
                                *pos_proiettili[j].controllo = false;
                                pthread_cancel(pos_proiettili[j].proiettile);
                                pthread_join(pos_proiettili[j].thread_id, NULL);
                            }
                            // viene inviato un messaggio attraverso la pipe della rana per eliminare la granata dopo la collisione
                            if (pos_granate[i].thread_id != INVALID_THREAD)
                            {
                                *pos_granate[i].controllo = false;
                                pthread_cancel(pos_granate[i].thread_id);
                                pthread_join(pos_granate[i].thread_id, NULL);
                            }

                            pos_granate[i].thread_id = INVALID_THREAD;
                            pos_proiettili[j].thread_id = INVALID_THREAD;
                            granata_eg.thread_oggetto = INVALID_THREAD;
                            proiettile_eg.thread_oggetto = INVALID_THREAD;
                            proiettile.thread_oggetto = INVALID_THREAD;
                            granata.thread_oggetto = INVALID_THREAD;

                            break;
                        }
                    }
                }
            }
        }

        // stampa delle sprite
        // if (coccodrillo.thread_oggetto > 0)
        //  stampaSprite(coccodrillo);

        if (rana.thread_oggetto > 0)
            stampaSprite(rana);

        // if (granata.thread_oggetto > 0)
        //   stampaSprite(granata);

        // if (proiettile.thread_oggetto > 0)
        //  stampaSprite(proiettile);

        // stampe di debug

        // stampa dei pid delle granate e del proiettile (impostando un coccodrillo a schermo)
        // mvwprintw(gioco, 2, 3, "pid_proiettile:  %6d", pos_proiettili[0].thread_id);
        // mvwprintw(gioco, 3, 3, "pid_granata s?:  %6d", pos_granate[0].thread_id);
        // mvwprintw(gioco, 4, 3, "pid_granata d?:  %6d", pos_granate[1].thread_id);

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
            // chiusura o reset dei thread
            chiusuraFineManche(pos_c, pos_proiettili, pos_granate, rana.thread_oggetto);
            // controllo se la rana ha preso danno nell'erba intorno alle tane
            if (pos_r.x != posTane[0].x && pos_r.x != posTane[1].x && pos_r.x != posTane[2].x && pos_r.x != posTane[3].x && pos_r.x != posTane[4].x)
            {
                return (6);
            }

            // controllo se la rana è in una delle tane, se la tana è libera esce con un determianto stato altrimenti prende danno ed esce con un altro stato

            if (pos_r.x == posTane[0].x)
            {
                if (tana_status[0])
                {
                    return (1);
                }
                else
                {
                    return (6);
                }
            }

            if (pos_r.x == posTane[1].x)
            {
                if (tana_status[1])
                {
                    return (2);
                }
                else
                {
                    return (6);
                }
            }

            if (pos_r.x == posTane[2].x)
            {
                if (tana_status[2])
                {
                    return (3);
                }
                else
                {
                    return (6);
                }
            }

            if (pos_r.x == posTane[3].x)
            {
                if (tana_status[3])
                {
                    return (4);
                }
                else
                {
                    return (6);
                }
            }

            if (pos_r.x == posTane[4].x)
            {
                if (tana_status[4])
                {
                    return (5);
                }
                else
                {
                    return (6);
                }
            }
        }

        // controllo collisione tra rana e proiettile
        for (int i = 0; i < MAXCOCCODRILLI; i++)
        {
            if ((pos_proiettili[i].y == pos_r.y) && ((pos_proiettili[i].x == pos_r.x) || (pos_proiettili[i].x == pos_r.x + 1)))
            {
                if (pos_proiettili[i].thread_id != INVALID_THREAD)
                {
                    danno = false;
                    break;
                }
            }
        }

        // controllo gestione del danno, se ha preso danno chiude la manche ed esce con un determinato stato
        if (!danno)
        {
            chiusuraFineManche(pos_c, pos_proiettili, pos_granate, rana.thread_oggetto);
            return (6);
        }

    } while (true);
}

// funzione per la chiusura dei thread dopo una manche
void chiusuraFineManche(posizione pos_c[], posizione pos_proiettili[], posizione pos_granate[], pthread_t id_rana)
{
    controllo = false;

    if (id_rana != INVALID_THREAD && id_rana != 0)
    {
        pthread_cancel(id_rana);
        pthread_join(id_rana, NULL);
    }

    for (int i = 0; i < MAXCOCCODRILLI; i++)
    {
        if (pos_c[i].thread_id != INVALID_THREAD && pos_c[i].thread_id != 0)
        {
            pthread_cancel(pos_c[i].thread_id);
            pthread_join(pos_c[i].thread_id, NULL);
        }
        if (pos_proiettili[i].thread_id != INVALID_THREAD && pos_proiettili[i].thread_id != 0)
        {
            pthread_cancel(pos_proiettili[i].thread_id);
            pthread_join(pos_proiettili[i].thread_id, NULL);
        }
    }
    for (int i = 0; i < MAXGRANATE; i++)
    {
        if (pos_granate[i].thread_id != INVALID_THREAD && pos_granate[i].thread_id != 0)
        {
            pthread_cancel(pos_granate[i].thread_id);
            pthread_join(pos_granate[i].thread_id, NULL);
        }
    }

    dealloca_meccanismi_sincronizzazione();
}