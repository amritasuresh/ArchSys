#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    key_t key = ftok("shmem_ex.c", 'A'), id;
    printf("Reader key is: %d\n", key);
    if ((id = shmget(key, 1024, 0666)) == -1) {
        perror("reader shmget");
        exit(1);
    }

    char *adress; /* Adresse du segment de memoire partagee */
    /* Attachement de la zone memoire a l'espace memoire du processus */
    if ((int) (adress = shmat(id, NULL, 0)) == -1) {
        perror("failed attaching shared memory");
        shmctl(id, IPC_RMID, NULL); /* Removing segment */
        exit(1);
    }

    printf("You have to: %s\n", adress);
    shmdt(adress); /* Detaching memory */
    shmctl(id, IPC_RMID, NULL); /* Remove segment */
    return 0;
}
