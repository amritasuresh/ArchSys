#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <dispatch/dispatch.h>

#define THREAD_NUM 6

//pthread_mutex_t mutexFuel;
dispatch_semaphore_t semFuel;        //declare semaphores

int fuel = 50;

void* routine1(void* args) {
    while (1) {
        dispatch_semaphore_wait(semFuel, DISPATCH_TIME_FOREVER);
        //pthread_mutex_lock(&mutexFuel);
        fuel += 50;
        printf("Current value is %d\n", fuel);
    }
}

void* routine2(void* args) {
    while (1) {
        dispatch_semaphore_signal(semFuel);
        //pthread_mutex_unlock(&mutexFuel);
        usleep(5);
    }
}

int main(int argc, char *argv[]) {
    semFuel = dispatch_semaphore_create(1);
    pthread_t th[THREAD_NUM];
    //pthread_mutex_init(&mutexFuel, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&th[i], NULL, &routine1, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &routine2, NULL) != 0) {
                perror("Failed to create thread");
            }   
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    //pthread_mutex_destroy(&mutexFuel);
    return 0;
}