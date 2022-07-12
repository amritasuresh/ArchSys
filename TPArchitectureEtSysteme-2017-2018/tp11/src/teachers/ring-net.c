// délai moyen avant reception d'un message
#define DELAY 1.0

// les messages
#define MSG_VOISIN 'v'
#define MSG_PROCHAIN 'p'
#define MSG_GAGNANT 'g'

#include "ring-net.h"

// Comportement d'un nœud.
// Termine avec le numéro du gagnant comme code de sortie.

void protocole (int id)
{
	int actif = 1, nb;

	message("Mon identifiant est %d",id);

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
				message("nœud %d gagne",d);
				exit(d);
			}
			continue;
		}

		if (c == MSG_VOISIN)
		{
			nb = d;
			message("id du voisin = %d",d);
			envoyer(MSG_PROCHAIN,d);
		}
		else if (c == MSG_PROCHAIN)
		{
			// évaluer les trois identifiants
			message("ids = (%d,%d,%d)",id,nb,d);
			if (d == id && nb >= d)
			{
				message("J'ai gagné !!");
				envoyer(MSG_GAGNANT,id);
			}
			else if (nb > id && nb > d)
			{
				message("Je reste actif");
				envoyer(MSG_VOISIN,id);
			}
			else
			{
				message("Je deviens passif");
				actif = 0;
			}
		}
		else if (c == MSG_GAGNANT)
		{
			if (d != id)
				message("erreur : quelqu'un a gagné alors "
					"que je suis actif?? (%d,%d)",d,id);
			exit(id);
		}
	}
}

// Les arguments qu'on donne sur la ligne de commande
int sock_server;
char *hostname;
int sock_client;

void* server_thread (void *arg)
{
        // accepter la connection de notre voisin à gauche
        node.fd_in = accept_connection(sock_server);
	return NULL;
}

void* client_thread (void *arg)
{
        // nous connecter à notre voisin à droite
        node.fd_out = connect_to_server(hostname,sock_client);
	return NULL;
}

int main (int argc, char **argv)
{
	pthread_t t1, t2;

	if (argc != 4)
	{
		fprintf(stderr,"usage: ring <sock1> <host> <sock2>\n");
		exit(1);
	}

	sock_server = atoi(argv[1]);
	hostname = argv[2];
	sock_client = atoi(argv[3]);

	pthread_create(&t1, NULL, server_thread, NULL);
	pthread_create(&t2, NULL, client_thread, NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	// générer et lancer le nœud
	create_node();
	protocole(node.id);
	return 0;
}
