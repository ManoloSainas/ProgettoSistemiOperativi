// #define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>


// Librerie per le chiamate a sistema
#include <sys/types.h>
#include <sys/wait.h>

#include <ncurses.h>  //Libreria ncurses
#include <sys/time.h> //Libreria per tenere traccia del tempo
#include <time.h>     //Libreria per la generazione casuale

// Librerie per la gestione dei thread
#include <pthread.h>
#include <semaphore.h>

// Macro

#define DIM_BUFFER 250 // Dimensione del buffer circolare con l'ausilio dei threads

// Numero dati
#define NUM_VITE_RANA 3
#define NUM_TANE 5
#define NUM_COCCODRILLI_FLUSSO 4
#define NUM_FLUSSI_FIUME 8

// quantità oggetti su schermo
#define MAXCOCCODRILLI 40
#define MAXGRANATE 12

// velocità flusso fiume
#define VELOCITA_FLUSSO 20000

// Tempo di gioco
#define TEMPO_TOTALE 90; // Tempo totale di gioco (per round)

// Gestione proiettili
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

#define RIGHE_SPRITE_PROIETTILE 1
#define COLONNE_SPRITE_PROIETTILE 1

#define RIGHE_SPRITE_GRANATA 1
#define COLONNE_SPRITE_GRANATA 1

#define DIMENSIONE_TANA 2

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

// Colori rana
#define COLORE_RANA_ARGINE 9
#define COLORE_RANA_COCCODRILLO 10
#define COLORE_RANA_TANA 11

// Colori coccodrillo
#define COLORE_COCCODRILLO 12

// Colori sfondo
#define SFONDO_MARCIAPIEDE 13
#define SFONDO_ACQUA 14
#define SFONDO_ERBA 15
#define SFONDO_TANE 16
#define COLORE_TANE 17

// Colori granata
#define COLORE_GRANATA_ARGINE 18
#define COLORE_GRANATA_ACQUA 19

// Colori proiettile coccodrillo
#define COLORE_PROIETTILE_COCCODRILLO 20

#define INVALID_THREAD -500 // Valore di default per il PID

// Indici per l'utilizzo del buffer
int indexAggiunta, indexRimozione;

// Semafori e mutex per il coordinamento dei thread
pthread_mutex_t mutex;
sem_t bufferPieno, bufferVuoto;

elementoGioco buffer[DIM_BUFFER]; // Buffer
elementoGioco oggettoPreso;       // oggetto che viene preso e rimosso dal buffer

//variabile da dare ai thread
typedef struct info_elemento{
int x;
int y;
int speed;
char tipo;
DirezioneFlusso direzione;

}info_elemento;

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
    bool *controllo;
    pthread_t thread_oggetto;
    int velocita;
    DirezioneFlusso direzione;
    pid_t proiettile; // usato dai proiettili per identificare il coccodrillo che li ha generati e viceversa il coccodrillo per identificare il proprio proiettile
} elementoGioco;


// struttura per rappresentare la posizione di un oggetto
typedef struct posizione
{
    int x;
    int y;
    int thread_id;
    pid_t proiettile; // usato dai proiettili per identificare il coccodrillo che li ha generati e viceversa il coccodrillo per identificare il proprio proiettile
    DirezioneFlusso direzione;
} posizione;



// struttura per rappresentare la posizione delle tane
typedef struct posizioneTane
{
    int x;
    int y;
} posizioneTane;

// struttura per rappresentare la corrente del fiume
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

//variabile per la lettura dei flussi
extern corrente flussi[NUM_FLUSSI_FIUME+3];


// Variabile per la posizione delle tane
extern posizioneTane posTane[NUM_TANE];

/*
funzioni grafiche
*/
void inizializzazioneSchermo();
void graficaGioco(bool tana_status[], int punteggio, int vite);
bool schermataFineGioco(bool esitoPartita, int score);
void stampaSprite(elementoGioco elemento);
void cancellaSprite(elementoGioco elemento);
void cancellaProiettile(elementoGioco elemento);

// Funzioni per la gestione dei processi
void *rana(corrente flussi[]);
void coccodrillo(int pipeout, int riga, int id_coccodrillo, corrente flusso);
void proiettile(int pipeout, int y, int x, int velocita, DirezioneFlusso direzione, char tipo);
void handler_coccodrillo(int sig); // gestione segnali coccodrillo
void handler_rana(int sig);        // gestione segnali rana

void inizializzazionePipe(int filedes[]);
void chiudiProcessi(pid_t pid);

// gestione gioco
void gestioneFlussi(corrente *flussi, int *coccodrilli_flusso);
void avviaGioco(bool tana_status[], int punteggio, int vita);
int controlloGioco( int vita, bool tana_status[], int tempoRimanente, int punteggio);
void chiusuraFineManche(posizione pos_c[], posizione pos_granate[], int pipeRana, pid_t pid_rana, int pipein);
void terminaGioco();

bool verificaTanaStatus(bool tana_status[]); // verifica se tutte le tane sono state raggiunte


//funzioni e variabili globali per thread e semafori 
sem_t sem_liberi, sem_occupati;
elementoGioco lista_elementi[DIM_BUFFER];
bool controllo=true; //variabile di controllo per la terminazione


int in = 0;    // Indice per inserire nuovi elementi
int out = 0;   // Indice per rimuovere elementi

 void inizializza_meccanismi_sincronizzazione() {
 sem_init(&sem_occupati, 0, 0); // All'inizio 0 elementi sono presenti nel buffer
 sem_init(&sem_liberi, 0, DIM_BUFFER); // All'inizio tutti i posti del buffer sono liberi
 }
 void dealloca_meccanismi_sincronizzazione() {
 sem_destroy(&sem_occupati);
 sem_destroy(&sem_liberi);
 }

 void wait_produttore() { sem_wait(&sem_liberi); }
 void signal_produttore() { sem_post(&sem_occupati); }

 void wait_consumatore() { sem_wait(&sem_occupati); }
 void signal_consumatore() { sem_post(&sem_liberi); }

 int get_contatore_occupati() {
 int contatore_occupati;
 sem_getvalue(&sem_occupati, &contatore_occupati);
 return contatore_occupati;
 }