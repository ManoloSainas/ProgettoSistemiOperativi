#include <stdio.h>
#include <stdlib.h>

// Librerie per la gestione dei processi e delle pipes
#include <signal.h>
#include <unistd.h>

// Librerie per le chiamate a sistema
#include <sys/types.h>
#include <sys/wait.h>

#include <ncurses.h>  // Libreria ncurses
#include <sys/time.h> // Libreria per tenere traccia del tempo
#include <time.h>     // Libreria per la generazione casuale di numeri

// Definizione costanti di gioco
#define NUM_VITE_RANA 3
#define NUM_PROIETTILI_RANA 10 // Numero proiettili visibili sullo schermo contemporaneamente
#define NUM_TANE 5
#define NUM_PIANTE 3
#define NUM_MIN_COCCODRILLI_FLUSSO 1 // Numero minimo di coccodrilli per flusso
#define NUM_MAX_COCCODRILLI_FLUSSO 8 // Numero massimo di coccodrilli per flusso
#define NUM_FLUSSI_FIUME 8
// #define NUM_TOT_COCCODRILLI 32 // Numero totale di coccodrilli presenti sullo schermo
#define NUM_TOT_COCCODRILLI 1 // Numero totale di coccodrilli presenti sullo schermo

#define TEMPO_TOTALE 60; // Tempo totale di gioco (per round)

#define KEY_SPACE 32 // Valore relativo al carattere spazio (per sparare proiettili con la rana)

// Colori utilizzati per lo sfondo e per gli oggetti
#define COLORE_STANDARD 1
#define COLORE_CANCELLAZIONE 2
#define COLORE_ROSSO 3
#define COLORE_CIANO 4
#define COLORE_BLU 5
#define COLORE_VERDE 6
#define COLORE_GIALLO 7
#define COLORE_MAGENTA 8

#define SFONDO_ARGINE 9
#define SFONDO_ACQUA 10
#define SFONDO_TANE 11
#define COLORE_TANE 12
#define COLORE_PIANTA 13
#define COLORE_COCCODRILLO_BUONO 14
#define COLORE_COCCODRILLO_CATTIVO 15
#define COLORE_RANA 16
#define COLORE_COCCODRILLO_IMMERSIONE 17

// Costanti per la gestione dello spostamento degli oggetti
#define SPOSTAMENTO_RANA 1

#define SPOSTAMENTO_X_PROIETTILI_RANA 0
#define SPOSTAMENTO_Y_PROIETTILI_RANA 1

#define SPOSTAMENTO_X_COCCODRILLO 1
#define SPOSTAMENTO_Y_COCCODRILLO 0

#define SPOSTAMENTO_X_PROIETTILI_PIANTA 0
#define SPOSTAMENTO_Y_PROIETTILI_PIANTA 1

// Dimensione degli sprite
#define RIGHE_SPRITE_RANA 1
#define COLONNE_SPRITE_RANA 2

#define RIGHE_SPRITE_PROIETTILE_RANA 1
#define COLONNE_SPRITE_PROIETTILE_RANA 1

#define RIGHE_SPRITE_COCCODRILLO 1
#define COLONNE_SPRITE_COCCODRILLO 4

#define RIGHE_SPRITE_PIANTA 1
#define COLONNE_SPRITE_PIANTA 2

#define RIGHE_SPRITE_PROIETTILE_PIANTA 1
#define COLONNE_SPRITE_PROIETTILE_PIANTA 1

// Gestione proiettili, tempo di ricarica e velocità
#define RICARICA_PROIETTILI 9000
#define SPEED_PROIETTILI 60000

// Gestione fiume, velocità minima e massima
#define VELOCITA_MAX_FLUSSO 500000
#define VELOCITA_MIN_FLUSSO 300000

// Primo e ultimo flusso del fiume
#define PRIMO_FLUSSO maxy - 1
#define ULTIMO_FLUSSO maxy - 9

// Distanza minima tra piante
#define DISTANZA_PIANTE 2

// Definizione degli sprite
extern char spriteRana[COLONNE_SPRITE_RANA + 1];
extern char spriteProiettileRana[COLONNE_SPRITE_PROIETTILE_RANA + 1];
extern char spriteCoccodrillo[COLONNE_SPRITE_COCCODRILLO + 1];
extern char spriteCoccodrilloImmersione[COLONNE_SPRITE_COCCODRILLO + 1];
extern char spritePianta[COLONNE_SPRITE_PIANTA + 1];
extern char spriteProiettilePianta[COLONNE_SPRITE_PROIETTILE_PIANTA + 1];

// Direzione flusso del fiume
typedef enum
{
    SINISTRA,
    DESTRA,
    NESSUNA_DIREZIONE
} DirezioneFiume;

// Tipologia del coccodrillo
typedef enum
{
    BUONO,
    CATTIVO,
    NESSUNA_TIPOLOGIA
} TipologiaCoccodrillo;

// Tipologia di oggetto presente su schermo
typedef enum tipoOggetto
{
    RANA,
    COCCODRILLO_BUONO,
    COCCODRILLO_CATTIVO,
    COCCODRILLO_IMMERSIONE,
    PIANTA,
    PROIETTILE_RANA,
    PROIETTILE_PIANTA
} tipoOggetto;

