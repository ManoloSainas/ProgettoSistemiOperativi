#include "frogger.h"

void coccodrillo(int pipeout, int pipein, int riga, int id_coccodrillo, corrente flusso)
{

    elementoGioco coccodrillo;
    coccodrillo.tipo = COCCODRILLO;
    coccodrillo.y = maxy - riga - 2;
    coccodrillo.pid_oggetto = getpid();
    coccodrillo.direzione = flusso.direzione;
    coccodrillo.velocita = flusso.velocita;

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

        // velocita del coccodrillo 2s- velocita flusso*10000
        usleep(500000 - coccodrillo.velocita);
    }
    // terminazione processo se il coccodrillo esce dallo schermo
}