#include "frogger.h"
pid_t pid_sparo;
void coccodrillo(int pipeout, int pipein, int riga, int id_coccodrillo, corrente flusso)
{

    fcntl(pipein, F_SETFL, O_NONBLOCK); // pipe in scrittura del coccodrillo non bloccante

    elementoGioco coccodrillo;
    coccodrillo.tipo = COCCODRILLO;
    coccodrillo.y = maxy - riga - 2;
    coccodrillo.pid_oggetto = getpid();
    coccodrillo.direzione = flusso.direzione;
    coccodrillo.velocita = flusso.velocita;
    double durata_sparo, start_sparo, fine_sparo;
    pid_sparo = -500;

    start_sparo = clock();
    if (coccodrillo.direzione == DESTRA)
    {

        coccodrillo.x = minx - 2;
    }
    else
    {
        coccodrillo.x = maxx;
    }

    while (true)
    {
        coccodrillo.proiettile = pid_sparo;
        fine_sparo = clock();
        durata_sparo = (double)(fine_sparo - start_sparo) / CLOCKS_PER_SEC;
        if (rand() % 100 < 20 && durata_sparo < 2 && (pid_sparo == -500))
        {
            pid_sparo = fork();
            if (pid_sparo == 0)
            {
                proiettile(pipeout, coccodrillo.y, coccodrillo.x, coccodrillo.velocita, coccodrillo.direzione, 'c');
                _exit(0);
            }

            start_sparo = clock();
        }

        switch (coccodrillo.direzione)
        {
        case DESTRA:
            coccodrillo.x += 1;
            if (coccodrillo.x > maxx + 2)
            {
                coccodrillo.x = minx - 2;
                sleep(1);
            }

            break;
        case SINISTRA:
            coccodrillo.x -= 1;
            if (coccodrillo.x < minx - 4)
            {
                coccodrillo.x = maxx;
                sleep(1);
            }

            break;

        default:
            break;
        }

        // scrittura del coccodrillo nella pipe
        if (write(pipeout, &coccodrillo, sizeof(elementoGioco)) == -1)
        {
            perror("Errore nella scrittura sulla pipe");
            _exit(1);
        }

        signal(SIGUSR1, handler);

        // velocita del coccodrillo 2s- velocita flusso*10000
        usleep(500000 - coccodrillo.velocita);
    }
    // terminazione processo se il coccodrillo esce dallo schermo
}

void handler(int sig)
{
    if (sig == SIGUSR1)
    {

        chiudiProcessi(pid_sparo);
        pid_sparo = -500;
    }
}