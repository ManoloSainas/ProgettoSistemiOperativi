#include "frogger.h"
#include <unistd.h> // Include the header for usleep

void *coccodrillo(void *info)
{
    // Inizializzazione del coccodrillo
    srand(time(NULL) + (unsigned long)pthread_self());
    info_elemento *info_cocco = (info_elemento *)info;

    posizione pos_c;
    elementoGioco coccodrillo;
    coccodrillo.tipo = COCCODRILLO;
    coccodrillo.y = info_cocco->y;
    coccodrillo.thread_oggetto = pthread_self();
    coccodrillo.direzione = info_cocco->direzione;
    coccodrillo.velocita = info_cocco->speed;
    coccodrillo.proiettile = INVALID_THREAD;
    info_elemento proiettile_info;
    bool controllo_coccodrillo = true;
    coccodrillo.controllo = &controllo_coccodrillo;
    // gestione sparo
    int tempo_sparo = rand() % 4 + 3;
    void *var_proiettile;
    // inizializzazione variabili globali
    time_t start_sparo = time(NULL);
    proiettile_info.y = coccodrillo.y;
    proiettile_info.x = coccodrillo.x;
    proiettile_info.speed = SPEED_PROIETTILI;
    proiettile_info.tipo = 'c';
    proiettile_info.direzione = coccodrillo.direzione;
    // gestione segnale tramite handler
    var_proiettile = &proiettile_info;
    // Inizializza posizione iniziale del coccodrillo
    coccodrillo.x = (coccodrillo.direzione == DESTRA) ? (minx - 1) : maxx - 1;

    while (controllo && *coccodrillo.controllo)
    {
        proiettile_info.x = coccodrillo.x;

        time_t fine_sparo = time(NULL);
        if (difftime(fine_sparo, start_sparo) >= tempo_sparo) // se tempo_sparo è passato spara
        {
            // creazione thread sparo
            if (pthread_create(&coccodrillo.proiettile, NULL, &proiettile, var_proiettile) == 0)
            {
                start_sparo = time(NULL);
            }
            else
            {
                perror("errore creazione thread");
            }
        }

        // Movimento del coccodrillo
        switch (coccodrillo.direzione)
        {
        case DESTRA:
            coccodrillo.x += SPOSTAMENTO_COCCODRILLO;
            break;
        case SINISTRA:
            coccodrillo.x -= SPOSTAMENTO_COCCODRILLO;
            break;
        }

        // Scrive l'elemento coccodrillo nella lista degli elementi in comune
        wait_produttore();
        lista_elementi[in] = coccodrillo;
        in = (in + 1) % DIM_BUFFER;
        signal_produttore();

        // Calcolo del ritardo basato sulla velocità
        int delay = 500000 - coccodrillo.velocita;
        if (delay < 0)
            delay = 0;
        usleep(delay);
    }

    return NULL;
}