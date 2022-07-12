#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 100000000

int counter = 0;
char f0 = 0, f1 = 0, v = 0;

void* P0 (void* data)
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		f0 = 1;		// algorithme de Peterson pour P0
		v = 0;
		__sync_synchronize();	// barrière de mémoire
		while (f1 && v == 0);

		// section critique
		counter++;

		f0 = 0;		// on quitte la section critique
	}
	return NULL;
}

void* P1 (void* data)
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		f1 = 1;		// algorithme de Peterson pour P1
		v = 1;
		__sync_synchronize();	// barrière de mémoire
		while (f0 && v == 1);

		// section critique
		counter++;

		f1 = 0;		// on quitte la section critique
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
