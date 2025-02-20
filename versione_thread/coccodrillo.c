#include "frogger.h"





void *coccodrillo(void *info)
{
    // Inizializzazione del coccodrillo
    srand(time(NULL)+pthread_self());
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
     *(coccodrillo.controllo)=true;
    // gestione sparo
    double durata_sparo, fine_sparo, start_sparo, start_timer, fine_timer, durata_timer;
    int tempo_sparo = rand() % 4 + 1;
    void *var_proiettile;
    // inizializzazione variabili globali
    start_sparo = time(NULL);
    start_timer= time(NULL) ;
    proiettile_info.y=coccodrillo.y;
    proiettile_info.speed= SPEED_PROIETTILI;
    proiettile_info.tipo='c';
    proiettile_info.direzione=coccodrillo.direzione;
    // gestione segnale tramite handler
    var_proiettile=&proiettile_info;
    // Inizializza posizione iniziale del coccodrillo
    coccodrillo.x = (coccodrillo.direzione == DESTRA) ? (minx - 1) : maxx - 1;

    
    durata_timer=  ((2500000 + rand() % 5000000 + 2000000)/1000000) * info_cocco->x;
    while (controllo && *coccodrillo.controllo)
    {

        fine_timer=time(NULL);
        if(difftime(fine_timer, start_timer)>=durata_timer){
        proiettile_info.x=coccodrillo.x;
        durata_timer=((500000-coccodrillo.velocita)/1000000);
        
        fine_sparo = time(NULL);

        if (difftime(fine_sparo, start_sparo) >= tempo_sparo) // se  tempo_sparo è passato spara
        {
            
                //creazioen thread sparo
                if(pthread_create(gioco, NULL, &proiettile, var_proiettile )==0){
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

         wait_produttore();
    lista_elementi[in]=coccodrillo;
    in=(in+1)%DIM_BUFFER;
     signal_produttore(); 

    start_timer=time(NULL);
     
      
}
    }
}}

