#include "frogger.h"

// Definizione delle variabili globali
int minx, miny;
WINDOW *gioco;

bool verificaTanaStatus(bool tana_status[])
{
    for (int i = 0; i < NUM_TANE; i++)
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

int main()
{
    int status;
    pid_t pid;
    bool tana_status[NUM_TANE];
    bool esitoPartita;
    int punteggio;
    int vite;

    inizializzazioneSchermo();
    srand(time(NULL));
    do
    {
        vite = NUM_VITE_RANA;
        punteggio = 0;

        for (int i = 0; i < NUM_TANE; i++)
        {
            tana_status[i] = true;
        }
        while (vite > 0 && verificaTanaStatus(tana_status))
        {

            pid = fork();
            switch (pid)
            {
            case -1:
                perror("Errore nella creazione del processo gioco");
                _exit(1);
                break;
            case 0:
                avviaGioco(vite, tana_status, punteggio);
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
                        punteggio -= vite * 5;
                        vite--;
                        break;
                    case 1:
                        tana_status[0] = false;
                        punteggio += vite * 10;
                        break;
                    case 2:
                        tana_status[1] = false;
                        punteggio += vite * 10;
                        break;
                    case 3:
                        tana_status[2] = false;
                        punteggio += vite * 10;
                        break;
                    case 4:
                        tana_status[3] = false;
                        punteggio += vite * 10;
                        break;
                    case 5:
                        tana_status[4] = false;
                        punteggio += vite * 10;
                        break;
                    default:
                        mvwprintw(gioco, 4, 1, "Figlio terminato.");
                        break;
                    }
                }

                wrefresh(gioco);
            }
        }

        if (vite > 0)
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
