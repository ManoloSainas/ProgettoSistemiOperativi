
// ● Codificare in C un programma che da un processo padre crei un processo figlio
// ● Il processo padre deve acquisire dall’utente un array di 6 interi
// ● Il processo padre deve calcolare la somma dei numeri pari presenti in tale array
// ● Il processo figlio deve calcolare il prodotto di tutti i numeri dispari in tale array
// ● Il codice per la somma e il prodotto deve essere inserito in funzioni dedicate riportate 
// nei due rispettivi file somma.c e prodotto.c
// ● Compilare ed eseguire il programma attraverso l’uso di un makefile


#include <stdio.h>
#include <unistd.h>
#include "main.h"
#include "somma.h"
#include "prodotto.h"
#include <sys/types.h>

int main() {
    int numbers[DIM_ARRAY], i, prodOdd = 1, sumEven = 0;
    pid_t pid;

    printf("Inserisci sei numeri interi per popolare l'array!\n");
    for (i=0;i<DIM_ARRAY;i++) {
        printf("%d) ", i+1); 
        scanf("%d", &(numbers[i])); 
    }

    printf("Hai inserito i seguenti numeri:\n");
    for (i=0;i<DIM_ARRAY;i++)
        printf("%d\n",numbers[i]);

    pid = fork();

    if (pid == 0) { // Siamo nel processo figlio
        printf("Sono il processo figlio, PID %d\n", pid);
        prodOdd = prodotto(DIM_ARRAY, numbers);
        printf("Il prodotto dei numeri dispari e': %d\n", prodOdd);
    } else if (pid > 0) { // Siamo nel processo padre
        printf("Sono il processo padre, PID %d\n", pid);
        sumEven = somma(DIM_ARRAY, numbers);
        printf("La somma dei numeri pari e': %d\n", sumEven);
    } else
    printf("Errore nella fork!");

    return 0;
}