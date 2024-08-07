#include "controllo.h"
#include "main.h"

void controllo(int pipein) {
  struct pos ladro, guardia, valore_letto;
  ladro.x = -1;
  guardia.x = -1;
  do {
    read(pipein, &valore_letto, sizeof(valore_letto)); /* leggo dalla pipe */
    if (valore_letto.c == '$') {
      if (ladro.x >= 0) { /* cancello la 'vecchia' posizione del ladro */
        mvaddch(ladro.y, ladro.x, ' ');
      }
      ladro = valore_letto;
    } else {
      if (guardia.x >= 0) { /* cancello la 'vecchia' posizione della guardia */
        mvaddch(guardia.y, guardia.x, ' ');
      }
      guardia = valore_letto;
    }
    /* visualizzo l'oggetto nella posizione aggiornata */
    mvaddch(valore_letto.y, valore_letto.x, valore_letto.c);
    curs_set(0);
    refresh();
  } while (guardia.x != ladro.x ||
           guardia.y != ladro.y); /* ciclo fino al verificarsi di una collisione
                                     ladro/guardia */
}
