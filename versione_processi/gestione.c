#include "frogger.h"

void chiudiProcessi(elementoGioco *elemento){
    if (elemento == NULL){
        fprintf(stderr, "Errore: puntatore nullo passato a chiudiProcessi\n");
        return;
    }
switch (elemento->tipo)
{
case RANA:
   if (kill(elemento->pid_oggetto, SIGKILL) == -1)
        {
            perror("Errore nel terminare il processo della rana");
        }
    break;
case COCCODRILLO:
if (kill(elemento->pid_oggetto, SIGKILL) == -1)
        {
            perror("Errore nel terminare il processo coccodrillo");
        }
    break;
default:
    break;
}
    
}