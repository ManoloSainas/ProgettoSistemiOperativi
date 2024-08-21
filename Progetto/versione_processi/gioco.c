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
                pianta(filedes[SCRITTURA], i);
                _exit(0);
                break;
            }
        }
        for (int j = 0; j < NUM_TOT_COCCODIRLLI; j++)
        {
            pid_gioco = fork();
            switch (pid_gioco)
            {
            case -1:
                perror("Errore nella creazione del processo per il coccodrillo");
                _exit(1);
                break;
            case 0:
                close(filedes[LETTURA]);
                srand(time(NULL) + j); // Unique seed for each crocodile
                coccodrillo(filedes[SCRITTURA], j);
                _exit(0);
                break;
            }
        }
        close(filedes[SCRITTURA]);
        controlloGioco(filedes[LETTURA]);
        break;
    }
    // Termina gioco
    terminaGioco();
}
// Function to control the game, print objects, and check for collisions
void controlloGioco(int pipein)
{
    int viteRana = NUM_VITE_RANA;      // Initialize frog lives
    int tempoRimanente = TEMPO_TOTALE; // Initialize timer
    time_t inizioTempo = time(NULL);   // Store start time

    oggetto valoreLetto; // Will contain the value read from the pipe
    oggetto rana, proiettiliRana[NUM_PROIETTILI_RANA], piante[NUM_PIANTE], proiettiliPianta[NUM_PIANTE], coccodrilli[NUM_TOT_COCCODIRLLI];

    initOggetto(&rana);

    for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
    {
        initOggetto(&proiettiliRana[i]);
    }

    for (int i = 0; i < NUM_PIANTE; i++)
    {
        initOggetto(&piante[i]);
    }

    for (int i = 0; i < NUM_PIANTE; i++)
    {
        initOggetto(&proiettiliPianta[i]);
    }

    for (int i = 0; i < NUM_TOT_COCCODIRLLI; i++)
    {
        initOggetto(&coccodrilli[i]);
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
            // Clear the current sprite
            if (valoreLetto.tipo == RANA && rana.status == ATTIVO)
                cancellaSprite(rana);
            else if (valoreLetto.tipo == PROIETTILE_RANA && proiettiliRana[valoreLetto.index].status == ATTIVO)
                cancellaSprite(proiettiliRana[valoreLetto.index]);
            else if (valoreLetto.tipo == PIANTA && piante[valoreLetto.index].status == ATTIVO)
                cancellaSprite(piante[valoreLetto.index]);
            else if (valoreLetto.tipo == PROIETTILE_PIANTA && proiettiliPianta[valoreLetto.index].status == ATTIVO)
                cancellaSprite(proiettiliPianta[valoreLetto.index]);
            else if (valoreLetto.tipo == COCCODRILLO_BUONO && coccodrilli[valoreLetto.index].status == ATTIVO)
                cancellaSprite(coccodrilli[valoreLetto.index]);
            else if (valoreLetto.tipo == COCCODRILLO_CATTIVO && coccodrilli[valoreLetto.index].status == ATTIVO)
                cancellaSprite(coccodrilli[valoreLetto.index]);

            // Update the object array
            switch (valoreLetto.tipo)
            {
            case RANA:
                rana = valoreLetto;
                break;
            case PROIETTILE_RANA:
                proiettiliRana[valoreLetto.index] = valoreLetto;
                break;
            case PIANTA:
                piante[valoreLetto.index] = valoreLetto;
                break;
            case PROIETTILE_PIANTA:
                proiettiliPianta[valoreLetto.index] = valoreLetto;
                break;
            case COCCODRILLO_BUONO:
            case COCCODRILLO_CATTIVO:
                coccodrilli[valoreLetto.index] = valoreLetto;
                break;
            }

            // Print the background and all objects
            graficaGioco(viteRana, tempoRimanente);

            // Display the objects

            for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
            {
                if (proiettiliRana[i].status != TERMINATO)
                    stampaSprite(proiettiliRana[i]);
            }
            for (int i = 0; i < NUM_PIANTE; i++)
            {
                if (piante[i].status != TERMINATO)
                    stampaSprite(piante[i]);
            }
            for (int i = 0; i < NUM_PIANTE; i++)
            {
                if (proiettiliPianta[i].status != TERMINATO)
                    stampaSprite(proiettiliPianta[i]);
            }
            for (int i = 0; i < NUM_TOT_COCCODIRLLI; i++)
            {
                if (coccodrilli[i].status != TERMINATO)
                    stampaSprite(coccodrilli[i]);
            }
            if (rana.status != TERMINATO)
                stampaSprite(rana);

            curs_set(0);
            wrefresh(gioco);
        }

    } while (viteRana > 0); // Continue while there are lives

    chiudiProcessi(proiettiliRana, &rana, piante, proiettiliPianta, coccodrilli);
}

// Function to terminate the game
void terminaGioco()
{
    delwin(gioco);           // Delete the window
    system("killall aplay"); // Terminate any sounds
    endwin();                // End ncurses
}
