#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <dispatch/dispatch.h>

// Producer/consumer: Two threads exchange data via a shared buffer.

// The shared buffer has capacity CAP
#define CAP 10
char buffer[CAP];
int rdptr = 0, wrptr = 0;
//sem_t full, empty;
dispatch_semaphore_t full, empty;

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
		dispatch_semaphore_wait(empty, DISPATCH_TIME_FOREVER);
		//printf("Did wait\n");
		//sem_wait(&empty);
		if (!read(fd,&c,1)) break;
		//printf("Did read\n");
		put(c);
		//printf("Did put\n");
		dispatch_semaphore_signal(full);
		//sem_post(&full);
	}
	put(EOF);
	dispatch_semaphore_signal(full);
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
		//sem_wait(&full);
		dispatch_semaphore_wait(full, DISPATCH_TIME_FOREVER);
		char c = get();
		if (c == EOF) break;
		putchar(c);
		//sem_signal(&empty);
		dispatch_semaphore_signal(empty);

	}
	return NULL;
}

// The main program creates both threads and waits for them to finish.
int main ()
{
	pthread_t t1, t2;

	//sem_init(&full, 0, 0);
	//sem_init(&empty, 0, CAP);
	full = dispatch_semaphore_create(0);
	empty = dispatch_semaphore_create(CAP);


	// create threads
	pthread_create(&t1,NULL,producer,NULL);
	pthread_create(&t2,NULL,consumer,NULL);



	// wait for finish
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);

    printf("\n Done\n");

	return 0;
}
