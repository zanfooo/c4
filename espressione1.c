#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int sommaF1, sommaF2;
    int status;

    int F1 = fork();  
    if (F1 == 0) {  // Processo figlio F1
        printf("F1: mio PID = %d, mio padre P ha PID = %d\n", getpid(), getppid());
        exit(1 + 2);  // Restituisce la somma
    }

    int F2 = fork();
    if (F2 == 0) {  // Processo figlio F2
        printf("F2: mio PID = %d, mio padre P ha PID = %d\n", getpid(), getppid());
        exit(3 + 4);  // Restituisce la somma
    }

    // Il padre stampa i PID dei figli
    printf("P: mio PID = %d, mio figlio F1 ha PID = %d\n", getpid(), F1);
    printf("P: mio PID = %d, mio figlio F2 ha PID = %d\n", getpid(), F2);

    // Il padre aspetta i figli e raccoglie i risultati
    waitpid(F1, &status, 0);
    if (WIFEXITED(status)) {
        sommaF1 = WEXITSTATUS(status);
    }

    waitpid(F2, &status, 0);
    if (WIFEXITED(status)) {
        sommaF2 = WEXITSTATUS(status);
    }

    int sommaFinale = sommaF1 + sommaF2;
    printf("P: la somma totale è %d\n", sommaFinale);

    return 0;
}
