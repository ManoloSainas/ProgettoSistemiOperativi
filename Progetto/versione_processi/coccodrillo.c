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

void coccodrillo(int pipeout)
{
}