#include "frogger.h"
elementoGioco ranaGiocatore;

void rana(int pipeout, int pipein, corrente flussi[])
{
    fcntl(pipein, F_SETFL, O_NONBLOCK); // pipe in scrittura della rana non bloccante
   
  
    signal(SIGUSR1, handler_rana); //gestione del segnale in caso la rana sia sopra un coccodrillo


    keypad(gioco, TRUE);

    //variabile per il controllo della quantità di spari che il giocatore può fare
    int num_spari = 0;
    //inizializazione dell'elemento rana
    ranaGiocatore.tipo = RANA;
    ranaGiocatore.x = RANA_X;
    ranaGiocatore.y = RANA_Y;
    ranaGiocatore.pid_oggetto = getpid();
    ranaGiocatore.velocita = 0;

    //double durata_f, durata_p;
    bool puoi_sparare = true;
    
    pid_t proiettilePid;
    //elemento proiettile che verrà sovrascritto quando leggerà la pipe
    posizione dati_p;

    while (1)
    {
    
        // Calcolo del ritardo basato sulla velocità
        int delay = 500000 - ranaGiocatore.velocita;
        if (delay < 0)
            delay = 0;

        //gestione movimento rana e tasto sparo
        int ch = wgetch(gioco);
        switch (ch)
        {
        case KEY_UP:
            if (ranaGiocatore.y > miny + 5)
                ranaGiocatore.y -= SPOSTAMENTO_RANA;
            break;
        case KEY_DOWN:
            if (ranaGiocatore.y < maxy - 2)
                ranaGiocatore.y += SPOSTAMENTO_RANA;
            break;
        case KEY_LEFT:
            if (ranaGiocatore.x > minx)
                ranaGiocatore.x -= SPOSTAMENTO_RANA;
            break;
        case KEY_RIGHT:
            if (ranaGiocatore.x < maxx - 3)
                ranaGiocatore.x += SPOSTAMENTO_RANA;
            break;
        case KEY_SPACE:

            if (num_spari <= MAXGRANATE - 2)//se la rana non può sparare almeno 2 proiettili il tasto spazio non farà nulla
            {
                num_spari++;
                switch (fork())//creazione processo granata sinistra
                {
                case -1:
                    mvwprintw(gioco, 3, maxx - 10, "spara sinistra fallito");

                    perror("Errore nell'esecuzione della fork sparo sinistra.");

                    _exit(1);
                case 0:
                    // Processo proiettile
                    proiettile(pipeout, ranaGiocatore.y, ranaGiocatore.x, SPEED_GRANATE, DESTRA, 'r');

                    break;
                default:
                        //creazione processo granata destra
                        num_spari++;
                        switch (fork())
                        {
                        case -1:
                            perror("Errore nell'esecuzione della fork sparo destra");
                            mvwprintw(gioco, 3, maxx - 10, "spara sinistra fallito");
                            _exit(1);
                        case 0:
                            // Processo proiettile
                            proiettile(pipeout, ranaGiocatore.y, ranaGiocatore.x, SPEED_GRANATE, SINISTRA, 'r');

                            break;
                        default:

                           

                            break;
                        }
                        break;
                    
                    break;
                }
            }
        }

      
        ranaGiocatore.proiettile = num_spari;
        // Scrittura nella pipe delle informazioni della rana
        if (write(pipeout, &ranaGiocatore, sizeof(elementoGioco)) == -1)
        {
            perror("Errore nella scrittura sulla pipe rana->main");
            _exit(1);
        }
        //lettura della pipe per l'eliminazione dei proiettili
        if (read(pipein, &dati_p, sizeof(posizione)) > 0)
        {
            if (kill(dati_p.pid, 0) == 0)//per accertarsi che il processo sia effettivamente in vita
            {
                chiudiProcessi(dati_p.pid);
                num_spari--;
            }
        }
    }

    _exit(1);
}

void handler_rana(int sig)//gestione del segnale di quando un coccodrillo si muove con la rana sopra
{
    if (sig == SIGUSR1)
    {
        if (ranaGiocatore.direzione == DESTRA)
            ranaGiocatore.x++;
        if (ranaGiocatore.direzione == SINISTRA)
            ranaGiocatore.x--;
    }
}