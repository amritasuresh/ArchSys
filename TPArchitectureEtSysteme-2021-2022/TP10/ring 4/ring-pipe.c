//average time before receiving a message
#define DELAY 1.0

// the messages
#define MSG_VOISIN 'v'
#define MSG_PROCHAIN 'p'
#define MSG_GAGNANT 'g'

#include "ring-pipe.h"

// Behavior of a node.

// This function is supposed to call exit() when the winner is known,
// with the winner's identifier as the exit code.

void protocol (int id)
{
	int active = 1;

	message("My identifier is %d",id);

	send(MSG_VOISIN,id);	// sends its own id to neighbour

	while (1)
	{
		// accepts a message
		char c; int d;
		receive(&c,&d);

		// evaluates the message et reacts accordingly, to be filled

		if (c == MSG_VOISIN)
		{
			// ...
		}
		else if (c == MSG_PROCHAIN)
		{
			// ...
		}
		else if (c == MSG_GAGNANT)
		{
			// ...
		}
	}
}

int main (int argc, char **argv)
{
	int i, n, status;

	if (argc != 2)
	{
		fprintf(stderr,"usage: ring <n>\n");
		exit(1);
	}

	// Generates n clients. Each client is a child process
	// which calles protocol() with the information in the struct node
	// prefilled.
	n = atoi(argv[1]);
	generate_nodes(n);

	// We wait for the termination of all processes before declaring the winner
	for (i = 0; i < n; i++) wait(&status);
	status = WEXITSTATUS(status);
	printf("[main] %d wins\n",status);
	exit(status);
}
