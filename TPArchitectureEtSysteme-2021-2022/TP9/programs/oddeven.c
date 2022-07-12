#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include<semaphore.h>

#define MAX 1000000


sem_t semodd; //declare semaphores
sem_t semeven; //declare semaphores

void* odd(){

    for (int i =1; i<100; i+=2){

        sem_wait(&semodd);
        printf("odd thread output = %d\n", i);
        sem_post(&semeven);
    }

    return NULL;
}

void* even(){

    for (int i =2; i<=100; i+=2){

        sem_wait(&semeven);
        printf("even thread output = %d\n", i);
        sem_post(&semodd);

    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2;
    sem_init(&semodd, 0, 1);  
    sem_init(&semeven, 0, 0);//we initialize the even semaphore to 0 so that the odd one can go first
    pthread_create(&p1, NULL, &odd, NULL);
    pthread_create(&p2, NULL, &even, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    sem_destroy(&semeven);
    sem_destroy(&semodd);

    return 0;
}