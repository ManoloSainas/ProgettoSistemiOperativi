#include "frogger.h"

void gestioneFlussi(corrente *flussi, int *coccodrilli_flusso)
{
    // inizializzazione flussi del fiume
    flussi[0].direzione = NESSUNA;
    flussi[NUM_FLUSSI_FIUME + 2].direzione = NESSUNA;
    flussi[NUM_FLUSSI_FIUME + 1].direzione = NESSUNA;
    flussi[0].velocita = 0;
    if (rand() % 2 == 0)
        flussi[1].direzione = DESTRA;
    else
    {
        flussi[1].direzione = SINISTRA;
    }

    flussi[NUM_FLUSSI_FIUME + 2].velocita = 0;
    flussi[NUM_FLUSSI_FIUME + 1].velocita = 0;
     flussi[1].velocita = rand() % (350000) + 100000;
    
    for (int i = 2; i <= NUM_FLUSSI_FIUME; i++)
    {
        if (flussi[i - 1].direzione == DESTRA)
        {
            flussi[i].direzione = SINISTRA;
        }
        else
        {
            flussi[i].direzione = DESTRA;
        }
        // flussi[i].velocita = rand() % (100) + 1;
        flussi[i].velocita = 1;
    }
    // inizializazione numero di coccodrilli per flusso
    for (int i = 0; i < NUM_FLUSSI_FIUME; i++)
    {
        coccodrilli_flusso[i] = rand() % (15 + 1 - 5) + 5;
    }
}



void avviaGioco(int vita, bool tana_status[])
{

    int filedes[2], pipeRana[2], pipeCocco[2];
    int pid_gioco;
    int coccodrilli_flusso[NUM_FLUSSI_FIUME], tot_coc=0;
    corrente flussi[NUM_FLUSSI_FIUME + 3];

    for(int i=0; i<NUM_FLUSSI_FIUME; i++){
        tot_coc+=coccodrilli_flusso[i];
    }

    graficaGioco();
    gestioneFlussi(flussi, coccodrilli_flusso);
    inizializzazionePipe(filedes);
    inizializzazionePipe(pipeRana);
    inizializzazionePipe(pipeCocco);

    pid_gioco = fork();

    switch (pid_gioco)
    {
    case -1:
        perror("Errore nella creazione del processo per la rana");
        _exit(1);
        break;
    case 0:
        close(filedes[LETTURA]);
        close(pipeRana[SCRITTURA]);
        rana(filedes[SCRITTURA], pipeRana[LETTURA],flussi);
        _exit(0);
        break;
    default:
        for (int i = 1; i <= NUM_FLUSSI_FIUME; i++)
        {
            for (int j = 1; j <= MAXCOCCODRILLI/NUM_FLUSSI_FIUME; j++)
            {
                pid_gioco = fork();
                switch (pid_gioco)
                {
                case -1:
                    perror("Errore nella creazione del processo per il coccodrillo");
                    _exit(1);
                    break;
                case 0:
                    close(pipeCocco[SCRITTURA]);
                    close(filedes[LETTURA]);
                    srand(time(NULL)+i );
                    j == 1 ? usleep(3000000 - flussi[i].velocita + (rand() % 6000000)) : usleep((3000000 - flussi[i].velocita + rand() % 5000000 + 2000000) * j );
                    coccodrillo(filedes[SCRITTURA], pipeCocco[LETTURA],i, j, flussi[i]);
                    _exit(0);
                    break;
                default:
                    break;
                }
            }
        }
            
        close(filedes[SCRITTURA]);
        close(pipeRana[LETTURA]);
        close(pipeCocco[LETTURA]);
        controlloGioco(filedes[LETTURA],pipeRana[SCRITTURA], pipeCocco[SCRITTURA], vita, tana_status);

        break;
    }

    // terminaGioco();
}

void terminaGioco()
{
    wclear(gioco);
    wrefresh(gioco);
    clear();
    refresh();
    delwin(gioco); // Delete the window
    endwin();      // End ncurses 
}

