#include "frogger.h"

// Definizione delle variabili globali
int minx, miny;
WINDOW *gioco;
// int viteRana = 3;

// bool tane[5] = {false, false, false, false, false};

// posizioneTane posTane[5] = {
//     {11, 12, 6},
//     {23, 24, 6},
//     {35, 36, 6},
//     {47, 48, 6},
//     {59, 60, 6}};

bool verificaTanaStatus(bool tana_status[]){
    for(int i = 0; i < 5; i++){
        if (tana_status[i]){
            return true;
        }
    }
    return false;
}

bool schermataFineGioco(bool esitoPartita, int score){
    int risposta;

    wclear(gioco);
    esitoPartita  ? mvwprintw(gioco, 1, 2, "HAI VINTO!") : mvwprintw(gioco, 1, 2, "HAI PERSO!");
    mvwprintw(gioco, 2, 2, "Score: %d", score);
    mvwprintw(gioco, 4, 2, "Vuoi rigiocare? (y,n)");
    wrefresh(gioco);

    while(true){
        risposta = wgetch(gioco); 

        if(risposta == 'y' || risposta == 'Y'){wclear(gioco);wrefresh(gioco); return true;}
        else if (risposta == 'n' || risposta == 'N') {wclear(gioco);wrefresh(gioco);return false;}
    }
}

int main()
{
    int status;
    pid_t pid;
    bool tana_status[5];
    bool esitoPartita;
    int punteggio;
    int vita;
    inizializzazioneSchermo();
    srand(time(NULL));
    do{
        vita = 3;
        punteggio = 0;

        for(int i=0;i<5;i++){
        tana_status[i] = true;
        }
        while (vita > 0 && verificaTanaStatus(tana_status))
        {
            pid = fork();
            switch (pid)
            {
            case -1:
                perror("Errore nella creazione del processo gioco");
                _exit(1);
                break;
            case 0:
                avviaGioco(vita, tana_status, punteggio);
                break;
            default:
                waitpid(pid, &status, 0);

                if (WIFEXITED(status) > 0)
                {
                    switch (WEXITSTATUS(status))
                    {
                    case -1:
                        mvwprintw(gioco, 1, 1, "errore");
                        break;
                    case 6:
                        // mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                        // mvwprintw(gioco, 3, 1, "Vita persa.");
                        punteggio -= vita * 5;
                        vita--;
                        break;
                    case 1:
                        // mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                        // mvwprintw(gioco, 3, 1, "tana chiusa 1");
                        tana_status[0] = false;
                        punteggio += vita * 10;
                        break;
                    case 2:
                        // mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                        // mvwprintw(gioco, 3, 1, "tana chiusa 2");
                        tana_status[1] = false;
                        punteggio += vita * 10;
                        break;
                    case 3:
                        // mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                        // mvwprintw(gioco, 3, 1, "tana chiusa 3");
                        tana_status[2] = false;
                        punteggio += vita * 10;
                        break;
                    case 4:
                        // mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                        // mvwprintw(gioco, 3, 1, "tana chiusa 4");
                        tana_status[3] = false;
                        punteggio += vita * 10;
                        break;
                    case 5:
                        // mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                        // mvwprintw(gioco, 3, 1, "tana chiusa 5");
                        tana_status[4] = false;
                        punteggio += vita * 10;
                        break;
                    default:
                        mvwprintw(gioco, 4, 1, "Figlio terminato.");
                        break;
                    }
                }

                wrefresh(gioco);
            }
        }
   
    if(vita > 0){esitoPartita = true;} else {esitoPartita = false;}
    }while(schermataFineGioco(esitoPartita, punteggio));
    terminaGioco();
    return 0;
}
