#include "frogger.h"

pid_t pid_sparo;

void coccodrillo(int pipeout, int pipein, int riga, int id_coccodrillo, corrente flusso)
{

    elementoGioco coccodrillo;
    coccodrillo.tipo = COCCODRILLO;
    coccodrillo.y = maxy - riga - 2;
    coccodrillo.pid_oggetto = getpid();
    coccodrillo.direzione = flusso.direzione;
    coccodrillo.velocita = flusso.velocita;
    coccodrillo.proiettile = INVALID_PID;

    double durata_sparo, start_sparo, fine_sparo;
    pid_sparo = INVALID_PID;

    signal(SIGUSR1, handler);

    start_sparo = clock();

    // Inizializza posizione iniziale del coccodrillo
    coccodrillo.x = (coccodrillo.direzione == DESTRA) ? (minx - 1) : maxx - 1;

    while (true)
    {

        coccodrillo.proiettile = pid_sparo;
        fine_sparo = clock();
        durata_sparo = (double)(fine_sparo - start_sparo) / CLOCKS_PER_SEC;

        // // Logica di sparo con fork
        // if ((rand() % 100) < 20 && pid_sparo == INVALID_PID)
        // {
        //     pid_sparo = fork();
        //     if (pid_sparo == 0)
        //     {
        //         proiettile(pipeout, coccodrillo.y, coccodrillo.x, coccodrillo.velocita, coccodrillo.direzione, 'c');
        //         _exit(0);
        //     }
        //     start_sparo = clock();
        // }

        // Movimento del coccodrillo
        switch (coccodrillo.direzione)
        {
        case DESTRA:
            coccodrillo.x += 1;
            // if (coccodrillo.x > maxx + 2)
            // {
            //     coccodrillo.x = minx - 2;
            // }
            break;
        case SINISTRA:
            coccodrillo.x -= 1;
            // if (coccodrillo.x < minx - 4)
            // {
            //     coccodrillo.x = maxx;
            // }
            break;
        }

        // Scrive lo stato del coccodrillo nella pipe
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

        // // reset delle coordinate nel caso il coccodrillo sia uscito fuori dallo schermo
        // posizione pos_c;
        // if (read(pipein, &pos_c, sizeof(posizione)) > 0)
        // {
        //     coccodrillo.x = pos_c.x;
        //     coccodrillo.y = pos_c.y;
        // }
    }
}

void handler(int sig)
{
    if (sig == SIGUSR1)
    {
        chiudiProcessi(pid_sparo);
        pid_sparo = INVALID_PID;
    }
}
