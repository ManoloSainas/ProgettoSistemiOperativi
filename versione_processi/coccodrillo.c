#include "frogger.h"

void coccodrillo(int pipeout, int riga, corrente flusso)
{

    elementoGioco coccodrillo;

    coccodrillo.tipo = COCCODRILLO;
    coccodrillo.y = maxy - riga - 2;
    coccodrillo.pid_oggetto = getpid();
    coccodrillo.direzione = flusso.direzione;
    coccodrillo.velocita = flusso.velocita;

    bool status_flag = true;

    if (coccodrillo.direzione == DESTRA)
    {
       
        coccodrillo.x = minx;
    }
    else
    {
        coccodrillo.x = maxx;
    }

    while (status_flag)
    {

        switch (coccodrillo.direzione)
        {
        case DESTRA:
            coccodrillo.x += 1;
            if (coccodrillo.x > maxx -4)
            {
                status_flag = false;
            }

            break;
        case SINISTRA:
            coccodrillo.x -= 1;
            if (coccodrillo.x < minx - 2)
            {
                status_flag = false;
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
        usleep(1000000 - coccodrillo.velocita * 10000);
    }
    // terminazione processo se il coccodrillo esce dallo schermo
}