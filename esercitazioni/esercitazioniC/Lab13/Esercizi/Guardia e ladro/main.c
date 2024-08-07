#include "main.h"
#include "ladro.h"

int main() {
  int filedes[2];
  int pid_ladro;
  int pid_guardia;

  initscr(); /* inizializzazione dello schermo */
  noecho();  /* i caratteri corrispondenti ai tasti premuti non saranno
                visualizzati sullo schermo del terminale */
  srand((int)time(0)); /* inizializziamo il generatore di numeri random */
  curs_set(0);         /* nasconde il cursore */

  if (pipe(filedes) == -1) {
    perror("Errore nella creazione della pipe!");
    _exit(1);
  }

  pid_ladro = fork();
  switch (pid_ladro) {
  case -1:
    perror("Errore nell'esecuzione della fork.");
    exit(1);
  case 0:
    close(filedes[0]); /* chiusura del descrittore di lettura */
    ladro(filedes[1]); /* il primo processo figlio invoca la funzione guardia */
  default:
    pid_guardia = fork();

    switch (pid_guardia) {
    case -1:
      perror("Errore nell'esecuzione della fork.");
      _exit(1);

    case 0:
      close(filedes[0]);   /* chiusura del descrittore di lettura */
      guardia(filedes[1]); /* il secondo processo figlio invoca la funzione
                              guardia */

    default:
      close(filedes[1]); /* chiusura del descrittore di scrittura */
      controllo(
          filedes[0]); /* il processo padre invoca la funzione di controllo */
    }
  }

  /* siamo usciti dalla funzione di controllo e vengono terminati i 2 processi
   * figli e ripristinato il normale modo operativo dello schermo */
  kill(pid_ladro, 1);
  kill(pid_guardia, 1);
  endwin();
  printf("Fine del gioco\n");
  return 0;
}
