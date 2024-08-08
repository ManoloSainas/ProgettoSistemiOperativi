#include "main.h"

void rana() {
  int max_x, max_y;
  getmaxyx(stdscr, max_y,
           max_x); // Get the current dimensions of the terminal window

  int x =
      (max_x - LARGHEZZA_RANA) / 2; // Posizione iniziale X (centro in basso)
  int y = max_y - ALTEZZA_RANA;     // Posizione iniziale Y (in basso)
  int ch;

  // Disegna la rana nella posizione iniziale
  mvprintw(y, x, RANA);
  refresh();

  while ((ch = getch()) != 'q') {
    // Cancella la rana dalla posizione attuale
    mvprintw(y, x, "  ");

    // Aggiorna la posizione della rana in base all'input dell'utente
    switch (ch) {
    case SU:
      if (y - SALTO_RANA_SU >= 0) {
        y -= SALTO_RANA_SU;
      }
      break;
    case GIU:
      if (y + SALTO_RANA_GIU < max_y) {
        y += SALTO_RANA_GIU;
      }
      break;
    case SINISTRA:
      if (x - SALTO_RANA_SINISTRA >= 0) {
        x -= SALTO_RANA_SINISTRA;
      }
      break;
    case DESTRA:
      if (x + SALTO_RANA_DESTRA < max_x - LARGHEZZA_RANA) {
        x += SALTO_RANA_DESTRA;
      }
      break;
    default:
      break;
    }

    // Disegna la rana nella nuova posizione
    mvprintw(y, x, RANA);
    refresh();
  }
}