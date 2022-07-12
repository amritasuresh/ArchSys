/*
    Client
        Call with: client <hostname> <service-nr> <playername>
*/

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

#include "packets.h"

// port must be the same as in server
#define PORT 4004

// This connects to the server at the given port and returns a socket
// for communication.
int connect_to_server (char *hostname, int port)
{
	// Create a TCP socket
	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) fatal_error("Client: No socket available!");

	// Find the IP address of the host machine (where the server is
	// supposed to run).
	struct hostent *host_address = gethostbyname(hostname);
	if (!host_address) fatal_error("Unknown host");

	// Build the address from hostname and port.
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	memcpy(&server_address.sin_addr,
		host_address->h_addr_list[0],
		host_address->h_length);
	server_address.sin_port = htons(port);

	// Connect to the server
	if (connect(s,(struct sockaddr *)&server_address,
                     sizeof server_address) < 0)
		fatal_error("Client: Cannot connect to server");

	return s;
}

// usage: client [<server> [<port>]]
// If no server or port are given, localhost and 4004 (PORT)
// are used as default.
int main (int argc, char **argv)
{
	char msg[100];
	char *server = (argc >= 2)? argv[1] : "localhost";
	int port = (argc >= 3)? atoi(argv[2]) : PORT;

	// connect to server
	int socket = connect_to_server(server,port);

	// Read a string on the console and send it over the network.
	// send_string() is contained in packets.c.
	// Note that using a fixed-length array for msg[] is bad-programming
	// style and can lead to overflows. Use, e.g., the readline library
	// instead.
        while (1)
        {
		scanf("%s",msg);
		send_string(socket,msg);
        }

	return 0;
}
