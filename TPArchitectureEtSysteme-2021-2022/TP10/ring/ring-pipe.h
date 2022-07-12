#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>

// This struct stores information relating to a node
struct { 
	int id;				// identifier of a node
	FILE *chan_in, *chan_out;	// for communication
	int fd_in, fd_out;		// the functions receive, send
} node;

void protocol(int id);

//Random delay, exponential distribution 
void delay ()
{
	double d = drand48();
	double r = -log(d) * DELAY;
	usleep(r*1e6);
}

// Receives a message (this consists of either 'v','p','g', along with d an id)
void receive (char *c, int *d)
{
	char pb;	// for EOL
	fscanf(node.chan_in,"%c %d%c",c,d,&pb);
}

// Send a message
void send (char c, int d)
{
	delay(); // pour rendre le comportement un peu moins déterministe
	fprintf(node.chan_out,"%c %d\n",c,d);
}

// Writes a message to the screen. Adds a newline at the end
void message (char* msg, ...)
{
        va_list args;
	printf("[node %d] ",node.id);
        va_start(args,msg);
        vprintf(msg,args);
        va_end(args);
	puts("");
}

// Create the structures around a node
void create_node (int id, int readfd, int writefd)
{
	srand48(time(NULL) + id);

	node.id = id;
	node.fd_in = readfd;
	node.fd_out = writefd;

	// we deal with streams on the descriptors
	node.chan_in = fdopen(node.fd_in,"r"); 
	node.chan_out = fdopen(node.fd_out,"w"); 

	// put streams in unbuffered mode (saves fflush)
	setlinebuf(node.chan_in); setlinebuf(node.chan_out);

	protocol(id);
}

// Generates a random permutation of 1..n for the ring
int* create_perm (int n)
{
	int i, j, *p = malloc(n*sizeof(int));
	for (i = 0; i < n; i++) p[i] = i;

	for (j = 0; j < i*i; i++)
	{
		int k = (int)(drand48()*n);
		int l = (int)(drand48()*n);
		int tmp = p[k]; p[k] = p[l]; p[l] = tmp;
	}
	return p;
}

// Generates the n nodes
void generate_nodes (int n)
{
	int i, *perm;
	int pp[2], nextread, lastwrite;

	srand48(time(NULL));
	perm = create_perm(n);

	// We generate the n nodes with their pipes
	pipe(pp);
	lastwrite = pp[1];
	nextread = pp[0];

	printf("Ring : ");
	for (i = 0; i < n; i++) printf("%d - ",perm[i]);
	printf("%d\n",perm[0]);

	for (i = 0; i < n-1; i++)
	{
		pipe(pp);
		if (!fork())
			create_node(perm[i],nextread,pp[1]);
		close(nextread);
		close(pp[1]);
		nextread = pp[0];
	}
	if (!fork()) create_node(perm[i],nextread,lastwrite);
}
