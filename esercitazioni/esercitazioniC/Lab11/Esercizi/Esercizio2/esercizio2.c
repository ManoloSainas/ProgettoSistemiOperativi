
// ● Partendo dal codice dell’esempio 05 visto a lezione, codificare in C e con l’utilizzo 
// delle ncurses un programma che simuli un semplice gioco singolo processo
// ○ L’utente controlla un carattere # con le frecce, e il carattere q induce la terminazione
// ■ Si consiglia di impostare un comportamento non-bloccante per l’input utente
// ○ L’utente deve raccogliere i caratteri $ per accumulare punti
// ■ I caratteri $ sono generati casualmente sullo schermo, fino ad un massimo di N
// ■ Ogni carattere raccolto da sempre lo stesso numero di punti
// ■ Raccolti dei $, altri possono essere generati in seguito finché non sono nuovamente N
// ○ Lo schermo si compone di due finestre separate orizzontali
// ■ Le prime 3 righe sono riservate alla stampa del punteggio
// ■ Tutte le restanti righe sono la zona di gioco in cui compaiono i caratteri # e $
// ○ Per verificare che il carattere # dell’utente sia passato o meno sopra un carattere $
// leggere direttamente dalla finestra (non usare strutture dati ausiliarie)


#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX_COINS 10
#define COIN_POINTS 10

int main() {
    initscr(); // Inizializza la libreria ncurses e imposta lo schermo.
    cbreak(); // Disabilita il buffering di linea, rendendo i caratteri di input immediatamente disponibili al programma.
    noecho(); // Disabilita l'eco dei caratteri digitati sullo schermo.
    nodelay(stdscr, TRUE); // Rende getch() non bloccante, quindi restituisce ERR se non ci sono input disponibili.
    curs_set(0); // Nasconde il cursore.
    timeout(0); // Imposta getch() per essere non bloccante con un timeout di 0 millisecondi.
    keypad(stdscr, TRUE); // Abilita la lettura dei tasti funzione come le frecce.
    srand(time(NULL)); // Inizializza il generatore di numeri casuali con l'ora corrente.
    int x = COLS / 2, y = LINES / 2; // Dichiara le variabili per le coordinate x e y.
    int score = 0; // Dichiara la variabile per il punteggio.
    int coins = 0; // Dichiara la variabile per il numero di monete.
    int coin_x[MAX_COINS], coin_y[MAX_COINS]; // Dichiara le variabili per le coordinate x e y delle monete.
    for (int i = 0; i < MAX_COINS; i++) { // Inizializza le coordinate delle monete.
        coin_x[i] = rand() % COLS; // Genera una coordinata x casuale all'interno della larghezza dello schermo.
        coin_y[i] = rand() % (LINES - 3) + 3; // Genera una coordinata y casuale all'interno dell'altezza dello schermo escludendo le prime 3 righe.
    }
    while (1) { // Inizia un ciclo infinito.
        mvprintw(0, 0, "Score: %d", score); // Stampa il punteggio nella prima riga.
        mvprintw(1, 0, "Coins: %d", coins); // Stampa il numero di monete nella seconda riga.
        mvaddch(y, x, '#'); // Sposta il cursore a (y, x) e aggiunge il carattere '#'.
        for (int i = 0; i < MAX_COINS; i++) { // Per ogni moneta.
            if (coin_x[i] == x && coin_y[i] == y) { // Se il carattere '#' è sopra una moneta.
                score += COIN_POINTS; // Aggiungi i punti della moneta al punteggio.
                coins++; // Incrementa il numero di monete raccolte.
                coin_x[i] = rand() % COLS; // Genera una nuova coordinata x casuale per la moneta.
                coin_y[i] = rand() % (LINES - 3) + 3; // Genera una nuova coordinata y casuale per la moneta escludendo le prime 3 righe.
            }
            mvaddch(coin_y[i], coin_x[i], '$'); // Sposta il cursore alla posizione della moneta e aggiunge il carattere '$'.
        }
        refresh(); // Aggiorna lo schermo per mostrare le modifiche.
        int key = getch(); // Legge un carattere di input.
        if (key == 'q') { // Se il carattere di input è 'q'.
            break; // Esce dal ciclo.
        }
        mvaddch(y, x, ' '); // Cancella il carattere '#' dalla posizione attuale.
        if (key == KEY_UP && y > 3) { // Se il carattere di input è la freccia su e il carattere '#' non è al bordo superiore.
            y--; // Sposta il carattere '#' verso l'alto.
        }
        if (key == KEY_DOWN && y < LINES - 1) { // Se il carattere di input è la freccia giù e il carattere '#' non è al bordo inferiore.
            y++; // Sposta il carattere '#' verso il basso.
        }
        if (key == KEY_LEFT && x > 0) { // Se il carattere di input è la freccia sinistra e il carattere '#' non è al bordo sinistro.
            x--; // Sposta il carattere '#' verso sinistra.
        }
        if (key == KEY_RIGHT && x < COLS - 1) { // Se il carattere di input è la freccia destra e il carattere '#' non è al bordo destro.
            x++; // Sposta il carattere '#' verso destra.
        }
    }
    endwin(); // Termina la sessione ncurses e ripristina le impostazioni del terminale.
    return 0; // Restituisce 0 per indicare un'esecuzione riuscita.
}