// import, define, costanti, etc.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Librerie per la gestione dei processi e delle pipes
#include <signal.h>
#include <unistd.h>

// Librerie per le chiamate a sistema
#include <sys/types.h>
#include <sys/wait.h>

#include <ncurses.h>  //Libreria ncurses
#include <sys/time.h> //Libreria per tenere traccia del tempo
#include <time.h>     //Libreria per la generazione casuale

#define SU 65       /* Cursore sopra */
#define GIU 66      /* Cursore sotto */
#define SINISTRA 68 /* Cursore sinistra */
#define DESTRA 67   /* Cursore destra */
#define SPAZIO ' '  // Tasto Spazio

#define MAXX 80 /* Dimensione dello schermo di output (colonne) */
#define MAXY 20 /* Dimensione dello schermo di output (righe)   */

#define NUMERO_VITE 3  // Numero di vite iniziali
#define TEMPO_GIOCO 60 // Tempo di gioco in secondi
#define PUNTEGGIO 0    // Punteggio iniziale

#define ALTEZZA_INFORMAZIONI 2 // Linea in cui vengono visualizzate le varie informazioni
#define LARGEZZA_INFORMAZIONI 80 // Larghezza della barra delle informazioni

// Rana
#define RANA "><"             // Carattere rappresentante la rana
#define LARGHEZZA_RANA 2      // Numero spazi occupato dalla rana in larghezza
#define ALTEZZA_RANA 1        // Numero di spazi occupati dalla rana in altezza
#define SALTO_RANA_SU 1       // Numero di spazi che la rana salta in alto
#define SALTO_RANA_GIU 1      // Numero di spazi che la rana salta in basso
#define SALTO_RANA_SINISTRA 2 // Numero di spazi che la rana salta a sinistra
#define SALTO_RANA_DESTRA 2   // Numero di spazi che la rana salta a destra

// Proiettile sparato dalla rana
#define PROIETTILE_RANA                                                        \
  "^" // Carattere rappresentante il proiettile sparato dalla rana
#define LARGHEZZA_PROIETTILE                                                   \
  1 // Numero spazi occupato dal proiettile in larghezza
#define ALTEZZA_PROIETTILE 1  // Numero spazi occupati dal proiettile in altezza
#define VELOCITA_PROIETTILE 1 // Velocità di spostamento del proiettile

// Marciapiede dell'argine
#define LARGHEZZA_MARCIPIEDE 80 // Larghezza del marciapiede
#define ALTEZZA_MARCIPIEDE 2    // Altezza del marciapiede

// Fiume
#define LARGHEZZA_FIUME 80 // Larghezza del fiume
#define ALTEZZA_FIUME                                                          \
  8 // Altezza del fiume, formato da 8 flussi d'acqua di altezza 1

// Coccodrillo
#define COCCODRILLO "====" // Carattere rappresentante il coccodrillo
#define LARGHEZZA_COCCODRILLO                                                  \
  4 // Numero spazi occupato dal coccodrillo in larghezza
#define ALTEZZA_COCCODRILLO                                                    \
  1 // Numero spazi occupati dal coccodrillo in altezza
#define VELOCITA_COCCODRILLO 1 // Velocità di spostamento del coccodrillo
#define COCCODRILLO_SPARISCE                                                   \
  "-==-" // Carattere rappresentante il coccodrillo mentre sta sparendo
#define COCCODRILLO_SPARITO                                                    \
  "    " // Carattere rappresentante il coccodrillo sparito

// velocità casuale dei coccodrilli
#define VELOCITA_MIN_COCCODRILLO                                               \
  1 // Velocità minima di spostamento del coccodrillo
#define VELOCITA_MAX_COCCODRILLO                                               \
  3 // Velocità massima di spostamento del coccodrillo

// la generazione dei coccodrilli avviene utilizzando degli intervalli di tempo
// casuale
#define INTERVALLO_MIN_COCCODRILLO                                             \
  1 // Intervallo minimo di tempo per la generazione
#define INTERVALLO_MAX_COCCODRILLO                                             \
  5 // Intervallo massimo di tempo per la generazione

// Sponde d'erba
#define LARGHEZZA_ERBA 80 // Larghezza del prato
#define ALTEZZA_ERBA 2    // Altezza del prato

// Piante malvagie
#define PIANTA_MALVAGIA "@@" // Carattere rappresentante la pianta malvagia
#define LARGHEZZA_PIANTA                                                       \
  2 // Numero spazi occupato dalla pianta malvagia in larghezza
#define ALTEZZA_PIANTA                                                         \
  1 // Numero spazi occupati dalla pianta malvagia in altezza
#define NUMERO_PIANTE 3 // Numero di piante malvagie

// Proiettile sparato dalla pianta
#define PROIETTILE_PIANTA                                                      \
  "|" // Carattere rappresentante il proiettile sparato dalla pianta
#define LARGHEZZA_PROIETTILE                                                   \
  1 // Numero spazi occupato dal proiettile in larghezza
#define ALTEZZA_PROIETTILE 1  // Numero spazi occupati dal proiettile in altezza
#define VELOCITA_PROIETTILE 1 // Velocità di spostamento del proiettile

// Tane
#define LARGHEZZA_TANA 2 // Numero spazi occupato dalla tana in larghezza
#define ALTEZZA_TANA 1   // Numero spazi occupati dalla tana in altezza
#define NUMERO_TANE 5    // Numero di tane
#define TANA_CHIUSA "X"  // Carattere rappresentante la tana chiusa

/* Struttura adoperata per veicolare le coordinate */
struct posizione {
  char c; /* Identificatore dell'entità che invia i dati */
  int x;  /* Coordinata X */
  int y;  /* Coordinata Y */
};

// Funzioni
void avvia_gioco();
void rana();
void gioco();
void inizializza_colori();
void disegna_rana(int x, int y);
void disegna_barra_inferiore();
void disegna_marciapiede();
