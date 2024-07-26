#include <stdio.h>
#include "somma.h"

int somma(int dim, int numbers[dim]) {
    int i, sumEven = 0, countEven = 0;

    for (i = 0; i < dim; i++) {
        if (numbers[i] % 2 == 0) {
            sumEven = sumEven + numbers[i]; countEven++; }
    }

    printf("Ci sono %d numeri pari\n", countEven);
    
    return sumEven;
}