#include "frogger.h"

void rana(int pipeout)
{
    int pid_rana = getpid();
    keypad(gioco, TRUE);

    elementoGioco oggetto_rana;
    oggetto_rana.tipo = RANA;
    oggetto_rana.x = 36;
    oggetto_rana.y = 16;
    oggetto_rana.pid_oggetto = pid_rana;
    oggetto_rana.velocita = 0;

    write(pipeout, &oggetto_rana, sizeof(elementoGioco));

    while (1)
    {
        int inputUtente = wgetch(gioco);

        switch (inputUtente)
        {
        case KEY_UP:
            if (oggetto_rana.y > miny)
                oggetto_rana.y -= SPOSTAMENTO_RANA;
            break;
        case KEY_DOWN:
            if (oggetto_rana.y < maxy - 1)
                oggetto_rana.y += SPOSTAMENTO_RANA;
            break;
        case KEY_LEFT:
            if (oggetto_rana.x > minx)
                oggetto_rana.x -= SPOSTAMENTO_RANA;
            break;
        case KEY_RIGHT:
            if (oggetto_rana.x < maxx - COLONNE_SPRITE_RANA)
                oggetto_rana.x += SPOSTAMENTO_RANA;
            break;
        }
        // Scrittura nella pipe delle informazioni della rana
        write(pipeout, &oggetto_rana, sizeof(elementoGioco));
    }

    _exit(1);
}