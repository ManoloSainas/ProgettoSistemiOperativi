#include "frogger.h"

// Definizione delle variabili globali
int minx, miny;
WINDOW *gioco;

bool verificaTanaStatus(bool tana_status[])
{
    for (int i = 0; i < 5; i++)
    {
        if (tana_status[i])
        {
            return true;
        }
    }
    return false;
}

bool schermataFineGioco(bool esitoPartita, int score)
{
    int risposta;

    wclear(gioco);
    esitoPartita ? mvwprintw(gioco, 1, 2, "HAI VINTO!") : mvwprintw(gioco, 1, 2, "HAI PERSO!");
    mvwprintw(gioco, 2, 2, "Score: %d", score);
    mvwprintw(gioco, 4, 2, "Vuoi rigiocare? (y,n)");
    wrefresh(gioco);

    while (true)
    {
        risposta = wgetch(gioco);

        if (risposta == 'y' || risposta == 'Y')
        {
            wclear(gioco);
            wrefresh(gioco);
            return true;
        }
        else if (risposta == 'n' || risposta == 'N')
        {
            wclear(gioco);
            wrefresh(gioco);
            return false;
        }
    }
}

void *avviaGiocoThread(void *arg)
{
    // Extract arguments
    int *args = (int *)arg;
    int vita = args[0];
    bool *tana_status = (bool *)args[1];
    int punteggio = args[2];

    int status = avviaGioco(vita, tana_status, punteggio);
    return (void *)(intptr_t)status;
}

int main()
{
    int status;
    pthread_t thread;
    bool tana_status[5];
    bool esitoPartita;
    int punteggio;
    int vita;
    inizializzazioneSchermo();
    srand(time(NULL));
    do
    {
        vita = 3;
        punteggio = 0;

        for (int i = 0; i < 5; i++)
        {
            tana_status[i] = true;
        }
        while (vita > 0 && verificaTanaStatus(tana_status))
        {
            int args[3] = {vita, (int)tana_status, punteggio};
            pthread_create(&thread, NULL, avviaGiocoThread, (void *)args);
            pthread_join(thread, (void **)&status);

            if (status >= 0)
            {
                switch (status)
                {
                case -1:
                    mvwprintw(gioco, 1, 1, "errore");
                    break;
                case 6:
                    punteggio -= vita * 5;
                    vita--;
                    break;
                case 1:
                    tana_status[0] = false;
                    punteggio += vita * 10;
                    break;
                case 2:
                    tana_status[1] = false;
                    punteggio += vita * 10;
                    break;
                case 3:
                    tana_status[2] = false;
                    punteggio += vita * 10;
                    break;
                case 4:
                    tana_status[3] = false;
                    punteggio += vita * 10;
                    break;
                case 5:
                    tana_status[4] = false;
                    punteggio += vita * 10;
                    break;
                default:
                    mvwprintw(gioco, 4, 1, "Thread terminato.");
                    break;
                }
            }

            wrefresh(gioco);
        }

        if (vita > 0)
        {
            esitoPartita = true;
        }
        else
        {
            esitoPartita = false;
        }
    } while (schermataFineGioco(esitoPartita, punteggio));
    terminaGioco();
    return 0;
}