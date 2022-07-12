// délai moyen avant reception d'un message
#define DELAY 1.0

// les messages
#define MSG_VOISIN 'v'
#define MSG_PROCHAIN 'p'
#define MSG_GAGNANT 'g'

#include "ring-net.h"

// Comportement d'un nœud, la même fonction que dans ring-pipe.c

void protocole (int id)
{
	int actif = 1;

	message("Mon identifiant est %d",id);

	envoyer(MSG_VOISIN,id);	// envoyer son identifiant au voisin

	while (1)
	{
		// accepter un message
		char c; int d;
		recevoir(&c,&d);

		// évaluer le message et réagir, à compléter

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

// Les arguments qu'on donne sur la ligne de commande
int sock_server;
char *hostname;
int sock_client;

void* servermethod(){
	node.fd_in = accept_connection(sock_server);
	return NULL;
}

void* clientmethod(){
	node.fd_out = connect_to_server(hostname,sock_client);
	return NULL;
}

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

	pthread_t clientthread, serverthread;

	pthread_create(&clientthread, NULL, &clientmethod, NULL);
	pthread_create(&serverthread, NULL, &servermethod, NULL);

	// accepter la connection de notre voisin à gauche
	//node.fd_in = accept_connection(sock_server);
	// nous connecter à notre voisin à droite
	//node.fd_out = connect_to_server(hostname,sock_client);

	pthread_join(clientthread, NULL);
	pthread_join(serverthread, NULL);

	// générer et lancer le nœud
	create_node();
	protocole(node.id);
	return 0;
}
