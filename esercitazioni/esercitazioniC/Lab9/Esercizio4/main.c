#include <stdio.h>

//  Acquisisce due interi da tastiera, ed un terzo intero da utilizzare come opzione
// ■ Se l’intero indicato come opzione è 0, stampa il maggiore tra gli altri due numeri
// ■ Se l’intero indicato come opzione è diverso da 0, stampa invece il minore
// ○ Definisce e utilizza una funzione dedicata max(n,m) per il calcolo del massimo
// ○ Definisce e utilizza una funzione dedicata min(n,m) per il calcolo del minimo

int max(int n, int m) {
    if (n > m) {
        return n;
    } else {
        return m;
    }
}

int min(int n, int m) {
    if (n < m) {
        return n;
    } else {
        return m;
    }
}

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

