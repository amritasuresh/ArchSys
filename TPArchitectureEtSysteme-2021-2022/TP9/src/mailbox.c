#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define MAX 10

int mails = 0;

void* routine() {
    for (int i = 0; i < MAX; i++) {
        mails++;
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2, p3, p4;
    pthread_create(&p1, NULL, &routine, NULL);
    pthread_create(&p2, NULL, &routine, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Number of mails: %d\n", mails);
    return 0;
}