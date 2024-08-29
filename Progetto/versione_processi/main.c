#include "main.h"

// Definizione variabili globali

// Schermata di gioco e dimensioni
int minx, miny;
int maxx, maxy;
WINDOW *gioco;

// Posizione iniziale delle piante
int posizionePianteX[NUM_PIANTE] = {19, 29, 39};

// Informazioni sui flussi del fiume
InformazioniFiume infoFiume;

// Inizializza una direzione casuale, utilizzata in fase di creazione dei flussi
DirezioneFiume getDirezioneFiume()
{

    return rand() % 2 == 0 ? SINISTRA : DESTRA;
}

// Inizializza una velocità casuale, utilizzata in fase di creazione dei flussi
int getVelocitaFlussoFiume()
{

    return VELOCITA_MIN_FLUSSO + (rand() % (VELOCITA_MAX_FLUSSO - VELOCITA_MIN_FLUSSO + 1));
}

// Inizializza i flussi del fiume, assegnando a ciascuno una direzione e una velocità casuali
void inizializzaFlussiFiume(InformazioniFiume *infoFiume)
{

    for (int i = 0; i < NUM_FLUSSI_FIUME; i++)
    {
        infoFiume->direzioneFlussi[i] = rand() % 2 == 0 ? SINISTRA : DESTRA; // Direzione casuale per ogni flusso
        infoFiume->velocitaFlussi[i] = getVelocitaFlussoFiume();             // Velocità casuale per ogni flusso
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