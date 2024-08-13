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
    int viteRana = NUM_VITE_RANA;      // Inizializza le vite della rana
    int tempoRimanente = TEMPO_TOTALE; // Timer iniziale
    time_t inizioTempo = time(NULL);   // Memorizza il tempo di inizio

    oggetto valoreLetto; // Conterrà il valore letto nella pipe
    oggetto rana, proiettileRana[NUM_PROIETTILI_RANA];

    initOggetto(&rana);

    for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
    {
        initOggetto(&proiettileRana[i]);
    }

    do
    {
        // Aggiorna il timer
        time_t tempoAttuale = time(NULL);
        if (difftime(tempoAttuale, inizioTempo) >= 1) // Se è passato un secondo
        {
            tempoRimanente--;           // Decrementa il contatore
            inizioTempo = tempoAttuale; // Aggiorna il tempo di inizio

            // Se il tempo è scaduto
            if (tempoRimanente <= 0)
            {
                viteRana--; // Decrementa le vite della rana
                // Ripristina il timer
                tempoRimanente = TEMPO_TOTALE;
            }
        }

        // Legge dalla pipe
        if (read(pipein, &valoreLetto, sizeof(valoreLetto)) > 0)
        {
            // Cancella lo sprite attuale
            if (valoreLetto.tipo == RANA && rana.status == ATTIVO)
                cancellaSprite(rana);
            else if (valoreLetto.tipo == PROIETTILE_RANA && proiettileRana[valoreLetto.index].status == ATTIVO)
                cancellaSprite(proiettileRana[valoreLetto.index]);

            // Aggiorna l'oggetto con il nuovo valore
            if (valoreLetto.tipo == RANA)
            {
                rana = valoreLetto;
            }
            else if (valoreLetto.tipo == PROIETTILE_RANA)
            {
                proiettileRana[valoreLetto.index] = valoreLetto;
            }

            // Stampa lo sfondo e tutti gli oggetti
            graficaGioco(viteRana, tempoRimanente);

            // Visualizza gli oggetti
            if (rana.status != TERMINATO)
                stampaSprite(rana, viteRana);
            for (int i = 0; i < NUM_PROIETTILI_RANA; i++)
            {
                if (proiettileRana[i].status != TERMINATO)
                    stampaSprite(proiettileRana[i], viteRana);
            }

            curs_set(0);
            wrefresh(gioco);
        }

    } while (viteRana > 0); // Continua finché ci sono vite

    chiudiProcessi(proiettileRana, &rana);
}

// Funzione per la terminazione del gioco
void terminaGioco()
{
    delwin(gioco);           // Elimina la finestra
    system("killall aplay"); // Termina eventuali suoni
    endwin();                // Termina ncurses
}
