#include "main.h"

// Definizione delle variabili globali
int minx, miny;
int maxx, maxy;
WINDOW *gioco;

// Inizializza un'array di posizioni delle piante inizializzato a -1 per tutte e tre le piante
int posizionePianteX[NUM_PIANTE] = {-1, -1, -1};

// Inizializza le informazioni sui flussi del fiume
InformazioniFiume infoFiume;

// inizializza una direzione casuale
DirezioneFiume getDirezioneFiume()
{

    return rand() % 2 == 0 ? SINISTRA : DESTRA;
}

int getVelocitaFlussoFiume()
{

    return VELOCITA_MIN_FLUSSO + (rand() % (VELOCITA_MAX_FLUSSO - VELOCITA_MIN_FLUSSO + 1));
}

void inizializzaFlussiFiume(InformazioniFiume *infoFiume)
{

    for (int i = 0; i < NUM_FLUSSI_FIUME; i++)
    {
        // Initialize direction and speed for each row
        infoFiume->direzioneFlussi[i] = rand() % 2 == 0 ? SINISTRA : DESTRA;                                                                           // Random direction for each row
        infoFiume->velocitaFlussi[i] = getVelocitaFlussoFiume();                                                                                       // Random speed for each row
        infoFiume->numeroCoccodrilliFlussi[i] = NUM_MIN_COCCODRILLI_FLUSSO + (rand() % (NUM_MAX_COCCODRILLI_FLUSSO - NUM_MIN_COCCODRILLI_FLUSSO + 1)); // Random number of crocodiles
    }
}

int main()
{
    srand(time(NULL));

    inizializzazioneSchermo();

    inizializzaFlussiFiume(&infoFiume);

    avviaGioco();

    return 0;
}