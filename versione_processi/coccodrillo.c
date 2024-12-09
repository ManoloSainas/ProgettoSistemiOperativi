#include "frogger.h"

void coccodrillo(int pipeout, int row, int indice, corrente flusso){

elementoGioco coccodrillo;

coccodrillo.tipo= COCCODRILLO;
coccodrillo.y= row;
coccodrillo.pid_oggetto= getpid();
coccodrillo.direzione= flusso.direzione;
coccodrillo.velocita= flusso.velocita;

bool status_flag=true;

if(coccodrillo.direzione==DESTRA){
    coccodrillo.x=minx;
}else{
    coccodrillo.x=maxx;
}

while(status_flag){


switch (coccodrillo.direzione)
{
case DESTRA:
    coccodrillo.x += 1;
    if(coccodrillo.x>maxx+COLONNE_SPRITE_COCCODRILLO){
        status_flag=false;
    }
    
    break;
case SINISTRA:
      coccodrillo.x -= 1;
    if(coccodrillo.x<minx-COLONNE_SPRITE_COCCODRILLO){
        status_flag=false;
    }
    
    break;   

default:
    break;
}
usleep(2000000 - coccodrillo.velocita*10000); //velocita del coccodrillo 2s- velocita flusso*10000

//scrittura del coccodrillo nella pipe
write(pipeout, &coccodrillo, sizeof(elementoGioco));

}
//terminazione processo se il coccodrillo esce dallo schermo
_exit(0);

}