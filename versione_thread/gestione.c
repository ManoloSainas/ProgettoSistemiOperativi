#include "frogger.h"

// Definizione delle variabili dichiarate come extern
int indexAggiunta, indexRimozione;
sem_t sem_liberi, sem_occupati;
elementoGioco lista_elementi[DIM_BUFFER];
bool controllo = true; // variabile di controllo per la terminazione
int in = 0;            // Indice per inserire nuovi elementi
int out = 0;           // Indice per rimuovere elementi

// Function definitions
void inizializza_meccanismi_sincronizzazione()
{
    sem_init(&sem_occupati, 0, 0);        // All'inizio 0 elementi sono presenti nel buffer
    sem_init(&sem_liberi, 0, DIM_BUFFER); // All'inizio tutti i posti del buffer sono liberi
}

void dealloca_meccanismi_sincronizzazione()
{
    sem_destroy(&sem_occupati);
    sem_destroy(&sem_liberi);
}

void wait_produttore() { sem_wait(&sem_liberi); }
void signal_produttore() { sem_post(&sem_occupati); }

void wait_consumatore() { sem_wait(&sem_occupati); }
void signal_consumatore() { sem_post(&sem_liberi); }

int get_contatore_occupati()
{
    int contatore_occupati;
    sem_getvalue(&sem_occupati, &contatore_occupati);
    return contatore_occupati;
}
