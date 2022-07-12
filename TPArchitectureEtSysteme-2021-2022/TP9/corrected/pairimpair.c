#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t s1;
sem_t s2;
int cpt = 0 ;

#define MAX 1000

struct thread_info {
 sem_t *semA;
 sem_t *semB;
 char pOi ;
};

void* pairimpair (void* data) {
 struct thread_info *params = data;
 while(cpt<MAX) {
  sem_wait(params->semA);
  printf("%c %d\n", params->pOi, cpt++);
  sem_post(params->semB);
 }
 return NULL;
}

int main () {
 pthread_t t1, t2;

 sem_init(&s1,0,1);
 sem_init(&s2,0,0);

 struct thread_info pair={&s1,&s2,'p'}, impair={&s2,&s1,'i'};

 pthread_create(&t2,NULL,pairimpair,&pair);
 pthread_create(&t1,NULL,pairimpair,&impair);

 pthread_join(t1,NULL);  // wait for first thread
 pthread_join(t2,NULL);  // wait for second thread
 return 0;
}
