#include "frogger.h"

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
    double durata_sparo, start_sparo, start_timer, durata_timer;
    int tempo_sparo = rand() % 4 + 3;
    void *var_proiettile;
    start_sparo = time(NULL);
    start_timer = time(NULL);
    proiettile_info.y = coccodrillo.y;
    proiettile_info.speed = SPEED_PROIETTILI;
    proiettile_info.tipo = 'c';
    proiettile_info.direzione = coccodrillo.direzione;
    var_proiettile = &proiettile_info;
    // Inizializza posizione iniziale del coccodrillo
    coccodrillo.x = (coccodrillo.direzione == DESTRA) ? (minx - 1) : maxx - 1;

    while (controllo && *coccodrillo.controllo)
    {

        proiettile_info.x = coccodrillo.x;

        // if (difftime(time(NULL), start_sparo) >= tempo_sparo) // se tempo_sparo è passato spara
        // {
        //     // creazione thread sparo
        //     if (pthread_create(&coccodrillo.proiettile, NULL, &proiettile, var_proiettile) == 0)
        //     {
        //         start_sparo = time(NULL);
        //     }
        //     else
        //     {
        //         perror("errore creazione thread");
        //     }
        // }
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

        // Scrive l'elemento coccodrillo nella lista degli elementi
        wait_produttore();
        lista_elementi[in] = coccodrillo;
        in = (in + 1) % DIM_BUFFER;
        signal_produttore();

        start_timer = time(NULL);

        // Calcolo del ritardo basato sulla velocità
        int delay = 500000 - coccodrillo.velocita;
        if (delay < 0)
            delay = 0;
        usleep(delay);
    }
    return NULL;
}