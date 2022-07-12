#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include<semaphore.h>

#define MAX 1000000

int mails = 0;
pthread_mutex_t m;
sem_t s;

void* routine() {
    //int t1 = 0;
    for (int i = 0; i < MAX; i++) {
        sem_wait(&s);
        pthread_mutex_lock(&m);
        mails++; //CS
        sem_post(&s);
        pthread_mutex_unlock(&m);
        //t1++;
    }
    //printf("t1 = %d\n", t1);
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2;
    sem_init(&s, 0, 1);
    pthread_mutex_init(&m, NULL);
    pthread_create(&p1, NULL, &routine, NULL);
    pthread_create(&p2, NULL, &routine, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Number of mails: %d\n", mails);
    return 0;
}