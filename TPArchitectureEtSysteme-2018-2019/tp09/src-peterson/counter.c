#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 100000000

int counter = 0;

void* P0 (void* data)
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		counter++;
	}
	return NULL;
}

void* P1 (void* data)
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		counter++;
	}
	return NULL;
}

int main ()
{
	pthread_t t1;
	pthread_t t2;

	pthread_create(&t1, NULL, P0, NULL);	// create first thread
	pthread_create(&t2, NULL, P1,   NULL);	// create second thread

	pthread_join(t1, NULL);  // wait for first thread
	pthread_join(t2, NULL);  // wait for second thread

	printf("Counter: %d\n", counter);
	return 0;
}
