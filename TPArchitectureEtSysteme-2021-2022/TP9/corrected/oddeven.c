#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

dispatch_semaphore_t semaphore;
dispatch_semaphore_t semaphore;

int cpt = 0 ;

#define MAX 10

struct thread_info {
 sem_t *semA;
 sem_t *semB;
 char eOo ;
};

void* odd () {
 //struct thread_info *params = data;
 while(cpt<MAX) {
  //sem_wait(params->semA);
  sem_wait(&s1);
  //printf("%c %d\n", params->eOo, cpt++);
  printf(" 'o' %d\n", cpt++);
  sem_post(&s2);
  //sem_post(params->semB);
 }
 return NULL;
}

void* even () {
 while(cpt<MAX) {
  sem_wait(&s2);
  printf(" 'e '%d\n", cpt++);
  sem_post(&s1);
 }
 return NULL;
}

int main () {
 pthread_t t1, t2;

 sem_init(&s1,0,0);
 sem_init(&s2,0,0);

 struct thread_info even={&s1,&s2,'e'}, odd={&s2,&s1,'o'};

 pthread_create(&t2,NULL,&even,NULL);
 pthread_create(&t1,NULL,&odd, NULL);

 pthread_join(t1,NULL);  // wait for first thread
 pthread_join(t2,NULL);  // wait for second thread
 return 0;
}
