#include "frogger.h"


void rana(int pipeout, int pipein, corrente flussi[])
{

    // gestione tempo sparo
    struct timeval start, end;
    int tempoTrascorso, tempoRicarica = RICARICA_PROIETTILI * maxx; // Adatta la velocità di ricarica dei proiettili a seconda della dimensione dello schermo

    tempoTrascorso = tempoRicarica + 1; // si aggiunge uno perchè altrimenti il primo proiettile non verrebbe sparato

    int pid_rana = getpid();
    bool primoSparo = false; // true se sono state sparate le granate
    keypad(gioco, TRUE);

    elementoGioco rana;
    rana.tipo = RANA;
    rana.x = 36;
    rana.y = 16;
    rana.pid_oggetto = getpid();
    rana.pid_oggetto = pid_rana;
    rana.velocita = 0;
    rana.proiettili = TRUE;
    bool danno;
    clock_t start_m, stop_m;
    long int x;
    double duration;

    start_m = clock();


    while (1)
    {
        rana.direzione=flussi[16-rana.y].direzione;
        rana.velocita=flussi[16-rana.y].velocita;

        if (primoSparo)
        {
            gettimeofday(&end, NULL);
            tempoTrascorso = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);

            // Comunica se si può sparare la prossima coppia di proiettili
            if (tempoTrascorso > tempoRicarica)
            {
                rana.proiettili = true;
                write(pipeout, &rana, sizeof(elementoGioco));
            }
        }

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
            if (tempoTrascorso > tempoRicarica)
            {

                pid_rana = fork();

                switch (pid_rana)
                {
                case -1:
                    perror("Errore nell'esecuzione della fork.");
                    _exit(1);
                case 0:
                    // Processo proiettile
                   granateRana(pipeout, rana.y, rana.x);
                    break;
                default:
                    rana.proiettili = false;
                    gettimeofday(&start, NULL);
                    primoSparo = true;

                    tempoTrascorso = 0;
                    break;
                }
            }
        }
      
      if(!danno){
        sleep(1);
        danno=true;
      }
       duration = ( double ) ( stop_m - start_m ) / CLOCKS_PER_SEC;
        stop_m = clock();  // stop timer movimento

       if(duration >= (double)(500000-rana.velocita)/2500000){
       
        if(rana.direzione==DESTRA) {rana.x++;}
        if(rana.direzione==SINISTRA){ rana.x--;}
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