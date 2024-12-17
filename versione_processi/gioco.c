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
    // flussi[1].velocita = rand() % (10) + 1;
    flussi[1].velocita = 1;

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

void avviaGioco()
{

    int filedes[2];
    int pid_gioco;
    int coccodrilli_flusso[NUM_FLUSSI_FIUME];
    corrente flussi[NUM_FLUSSI_FIUME + 3];

    graficaGioco();
    gestioneFlussi(flussi, coccodrilli_flusso);
    inizializzazionePipe(filedes);

    pid_gioco = fork();

    switch (pid_gioco)
    {
    case -1:
        perror("Errore nella creazione del processo per la rana");
        _exit(1);
        break;
    case 0:
        close(filedes[LETTURA]);
        rana(filedes[SCRITTURA]);
        _exit(0);
        break;
    default:
        for (int i = 1; i <= NUM_FLUSSI_FIUME; i++)
        {
            for (int j = 0; j < coccodrilli_flusso[i]; j++)
            {

                pid_gioco = fork();
                switch (pid_gioco)
                {
                case -1:
                    perror("Errore nella creazione del processo per il coccodrillo");
                    _exit(1);
                    break;
                case 0:
                    close(filedes[LETTURA]);
                    coccodrillo(filedes[SCRITTURA], i, flussi[i]);
                    _exit(0);
                    break;
                default:
                    break;
                }
            }
        }
        close(filedes[SCRITTURA]);
        controlloGioco(filedes[LETTURA]);
        // Wait for all child processes to complete
        for (int i = 1; i <= NUM_FLUSSI_FIUME; i++)
        {
            for (int j = 0; j < coccodrilli_flusso[i]; j++)
            {
                waitpid(-1, NULL, 0);
            }
        }
        break;
    }

    // terminaGioco();
}

void terminaGioco()
{
    delwin(gioco); // Delete the window
    endwin();      // End ncurses
}

void controlloGioco(int pipein)
{
    int vita = 3;
    elementoGioco valoreLetto;
    elementoGioco rana, coccodrillo;

    do
    {

        if (read(pipein, &valoreLetto, sizeof(valoreLetto)) > 0)
        {
            elementoGioco oggettoLetto;
            switch (valoreLetto.tipo)
            {
            case RANA:
                oggettoLetto = rana;
                break;
            case COCCODRILLO:
                oggettoLetto = coccodrillo;
                break;
            }
            cancellaSprite(oggettoLetto);

            switch (valoreLetto.tipo)
            {
            case RANA:
                rana = valoreLetto;
                break;
            case COCCODRILLO:
                coccodrillo = valoreLetto;
                break;
            }

            // graficaGioco();

            stampaSprite(coccodrillo);
            stampaSprite(rana);

            wrefresh(gioco);
        }

    } while (true);

    chiudiProcessi(&rana);
    chiudiProcessi(&coccodrillo);
    terminaGioco();
}