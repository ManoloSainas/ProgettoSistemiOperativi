#include "frogger.h"

void proiettile(int pipeout, int y, int x, int velocita, DirezioneFlusso direzione, char tipo)
{
    
    elementoGioco proiettile;
    proiettile.direzione = direzione;
    proiettile.pid_oggetto = getpid();
    proiettile.y = y;
    if (direzione == DESTRA)
    {
        proiettile.x = x + 1;
    }

    switch (tipo)
    {
    case 'c':
        proiettile.tipo = PROIETTILE_COCCODRILLO;
        if (direzione == DESTRA)
        {
            proiettile.x = x + 4;
        }
        if (direzione == SINISTRA)
        {
            proiettile.x = x - 2;
        }
        proiettile.velocita = 500000 - (velocita * 250000);

        break;
    case 'r':
        proiettile.tipo = GRANATA;
        if (direzione == DESTRA)
        {
            proiettile.x = x + 2;
        }
        if (direzione == SINISTRA)
        {
            proiettile.x = x - 1;
        }
        proiettile.velocita = velocita;

        break;
    default:
        break;
    }

    write(pipeout, &proiettile, sizeof(elementoGioco));

    

    while (1)
    {
        usleep(proiettile.velocita);
        switch (proiettile.direzione)
        {
        case DESTRA:
            proiettile.x++;
            break;
        case SINISTRA:
            proiettile.x--;
            break;
        default:
            break;
        }

        write(pipeout, &proiettile, sizeof(elementoGioco));
        
    }
    _exit(1);
}

// void granataSinistraRana(int pipeout, int pos_ranay, int pos_ranax)
// {

//     elementoGioco granataSinistra;

//     // inizializzazione granata sinistra
//     granataSinistra.tipo = GRANATA_SINISTRA_RANA;
//     granataSinistra.x = pos_ranax - 1;
//     granataSinistra.y = pos_ranay;
//     granataSinistra.pid_oggetto = getpid();
//     granataSinistra.proiettili = FALSE;
//     granataSinistra.velocita = SPEED_PROIETTILI;
//     granataSinistra.direzione = SINISTRA;

//     write(pipeout, &granataSinistra, sizeof(elementoGioco));

//     while (1)
//     {
//         granataSinistra.x -= 1;

//         usleep(granataSinistra.velocita);

//         write(pipeout, &granataSinistra, sizeof(elementoGioco));
//     }
// }

// void granataDestraRana(int pipeout, int pos_ranay, int pos_ranax)
// {
//     elementoGioco granataDestra;

//     // inizializzazione granata destra
//     granataDestra.tipo = GRANATA_DESTRA_RANA;
//     granataDestra.x = pos_ranax + 1;
//     granataDestra.y = pos_ranay;
//     granataDestra.pid_oggetto = getpid();
//     granataDestra.proiettili = FALSE;
//     granataDestra.velocita = SPEED_PROIETTILI;
//     granataDestra.direzione = DESTRA;

//     write(pipeout, &granataDestra, sizeof(elementoGioco));

//     while (1)
//     {
//         granataDestra.x += 1;

//         usleep(granataDestra.velocita);

//         write(pipeout, &granataDestra, sizeof(elementoGioco));
//     }
// }