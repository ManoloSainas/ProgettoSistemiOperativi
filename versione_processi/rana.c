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

    while (1)
    {

        switch (wgetch(gioco))
        {
        case KEY_UP:
            if (oggetto_rana.y > miny + 5)
                oggetto_rana.y -= SPOSTAMENTO_RANA;
            break;
        case KEY_DOWN:
            if (oggetto_rana.y < maxy - 2)
                oggetto_rana.y += SPOSTAMENTO_RANA;
            break;
        case KEY_LEFT:
            if (oggetto_rana.x > minx)
                oggetto_rana.x -= SPOSTAMENTO_RANA;
            break;
        case KEY_RIGHT:
            if (oggetto_rana.x < maxx - 3)
                oggetto_rana.x += SPOSTAMENTO_RANA;
            break;
        }
        // Scrittura nella pipe delle informazioni della rana
        if (write(pipeout, &oggetto_rana, sizeof(elementoGioco)) == -1) {
    perror("Errore nella scrittura sulla pipe");
    _exit(1);
}

    }

    _exit(1);
}