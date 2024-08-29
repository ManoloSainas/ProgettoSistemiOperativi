#include "main.h"

// Function to initialize a pipe
void inizializzazionePipe(int filedes[])
{
    if (pipe(filedes) == -1)
    {
        perror("Errore nella creazione della pipe");
        _exit(1);
    }
}

Posizione genera_posizione_fiume(StatoGioco *stato, InformazioniFiume *infoFiume)
{
    Posizione nuova;
    int i, id;
    /*~ Come generare la y:
     * Genera una y
     * Se non ci sono troppi coccodrilli, allora va bene,
     * generane un'altra altrimenti
     */

    short fl = rand() % 9;
    while (infoFiume->info_flusso[fl].presenti >= NUM_MAX_COCCODRILLI_FLUSSO)
        fl = rand() % 9;

    nuova.y = PRIMO_FLUSSO + fl; /* CHECKME */

    /*~ Come generare la x:
     * Per ogni coccodrillo in un flusso, controlla la sua posizione
     * Calcola lo spazio piu' grande possibile che consente di alloggiare un coccodrillo
     * Genera randomicamente la x entro quello spazio
     * Assegna la x a quel messaggio
     */

    /* Due casistiche: flusso senza coccodrilli, e flusso con dei coccodrilli */
    if (infoFiume->info_flusso[fl].presenti == 0)
    {
        nuova.x = (rand() % (maxx - COLONNE_SPRITE_COCCODRILLO)) + COLONNE_SPRITE_COCCODRILLO;
    }
    else
    {
        int left = 0, right;
        id = infoFiume->info_flusso[fl].id_coccodrilli[0];

        for (i = 0; i < infoFiume->info_flusso[fl].presenti; i++)
        {
            /* ATTENZIONE: suppone che se un ID viene eliminato, gli ID vengano spostati tutti
             * di una posizione indietro.
             * Per rendere la cosa più efficiente servono le liste
             */
            id = infoFiume->info_flusso[fl].id_coccodrilli[i];

            /* Selezione della coordinata destra: quando si raggiunge l'ultimo
             * coccodrillo, la coodrinata destra deve essere per forza il bordo destro
             * dello schermo
             */
            right = i + 1 < infoFiume->info_flusso[fl].presenti
                        ? stato->coccodrilli[id].x
                        : maxx;

            if ((right - left) <= (COLONNE_SPRITE_COCCODRILLO * 2) + 1)
            {
                nuova.x = rand() % (COLONNE_SPRITE_COCCODRILLO) + 1; /* Almeno uno spazio a sinistra */
                break;
            }

            /* Aggiorna */
            left = right;

            /* Side note: se non trova uno spazio, deve rigenerare un'altra y */
        }
    }
    /*fprintf(stderr, "NUOVA X: %d\n", nuova.x);
    fprintf(stderr, "NUOVA Y: %d\n", nuova.y);
    fflush(stderr);*/

    /* Inserire l'id del coccodrillo da inserire "in mezzo" agli altri nell'array.
     * Copia, poi inserisci.
     * Side effect
     */
    int old_i = i;
    if (i < infoFiume->info_flusso[fl].presenti)
    {
        for (; i < infoFiume->info_flusso[fl].presenti - 1; i++)
        {
            infoFiume->info_flusso[fl].id_coccodrilli[i + 1] = infoFiume->info_flusso[fl].id_coccodrilli[i];
        }
        infoFiume->info_flusso[fl].id_coccodrilli[old_i] = id;
    }
}

