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

// Numero dati
#define NUM_VITE_RANA 3
#define NUM_TANE 5
#define NUM_MIN_COCCODRILLI_FLUSSO 5
#define NUM_FLUSSI_FIUME 8

// Tempo di gioco
#define TEMPO_TOTALE 60; // Tempo totale di gioco (per round)

#define KEY_SPACE 32 // Valore relativo al carattere spazio

// Colori standard
#define COLORE_STANDARD 1
#define COLORE_CANCELLAZIONE 2
#define COLORE_ROSSO 3
#define COLORE_CIANO 4
#define COLORE_BLU 5
#define COLORE_VERDE 6
#define COLORE_GIALLO 7
#define COLORE_MAGENTA 8

// Colori gioco
#define COLORE_RANA 9
#define COLORE_COCCODRILLO 10
#define COLORE_COCCODRILLO_SPARO 11
#define SFONDO_MARCIAPIEDE 12
#define SFONDO_ACQUA 13
#define SFONDO_ERBA 14
#define SFONDO_TANE 15
#define COLORE_TANE 16

// Coordinate dell'area di gioco
extern int minx, miny;
extern int maxx, maxy;

// Schermo ncurses
extern WINDOW *gioco;

int inizializzazioneSchermo();