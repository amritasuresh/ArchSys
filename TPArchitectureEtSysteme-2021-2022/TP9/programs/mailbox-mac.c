#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <dispatch/dispatch.h>
#include<semaphore.h>

#define MAX 1000000

int mails = 0;
//pthread_mutex_t m;
//sem_t semodd;
//sem_t semeven;
dispatch_semaphore_t semodd;        //declare semaphores
dispatch_semaphore_t semeven;

void* routine() {
    //int t1 = 0;

    for (int i = 0; i < MAX; i++) {
        //sem_wait(&s);
        //dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        //pthread_mutex_lock(&m);
        mails++; //CS
        //sem_post(&s);
        //dispatch_semaphore_signal(semaphore);
        //pthread_mutex_unlock(&m);
        //t1++;
    }
    //printf("t1 = %d\n", t1);
    return NULL;
}


void* odd(){

    for (int i =1; i<100; i+=2){

        //sem_wait(&semodd);
        dispatch_semaphore_wait(semodd, DISPATCH_TIME_FOREVER);
        printf("odd thread output = %d\n", i);
        dispatch_semaphore_signal(semeven); //post



    }

    return NULL;
}

void* even(){

    for (int i =2; i<=100; i+=2){

        //sem_wait(&semodd);
        dispatch_semaphore_wait(semeven, DISPATCH_TIME_FOREVER);
        printf("even thread output = %d\n", i);
        dispatch_semaphore_signal(semodd); //post



    }


    return NULL;
}

int main(int argc, char* argv[]) {
    semodd = dispatch_semaphore_create(1);
    semeven = dispatch_semaphore_create(0); //initialize semaphores
    pthread_t p1, p2;
    //sem_init(&semodd, 0, 0);
    //sem_init(&semeven, 0, 1);
    //pthread_mutex_init(&m, NULL);
    pthread_create(&p1, NULL, &odd, NULL);
    pthread_create(&p2, NULL, &even, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    dispatch_release(semeven);
    dispatch_release(semodd);

    //printf("Number of mails: %d\n", mails);
    return 0;
}