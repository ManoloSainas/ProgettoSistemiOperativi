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

void avviaGioco()
{
    wrefresh(gioco);

    int filedes[2]; // File descriptors for the pipe
    int pid_gioco;  // Process ID for the game processes
    StatoGioco stato;

    inizializzazionePipe(filedes);

    // Create process for the frog
    pid_gioco = fork();
    switch (pid_gioco)
    {
    case -1:
        perror("Errore nella creazione del processo per la rana");
        _exit(1);
        break;
    case 0:
        close(filedes[LETTURA]);
        rana(filedes[SCRITTURA]);
        _exit(0);
        break;
    default:
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
                close(filedes[LETTURA]);
                srand(time(NULL) + i); // Unique seed for each plant
                pianta(filedes[SCRITTURA], i, posizionePianteX[i]);
                _exit(0);
                break;
            }
        }

        for (int i = 0; i < NUM_TOT_COCCODRILLI; i++)
        {
            pid_gioco = fork();

            inizializzaPipe(stato.pipe_coccodrilli[i]);

            switch (pid_gioco)
            {
            case -1:
                perror("Errore nella creazione del processo per il coccodrillo");
                _exit(1);
                break;
            case 0:
                close(filedes[LETTURA]);
                close(stato.pipe_coccodrilli[i][SCRITTURA]);
                srand(time(NULL) + i); // Unique seed for each crocodile

                coccodrillo(filedes[SCRITTURA], stato.pipe_coccodrilli[i][LETTURA], i, &infoFiume);

                _exit(0);
                break;
            default:
                /* Controlla tutti i coccodrilli finora generati (quindi fino la i corrente),
                 * e vedi dove sono spawnati.
                 * Decidi un flusso casuale dove farlo spawnare, e dagli la posizione.
                 */
                Messaggio msg;
                msg.codice = SVEGLIA;

                /*~ Come generare la y:
                 * Genera una y
                 * Se non ci sono troppi coccodrilli, allora va bene,
                 * generane un'altra altrimenti
                 */

                msg.posizione.y = maxy - 9 + rand() % 9;
                while (infoFiume.numeroCoccodrilliFlussi[msg.posizione.y - (maxy - 9)] >= NUM_MAX_COCCODRILLI_FLUSSO)
                {
                    msg.posizione.y = maxy - 9 + rand() % 9;
                }

                /*~ Come generare la x:
                 * Per ogni coccodrillo in un flusso, controlla la sua posizione
                 * Calcola lo spazio piu' grande possibile che consente di alloggiare un coccodrillo
                 * Genera randomicamente la x entro quello spazio
                 * Assegna la x a quel messaggio
                 */

                close(stato.pipe_coccodrilli[i][LETTURA]);
                write(stato.pipe_coccodrilli[i][SCRITTURA], &msg, sizeof(msg));
                break;
            }
        }
        close(filedes[SCRITTURA]);
        controlloGioco(filedes[LETTURA], &stato);
        break;
    }
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

    initOggetto(&rana);

    for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
    {
        initOggetto(&(stato->proiettiliRana[i])); /// E' GIUSTO??????????????????????????????????????????????????????????
    }

    for (int i = 0; i < NUM_PIANTE; i++)
    {
        initOggetto(&(stato->piante[i]));
    }

    for (int i = 0; i < NUM_PIANTE; i++)
    {
        initOggetto(&(stato->proiettiliPianta[i]));
    }

    for (int i = 0; i < NUM_TOT_COCCODRILLI; i++)
    {
        initOggetto(&(stato->coccodrilli[i]));
    }

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

            oggetto oggettoLetto;
            switch (valoreLetto.tipo)
            {
            case RANA:
                if (stato->rana.status == ATTIVO)
                    oggettoLetto = stato->rana;
            case PROIETTILE_RANA:
                if (stato->proiettiliRana[valoreLetto.index].status == ATTIVO)
                    oggettoLetto = stato->proiettiliRana[valoreLetto.index];
            case PIANTA:
                if (stato->piante[valoreLetto.index].status == ATTIVO)
                    oggettoLetto = stato->piante[valoreLetto.index];
            case PROIETTILE_PIANTA:
                if (stato->proiettiliPianta[valoreLetto.index].status == ATTIVO)
                    oggettoLetto = stato->proiettiliPianta[valoreLetto.index];
            case COCCODRILLO_BUONO:
            case COCCODRILLO_CATTIVO:
            case COCCODRILLO_IMMERSIONE:
                if (stato->coccodrilli[valoreLetto.index].status == ATTIVO)
                    oggettoLetto = stato->coccodrilli[valoreLetto.index];
                /*~ Se lo stato del coccodrillo è sospeso, allora genera una nuova posizione
                 * (copia-incolla), e scrivi nella pipe
                 */
            }
            cancellaSprite(oggettoLetto);

            // Update the object array
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
                break;
            case PROIETTILE_PIANTA:
                stato->proiettiliPianta[valoreLetto.index] = valoreLetto;
                break;
            case COCCODRILLO_BUONO:
            case COCCODRILLO_CATTIVO:
            case COCCODRILLO_IMMERSIONE:
                stato->coccodrilli[valoreLetto.index] = valoreLetto;
                break;
            }

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
                if (stato->piante[i].status != TERMINATO)
                    stampaSprite(stato->piante[i]);
            }
            for (int i = 0; i < NUM_PIANTE; i++)
            {
                if (stato->proiettiliPianta[i].status != TERMINATO)
                    stampaSprite(stato->proiettiliPianta[i]);
            }
            for (int i = 0; i < NUM_TOT_COCCODRILLI; i++)
            {
                if (stato->coccodrilli[i].status != TERMINATO)
                    stampaSprite(stato->coccodrilli[i]);
            }
            if (stato->rana.status != TERMINATO)
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
    system("killall aplay"); // Terminate any sounds
    endwin();                // End ncurses
}
