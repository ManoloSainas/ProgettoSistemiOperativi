#include "main.h"

void pianta(int pipeout, int indice)
{
    oggetto pianta;

    srand(time(NULL)); // Inizializza il generatore di numeri casuali

    // Inizializzazione pianta
    pianta.tipo = PIANTA;
    // la posizione x dev'essere casuale all'interno della finestra di gioco
    pianta.x = minx + (rand() % (maxx - minx - 10)); // Posizione casuale
    pianta.y = maxy - 11;                            // Posizionata sulla sponda d'erba
    pianta.status = ATTIVO;
    pianta.pid_oggetto = getpid();
    pianta.index = indice;

    // Scrivi l'oggetto pianta nella pipe
    write(pipeout, &pianta, sizeof(oggetto));

    // Genera proiettili casualmente
    while (true)
    {
        usleep(1000000 + (rand() % 2000000)); // Intervallo casuale tra 1 e 3 secondi

        // Crea un nuovo processo per il proiettile
        if (fork() == 0)
        {
            proiettilePianta(pipeout, pianta.x, pianta.y, indice); // Posizione iniziale del proiettile = posizione della pianta
            _exit(0);
        }

        // Tempo di ricarica tra un proiettile e l'altro
        usleep(RICARICA_PROIETTILI);
    }
}

void proiettilePianta(int pipeout, int x_iniziale, int y_iniziale, int index_pianta)
{
    oggetto proiettile;

    proiettile.tipo = PROIETTILE_PIANTA;
    proiettile.x = x_iniziale;
    proiettile.y = y_iniziale + 1; // Il proiettile parte appena sotto la pianta
    proiettile.status = ATTIVO;
    proiettile.index = index_pianta;
    proiettile.pid_oggetto = getpid();

    // Movimento del proiettile verso il basso
    while (proiettile.y < maxy && proiettile.status == ATTIVO)
    {
        proiettile.y++; // Sposta il proiettile verso il basso

        usleep(SPEED_PROIETTILI); // Ritardo per il movimento del proiettile

        // Scrivi l'oggetto proiettile nella pipe
        write(pipeout, &proiettile, sizeof(oggetto));
    }

    // Termina il processo quando il proiettile esce dallo schermo
    _exit(0);
}