#include "main.h"

// Inizializzazione Pipe
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

    int filedes[2]; // Dichiarazione file descriptor per lavorare con le pipe
    int pid_gioco;  // Dichiarazione variabile utile all'utilizzo di più processi

    inizializzazionePipe(filedes);

    pid_gioco = fork(); // Creazione di un processo figlio

    switch (pid_gioco)
    {
    case -1:
        perror("Errore nella creazione del processo");
        _exit(1);
        break;
    case 0:
        close(filedes[LETTURA]);
        // Processo rana
        rana(filedes[SCRITTURA]);
        break;
    default:
        close(filedes[SCRITTURA]);
        controlloGioco(filedes[LETTURA]);
    }

    // chiusura del gioco
    terminaGioco();
}

// Funzione per la stampa degli oggetti e il controllo delle collisioni
void controlloGioco(int pipein)
{
    oggetto valoreLetto; // Conterrà il valore letto nella pipe

    // Dichiarazione delle variabili che conterranno la posizione e le altre informazioni di tutti gli elementi del gioco
    oggetto rana, proiettileRana[NUM_PROIETTILI_RANA];

    int viteRana; // Variabile che conterrà il numero di vite della rana

    int i;

    initOggetto(&rana);
    viteRana = NUM_VITE_RANA;

    for (i = 0; i < NUM_PROIETTILI_RANA; i++)
    {
        initOggetto(&proiettileRana[i]);
    }

    do
    {
        // Legge dalla pipe
        read(pipein, &valoreLetto, sizeof(valoreLetto));

        switch (valoreLetto.tipo)
        {
        case RANA:
            if (rana.status == ATTIVO)
                cancellaSprite(rana);

            rana = valoreLetto;
            break;
        case PROIETTILE_RANA:
            if (proiettileRana[valoreLetto.index].status == ATTIVO)
                cancellaSprite(proiettileRana[valoreLetto.index]);

            proiettileRana[valoreLetto.index] = valoreLetto;
            break;
        }

        // Visualizza l'oggetto nella posizione aggiornata
        wattroff(gioco, COLOR_PAIR(COLORE_STANDARD));
        if (valoreLetto.status != TERMINATO)
        {
            stampaSprite(valoreLetto, viteRana);
        }
        else
        {
            cancellaSprite(valoreLetto);
        }
        curs_set(0);

        wrefresh(gioco);

    } while (viteRana > 0);

    chiudiProcessi(proiettileRana, &rana);
}

// Funzione per la terminazione del gioco
void terminaGioco()
{

    delwin(gioco);           // Elimina la finestra
    system("killall aplay"); // Termina eventuali suoni
    endwin();                // Termina ncurses
}