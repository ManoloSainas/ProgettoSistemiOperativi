// #define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>

// Librerie per la gestione dei processi e delle pipes
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

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
#define NUM_COCCODRILLI_FLUSSO 3
#define NUM_FLUSSI_FIUME 8

// quantità oggetti su schermo
#define MAXCOCCODRILLI 40
#define MAXGRANATE 12

// velocità flusso fiume
#define VELOCITA_FLUSSO 20000

// Tempo di gioco
#define TEMPO_TOTALE 90; // Tempo totale di gioco (per round)

// Gestione proiettili
#define RICARICA_PROIETTILI 1
#define SPEED_PROIETTILI 200000
#define SPEED_GRANATE 100000

// Posizione iniziale rana
#define RANA_X 36
#define RANA_Y 16

// Spostamento oggetti
#define SPOSTAMENTO_RANA 1
#define SPOSTAMENTO_COCCODRILLO 1
#define SPOSTAMENTO_PROIETTILE 1

// Dimensione campo da gioco
#define maxy 18
#define maxx 72

// Dimensione Sprite
#define RIGHE_SPRITE_RANA 1
#define COLONNE_SPRITE_RANA 2

#define RIGHE_SPRITE_COCCODRILLO 1
#define COLONNE_SPRITE_COCCODRILLO 4

#define DIMENSIONE_TANA 2

#define RIGHE_SPRITE_PROIETTILE 1
#define COLONNE_SPRITE_PROIETTILE 1

#define RIGHE_SPRITE_GRANATA 1
#define COLONNE_SPRITE_GRANATA 1

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

#define SFONDO_MARCIAPIEDE 13
#define SFONDO_ACQUA 14
#define SFONDO_ERBA 15
#define SFONDO_TANE 16
#define COLORE_TANE 17

#define COLORE_GRANATA_ARGINE 18
#define COLORE_GRANATA_ACQUA 19

#define COLORE_PROIETTILE_COCCODRILLO 20

#define INVALID_PID -500 // Valore di default per il PID

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
    GRANATA,
    GRANATA_SINISTRA_RANA,
    GRANATA_DESTRA_RANA,
    PROIETTILE_COCCODRILLO
} tipoOggetto;

// struttura per rappresentare i vari elementi non statici del gioco
typedef struct elementoGioco
{
    tipoOggetto tipo;
    int x;
    int y;
    int pid_oggetto;
    int velocita;
    DirezioneFlusso direzione;
    pid_t proiettile;
    // statusOggetto status;
} elementoGioco;

typedef struct posizione
{
    int x;
    int y;
    int pid;
    pid_t proiettile;
    DirezioneFlusso direzione;
} posizione;

typedef struct posizioneTane
{
    int x;
    int y;
} posizioneTane;

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

// Schermo ncurses
extern WINDOW *gioco;
extern posizioneTane posTane[NUM_TANE];

void inizializzazioneSchermo();
void graficaGioco(bool tana_status[], int punteggio, int vite);
void avviaGioco(int vite, bool tana_status[], int punteggio);

void stampaSprite(elementoGioco elemento);
void cancellaSprite(elementoGioco elemento);
void cancellaProiettile(elementoGioco elemento);

void rana(int pipeout, int pipein, corrente flussi[]);
void coccodrillo(int pipeout, int riga, int id_coccodrillo, corrente flusso);
void proiettile(int pipeout, int y, int x, int velocita, DirezioneFlusso direzione, char tipo);

void controlloGioco(int pipein, int pipeRana, int vite, bool tana_status[], int tempoRimanente);
void terminaGioco();
void chiudiProcessi(pid_t pid);
void chiusuraFineManche(posizione pos_c[], posizione pos_granate[], int pipeRana, pid_t pid_rana, int pipein);
void inizializzazionePipe(int filedes[]);

void gestioneFlussi(corrente *flussi, int *coccodrilli_flusso);

void handler(int sig);
void handler_rana(int sig);

bool schermataFineGioco(bool esitoPartita, int score);
bool verificaTanaStatus(bool tana_status[]);

void cancellaProiettile(elementoGioco elemento);
