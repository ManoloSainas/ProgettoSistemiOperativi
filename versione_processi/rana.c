#include "frogger.h"


void rana(int pipeout, int pipein, corrente flussi[])
{

    // gestione tempo sparo
    
    int tempoTrascorso, tempoRicarica = 2; // Adatta la velocità di ricarica dei proiettili a seconda della dimensione dello schermo

    tempoTrascorso = tempoRicarica + 1; // si aggiunge uno perchè altrimenti il primo proiettile non verrebbe sparato

    int pid_rana = getpid();
    bool primoSparo = false; // true se sono state sparate le granate
    keypad(gioco, TRUE);

    elementoGioco rana;
    int debug_num_spari=0;
    rana.tipo = RANA;
    rana.x = 36;
    rana.y = 16;
    rana.pid_oggetto = getpid();
    rana.pid_oggetto = pid_rana;
    rana.velocita = 0;
    rana.proiettili = TRUE;
    bool danno;
    clock_t start_m, stop_m, start_p, stop_p;
    long int x;
    double durata_f, durata_p;
    bool puoi_sparare=true;
    start_m = clock();
    start_p = clock();

    while (1)
    {
        primoSparo=true;
        
        
        stop_p=clock();    
        durata_p = (double)(stop_p - start_p) / CLOCKS_PER_SEC;

        if (durata_p > tempoRicarica && puoi_sparare){
            beep();
            puoi_sparare=false;
        }
        
        rana.proiettili =false;
        switch (wgetch(gioco))
        {
        case KEY_UP:
            if (rana.y > miny + 5)
                rana.y -= SPOSTAMENTO_RANA;
            break;
        case KEY_DOWN:
            if (rana.y < maxy - 2)
                rana.y += SPOSTAMENTO_RANA;
            break;
        case KEY_LEFT:
            if (rana.x > minx)
                rana.x -= SPOSTAMENTO_RANA;
            break;
        case KEY_RIGHT:
            if (rana.x < maxx - 3)
                rana.x += SPOSTAMENTO_RANA;
            break;
        case KEY_SPACE:
            
            if (durata_p > tempoRicarica)
            {
                puoi_sparare=true;                

                switch (fork())
                {
                case -1:
                                    mvwprintw(gioco,3,maxx-10, "spara sinistra fallito");

                    perror("Errore nell'esecuzione della fork.");

                    _exit(1);
                case 0:
                    // Processo proiettile
                     mvwprintw(gioco,3,3, "spara destra");
                    proiettile(pipeout, rana.y, rana.x,100000,DESTRA,'r');
                    
                    break;
                default:
                    switch (fork())
                    {
                    case -1:
                        perror("Errore nell'esecuzione della fork.");
                        mvwprintw(gioco,3,maxx-10, "spara sinistra fallito");
                        _exit(1);
                    case 0:
                        // Processo proiettile
                         mvwprintw(gioco,3,maxx-10, "spara sinistra");
                        proiettile(pipeout, rana.y, rana.x,100000,SINISTRA,'r');
                        
                        break;
                    default:
                        rana.proiettili = false;
                        start_p=clock();
                        debug_num_spari++;
                        
                        break;
                }break;
            }break;
        }}
      

      if(!danno){
        sleep(1);
        danno=true;
      }
       durata_f= ( double ) ( stop_m - start_m ) / CLOCKS_PER_SEC;
        stop_m = clock();  // stop timer movimento
        rana.direzione=flussi[16-rana.y].direzione;
        rana.velocita=flussi[16-rana.y].velocita;
       if(durata_f*1850000 >= (double)(500000-rana.velocita)){
        if(rana.direzione==DESTRA) {rana.x++;}
        if(rana.direzione==SINISTRA ){ rana.x--;}
        start_m=clock(); 
       }

        // Scrittura nella pipe delle informazioni della rana
        if (write(pipeout, &rana, sizeof(elementoGioco)) == -1)
        {
            perror("Errore nella scrittura sulla pipe");
            _exit(1);
        }
if (read(pipein, &danno, sizeof(danno)) > 0){
       if(!danno){
            wgetch(gioco);
            rana.x = 36;
            rana.y = 16;
            danno=true;
        }
    }
    


    }

    _exit(1);
}