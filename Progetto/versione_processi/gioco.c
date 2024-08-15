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
        // Create processes for each plant
        for (int i = 0; i < NUM_PIANTE; i++)
        {
            pid_gioco = fork();
            if (pid_gioco == -1)
            {
                perror("Errore nella creazione del processo per la pianta");
                _exit(1);
            }
            else if (pid_gioco == 0)
            {
                close(filedes[LETTURA]);
                pianta(filedes[SCRITTURA], i);
                _exit(0);
            }
        }
        close(filedes[SCRITTURA]);

        // The parent process will now control the game
        controlloGioco(filedes[LETTURA]);
    }

    // Close the game
    terminaGioco();
}

// Function to control the game, print objects, and check for collisions
void controlloGioco(int pipein)
{
    int viteRana = NUM_VITE_RANA;      // Initialize frog lives
    int tempoRimanente = TEMPO_TOTALE; // Initialize timer
    time_t inizioTempo = time(NULL);   // Store start time

    oggetto valoreLetto; // Will contain the value read from the pipe
    oggetto rana, proiettileRana[NUM_PROIETTILI_RANA], piante[NUM_PIANTE], proiettilePianta[NUM_PIANTE];

    initOggetto(&rana);

    for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
    {
        initOggetto(&proiettileRana[i]);
    }

    for (int i = 0; i < NUM_PIANTE; i++)
    {
        initOggetto(&piante[i]);
    }

    for (int i = 0; i < NUM_PIANTE; i++)
    {
        initOggetto(&proiettilePianta[i]);
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
            else if (valoreLetto.tipo == PROIETTILE_RANA && proiettileRana[valoreLetto.index].status == ATTIVO)
                cancellaSprite(proiettileRana[valoreLetto.index]);
            else if (valoreLetto.tipo == PIANTA && piante[valoreLetto.index].status == ATTIVO)
                cancellaSprite(piante[valoreLetto.index]);
            else if (valoreLetto.tipo == PROIETTILE_PIANTA && proiettilePianta[valoreLetto.index].status == ATTIVO)
                cancellaSprite(proiettilePianta[valoreLetto.index]);

            // Update the object with the new value
            if (valoreLetto.tipo == RANA)
            {
                rana = valoreLetto;
            }
            else if (valoreLetto.tipo == PROIETTILE_RANA)
            {
                proiettileRana[valoreLetto.index] = valoreLetto;
            }
            else if (valoreLetto.tipo == PIANTA)
            {
                piante[valoreLetto.index] = valoreLetto;
            }
            else if (valoreLetto.tipo == PROIETTILE_PIANTA)
            {
                proiettilePianta[valoreLetto.index] = valoreLetto;
            }

            // Print the background and all objects
            graficaGioco(viteRana, tempoRimanente);

            // Display the objects
            if (rana.status != TERMINATO)
                stampaSprite(rana);
            for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
            {
                if (proiettileRana[i].status != TERMINATO)
                    stampaSprite(proiettileRana[i]);
            }
            for (int i = 0; i < NUM_PIANTE; i++)
            {
                if (piante[i].status != TERMINATO)
                    stampaSprite(piante[i]);
            }
            for (int i = 0; i < NUM_PIANTE; i++)
            {
                if (proiettilePianta[i].status != TERMINATO)
                    stampaSprite(proiettilePianta[i]);
            }

            curs_set(0);
            wrefresh(gioco);
        }

    } while (viteRana > 0); // Continue while there are lives

    chiudiProcessi(proiettileRana, &rana, piante, proiettilePianta);
}

// Function to terminate the game
void terminaGioco()
{
    delwin(gioco);           // Delete the window
    system("killall aplay"); // Terminate any sounds
    endwin();                // End ncurses
}
