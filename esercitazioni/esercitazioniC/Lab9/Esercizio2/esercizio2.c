#include <stdio.h>

// Prende due input da tastiera e stampa il maggiore
// utilizza una funzione dedicata max(n,m) per il calcolo del massimo

int max(int n, int m) {
    if (n > m) {
        return n;
    } else {
        return m;
    }
}

int main() {
    int n, m;
    printf("Inserisci due numeri interi: ");
    scanf("%d %d", &n, &m);
    printf("Il maggiore tra %d e %d e' %d\n", n, m, max(n, m));
    return 0;
}
