
// ● Vediamo un esempio di un processo padre che comunica con il figlio:
// ○ Il figlio scrive, inviando tramite una pipe 3 messaggi al processo padre
// ○ Il padre legge, ricevendo i rispettivi messaggi dal figlio e stampandoli a video
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MSGSIZE 15

char msg1[MSGSIZE] = "Hello world #1";
char msg2[MSGSIZE] = "Hello world #2";
char msg3[MSGSIZE] = "Hello world #3";

int main() {
    char msg_temp[MSGSIZE]; int pipe_fds[2], i; pid_t pid;
    // Crea la pipe, fornendo i file descriptors in "pipe_fds"
    if(pipe(pipe_fds) == -1) {
        // Scrive su stderr, ma include la descrizione dell'ultimo errore che si è presentato
        perror("Pipe call");
        exit(1);
    }
    // Chiede la creazione di un processo figlio
    pid=fork();
    if(pid < 0) {perror("fork call"); _exit(2);}  // Check fork a buon fine
    // Controllo: che processo sono?
    else if (pid == 0) {  // Processo figlio
        close(pipe_fds[0]);  // Chiusura lettura, il figlio scrive
        // Scrivi i messaggi sul buffer della pipe
        write(pipe_fds[1], msg1, MSGSIZE);
        write(pipe_fds[1], msg2, MSGSIZE);
        write(pipe_fds[1], msg3, MSGSIZE);
    } else {  // Processo padre
        close(pipe_fds[1]);  // Chiusura scrittura, il padre legge
        // Leggi i messaggi dal buffer della pipe
        for(i = 0; i < 3; i++) {
            // Scrivi su una memoria intermedia e stampa
            read(pipe_fds[0], msg_temp, MSGSIZE);
            printf("%s\n", msg_temp);
        }
        wait(NULL);  // Ricongiungi con il figlio
    }
    _exit(0);
}