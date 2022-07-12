#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t s2;
sem_t s3;

void* first (void* data)
{
	printf("First\n");
	sem_post(&s2);
	return NULL;
}

void* second (void* data)
{
	sem_wait(&s2);
	printf("Second\n");
	sem_post(&s3);
	return NULL;
}

void* third (void* data)
{
	sem_wait(&s3);
	printf("Third\n");
	return NULL;
}

int main ()
{
	pthread_t t1;
	pthread_t t2;
	pthread_t t3;

	sem_init(&s2,0,0);
	sem_init(&s3,0,0);

	pthread_create(&t3,NULL,third,NULL);
	pthread_create(&t2,NULL,second,NULL);
	pthread_create(&t1,NULL,first,NULL);

	pthread_join(t1,NULL);  // wait for first thread
	pthread_join(t2,NULL);  // wait for second thread
	pthread_join(t3,NULL);  // wait for second thread
	return 0;
}
