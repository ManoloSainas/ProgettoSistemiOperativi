// Main del programma

#include "main.h"

int main() {
  // Mantieni la rana attiva finché l'utente non preme 'q'
  int ch;

  initscr(); /* inizializzazione dello schermo */
  noecho();  /* i caratteri corrispondenti ai tasti premuti non saranno
                visualizzati sullo schermo del terminale */
  srand((int)time(0)); /* inizializziamo il generatore di numeri random */
  curs_set(0);         /* nasconde il cursore */

  // crea la rana
  rana();

  while ((ch = getch()) != 'q') {
    // Puoi aggiungere qui eventuali aggiornamenti o logica aggiuntiva
  }

  endwin(); /* termina la modalità ncurses */

  return 0;
}