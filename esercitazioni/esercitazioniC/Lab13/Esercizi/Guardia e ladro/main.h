#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define PASSO 1     /* Entita spostamento del ladro */
#define SU 65       /* Freccia su */
#define GIU 66      /* Freccia giu */
#define SINISTRA 68 /* Freccia sinistra */
#define DESTRA 67   /* Freccia destra */
#define MAXX 80     /* Numero di colonne dello schermo */
#define MAXY 24     /* Numero di righe dello schermo */

/* Struttura per la comunicazione tra figli e padre */
struct pos {
  char c; /* soggetto che invia il dato: ladro o guardia */
  int x;  /* coordinata x */
  int y;  /* coordinata y */
};

void ladro(int pipeout);
void guardia(int pipeout);
void controllo(int pipein);