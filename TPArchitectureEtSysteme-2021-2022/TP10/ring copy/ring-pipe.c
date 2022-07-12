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
	int active = 1, neighbour;

	message("My identifier is %d",id);

	send(MSG_VOISIN,id);	// sends its own id to neighbour

	while (1)
	{
		// accepts a message
		char c; int d;
		receive(&c,&d);

		if (!active){
			//forward all messages
			send(c, d);
			if(c == MSG_GAGNANT){
				printf("%d is the winner\n", d);
				exit(d);
			}
			continue;
		}

		// evaluates the message et reacts accordingly, to be filled

		if (c == MSG_VOISIN) //if message is from neighbour
		{
			neighbour = d;
			send(MSG_PROCHAIN, d);
			printf("[%d] I received from neighbour %d", id, d);
			
		}
		else if (c == MSG_PROCHAIN)
		{
			message("ids received this round = (%d,%d,%d)",id,neighbour,d);

			if (id == d && neighbour >= d)  {
				//id = neighbour;
				send(MSG_GAGNANT, d);
				printf("I am the winner %d!\n", d);
				exit(d);
			}
			else if (neighbour > d && neighbour > id){
				printf("[%d] Still active\n", id);
				send(MSG_VOISIN, id);
			}

			else{
				active = 0; //becomes inactive
				printf("[%d] I have become passive\n", id);
			}
		}
		else if (c == MSG_GAGNANT)
		{
			if(d != id)
				printf("I am active, but received another winner. Error? %d>%d\n", d, id);
				exit(id);
			
		}
	}
}

int main (int argc, char **argv)
{
	int i, n, status;

	if (argc != 2)
	{
		fprintf(stderr,"usage: ./ring-pipe <n>\n");
		exit(1);
	}

	// Generates n clients. Each client is a child process
	// which calles protocol() with the information in the struct node
	// prefilled.
	n = atoi(argv[1]); //converts string to int
	generate_nodes(n);

	// We wait for the termination of all processes before declaring the winner
	for (i = 0; i < n; i++) wait(&status);
	status = WEXITSTATUS(status);
	printf("[main] %d wins\n",status);
	exit(status);
}
