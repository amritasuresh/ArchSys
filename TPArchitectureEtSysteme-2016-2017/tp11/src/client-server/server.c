/*   server
	Call with: server <portnr> <number-of-players>
*/

#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <ulimit.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "packets.h"

// maximum number of files that a process can open
#define MAXFILES (ulimit(4))

// the TCP port on which the server installs itself (must be above 1023)
#define PORT 4004

// maximal size of a file that we'll allow
#define MAXSIZE 65535

// name of history file
#define HISTORY "history.txt"

int get_server_socket (int port)
{
	struct sockaddr_in server_address;

	// create a TCP socket
	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) fatal_error("No socket available");

	// connect the socket to the chosen port
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);
	if (bind(s,(struct sockaddr *)&server_address,
				sizeof server_address))
		fatal_error("Can't bind port");

	// make the socket listen for connections
	if (listen(s,5)) fatal_error("listen failed");

	return s;
}


int wait_for_client (int socket)
{
	while (1)
	{
		// let a client connect to the socket
		int client_socket = accept(socket,NULL,NULL);

		if (client_socket < 0)
		{
			if (errno == EINTR || errno == EWOULDBLOCK)
				continue;
			else
				fatal_error("Can't accept connection");
		}

		return client_socket;
	}
}

#define MAXFILES (ulimit(4))

int main (int argc, char **argv)
{
	char *msg;
	fd_set readfds;

	// install the port
	int port = (argc >= 2)? atoi(argv[1]) : PORT;
	int socket = get_server_socket(port);

	printf("Waiting for clients\n");
	int csock1 = wait_for_client(socket);
	int csock2 = wait_for_client(socket);

	printf("Waiting for input\n");
	for (;;)
	{
		// wait for input...
		FD_ZERO(&readfds);
		FD_SET(csock1,&readfds);	// .. from first client
		FD_SET(csock2,&readfds);	// .. from second client
		FD_SET(0,&readfds);		// .. from console
		FD_SET(socket,&readfds);	// .. another connection
		if (select(MAXFILES, &readfds, NULL, NULL, NULL) <0) continue;

		// check where input arrived
		if (FD_ISSET(csock1,&readfds))
		{
			receive_packet(csock1,(void**)&msg);
			printf("client 1 said: %s\n",msg);
		}
		if (FD_ISSET(csock2,&readfds))
		{
			receive_packet(csock2,(void**)&msg);
			printf("client 2 said: %s\n",msg);
		}
		if (FD_ISSET(0,&readfds))
		{
			char msg[100]; // evil
			scanf("%s",msg);
			printf("server said: %s\n",msg);
		}
		if (FD_ISSET(socket,&readfds))
		{
			printf("another client tries to join, sorry we're full\n");
			close(wait_for_client(socket));
		}
	}
}
