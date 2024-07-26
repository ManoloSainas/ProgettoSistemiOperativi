// codificare in C un programma che da un processo padre crea un processo figlio, il processo figlio deve stampare Hello e identificarsi
// scrivendo il suo PID. Il processo padre deve stampare World e identificarsi scrivendo il suo PID. Assicurarsi che il processo figlio stampi sempre prima
// del processo padre.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("Processo iniziale con PID:%d\n", (int)getpid());
    pid_t pid = fork();
    if (pid < 0) { // fork fallita, chiusura del programma
        fprintf(stderr, "fork fallita \n");
        exit(1);
    } else if (pid == 0) { // Processo figlio
       printf("Hello, sono il processo figlio con PID:%d\n", (int)getpid());
    } else { // Processo padre
      wait((int *)0); // Aspetta la conclusione del processo figlio
      printf("World, sono il processo padre di PID:%d, con mio PID:%d\n", pid, (int)getpid());
    }
    return 0;
}


