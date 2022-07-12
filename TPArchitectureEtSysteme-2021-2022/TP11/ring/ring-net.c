// délai moyen avant reception d'un message
#define DELAY 1.0

// les messages
#define MSG_VOISIN 'v'
#define MSG_PROCHAIN 'p'
#define MSG_GAGNANT 'g'

#include "ring-net.h"

// Comportement d'un nœud.
// Termine avec le numéro du gagnant comme code de sortie.

void *serverconnect(){
	node.fd_in = accept_connection(sock_server);
	return NULL;
}

void *clientconnect(){
	node.fd_out = connect_to_server(hostname,sock_client);
	return NULL;
}


void protocole (int id)
{
	int actif = 1, nb;

	message("My identifier is %d",id);

	envoyer(MSG_VOISIN,id);	// envoyer son identifiant au voisin

	while (1)
	{
		// accepter un message
		char c; int d;
		recevoir(&c,&d);

		if (!actif)
		{
			// faire passer le message
			envoyer(c,d);
			if (c == MSG_GAGNANT)
			{
				message("node %d wins",d);
				exit(d);
			}
			continue;
		}

		if (c == MSG_VOISIN)
		{
			nb = d;
			message("id of neighbour = %d",d);
			envoyer(MSG_PROCHAIN,d);
		}
		else if (c == MSG_PROCHAIN)
		{
			// évaluer les trois identifiants
			message("ids = (%d,%d,%d)",id,nb,d);
			if (d == id && nb >= d)
			{
				message("I won !!");
				envoyer(MSG_GAGNANT,id);
			}
			else if (nb > id && nb > d)
			{
				message("I stay active");
				envoyer(MSG_VOISIN,id);
			}
			else
			{
				message("I have become passive");
				actif = 0;
			}
		}
		else if (c == MSG_GAGNANT)
		{
			if (d != id)
				message("error : someone has won yet "
					"I am still active?? (%d,%d)",d,id);
			exit(id);
		}
	}
}

// Les arguments qu'on donne sur la ligne de commande
int sock_server;
char *hostname;
int sock_client;

int main (int argc, char **argv)
{
	pthread_t clientthread, serverthread;
	if (argc != 4)
	{
		fprintf(stderr,"usage: ring <sock1> <host> <sock2>\n");
		exit(1);
	}

	sock_server = atoi(argv[1]);
	hostname = argv[2];
	sock_client = atoi(argv[3]);

	pthread_create(&serverthread, NULL, serverconnect, NULL );
	pthread_create(&clientthread, NULL, clientconnect, NULL);

	pthread_join(serverthread, NULL);
	pthread_join(clientthread, NULL);

	// Attention, les lignes suivantes présentent un problème
	// qu'il faudra encore résoudre....

	// accepter la connection de notre voisin à gauche
	//node.fd_in = accept_connection(sock_server); //THREAD 1
	// nous connecter à notre voisin à droite
	//node.fd_out = connect_to_server(hostname,sock_client); //THREAD 2

	// générer et lancer le nœud
	create_node();
	protocole(node.id);
	return 0;
}
