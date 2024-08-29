#include "main.h"
#include <signal.h>

/* Routine debug */
void handler(int sig)
{
    fprintf(stderr, "Hello, this shouldn't happen! I am crocodile %d, and a %s was raised\n",
            getpid(),
            sig == SIGSEGV ? "Segmentation violation" : sig == SIGPIPE ? "broken pipe signal"
                                                                       : "unknown signal");
}

// void coccodrillo(int pipeout, int row, int indice, InformazioniFiume *infoFiume)
void coccodrillo(int pipeout, int pipein, int indice, InformazioniFiume *infoFiume)
{
    oggetto oggetto_coccodrillo;

    signal(SIGSEGV, handler);
    signal(SIGPIPE, handler);

    srand(time(NULL) + indice); // Unique seed for each crocodile

    // Inizializzazione coccodrillo
    oggetto_coccodrillo.tipo = (rand() % 2 == 0) ? COCCODRILLO_BUONO : COCCODRILLO_CATTIVO;
    // oggetto_coccodrillo.x = minx + rand() % (maxx - COLONNE_SPRITE_COCCODRILLO - minx);
    // oggetto_coccodrillo.y = row; // Set y coordinate based on row
    oggetto_coccodrillo.status = SOSPESO;
    oggetto_coccodrillo.pid_oggetto = getpid();
    oggetto_coccodrillo.index = indice;
    oggetto_coccodrillo.proiettili = false;

    // Random delay before generating the next crocodile
    // usleep(1000000 + (rand() % 6000000)); // Random delay between 2 and 5 seconds

    Messaggio msg = {0};
    {
        // read(pipein, &msg, sizeof(msg));

        oggetto_coccodrillo.status = ATTIVO;
        oggetto_coccodrillo.x = 40;
        oggetto_coccodrillo.y = maxy - 2;

        // oggetto_coccodrillo.direzioneFiume = infoFiume->direzioneFlussi[msg.posizione.y - (maxy - 9)]; // Use direction for the specific row
        // oggetto_coccodrillo.velocita = infoFiume->velocitaFlussi[msg.posizione.y - (maxy - 9)]; // Use speed for the specific row

        oggetto_coccodrillo.direzioneFiume = infoFiume->direzioneFlussi[0]; // Use direction for the specific row
        oggetto_coccodrillo.velocita = infoFiume->velocitaFlussi[0];        // Use speed for the specific row
    }

    /* TODO: revamp della formula per ottenere il flusso esatto. Si potrebbe evitare di usare
     * la coordinata, e mandare semplicemente un indice?
     */
    // Movimento del coccodrillo
    while (1)
    {
        /*~ Aspetta una risposta dal padre */
#ifdef disabled
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
                // oggetto_coccodrillo.tipologiaCoccodrillo = (oggetto_coccodrillo.tipo == COCCODRILLO_BUONO) ? BUONO : CATTIVO;
                oggetto_coccodrillo.velocita = infoFiume->velocitaFlussi[msg.posizione.y - (maxy - 9)]; // Use speed for the specific row
            }
            break;
        case SOSPENDI: /* Non dovrebbe verificarsi in teoria */
            if (oggetto_coccodrillo.status == ATTIVO)
                oggetto_coccodrillo.status = SOSPESO;
            else
                continue;
            break;
        case CAMBIA: /*~quando abbiamo una collisione, allora il coccodrillo diventa buono*/
            if (oggetto_coccodrillo.status == ATTIVO)
                oggetto_coccodrillo.tipologiaCoccodrillo = COCCODRILLO_BUONO;
            else
                continue;
            break;
        case CONT:
            if (oggetto_coccodrillo.status != ATTIVO)
                continue;
            break;
        }
#endif

        if (oggetto_coccodrillo.tipo == COCCODRILLO_CATTIVO)
        {
            // Randomly decide to submerge
            if (rand() % 100 < 5) // 5% chance to submerge
                oggetto_coccodrillo.status = SOSPESO;
        }
        else
        {

            if (oggetto_coccodrillo.direzioneFiume == SINISTRA)
                oggetto_coccodrillo.x -= SPOSTAMENTO_X_COCCODRILLO;
            else
                oggetto_coccodrillo.x += SPOSTAMENTO_X_COCCODRILLO;

            if (oggetto_coccodrillo.x < minx + 1 || oggetto_coccodrillo.x > maxx - COLONNE_SPRITE_COCCODRILLO - 1)
            {
                oggetto_coccodrillo.tipo = COCCODRILLO_IMMERSIONE;
                oggetto_coccodrillo.status = SOSPESO;
            }
        }

        // Scrivi l'oggetto coccodrillo nella pipe
        write(pipeout, &oggetto_coccodrillo, sizeof(oggetto));

        usleep(oggetto_coccodrillo.status == SOSPESO ? 2000000 : oggetto_coccodrillo.velocita);
    }
}