void avviaGioco()
{
    wrefresh(gioco);

    int pipe_padre[2]; // File descriptors for the pipe
    int pid_gioco;     // Process ID for the game processes
    StatoGioco stato;

    initOggetto(&(stato.rana));

    for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
        initOggetto(&(stato.proiettiliRana[i]));

    for (int i = 0; i < NUM_PIANTE; i++)
        initOggetto(&(stato.piante[i]));

    for (int i = 0; i < NUM_PIANTE; i++)
        initOggetto(&(stato.proiettiliPianta[i]));

    for (int i = 0; i < NUM_TOT_COCCODRILLI; i++)
        initOggetto(&(stato.coccodrilli[i]));

    inizializzazionePipe(pipe_padre);

    // Create process for the frog
    pid_gioco = fork();
    switch (pid_gioco)
    {
    case -1:
        perror("Errore nella creazione del processo per la rana");
        _exit(1);
        break;
    case 0:
        close(pipe_padre[LETTURA]);
        rana(pipe_padre[SCRITTURA]);
        break;
    }

#ifdef DISABLE
    /* Crea i processi per le piante */
    for (int i = 0; i < NUM_PIANTE; i++)
    {
        pid_gioco = fork();
        switch (pid_gioco)
        {
        case -1:
            perror("Errore nella creazione del processo per la pianta");
            _exit(1);
            break;
        case 0:
            close(pipe_padre[LETTURA]);
            srand(time(NULL) + i); // Unique seed for each plant
            pianta(pipe_padre[SCRITTURA], i, posizionePianteX[i]);
            break;
        }
    }
#endif

    for (int i = 0; i < NUM_TOT_COCCODRILLI; i++)
    {
        inizializzazionePipe(stato.pipe_coccodrilli[i]);

        pid_gioco = fork();

        switch (pid_gioco)
        {
        case -1:
            perror("Errore nella creazione del processo per il coccodrillo");
            _exit(1);
            break;
        case 0:
            close(pipe_padre[LETTURA]);
            close(stato.pipe_coccodrilli[i][SCRITTURA]);
            srand(time(NULL) + i); // Unique seed for each crocodile

            coccodrillo(pipe_padre[SCRITTURA], stato.pipe_coccodrilli[i][LETTURA], i, &infoFiume);
            /* Not reached */
            break;
        default:
            close(stato.pipe_coccodrilli[i][LETTURA]);
            break;
        }
    }

    /* Chiudi il file descriptor in scrittura */
    close(pipe_padre[SCRITTURA]);

    /* Controlla tutti i coccodrilli finora generati (quindi fino la i corrente),
     * e vedi dove sono spawnati.
     * Decidi un flusso casuale dove farlo spawnare, e dagli la posizione.
     */
    for (int i = 0; i < NUM_TOT_COCCODRILLI; i++)
    {
        Messaggio msg;
        msg.codice = SVEGLIA;
        msg.posizione = genera_posizione_fiume(&stato, &infoFiume);
        write(stato.pipe_coccodrilli[i][SCRITTURA], &msg, sizeof(msg));
    }

    controlloGioco(pipe_padre[LETTURA], &stato);

    // Termina gioco
    terminaGioco();
}

