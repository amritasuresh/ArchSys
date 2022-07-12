#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    key_t key = ftok("shmem_ex.c", 'A'), id;
    printf("Writer key is: %d\n", key);
    /* Creating a 80 bytes shared mem segment. */
    if ((id = shmget(key, 1024, 0666|IPC_CREAT)) == -1) {
        perror("writer shmget");
        exit(1);
    }

    char *adress;
    char msg[80] = "feed the birds.";

    /* Attaching memory to process */
    if ((int) (adress = shmat(id, NULL, 0)) == -1) {
        perror("failure shmem attachment");
        shmctl(id, IPC_RMID, NULL); /* Removing segment */
        exit(1);
    }

    /* Writing message */
    strcpy(adress, msg);
    shmdt(adress); /* Detaching memory */
    return 0;
}
