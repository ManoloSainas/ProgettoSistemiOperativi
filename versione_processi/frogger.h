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

// Dimensione Sprite
#define RIGHE_SPRITE_RANA 1
#define COLONNE_SPRITE_RANA 2

#define RIGHE_SPRITE_COCCODRILLO 1
#define COLONNE_SPRITE_COCCODRILLO 4

#define DIMENSIONE_TANA 2

#define RIGHE_SPRITE_PROIETTILE 1
#define COLONNE_SPRITE_PROIETTILE 1

#define RIGHE_SPRITE_GRANATA 1
#define COLONNE_SPRITE_gRANATA 1

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
#define COLORE_RANA_ARGINE 9
#define COLORE_RANA_COCCODRILLO 10
#define COLORE_RANA_TANA 11
#define COLORE_COCCODRILLO 12
#define COLORE_COCCODRILLO_SPARO 13
#define SFONDO_MARCIAPIEDE 14
#define SFONDO_ACQUA 15
#define SFONDO_ERBA 16
#define SFONDO_TANE 17
#define COLORE_TANE 18

// Spostamento oggetti
#define SPOSTAMENTO_RANA 1

// direzione flusso fiume
typedef enum
{
    SINISTRA,
    DESTRA,
    NESSUNA
} DirezioneFlusso;

// differenziazione tra oggetti
typedef enum tipoOggetto
{
    RANA,
    COCCODRILLO,
    GRANATA_RANA,
    PROIETTILE_COCCODRILLO
} tipoOggetto;

/*typedef enum statusOggetto
{
    NON_ATTIVO,
    ATTIVO,
    TERMINATO
} statusOggetto;
*/

// struttura per rappresentare i vari elementi non statici del gioco
typedef struct elementoGioco
{
    tipoOggetto tipo;
    int x;
    int y;
    int pid_oggetto;
    int velocita;
    DirezioneFlusso direzione;
    // statusOggetto status;
} elementoGioco;

typedef struct corrente
{
    DirezioneFlusso direzione;
    int velocita;
} corrente;

// Tipologia per distinguere le modalità di utilizzo della pipe
typedef enum tipoDescrittore
{
    LETTURA,
    SCRITTURA
} tipoDescrittore;

// Coordinate dell'area di gioco
extern int minx,
    miny;
extern int maxx, maxy;

// Schermo ncurses
extern WINDOW *gioco;

void inizializzazioneSchermo();
void graficaGioco();
void avviaGioco();
void stampaSprite(elementoGioco elemento);
void cancellaSprite(elementoGioco elemento);
void rana(int pipeout);
void coccodrillo(int pipeout, int riga, int id_coccodrillo, corrente flusso);
void controlloGioco(int pipein);
void terminaGioco();
void chiudiProcessi();
void gestioneFlussi(corrente *flussi, int *coccodrilli_flusso);
void inizializzazionePipe(int filedes[]);