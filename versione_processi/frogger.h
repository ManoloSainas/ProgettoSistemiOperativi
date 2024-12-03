#include <stdio.h>
#include <stdlib.h>

// Librerie per la gestione dei processi e delle pipes
#include <signal.h>
#include <unistd.h>

// Librerie per le chiamate a sistema
#include <sys/types.h>
#include <sys/wait.h>

#include <ncurses.h>  //Libreria ncurses
#include <sys/time.h> //Libreria per tenere traccia del tempo
#include <time.h>     //Libreria per la generazione casuale

// Macro

// Dimensioni del campo di gioco
#define MAXX 80
#define MAXY 24

// Coordinate dell'area di gioco
extern int minx, miny;
extern int maxx, maxy;

// Schermo ncurses
extern WINDOW *gioco;

int inizializzazioneSchermo();