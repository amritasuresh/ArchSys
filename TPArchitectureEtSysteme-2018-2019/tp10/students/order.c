#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

void* first (void* data)
{
	printf("First\n");
	return NULL;
}

void* second (void* data)
{
	printf("Second\n");
	return NULL;
}

void* third (void* data)
{
	printf("Third\n");
	return NULL;
}

int main ()
{
	pthread_t t1;
	pthread_t t2;
	pthread_t t3;

	pthread_create(&t3,NULL,third,NULL);
	pthread_create(&t2,NULL,second,NULL);
	pthread_create(&t1,NULL,first,NULL);

	pthread_join(t1,NULL);  // wait for first thread
	pthread_join(t2,NULL);  // wait for second thread
	pthread_join(t3,NULL);  // wait for second thread
	return 0;
}
