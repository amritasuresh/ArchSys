#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>  
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <ulimit.h>
#include <unistd.h>
#include "packets.h" 

#define MAXFILES 20

// print message and exit
void fatal_error(char *err_msg)
{
        fprintf(stderr,"%s\n",err_msg);
        exit(0);
}

// Create and return a TCP socket.
int create_tcp_socket ()
{
	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) fatal_error("Client: No socket available!");
	return sock;
}

// This connects the given socket to the server at the given port.
void connect_socket_to_server (int sock, char *hostname, int port)
{
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
	if (connect(sock,(struct sockaddr *)&server_address,
                     sizeof server_address) < 0)
		fatal_error("Client: Cannot connect to server");
}

// connect the given socket to the given port
void bind_to_port (int sock, int port)
{
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);
	if (bind(sock,(struct sockaddr *)&server_address,sizeof server_address))
		fatal_error("Can't bind port");
}

// Turn socket into listen mode
void listen_at_socket (int sock)
{
	// make the socket listen for connections
	if (listen(sock,5)) fatal_error("listen failed");
}

// Wait for a client connection at socket and return the socket
// for communicating with that client.
int accept_client (int sock)
{
	while (1)
	{
		// let a client connect to the socket
		int client_socket = accept(sock,NULL,NULL);

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

// Send a string to the given socket.
void send_string (int to_socket, char *str)
{
	send_packet(to_socket,str,strlen(str)+1);
}

// Read a string from the given socket. Memory for the string
// is automatically allocated. It is your responsibility to
// free that memory when it is no longer needed.
char* receive_string (int from_socket)
{
	char *msg;
	receive_packet(from_socket,(void**)&msg);
	return msg;
}
