/*
	The server, to be completed.
	Usage: server <port>
*/

#include <stdlib.h>
#include "network.h"

// the TCP port on which the server installs itself (must be above 1023)
#define PORT 4004

int main (int argc, char **argv)
{
	// the port where the server should install itself
	int port = (argc >= 2)? atoi(argv[1]) : PORT;

	// to be completed

	return 0;	// just to satisfy the compiler
}
