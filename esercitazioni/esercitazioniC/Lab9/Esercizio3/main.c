// acquisizione di due numeri in input e confronto richiamando la funzione max
#include <stdio.h>
#include "max.h"

int main() {
    int n, m;
    printf("Inserisci due numeri interi: ");
    scanf("%d %d", &n, &m);
    printf("Il maggiore tra %d e %d e' %d\n", n, m, max(n, m));
    return 0;
}