void controlloGioco(int pipein, int pipeRana, int pipeCocco, int vita, bool tana_status[])
{
    posizione pos_r, pos_c[MAXCOCCODRILLI];
    int score=0;
    elementoGioco valoreLetto;
    elementoGioco rana, coccodrillo, granataSinistra, granataDestra, granata;
    bool danno;
    pos_r.y=16;
    pos_r.x=36;
    for(int i=0;i<MAXCOCCODRILLI;i++){
        pos_c[i].pid=-1;
    }
    mvwprintw(gioco,1,7,"%d",vita);
        
    mvwprintw(gioco,1,maxx - 14,"%d",score);
    danno=true;
    do
    {
        danno=false;
        for(int i=0; i<MAXCOCCODRILLI; i++){
                if(pos_r.y==pos_c[i].y){
                    if(pos_c[i].direzione==SINISTRA){
                        if(pos_r.x>=pos_c[i].x-1 && pos_r.x<=pos_c[i].x+3){
                            danno=true;
                            break;
                        }
                    }else{
                        if(pos_r.x<pos_c[i].x+1 && pos_r.x>pos_c[i].x-5){
                            danno=true;
                            break;
                        }
                    }
                }
            } 
       if(pos_r.y==maxy-2 || pos_r.y==maxy-11 || pos_r.y==maxy-12){
                danno=true;
            }       
        
       
      

        if (read(pipein, &valoreLetto, sizeof(valoreLetto)) > 0){
            elementoGioco oggettoLetto;
            switch (valoreLetto.tipo)
            {
            case RANA:
                oggettoLetto = rana;
                break;
            case COCCODRILLO:
                oggettoLetto = coccodrillo;
                break;
            case GRANATA:
                oggettoLetto = granata;
                break;
            case GRANATA_DESTRA_RANA:
                oggettoLetto = granataDestra;
                break;
            case GRANATA_SINISTRA_RANA:
                oggettoLetto = granataSinistra;
                break;
            }
            cancellaSprite(oggettoLetto);

            switch (valoreLetto.tipo)
            {
            case RANA:
                rana = valoreLetto;
                pos_r.x= rana.x;
                pos_r.y= rana.y;
                    if(danno){
                        if (write(pipeRana, &danno, sizeof(danno)) == -1){
                                perror("Errore nella scrittura sulla pipe");
                                _exit(1);                            
                                }
                    }else{
                        
                                     
                        if (write(pipeRana, &danno, sizeof(danno)) == -1){
                                perror("Errore nella scrittura sulla pipe");
                                _exit(1);                           
                                }
                    }
                break;
            case COCCODRILLO:
                coccodrillo = valoreLetto;
                
                for (int i = 0; i < MAXCOCCODRILLI; i++) {
                  if(pos_c[i].pid==coccodrillo.pid_oggetto){
                        pos_c[i].x=coccodrillo.x;
                        pos_c[i].y=coccodrillo.y;
                        break;
                    }  
                    if(pos_c[i].pid==-1 ){
                        pos_c[i].pid=coccodrillo.pid_oggetto;
                        pos_c[i].x=coccodrillo.x;
                        pos_c[i].y=coccodrillo.y;
                        pos_c[i].direzione=coccodrillo.direzione;
                        break;
                    }
                    
                } break;

             default: break;   
            }
            
            
            }
            
            
            // graficaGioco();

            stampaSprite(coccodrillo);
            stampaSprite(rana);
            stampaSprite(granataSinistra);
            stampaSprite(granataDestra);
            stampaSprite(granata); 
            
            wrefresh(gioco);

    if(pos_r.x==11 && pos_r.y==6 && tana_status[0]){
        exit(1);
    }
    if(pos_r.x==23 && pos_r.y==6 && tana_status[1]){
        exit(2);
    } 
    if(pos_r.x==35 && pos_r.y==6 && tana_status[2]){
        exit(3);
    } 
    if(pos_r.x==47 && pos_r.y==6 && tana_status[3]){
        exit(4);
    } 
    if(pos_r.x==59 && pos_r.y==6 && tana_status[4]){
        exit(5);
    }         
            if(!danno){
                chiudiProcessi(rana.pid_oggetto);
    for(int i=0; i<MAXCOCCODRILLI; i++){
    chiudiProcessi(pos_c[i].pid, SIGTERM);
    }
    chiudiProcessi(granata.pid_oggetto, SIGTERM);
    exit(0);
        }

        }while(true );

    
}


bool collisioneTane(int ranaX, int ranaY){
    for(int i = 0; i < 5; i++){
        if(ranaX >= posTane[i].x && ranaX <= posTane[i].x2 && ranaY == posTane[i].y){
            tane[i] == false ? tane[i] = true : viteRana--;
            return true;
        }
    }
    return false;
}
