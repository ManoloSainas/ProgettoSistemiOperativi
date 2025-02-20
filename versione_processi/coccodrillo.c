#include "frogger.h"

// variabili globali per
pid_t pid_sparo;    // inizializzare il pid tramite segnale
bool flag_muro;     // per gestire la collisione con il muro
double start_sparo; // gestione tempistiche spari

void coccodrillo(int pipeout, int riga, int id_coccodrillo, corrente flusso)
{
    // Inizializzazione del coccodrillo
    posizione pos_c;
    elementoGioco coccodrillo;
    coccodrillo.tipo = COCCODRILLO;
    coccodrillo.y = maxy - riga - 2;
    coccodrillo.pid_oggetto = getpid();
    coccodrillo.direzione = flusso.direzione;
    coccodrillo.velocita = flusso.velocita;
    coccodrillo.proiettile = INVALID_PID;

    // gestione sparo
    double durata_sparo, fine_sparo;
    int tempo_sparo = rand() % 4 + 3;

    // inizializzazione variabili globali
    pid_sparo = INVALID_PID;
    flag_muro = false;
    start_sparo = time(NULL);

    // gestione segnale tramite handler
    signal(SIGUSR1, handler_coccodrillo);
    signal(SIGUSR2, handler_coccodrillo);

    // Inizializza posizione iniziale del coccodrillo
    coccodrillo.x = (coccodrillo.direzione == DESTRA) ? (minx - 1) : maxx - 1;

    while (true)
    {
        coccodrillo.proiettile = pid_sparo;
        fine_sparo = time(NULL);

        if (difftime(fine_sparo, start_sparo) >= tempo_sparo) // se due tempo_sparo è passato spara
        {
            // Logica di sparo con fork
            if (pid_sparo == INVALID_PID)
            {
                pid_sparo = fork();
                if (pid_sparo == 0)
                {
                    proiettile(pipeout, coccodrillo.y, coccodrillo.x, SPEED_PROIETTILI, coccodrillo.direzione, 'c');
                    _exit(0);
                }
                start_sparo = time(NULL);
            }
        }
        // Movimento del coccodrillo
        switch (coccodrillo.direzione)
        {
        case DESTRA:
            coccodrillo.x += SPOSTAMENTO_COCCODRILLO;
            break;
        case SINISTRA:
            coccodrillo.x -= SPOSTAMENTO_COCCODRILLO;
            break;
        }

        // Scrive l'elemento coccodrillo nella pipe
        if (write(pipeout, &coccodrillo, sizeof(elementoGioco)) == -1)
        {
            perror("Errore nella scrittura sulla pipe");
            _exit(1);
        }

        // Calcolo del ritardo basato sulla velocità
        int delay = 500000 - coccodrillo.velocita;
        if (delay < 0)
            delay = 0;
        usleep(delay);

        // reset delle coordinate nel caso il coccodrillo sia uscito fuori dallo schermo
        if (flag_muro)
        {
            if (coccodrillo.direzione == DESTRA)
                coccodrillo.x = minx - 2;

            if (coccodrillo.direzione == SINISTRA)
                coccodrillo.x = maxx;

            flag_muro = false;
        }
    }
}

// Funzione per gestire i segnali
void handler_coccodrillo(int sig)
{
    // gestione segnale per la chiusura del processo di sparo
    if (sig == SIGUSR1)
    {
        chiudiProcessi(pid_sparo);
        pid_sparo = INVALID_PID;
        start_sparo = time(NULL);
    }
    // gestione segnale per la collisione con il muro
    if (sig == SIGUSR2)
    {
        flag_muro = true;
    }
}
