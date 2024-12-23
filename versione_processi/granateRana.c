#include "frogger.h"

void granateRana(int pipeout, int pos_ranay, int pos_ranax)
{

    elementoGioco granataSinistra;
    elementoGioco granataDestra;

    // inizializzazione granata sinistra
    granataSinistra.tipo = GRANATA_SINISTRA_RANA;
    granataSinistra.x = pos_ranax;
    granataSinistra.y = pos_ranay;
    granataSinistra.pid_oggetto = getpid();
    granataSinistra.proiettili = FALSE;
    granataSinistra.velocita = SPEED_PROIETTILI;
    granataSinistra.direzione = NESSUNA;

    // inizializzazione granata destra
    granataDestra.tipo = GRANATA_DESTRA_RANA;
    granataDestra.x = pos_ranax + 1;
    granataDestra.y = pos_ranay;
    granataDestra.pid_oggetto = getpid();
    granataDestra.proiettili = FALSE;
    granataDestra.velocita = SPEED_PROIETTILI;
    granataSinistra.direzione = NESSUNA;

    
    write(pipeout, &granataSinistra, sizeof(elementoGioco));
    write(pipeout, &granataDestra, sizeof(elementoGioco));



    while (1)
    {
        granataSinistra.x -= 1;
        granataDestra.x += 1;

        usleep(granataSinistra.velocita);

        write(pipeout, &granataSinistra, sizeof(elementoGioco));
        write(pipeout, &granataDestra, sizeof(elementoGioco));

    }

  
}