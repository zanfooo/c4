#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// Funzione per generare un numero casuale in un intervallo
int random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main() {
    pid_t pid_F1, pid_F2, pid_F3, pid_F4;
    int n1, n2, n2_dim, n3_dim, status;

    srand(time(NULL));

    // Generazione di due numeri casuali distinti tra 1 e 20
    do {
        n1 = random_range(1, 20);
        n2 = random_range(1, 20);
    } while (n1 == n2);

    printf("Sono il padre, PID = %d\n", getpid());
    printf("Numeri generati: n1 = %d, n2 = %d\n", n1, n2);

    // Creazione di F2
    if ((pid_F2 = fork()) == 0) {
        printf("Sono F2, PID = %d, figlio di PID = %d\n", getpid(), getppid());
        sleep(n1);
        n2_dim = random_range(2, 6);
        exit(n2_dim); // Restituisce la dimensione del vettore
    }
    printf("Sono il padre, ho generato F2 con PID = %d\n", pid_F2);

    // Creazione di F3
    if ((pid_F3 = fork()) == 0) {
        printf("Sono F3, PID = %d, figlio di PID = %d\n", getpid(), getppid());
        sleep(n2);
        n3_dim = random_range(2, 10);
        exit(n3_dim); // Restituisce un valore, ma non viene utilizzato
    }
    printf("Sono il padre, ho generato F3 con PID = %d\n", pid_F3);

    // Attesa della terminazione di F2 e F3
    waitpid(pid_F2, &status, 0);
    n2_dim = WEXITSTATUS(status); // Recupera la dimensione del vettore
    waitpid(pid_F3, NULL, 0);

    // Creazione di F1
    if ((pid_F1 = fork()) == 0) {
        printf("Sono F1, PID = %d, figlio di PID = %d\n", getpid(), getppid());
        printf("Genererò un vettore di dimensione %d\n", n2_dim);

        int v[n2_dim], somma = 0;
        srand(time(NULL));

        printf("Vettore generato: ");
        for (int i = 0; i < n2_dim; i++) {
            v[i] = random_range(1, 10);
            printf("%d ", v[i]);
            somma += v[i];
        }
        printf("\nSomma = %d\n", somma);

        exit(somma % 256); // Restituisce la somma modulo 256 per limitarla a un byte
    }
    printf("Sono il padre, ho generato F1 con PID = %d\n", pid_F1);

    // Attende F1
    waitpid(pid_F1, &status, 0);
    int somma = WEXITSTATUS(status); // Recupera la somma dal figlio F1

    // Creazione di F4
    if ((pid_F4 = fork()) == 0) {
        printf("Sono F4, PID = %d, figlio di PID = %d\n", getpid(), getppid());
        int media = somma / n2_dim;
        printf("Sono F4, ho calcolato la media: %d\n", media);
        exit(media); // Restituisce la media
    }
    printf("Sono il padre, ho generato F4 con PID = %d\n", pid_F4);

    // Attende F4
    waitpid(pid_F4, &status, 0);
    int media = WEXITSTATUS(status); // Recupera la media

    printf("Sono il padre, il mio PID = %d, il valore medio dei numeri con cui hai popolato il vettore vale %d\n", getpid(), media);

    return 0;
}