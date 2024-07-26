#include <stdio.h>
#include "max.h"
#include "min.h"


int main() {
    int n, m, op;
    printf("Inserisci due numeri interi: ");
    scanf("%d %d", &n, &m);
    printf("Inserisci un intero opzione: ");
    scanf("%d", &op);
    if (op == 0) {
        printf("Il maggiore tra %d e %d e' %d\n", n, m, max(n, m));
    } else {
        printf("Il minore tra %d e %d e' %d\n", n, m, min(n, m));
    }
    return 0;
}
