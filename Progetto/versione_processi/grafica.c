// Grafica del gioco

#include "main.h"

// calcola la dimensione della finestra, scrive una scritta rossa su fondo
// bianco che indica che il gioco sta caricando e attende 2 secondi, poi comare
// una schermata totalmente verde

void menu_avvio() {
  // Inizializza la finestra
  initscr();
  // Nasconde il cursore
  curs_set(0);
  // Inizializza i colori
  start_color();
  // Inizializza i colori predefiniti
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_GREEN);
  // Calcola la dimensione della finestra
  int x, y;
  getmaxyx(stdscr, y, x);
  // Stampa la scritta "Caricamento..." al centro della finestra
  attron(COLOR_PAIR(1));
  mvprintw(y / 2, x / 2 - 5, "Caricamento Frogger...");
  attroff(COLOR_PAIR(1));
  // Aggiorna la finestra
  refresh();
  // Aspetta 2 secondi
  sleep(3);
  // Stampa la schermata verde
  attron(COLOR_PAIR(2));
  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {
      mvprintw(i, j, " ");
    }
  }
  attroff(COLOR_PAIR(2));
  // Aggiorna la finestra
  refresh();
  // Aspetta 2 secondi
  sleep(10);
  // Chiude la finestra
  endwin();
}