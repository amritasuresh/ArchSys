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

// Les informations relatives à un nœud
struct { 
	int id;				// identifiant du nœud
	FILE *chan_in, *chan_out;	// pour communication, utiliser
	int fd_in, fd_out;		// les fonctions recevoir, envoyer
} node;

void protocole (int id);

// Délai aléatoire, distribution exponentielle
void delay ()
{
	double d = drand48();
	double r = -log(d) * DELAY;
	usleep(r*1e6);
}

// Recevoir un message (c est parmi 'v','p','g', et d un identifiant)
void recevoir (char *c, int *d)
{
	char pb;	// pour EOL
	fscanf(node.chan_in,"%c %d%c",c,d,&pb);
}

// Envoyer un message
void envoyer (char c, int d)
{
	delay(); // pour rendre le comportement un peu moins déterministe
	fprintf(node.chan_out,"%c %d\n",c,d);
}

// Émettre un message sur l'écran. Rajoute une nouvelle ligne à la fin.
void message (char* msg, ...)
{
        va_list args;
	printf("[node %d] ",node.id);
        va_start(args,msg);
        vprintf(msg,args);
        va_end(args);
	puts("");
}

// Créer les structures autour d'un nœud
void create_node ()
{
	// générer un identifiant aléatoire
	srand48(time(NULL) + getpid());
	node.id = lrand48() & 0xffff;

	// on branche des stream sur les descripteurs
	node.chan_in = fdopen(node.fd_in,"r"); 
	node.chan_out = fdopen(node.fd_out,"w"); 

	// mettre les stream en mode non-tamponné (économise des fflush)
	setlinebuf(node.chan_in); setlinebuf(node.chan_out);
}

// Accepter une connexion sur port, renvoie un descripteur.
int accept_connection (int port)
{
        struct sockaddr_in server_address;

        // créer un socket TCP
        int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        // connecter le socket au port choisi
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(port);
        if (bind(s,(struct sockaddr *)&server_address,sizeof server_address))
	{
                fprintf(stderr,"Error : bind failed, "
				"port %d is already in use ?\n", port);
		exit(1);
	}

        // mettre le socket en mode écoute
        listen(s,1);

	printf("I am waiting for a connection on the port %d ...\n",port);
        while (1)
        {
                // laisser un client se connecter
                int client_socket = accept(s,NULL,NULL);

                if (client_socket < 0)
                {
                        if (errno == EINTR || errno == EWOULDBLOCK) continue;
			fprintf(stderr,"Error : client connection failed\n");
			exit(1);
                }

		printf("Connection accepted !\n");
		return client_socket;
        }
}

// Se connecter au serveur sur hostname:port, renvoie un descripteur
int connect_to_server (char *hostname, int port)
{
        // créer un socket TCP
        int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        // trouver l'adresse IP du serveur
        struct hostent *host_address = gethostbyname(hostname);
        if (!host_address)
	{
		fprintf(stderr,"Error : server %s unknown\n",hostname);
		exit(1);
	}

        // bricoler l'adresse avec le port
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        memcpy(&server_address.sin_addr,
                host_address->h_addr_list[0],
                host_address->h_length);
        server_address.sin_port = htons(port);

        // se connecter au serveur
	printf("Connection to server at %s:%d ...\n",hostname,port);
	while (1)
	{
		sleep(8);
		if (connect(s,(struct sockaddr *)&server_address,
                     sizeof server_address) == 0)
		{
			printf("Connected !\n");
			return s;
		}
		sleep(8);
		printf("I am retrying...\n");
	}

}
