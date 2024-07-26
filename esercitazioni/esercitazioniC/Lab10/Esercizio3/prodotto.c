#include <stdio.h>
#include "prodotto.h"

int prodotto(int dim, int numbers[dim]) {
    int i, prodOdd = 1, countOdd = 0;

    for (i = 0; i < dim; i++) {
        if (numbers[i] % 2 != 0) {
            prodOdd = prodOdd * numbers[i]; countOdd ++; }
    }

    printf("Ci sono %d numeri dispari\n", countOdd);
    
    return prodOdd;
}