#include "frogger.h"

// Definizione delle variabili globali
int minx, miny;
WINDOW *gioco;
int viteRana = 3;

bool tane[5] = {false, false, false, false, false};

posizioneTane posTane[5] = {
    {11, 12, 6},
    {23, 24, 6},
    {35, 36, 6},
    {47, 48, 6},
    {59, 60, 6}};

int main()
{
    int vita = 3;
    int status;
    pid_t pid;
    bool tana_status[5] = {true, true, true, true, true};
    srand(time(NULL));
    inizializzazioneSchermo();

    while (vita > 0)
    {
        pid = fork();
        switch (pid)
        {
        case -1:
            perror("Errore nella creazione del processo gioco");
            _exit(1);
            break;
        case 0:
            avviaGioco(vita, tana_status);
            break;
        default:
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) >= 0)
            {
                switch (WEXITSTATUS(status))
                {
                case -1:
                    mvwprintw(gioco, 1, 1, "errore");
                    break;
                case 0:
                    mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                    mvwprintw(gioco, 3, 1, "Vita persa.");
                    vita--;
                    break;
                case 1:
                    mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                    mvwprintw(gioco, 3, 1, "tana chiusa 1");
                    tana_status[0] = false;
                    break;
                case 2:
                    mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                    mvwprintw(gioco, 3, 1, "tana chiusa 2");
                    tana_status[1] = false;
                    break;
                case 3:
                    mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                    mvwprintw(gioco, 3, 1, "tana chiusa 3");
                    tana_status[2] = false;
                    break;
                case 4:
                    mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                    mvwprintw(gioco, 3, 1, "tana chiusa 4");
                    tana_status[3] = false;
                    break;
                case 5:
                    mvwprintw(gioco, 4, 1, "%d", WEXITSTATUS(status));
                    mvwprintw(gioco, 3, 1, "tana chiusa 5");
                    tana_status[4] = false;
                    break;
                default:
                    mvwprintw(gioco, 4, 1, "Figlio terminato.");
                    break;
                }
            }
            wrefresh(gioco);
        }
    }
    terminaGioco();
    return 0;
}
