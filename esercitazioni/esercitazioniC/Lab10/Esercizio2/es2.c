// ● Codificare in C un programma che da un processo padre crei due processi figlio
// ● Un processo figlio deve richiamare l’esecuzione del comando Bash pwd
// ● L’altro processo figlio deve richiamare l’esecuzione del comando Bash ls -l
// ● Il processo padre deve verificare che l’esecuzione dei due comandi da parte dei 
// processi figlio sia andata a buon fine
// ○ Stampando pwd OK per il primo processo figlio in caso affermativo
// ○ Stampando ls -l OK per il secondo processo figlio in caso affermativo

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;
    pid1 = fork();
    if(pid1 == 0) { // Primo figlio
        printf("Sono %d\n", pid1);
        execl("/bin/pwd", "pwd", (char *)0);
        printf("exec fallita\n");
    }
    else if (pid1 > 0) { // Padre
        wait((int *)0); // Attende che il primo figlio termini 
        fflush(stdin);
        printf("pwd OK\n");
        pid2 = fork();

        if (pid2 == 0) {
            // Secondo figlio
            execl("/bin/ls", "ls", "-l", (char*)0);
            printf("exec 2 failed\n");
        } else if ( pid2 > 0) {
            wait((int *)0); // Attende che il secondo figlio termini 
            fflush(stdin);
            printf("ls -l ok\n");
        } else
        printf("fork failed\n");
    } else
       printf("fork failed\n");
    return 0;
}
