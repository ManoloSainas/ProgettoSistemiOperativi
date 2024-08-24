#include "main.h"

// 3.4. Requisiti del fiume
// ● Il fiume è composto da un numero di flussi(equivalenti a “corsie” orizzontali)
//     non inferiore a 8;
// ● I flussi hanno una larghezza(coordinata x dello schermo) pari all’area di gioco
//     e una altezza minima(coordinata y nello schermo) sufficiente a contenere la
//     rana;
// ● Ogni flusso viene percorso da coccodrilli a pelo d’acqua, i quali avranno un’altezza(coordinata y) pari all’altezza della rana, una larghezza minima(coordinata x) pari al doppio della larghezza della rana e una larghezza massima pari a 3 volte la larghezza della rana;
// ● In maniera casuale prestabilita all’avvio di ogni nuova manche, ogni flusso del fiume potrà essere percorso orizzontalmente da sinistra verso destra oppure da destra verso sinistra : non è ammesso decidere in modo statico la direzione di percorrenza di ciascun flusso, a priori;
// ● Quando l’estremo di un coccodrillo arriva al limite del campo di gioco, il coccodrillo sparisce gradualmente dall’area di gioco, e il suo processo viene terminato(o eventualmente sfruttato per generare un nuovo coccodrillo in una nuova corsia);
// ● I coccodrilli di uno stesso flusso procedono tutti alla stessa velocità e si
//     muovono tutti nella stessa direzione,
//     pertanto NON devono / possono
//                               collidere tra loro.La velocità di ogni flusso è casuale : non dev’essere
//                                                                                             determinata in modo statico,
//     e non è ammesso che a ogni manche uno stesso
//         flusso abbia la stessa velocità delle manche precedenti,
//     preimpostata;
// ● La generazione degli oggetti coccodrillo avviene utilizzando degli intervalli di
//     tempo casuale,
//     in modo che ogni volta che ricomincia la manche(ad esempio
//                                                         perché il giocatore ha perso una vita) la scena di gioco sia sempre originale;
// ● I coccodrilli possono essere di due tipi : “buoni” o “cattivi” :
// ○ I coccodrilli buoni sono innocui : si comportano come delle “zattere” in
//                                           acqua,
//                                       sulle quali la rana può saltare e muoversi lateralmente,
//                                       per
//                                           poter attraversare il fiume verso l’argine opposto e raggiungere la
//                                               propria tana;
// ○ I coccodrilli cattivi, invece, rappresentati con un colore e / o una fisionomia differente rispetto ai buoni, si comportano anch’essi di base come zattere(la rana può quindi saltarci sopra e muoversi su di essi) : tuttavia, in modo casuale, improvviso e pertanto impredicibile, si immergono in acqua facendo annegare la rana se presente sul
//                                                                                                                                                                                                                                                                                         groppone.
// ■ I coccodrilli cattivi possono fornire un feedback grafico che
//                                                                                                                                                                                                                                                                                         preannunci l’immersione imminente.
// ○ La scelta di generazione tra coccodrillo buono o cattivo deve essere
//                                                                                                                                                                                                                                                                                         casuale(eventualmente in funzione della difficoltà di gioco);
// ● Se la rana è sopra un coccodrillo nel mentre che questo esce dall’area di
//     gioco,
//     la rana cadrà in acqua;
// ● Se la rana cade(o salta) in acqua, perde una vita e la manche

// inizializza una tipologia casuale
TipologiaCoccodrillo getTipologiaCoccodrillo()
{
    return rand() % 2 == 0 ? BUONO : CATTIVO;
}

void coccodrillo(int pipeout, int row, int indice, InformazioniFiume *infoFiume)
{
    oggetto oggetto_coccodrillo;

    srand(time(NULL) + indice); // Unique seed for each crocodile

    // Inizializzazione coccodrillo
    oggetto_coccodrillo.tipo = (rand() % 2 == 0) ? COCCODRILLO_BUONO : COCCODRILLO_CATTIVO;
    oggetto_coccodrillo.x = minx + rand() % (maxx - COLONNE_SPRITE_COCCODRILLO - minx);
    oggetto_coccodrillo.y = row; // Set y coordinate based on row
    oggetto_coccodrillo.status = ATTIVO;
    oggetto_coccodrillo.pid_oggetto = getpid();
    oggetto_coccodrillo.index = indice;
    oggetto_coccodrillo.proiettili = false;
    oggetto_coccodrillo.direzioneFiume = infoFiume->direzioneFlussi[row - (maxy - 9)]; // Use direction for the specific row
    oggetto_coccodrillo.tipologiaCoccodrillo = (oggetto_coccodrillo.tipo == COCCODRILLO_BUONO) ? BUONO : CATTIVO;
    oggetto_coccodrillo.velocita = infoFiume->velocitaFlussi[row - (maxy - 9)]; // Use speed for the specific row

    // Random delay before generating the next crocodile
    usleep(2000000 + (rand() % 5000000)); // Random delay between 2 and 5 seconds

    // Scrivi l'oggetto coccodrillo nella pipe
    write(pipeout, &oggetto_coccodrillo, sizeof(oggetto));

    // Movimento del coccodrillo
    while (oggetto_coccodrillo.status == ATTIVO)
    {
        if (oggetto_coccodrillo.tipo == COCCODRILLO_CATTIVO)
        {
            // Randomly decide to submerge
            if (rand() % 100 < 5) // 5% chance to submerge
            {
                oggetto_coccodrillo.status = TERMINATO;
                write(pipeout, &oggetto_coccodrillo, sizeof(oggetto));
                _exit(0);
            }
        }

        if (oggetto_coccodrillo.direzioneFiume == SINISTRA)
        {
            oggetto_coccodrillo.x -= SPOSTAMENTO_X_COCCODRILLO;
            if (oggetto_coccodrillo.x < minx)
            {
                oggetto_coccodrillo.status = TERMINATO;
            }
        }
        else
        {
            oggetto_coccodrillo.x += SPOSTAMENTO_X_COCCODRILLO;
            if (oggetto_coccodrillo.x > maxx)
            {
                oggetto_coccodrillo.status = TERMINATO;
            }
        }

        usleep(oggetto_coccodrillo.velocita);

        // Scrivi l'oggetto coccodrillo nella pipe
        write(pipeout, &oggetto_coccodrillo, sizeof(oggetto));
    }

    // Termina il processo quando il coccodrillo esce dallo schermo
    _exit(0);
}
