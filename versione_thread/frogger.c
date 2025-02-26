#include "frogger.h"

// Define the extern variables
int minx = 0;
int miny = 0;
WINDOW *gioco = NULL;
corrente flussi[NUM_FLUSSI_FIUME + 3];

// se tutte le tane sono chiuse (false) il gioco termina
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
    int risposta; // risposta dell'utente alla domanda se vuole rigiocare

    // stampa della schermata fine gioco
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
    bool tana_status[NUM_TANE]; // array che tiene traccia dello stato delle tane
    bool esitoPartita;          // esito della partita
    int punteggio;
    int vite, tempo;
    pthread_t gioco_thread;

    inizializzazioneSchermo();
    inizializza_meccanismi_sincronizzazione();

    do
    {
        // inizializzazzione vite e punti per una nuova partita
        vite = NUM_VITE_RANA;
        punteggio = 0;

        // inizializzazione status delle tane a true, quando chiuse viene impostata a false
        for (int i = 0; i < NUM_TANE; i++)
        {
            tana_status[i] = true;
        }
        tempo = TEMPO_TOTALE;
        // finché ci sono vite e tane aperte
        while (vite > 0 && verificaTanaStatus(tana_status))
        {

            int risultato = controlloGioco(vite, tana_status, tempo, punteggio);
            switch (risultato)
            {
            case -1:
                mvwprintw(gioco, 1, 1, "errore");
                break;
            case 6: // danno
                punteggio -= vite * 5;
                vite--;
                break;
            case 1: // prima tana chiusa
                tana_status[0] = false;
                punteggio += vite * 10;
                break;
            case 2: // seconda tana chiusa
                tana_status[1] = false;
                punteggio += vite * 10;
                break;
            case 3: // terza tana chiusa
                tana_status[2] = false;
                punteggio += vite * 10;
                break;
            case 4: // quarta tana chiusa
                tana_status[3] = false;
                punteggio += vite * 10;
                break;
            case 5: // quinta tana chiusa
                tana_status[4] = false;
                punteggio += vite * 10;
                break;
            default:
                break;
            }
        }
        wrefresh(gioco);

        // se le vite sono terminate la partita è persa
        if (vite > 0)
        {
            esitoPartita = true;
        }
        else
        {
            esitoPartita = false;
        }
    } while (schermataFineGioco(esitoPartita, punteggio)); // continua finchè l'utente vuole rigiocare
    terminaGioco(); // pulisce e chiude la finestra
    return 0;
}