// Tipologia per distinguere le modalità di utilizzo della pipe
typedef enum tipoDescrittore
{
    LETTURA = 0,
    SCRITTURA
} tipoDescrittore;

// Distingue un'oggetto in funzione tra un oggetto non in funzione (stato generale dell'oggetto)
typedef enum statusOggetto
{
    NON_ATTIVO,
    ATTIVO,
    SOSPESO,
    TERMINATO
} statusOggetto;

// Informazioni sul singolo oggetto (le informazioni sono comuni a tutti gli oggetti presenti sullo schermo)
typedef struct oggetto
{
    tipoOggetto tipo;                          // Tipologia
    int x;                                     // Posizione x
    int y;                                     // Posizione y
    int index;                                 // Indice in caso di oggetti multipli uguali
    int pid_oggetto;                           // Pid del processo che gestisce l'oggetto
    statusOggetto status;                      // Informazione sullo stato dell'oggetto
    bool proiettili;                           // Contiene i proiettili dell'oggetto (se è un oggetto che spara)
    DirezioneFiume direzioneFiume;             // Direzione del coccodrillo
    TipologiaCoccodrillo tipologiaCoccodrillo; // Tipo di coccodrillo (buono o cattivo)
    int velocita;                              // Velocità di spostamento dell'oggetto
    short indice_flusso;                       // indice del flusso del fiume (se `e un coccodrillo)
} oggetto;

// Struttura per contenere lo stato del gioco con tutte le varie informazioni (oggetti nello schermo e pipe per comunicare con i coccodrilli)
typedef struct stato_gioco
{
    oggetto
        rana,
        proiettiliRana[NUM_PROIETTILI_RANA],
        piante[NUM_PIANTE],
        proiettiliPianta[NUM_PIANTE],
        coccodrilli[NUM_TOT_COCCODRILLI];

    int pipe_coccodrilli[NUM_TOT_COCCODRILLI][2]; /* pipe dal padre verso i figli */
} StatoGioco;

// Informazioni sul fiume e sui flussi
typedef struct
{
    DirezioneFiume direzioneFlussi[NUM_FLUSSI_FIUME]; // Direzione dei flussi del fiume
    int velocitaFlussi[NUM_FLUSSI_FIUME];             // Velocità dei flussi del fiume
    // int numeroCoccodrilliFlussi[NUM_FLUSSI_FIUME];
    struct
    {
        short origine;                                    // la y dove inizia
        short id_coccodrilli[NUM_MAX_COCCODRILLI_FLUSSO]; // Identificativo dei coccodrilli presenti in un flusso
        short presenti;                                   // Numero di coccodrilli presenti in un flusso
    } info_flusso[NUM_FLUSSI_FIUME];                      // Informazioni sui coccodrilli presenti in un flusso
} InformazioniFiume;

// Indica se la tana è aperta o chiusa
typedef enum
{
    APERTA,
    CHIUSA
} StatoTana;

typedef struct posizione
{
    int x, y;
} Posizione;

// Struttura per gestire lo stato di un oggetto
typedef struct messaggio
{
    enum
    {
        SVEGLIA,
        SOSPENDI,
        CAMBIA,
        CONT
    } codice;

    Posizione posizione;
} Messaggio;

// Variabili globali

// Coordinate dell'area di gioco
extern int minx, miny;
extern int maxx, maxy;

extern int indexProiettileRana; // Contiene l'indice del proiettile rana

extern bool vittoria; // True se il giocatore ha vinto, False altrimenti

// int coccodrilliRimasti; // Contatore di nemici rimanenti

extern int numCoccodrilli; // Numero di coccodrilli presenti sullo schermo
extern int numPiante;      // Numero di piante presenti sullo schermo

extern int posizionePianteX[NUM_PIANTE]; // Posizioni delle piante

extern InformazioniFiume infoFiume; // Informazioni sui flussi del fiume

// Schermo ncurses
extern WINDOW *gioco;

// Definizione funzioni
void inizializzazioneSchermo();
void avviaGioco();
void terminaGioco();
void inizializzazionePipe(int filedes[]);

void rana(int pipeout);
void coccodrillo(int pipeout, int pipein, int indice, InformazioniFiume *infoFiume);
void pianta(int pipeout, int indice, int pos_x);
void proiettileRana(int pipeout, int pos_ranay, int pos_ranax);
void proiettilePianta(int pipeout, int pos_pianty, int pos_piantax, int index_pianta);

void controlloGioco(int pipein, StatoGioco *statoGioco);
void visualizzaTimer(int tempoRimanente);
void terminaGioco();

void resettaOggetto(oggetto *oggetto);
void initOggetto(oggetto *oggetto);
void chiudiProcessi(oggetto *proiettiliRana, oggetto *rana, oggetto *piante, oggetto *proiettilePianta, oggetto *coccodrilli);

void stampaSprite(oggetto sprite);
void cancellaSprite(oggetto sprite);
void graficaGioco();

DirezioneFiume getDirezioneFiume();
TipologiaCoccodrillo getTipologiaCoccodrillo();
int getVelocitaFlussoFiume();
void inizializzaFlussiFiume(InformazioniFiume *infoFiume);
Posizione genera_posizione_fiume(StatoGioco *stato, InformazioniFiume *infoFiume);