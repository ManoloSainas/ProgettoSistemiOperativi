#include <ncurses.h>
#include "main.h"

// funzione per visualizare il messaggio di avvio del gioco
void avvia_gioco() {
    // Inizializza ncurses
    initscr();
    noecho();
    curs_set(FALSE);

    // Ottieni le dimensioni dello schermo
    int righe, colonne;
    getmaxyx(stdscr, righe, colonne);

    // Il messaggio da visualizzare
    const char *messaggio = "HAI AVVIATO FROGGER - PREMI UN TASTO QUALSIASI PER GIOCARE!";
    int lunghezza_messaggio = strlen(messaggio);

    // Calcola la posizione per centrare il testo
    int inizio_riga = righe / 2;
    int inizio_colonna = (colonne - lunghezza_messaggio) / 2;

    // Stampa il messaggio alla posizione calcolata
    mvprintw(inizio_riga, inizio_colonna, "%s", messaggio);
    refresh();

    // Attendi l'input dell'utente
    getch();

    // Pulisci ncurses
    endwin();
}

// funzione per inizializzare i colori di ncurses
void inizializza_colori() {
    // Inizializza i colori
    start_color();

    // Definisci i colori personalizzati
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

}

// funzione per disegnare la rana
void disegna_rana(int x, int y) {
    attron(COLOR_PAIR(3) | A_BOLD); // Imposta il colore verde e il testo in grassetto
    mvprintw(y, x, RANA);
    attroff(COLOR_PAIR(3) | A_BOLD); // Disattiva il colore verde e il testo in grassetto
}

// funzione per disegnare la barra inferiore con il numero di vite e il tempo restante
void disegna_barra_inferiore(int vite, int tempo) {
    // Ottieni le dimensioni dello schermo
    int righe, colonne;
    getmaxyx(stdscr, righe, colonne);

    // Disegna la barra nella penultima e ultima riga
    attron(COLOR_PAIR(2) | A_BOLD); 
    mvprintw(righe - 1, 10, "VITE: %d", vite);  // Barra vite a sinistra
    mvprintw(righe - 1, colonne - 30, "TEMPO RIMASTO: %d", tempo); // Barra tempo a destra
    attroff(COLOR_PAIR(2) | A_BOLD); 

    
    refresh();
}

// funzione che disegna il marciapiede di partenza
// deve solo colorare lo sfondo per far capire che è un marciapiede, usando il colore magenta
void disegna_marciapiede() {
    // Ottieni le dimensioni dello schermo
    int righe, colonne;
    getmaxyx(stdscr, righe, colonne);

    attron(COLOR_PAIR(5)); // Imposta il colore magenta
    for (int i = 6; i < colonne-6; i++) {
        mvaddch(LINES - 3, i, ' ' | A_REVERSE); // Cambia solo il colore di sfondo
        mvaddch(LINES - 4, i, ' ' | A_REVERSE); // Cambia solo il colore di sfondo
    }
    attroff(COLOR_PAIR(5)); // Disattiva il colore magenta
}