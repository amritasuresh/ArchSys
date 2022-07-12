// usage: client [<server> [<port>]]
// If no server or port are given, localhost and 4004 (PORT)
// are used as default.

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "network.h"

// port must be the same as in server
#define PORT 4004

int main (int argc, char **argv)
{
	char *server = (argc >= 2)? argv[1] : "localhost";
	int port = (argc >= 3)? atoi(argv[2]) : PORT;

	// connect to server
	int socket = create_tcp_socket();
	connect_socket_to_server(socket,server,port);

	printf("connected to server (%s:%d)\n",server,port);

        for (;;)
        {
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(0,&fds);
		FD_SET(socket,&fds);
		if (select(MAXFILES,&fds,NULL,NULL,NULL) < 0) continue;

		if (FD_ISSET(0,&fds))
		{
			char msg[100];
			fgets(msg,99,stdin);
			send_string(socket,msg);
		}
		if (FD_ISSET(socket,&fds))
		{
			char *msg = receive_string(socket);
			printf("%s\n",msg);
			free(msg);
		}
        }

	return 0;
}
