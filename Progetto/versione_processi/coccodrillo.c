#include "main.h"

// inizializza una tipologia casuale
TipologiaCoccodrillo getTipologiaCoccodrillo()
{
    return rand() % 2 == 0 ? BUONO : CATTIVO;
}

// void coccodrillo(int pipeout, int row, int indice, InformazioniFiume *infoFiume)
void coccodrillo(int pipeout, int pipein, int indice, InformazioniFiume *infoFiume)
{
    oggetto oggetto_coccodrillo;

    srand(time(NULL) + indice); // Unique seed for each crocodile

    // Inizializzazione coccodrillo
    oggetto_coccodrillo.tipo = (rand() % 2 == 0) ? COCCODRILLO_BUONO : COCCODRILLO_CATTIVO;
    // oggetto_coccodrillo.x = minx + rand() % (maxx - COLONNE_SPRITE_COCCODRILLO - minx);
    // oggetto_coccodrillo.y = row; // Set y coordinate based on row
    oggetto_coccodrillo.status = SOSPESO;
    oggetto_coccodrillo.pid_oggetto = getpid();
    oggetto_coccodrillo.index = indice;
    oggetto_coccodrillo.proiettili = false;

    /*~
     * read from pipein the following parameters:
     * switch (msg) {
     * case STOP:
     * oggetto_coccodrillo.status = SOSPESO;
     * }
     */

    // Random delay before generating the next crocodile
    usleep(1000000 + (rand() % 6000000)); // Random delay between 2 and 5 seconds

    // Scrivi l'oggetto coccodrillo nella pipe
    write(pipeout, &oggetto_coccodrillo, sizeof(oggetto));

    Messaggio msg;
    // Movimento del coccodrillo
    // while (oggetto_coccodrillo.status == ATTIVO)
    while (1)
    {
        /*~ Aspetta una risposta dal padre */
        read(pipein, &msg, sizeof(msg));
        switch (msg.codice)
        {
        case SVEGLIA:
            /*~ re-inizializza il coccodrillo */
            if (oggetto_coccodrillo.status == SOSPESO ||
                oggetto_coccodrillo.status == NON_ATTIVO)
            {
                oggetto_coccodrillo.status = ATTIVO;
                oggetto_coccodrillo.x = msg.posizione.x;
                oggetto_coccodrillo.y = msg.posizione.y;

                oggetto_coccodrillo.direzioneFiume = infoFiume->direzioneFlussi[msg.posizione.y - (maxy - 9)]; // Use direction for the specific row
                oggetto_coccodrillo.tipologiaCoccodrillo = (oggetto_coccodrillo.tipo == COCCODRILLO_BUONO) ? BUONO : CATTIVO;
                oggetto_coccodrillo.velocita = infoFiume->velocitaFlussi[msg.posizione.y - (maxy - 9)]; // Use speed for the specific row
            }
            break;
        case SOSPENDI:
            if (oggetto_coccodrillo.status == ATTIVO)
            {
                oggetto_coccodrillo.status = SOSPESO;
            }
            else
                continue;
            break;
        case CAMBIA: /*~quando abbiamo una collisione, allora il coccodrillo diventa buono*/
            if (oggetto_coccodrillo.status == ATTIVO)
            {
                oggetto_coccodrillo.tipologiaCoccodrillo = COCCODRILLO_BUONO;
            }
            else
                continue;
            break;
        case CONT:
            if (oggetto_coccodrillo.status != ATTIVO)
                continue;
            break;
        }

        if (oggetto_coccodrillo.tipo == COCCODRILLO_CATTIVO)
        {
            // Randomly decide to submerge
            if (rand() % 100 < 5) // 5% chance to submerge
            {
                oggetto_coccodrillo.status = SOSPESO;

                // _exit(0);
            }
        }

        if (oggetto_coccodrillo.direzioneFiume == SINISTRA)
        {
            oggetto_coccodrillo.x -= SPOSTAMENTO_X_COCCODRILLO;
            // if (oggetto_coccodrillo.x < minx + 1)
            // {
            //     oggetto_coccodrillo.tipo = COCCODRILLO_IMMERSIONE;
            //     write(pipeout, &oggetto_coccodrillo, sizeof(oggetto));
            //     usleep(2000000); // Wait for 2 seconds
            //     oggetto_coccodrillo.status = SOSPESO;
            // }
        }
        else
        {
            oggetto_coccodrillo.x += SPOSTAMENTO_X_COCCODRILLO;
            // if (oggetto_coccodrillo.x > maxx - COLONNE_SPRITE_COCCODRILLO - 1)
            // {
            //     oggetto_coccodrillo.tipo = COCCODRILLO_IMMERSIONE;
            //     write(pipeout, &oggetto_coccodrillo, sizeof(oggetto));
            //     usleep(2000000); // Wait for 2 seconds
            //     oggetto_coccodrillo.status = SOSPESO;
            // }
        }

        /* Refactoring */
        if (oggetto_coccodrillo.x < minx + 1 || oggetto_coccodrillo.x > maxx - COLONNE_SPRITE_COCCODRILLO - 1)
        {
            oggetto_coccodrillo.tipo = COCCODRILLO_IMMERSIONE;
            oggetto_coccodrillo.status = SOSPESO;
        }

        usleep(oggetto_coccodrillo.status == SOSPESO ? 2000000 : oggetto_coccodrillo.velocita);

        // Scrivi l'oggetto coccodrillo nella pipe
        write(pipeout, &oggetto_coccodrillo, sizeof(oggetto));
    }

    // Termina il processo quando il coccodrillo esce dallo schermo
    // _exit(0);
}