#include "frogger.h"

void chiudiProcessi(elementoGioco *rana){
    if (rana == NULL){
        fprintf(stderr, "Errore: puntatore nullo passato a chiudiProcessi\n");
        return;
    }

    if (kill(rana->pid_oggetto, SIGKILL) == -1)
        {
            perror("Errore nel terminare il processo della rana");
        }
}