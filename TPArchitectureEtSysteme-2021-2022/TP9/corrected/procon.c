#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <dispatch/dispatch.h>
#include <semaphore.h>

// Producer/consumer: Two threads exchange data via a shared buffer.

// The shared buffer has capacity CAP
#define CAP 10

//sem_t full, empty;
dispatch_semaphore_t full, empty;

char buffer[CAP];
int rdptr = 0, wrptr = 0;

// The producer thread reads a file and puts its contents into the buffer.
void put (char c)
{
	//sem_wait(&empty);
	dispatch_semaphore_wait(empty, DISPATCH_TIME_FOREVER);
	buffer[wrptr] = c;
	//sem_post(&full);
	dispatch_semaphore_signal(full);
	wrptr = (wrptr+1) % CAP;
}

void* producer (void *arg)
{
	int fd = open("lorem.txt",O_RDONLY);
	while (1)
	{
		char c;
		if (!read(fd,&c,1)) break;
		put(c);
	}
	put(EOF);
	close(fd);
	return NULL;
}

// The consumer thread reads the contents of the buffer and prints them.
char get ()
{
	char c;
	//sem_wait(&full);
	dispatch_semaphore_wait(full, DISPATCH_TIME_FOREVER);
	c = buffer[rdptr];
	//sem_post(&empty);
	dispatch_semaphore_signal(empty);
	rdptr = (rdptr+1) % CAP;
	return c;
}

void* consumer (void *arg)
{
	while (1)
	{
		char c = get();
		if (c == EOF) break;
		putchar(c);
	}
	return NULL;
}

// The main program creates both threads and waits for them to finish.
int main ()
{
	pthread_t t1, t2;

	//sem_init(&empty,0,CAP);
	//sem_init(&full,0,0);

	full = dispatch_semaphore_create(0);
	empty = dispatch_semaphore_create(CAP);

	// create threads
	pthread_create(&t1,NULL,producer,NULL);
	pthread_create(&t2,NULL,consumer,NULL);

	// wait for finish
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);

	return 0;
}
