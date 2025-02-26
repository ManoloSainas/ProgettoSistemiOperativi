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

// Librerie per le usleep
#include <unistd.h>

// Macro

#define DIM_BUFFER 250 // Dimensione del buffer circolare con l'ausilio dei threads

// Numero dati
#define NUM_VITE_RANA 3
#define NUM_TANE 5
#define NUM_COCCODRILLI_FLUSSO 4
#define NUM_FLUSSI_FIUME 8

// quantità oggetti su schermo
#define MAXCOCCODRILLI 32
#define MAXGRANATE 12

// velocità flusso fiume
#define VELOCITA_FLUSSO 20000

// Tempo di gioco
#define TEMPO_TOTALE 90 // Tempo totale di gioco (per round)

// Gestione proiettili
#define SPEED_PROIETTILI 150000

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
extern int indexAggiunta, indexRimozione;

// Semafori e mutex per il coordinamento dei thread
/*pthread_mutex_t mutex;
sem_t bufferPieno, bufferVuoto;

elementoGioco buffer[DIM_BUFFER]; // Buffer
elementoGioco oggettoPreso;       // oggetto che viene preso e rimosso dal buffer*/

// direzione flusso fiume
typedef enum
{
    SINISTRA,
    DESTRA,
    NESSUNA
} DirezioneFlusso;

// variabile da dare ai thread
typedef struct info_elemento
{
    int x;
    int y;
    int speed;
    char tipo;
    DirezioneFlusso direzione;

} info_elemento;

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
    pthread_t proiettile; // usato dai proiettili per identificare il coccodrillo che li ha generati e viceversa il coccodrillo per identificare il proprio proiettile
} elementoGioco;

// struttura per rappresentare la posizione di un oggetto
typedef struct posizione
{
    int x;
    int y;
    int thread_id;
    pthread_t proiettile; // usato dai proiettili per identificare il coccodrillo che li ha generati e viceversa il coccodrillo per identificare il proprio proiettile
    DirezioneFlusso direzione;
    bool *controllo;
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

// Schermo ncurses
extern int minx;
extern int miny;
extern WINDOW *gioco;

// variabile per la lettura dei flussi
extern corrente flussi[NUM_FLUSSI_FIUME + 3];

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
void *rana(void *arg);
void *coccodrillo(void *info);
void *proiettile(void *info);
void handler_coccodrillo(int sig); // gestione segnali coccodrillo
void handler_rana(int sig);        // gestione segnali rana

void inizializzazionePipe(int filedes[]);
void chiudiProcessi(pid_t pid);

// gestione gioco
void gestioneFlussi(corrente *flussi, int *coccodrilli_flusso);
void avviaGioco(bool tana_status[], int punteggio, int vita);
int controlloGioco(int vita, bool tana_status[], int tempoRimanente, int punteggio);
void chiusuraFineManche(posizione pos_c[], posizione pos_proiettili[], posizione pos_granate[], pthread_t id_rana);
void terminaGioco();
bool verificaTanaStatus(bool tana_status[]); // verifica se tutte le tane sono state raggiunte

// Indici per l'utilizzo del buffer
extern int indexAggiunta, indexRimozione;

// Semafori e mutex per il coordinamento dei thread
extern sem_t sem_liberi, sem_occupati;

extern elementoGioco lista_elementi[DIM_BUFFER]; // Buffer
extern bool controllo;                           // variabile di controllo per la terminazione

extern int in;  // Indice per inserire nuovi elementi
extern int out; // Indice per rimuovere elementi

// Function declarations
void inizializza_meccanismi_sincronizzazione();
void dealloca_meccanismi_sincronizzazione();
void wait_produttore();
void signal_produttore();
void wait_consumatore();
void signal_consumatore();
int get_contatore_occupati();