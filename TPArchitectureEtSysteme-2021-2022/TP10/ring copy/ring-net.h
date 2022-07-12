#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

// This struct stores information relating to a node
struct { 
	int id;				// identifier of a node
	FILE *chan_in, *chan_out;	// for communication
	int fd_in, fd_out;		// the functions receive, send
} node;

void protocol (int id);

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
void create_node ()
{
	srand48(time(NULL) + getpid());
	node.id = lrand48() & 0xffff;

	// we deal with streams on the descriptors
	node.chan_in = fdopen(node.fd_in,"r"); 
	node.chan_out = fdopen(node.fd_out,"w"); 

	// put streams in unbuffered mode (saves fflush)
	setlinebuf(node.chan_in); setlinebuf(node.chan_out);
}

// Accept a connection on port, returns a descriptor.
int accept_connection (int port)
{
        struct sockaddr_in server_address;

        // create a TCP socket
        int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        // connect the socket to chosen port
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(port);
        if (bind(s,(struct sockaddr *)&server_address,sizeof server_address))
	{
                fprintf(stderr,"Error : bind failed, "
				"port %d already in use ?\n", port);
		exit(1);
	}

        // put the socket on listen mode
        listen(s,1);

	printf("I wait for a connection on the port %d ...\n",port);
        while (1)
        {
                // let a client connect
                int client_socket = accept(s,NULL,NULL);

                if (client_socket < 0)
                {
                        if (errno == EINTR || errno == EWOULDBLOCK) continue;
			fprintf(stderr,"Error : connection to client failed\n");
			exit(1);
                }

		printf("Connection accepted !\n");
		return client_socket;
        }
}

// Connect to the server on hostname: port, return a descriptor
int connect_to_server (char *hostname, int port)
{
        // create a TCP socket
        int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        // find IP address of the server
        struct hostent *host_address = gethostbyname(hostname);
        if (!host_address)
	{
		fprintf(stderr,"Erreur : serveur %s non connu\n",hostname);
		exit(1);
	}

        //binding socket
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        memcpy(&server_address.sin_addr,
                host_address->h_addr_list[0],
                host_address->h_length);
        server_address.sin_port = htons(port);

        // connection with server
	printf("Connection to serveur at %s:%d ...\n",hostname,port);
	while (1)
	{
		if (connect(s,(struct sockaddr *)&server_address,
                     sizeof server_address) == 0)
		{
			printf("Connected !\n");
			return s;
		}
		sleep(3);
		printf("I try again\n");
	}

}
