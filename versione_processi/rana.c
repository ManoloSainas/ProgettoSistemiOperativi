#include "frogger.h"

void rana(int pipeout)
{
    int pid_rana = getpid();
    keypad(gioco, TRUE);

    elementoGioco rana;
    rana.tipo = RANA;
    rana.x = 36;
    rana.y = 16;
    rana.pid_oggetto = pid_rana;
    rana.velocita = 0;
    rana.ha_sparato = false;

    while (1)
    {

        switch (wgetch(gioco))
        {
        case KEY_UP:
            if (rana.y > miny + 5)
                rana.y -= SPOSTAMENTO_RANA;
            break;
        case KEY_DOWN:
            if (rana.y < maxy - 2)
                rana.y += SPOSTAMENTO_RANA;
            break;
        case KEY_LEFT:
            if (rana.x > minx)
                rana.x -= SPOSTAMENTO_RANA;
            break;
        case KEY_RIGHT:
            if (rana.x < maxx - 3)
                rana.x += SPOSTAMENTO_RANA;
            break;
        case KEY_SPACE:
            rana.ha_sparato = true;
            usleep(1000000);
            rana.ha_sparato = false;
            break;
        }
        // Scrittura nella pipe delle informazioni della rana
        if (write(pipeout, &rana, sizeof(elementoGioco)) == -1)
        {
            perror("Errore nella scrittura sulla pipe");
            _exit(1);
        }
    }

    _exit(1);
}