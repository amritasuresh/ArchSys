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

// The arguments given on the command line
int sock_server;
char *hostname;
int sock_client;

int main (int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf(stderr,"usage: ring <sock1> <host> <sock2>\n");
		exit(1);
	}

	sock_server = atoi(argv[1]);
	hostname = argv[2];
	sock_client = atoi(argv[3]);

	// These following lines give us a problem. What? How do we fix it?

	// accept our left neighbour's connection
	node.fd_in = accept_connection(sock_server);
	// we connect to our right neighbour
	node.fd_out = connect_to_server(hostname,sock_client);

	// generate and launch the node
	create_node();
	protocol(node.id);
	return 0;
}
