#include "frogger.h"

// Definizione delle variabili dichiarate come extern
int indexAggiunta, indexRimozione;
sem_t sem_liberi, sem_occupati;
pthread_mutex_t mutex; // Aggiunta del mutex
elementoGioco lista_elementi[DIM_BUFFER];
bool controllo = true; // variabile di controllo per la terminazione
int in = 0;            // Indice per inserire nuovi elementi
int out = 0;           // Indice per rimuovere elementi

// Funzione per l'inizializzazione dei meccanismi di sincronizzazione
void inizializza_meccanismi_sincronizzazione()
{
    sem_init(&sem_occupati, 0, 0);        // All'inizio 0 elementi sono presenti nel buffer
    sem_init(&sem_liberi, 0, DIM_BUFFER); // All'inizio tutti i posti del buffer sono liberi
    pthread_mutex_init(&mutex, NULL);     // Inizializzazione del mutex
}

// Funzione per la deallocazione dei meccanismi di sincronizzazione
void dealloca_meccanismi_sincronizzazione()
{
    sem_destroy(&sem_occupati);
    sem_destroy(&sem_liberi);
    pthread_mutex_destroy(&mutex); // Deallocazione del mutex
}

// Funzioni per gestire i semafori
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

// Funzioni per gestire il mutex
void lock_mutex() { pthread_mutex_lock(&mutex); }
void unlock_mutex() { pthread_mutex_unlock(&mutex); }

// libera il buffer a fine gioco
void liberaBuffer()
{
    int i;

    for (i = 0; i < DIM_BUFFER; i++)
    {
        lista_elementi[i].tipo = '0';
        lista_elementi[i].x = 0;
        lista_elementi[i].y = 0;
        lista_elementi[i].thread_oggetto = INVALID_THREAD;
        lista_elementi[i].velocita = 0;
        lista_elementi[i].direzione = NESSUNA;
        lista_elementi[i].proiettile = INVALID_THREAD;
        lista_elementi[i].controllo = NULL;
    }
}