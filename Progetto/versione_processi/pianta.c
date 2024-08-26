#include "main.h"

void pianta(int pipeout, int indice, int posizione_x)
{
    oggetto oggetto_pianta;

    // Inizializzazione pianta
    oggetto_pianta.tipo = PIANTA;
    // la posizione x dev'essere casuale all'interno della finestra di gioco
    oggetto_pianta.x = posizione_x; // Posizione casuale
    oggetto_pianta.y = maxy - 11;   // Posizionata sulla sponda d'erba
    oggetto_pianta.status = ATTIVO;
    oggetto_pianta.pid_oggetto = getpid();
    oggetto_pianta.index = indice;
    oggetto_pianta.proiettili = true;
    oggetto_pianta.direzioneFiume = NESSUNA_DIREZIONE;
    oggetto_pianta.tipologiaCoccodrillo = NESSUNA_TIPOLOGIA;
    oggetto_pianta.velocita = 0;

    // Scrivi l'oggetto pianta nella pipe
    write(pipeout, &oggetto_pianta, sizeof(oggetto));

    // Genera proiettili casualmente
    while (true)
    {
        usleep(1000000 + (rand() % 2000000)); // Intervallo casuale tra 1 e 3 secondi

        // Crea un nuovo processo per il proiettile
        if (fork() == 0)
        {
            proiettilePianta(pipeout, oggetto_pianta.x, oggetto_pianta.y, indice); // Posizione iniziale del proiettile = posizione della pianta
            _exit(0);
        }

        // Tempo di ricarica tra un proiettile e l'altro
        usleep(RICARICA_PROIETTILI);
    }
}

void proiettilePianta(int pipeout, int x_iniziale, int y_iniziale, int index_pianta)
{
    oggetto oggetto_proiettile_pianta;

    oggetto_proiettile_pianta.tipo = PROIETTILE_PIANTA;
    oggetto_proiettile_pianta.x = x_iniziale;
    oggetto_proiettile_pianta.y = y_iniziale + 1; // Il proiettile parte appena sotto la pianta
    oggetto_proiettile_pianta.status = ATTIVO;
    oggetto_proiettile_pianta.index = index_pianta;
    oggetto_proiettile_pianta.pid_oggetto = getpid();
    oggetto_proiettile_pianta.proiettili = false;
    oggetto_proiettile_pianta.direzioneFiume = NESSUNA_DIREZIONE;
    oggetto_proiettile_pianta.tipologiaCoccodrillo = NESSUNA_TIPOLOGIA;
    oggetto_proiettile_pianta.velocita = SPEED_PROIETTILI;

    // Movimento del proiettile verso il basso
    while (oggetto_proiettile_pianta.y < maxy && oggetto_proiettile_pianta.status == ATTIVO)
    {
        oggetto_proiettile_pianta.y++; // Sposta il proiettile verso il basso

        usleep(oggetto_proiettile_pianta.velocita); // Ritardo per il movimento del proiettile

        // Scrivi l'oggetto proiettile nella pipe
        write(pipeout, &oggetto_proiettile_pianta, sizeof(oggetto));
    }

    // Termina il processo quando il proiettile esce dallo schermo
    _exit(0);
}