// Function to control the game, print objects, and check for collisions
void controlloGioco(int pipein, StatoGioco *stato)
{
    int viteRana = NUM_VITE_RANA;      // Initialize frog lives
    int tempoRimanente = TEMPO_TOTALE; // Initialize timer
    time_t inizioTempo = time(NULL);   // Store start time

    oggetto valoreLetto; // Will contain the value read from the pipe

    do
    {
        // Update the timer
        time_t tempoAttuale = time(NULL);
        if (difftime(tempoAttuale, inizioTempo) >= 1) // If a second has passed
        {
            tempoRimanente--;           // Decrement the counter
            inizioTempo = tempoAttuale; // Update the start time

            // If time has run out
            if (tempoRimanente <= 0)
            {
                viteRana--; // Decrement frog lives
                // Reset the timer
                tempoRimanente = TEMPO_TOTALE;
            }
        }

        // Read from the pipe
        if (read(pipein, &valoreLetto, sizeof(valoreLetto)) > 0)
        {
            switch (valoreLetto.tipo)
            {
            case RANA:
                stato->rana = valoreLetto;
                break;
            case PROIETTILE_RANA:
                stato->proiettiliRana[valoreLetto.index] = valoreLetto;
                break;
            case PIANTA:
                stato->piante[valoreLetto.index] = valoreLetto;
            case PROIETTILE_PIANTA:
                stato->proiettiliPianta[valoreLetto.index] = valoreLetto;
            case COCCODRILLO_BUONO:
            case COCCODRILLO_CATTIVO:
            case COCCODRILLO_IMMERSIONE:
                stato->coccodrilli[valoreLetto.index] = valoreLetto;
                // fprintf(stderr, "X: %d\n", valoreLetto.x);
                {
                    Messaggio risposta;
                    risposta.codice = CONT;
                    // if (stato->coccodrilli[valoreLetto.index].status == ATTIVO)
                    if (stato->coccodrilli[valoreLetto.index].status == SOSPESO)
                    {
                        int idx;
                        int dove = PRIMO_FLUSSO - stato->coccodrilli[valoreLetto.index].y;
                        int split;
                        /* TODO: controllare se la rana `e sopra il coccodrillo,
                         * nel caso togliere una vita e farla respawnare
                         */

                        /* Eliminare l'indice del coccodrillo dalle info del fiume */
                        if (valoreLetto.index == 0)
                            idx = 0;
                        else if (valoreLetto.index == infoFiume.info_flusso[dove].presenti - 1)
                        {
                            idx = infoFiume.info_flusso[dove].presenti - 1;
                        }
                        else
                        { /* Ne' a sinistra ne' a destra*/
                            for (int i = 1; i < infoFiume.info_flusso[dove].presenti - 1; i++)
                                if (valoreLetto.index == infoFiume.info_flusso[dove].id_coccodrilli[i])
                                {
                                    idx = i;
                                    break;
                                }
                        }

                        /* Ricopia i successivi */
                        for (; idx + 1 < infoFiume.info_flusso[dove].presenti; idx++)
                        {
                            infoFiume.info_flusso[dove].id_coccodrilli[idx] =
                                infoFiume.info_flusso[dove].id_coccodrilli[idx + 1];
                        }

                        risposta.codice = SVEGLIA;
                        /* Genera una nuova posizione per il coccodrillo se `e sospeso
                         * (si `e sommerso o 'e uscito dallo schermo)
                         */
                        risposta.posizione = genera_posizione_fiume(stato, &infoFiume);
                    }
                    /*~ Invia risposta al coccodrillo */
                    write(stato->pipe_coccodrilli[valoreLetto.index][SCRITTURA], &risposta, sizeof(risposta));
                }
            }
            cancellaSprite(valoreLetto);

            // Print the background and all objects
            graficaGioco(viteRana, tempoRimanente);

            // Display the objects

            for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
            {
                if (stato->proiettiliRana[i].status != TERMINATO)
                    stampaSprite(stato->proiettiliRana[i]);
            }
            for (int i = 0; i < NUM_PIANTE; i++)
            {
                if (stato->piante[i].status != SOSPESO)
                    stampaSprite(stato->piante[i]);
            }
            for (int i = 0; i < NUM_PIANTE; i++)
            {
                if (stato->proiettiliPianta[i].status != SOSPESO)
                    stampaSprite(stato->proiettiliPianta[i]);
            }
            for (int i = 0; i < NUM_TOT_COCCODRILLI; i++)
            {
                // if (stato->coccodrilli[i].status != SOSPESO)
                stampaSprite(stato->coccodrilli[i]);
            }

            /* La rana non viene mai terminata, quindi il redraw `e continuo */
            stampaSprite(stato->rana);

            curs_set(0);
            wrefresh(gioco);
        }

    } while (viteRana > 0); // Continue while there are lives

    chiudiProcessi(stato->proiettiliRana, &(stato->rana), stato->piante, stato->proiettiliPianta, stato->coccodrilli);
}

// Function to terminate the game
void terminaGioco()
{
    delwin(gioco);           // Delete the window
    system("killall aplay"); // Terminate
    endwin();                // End ncurses
}
