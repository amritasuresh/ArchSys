#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Producer/consumer: Two threads exchange data via a shared buffer.

// The shared buffer has capacity CAP
#define CAP 10
char buffer[CAP];
int rdptr = 0, wrptr = 0;

// The producer thread reads a file and puts its contents into the buffer.
void put (char c)
{
	buffer[wrptr] = c;
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
	c = buffer[rdptr];
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

	// create threads
	pthread_create(&t1,NULL,producer,NULL);
	pthread_create(&t2,NULL,consumer,NULL);

	// wait for finish
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);

	return 0;
}
