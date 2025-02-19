#include "frogger.h"





void *coccodrillo(void *info)
{
    // Inizializzazione del coccodrillo

    info_elemento *info_cocco = (info_elemento*) info;

    posizione pos_c;
    elementoGioco coccodrillo;
    coccodrillo.tipo = COCCODRILLO;
    coccodrillo.y = info_cocco->y;
    coccodrillo.thread_oggetto = pthread_self();
    coccodrillo.direzione = info_cocco->direzione;
    coccodrillo.velocita = info_cocco->speed;
    coccodrillo.proiettile = INVALID_THREAD;
     info_elemento proiettile_info;  
    // gestione sparo
    double durata_sparo, fine_sparo, start_sparo;
    int tempo_sparo = rand() % 4 + 1;

    // inizializzazione variabili globali
    start_sparo = time(NULL);

    proiettile_info.y=coccodrillo.y;
    proiettile_info.speed= SPEED_PROIETTILI;
    proiettile_info.tipo='c';
    proiettile_info.direzione=coccodrillo.direzione;
    // gestione segnale tramite handler

    // Inizializza posizione iniziale del coccodrillo
    coccodrillo.x = (coccodrillo.direzione == DESTRA) ? (minx - 1) : maxx - 1;

    while (controllo)
    {
        proiettile_info.x=coccodrillo.x;
        
        fine_sparo = time(NULL);

        if (difftime(fine_sparo, start_sparo) >= tempo_sparo) // se  tempo_sparo è passato spara
        {
            
                //creazioen thread sparo
                if(pthread_create(gioco, NULL, &proiettile, &proiettile)==0){
                start_sparo = time(NULL);}else{perror("errore creazione thread");
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

        // Scrive l'elemento coccodrillo nell lista degli elementi in comune
       

        // Calcolo del ritardo basato sulla velocità
        int delay = 500000 - coccodrillo.velocita;
        if (delay < 0)
            delay = 0;
        usleep(delay);

    }
}}

