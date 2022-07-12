#include <stdlib.h>
#include <stdio.h>
#include<semaphore.h>
#include <pthread.h>

#define MAX 1000000

int mails = 0;
sem_t s;
//pthread_mutex_t m;

void* routine() {


    for (int i = 0; i < MAX; i++) {
        sem_wait(&s);
        //pthread_mutex_lock(&m);
        mails++;
        sem_post(&s);
        //pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2, p3, p4;
    sem_open(&s, 0, 1);
    //pthread_mutex_init(&m, NULL);
    pthread_create(&p1, NULL, &routine, NULL);
    pthread_create(&p2, NULL, &routine, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    sem_destroy(&s);
    //pthread_mutex_destroy(&m);

    printf("Number of mails: %d\n", mails);
    return 0;
}
