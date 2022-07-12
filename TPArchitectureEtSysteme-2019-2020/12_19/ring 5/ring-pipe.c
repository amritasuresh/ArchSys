// délai moyen avant reception d'un message
#define DELAY 1.0

// les messages
#define MSG_VOISIN 'v'
#define MSG_PROCHAIN 'p'
#define MSG_GAGNANT 'g'

#include "ring-pipe.h"

// Comportement d'un nœud.

// Cette fonction est censé appeler exit() quand le gagnant est connu,
// avec l'identifiant du gagnant comme code de sortie.

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
				message("node %d has won",d);
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
			
			message("ids = (%d,%d,%d)",id,nb,d);
			if (d == id && nb >= d)
			{
				message("I won!! !!");
				envoyer(MSG_GAGNANT,id);
			}
			else if (nb > id && nb > d)
			{
				message("I remain active");
				envoyer(MSG_VOISIN,id);
			}
			else
			{
				message("I'm inactive now");
				actif = 0;
			}
		}
		else if (c == MSG_GAGNANT)
		{
			if (d != id)
				message("Error : Why am I active if someone  "
					"else won? (%d>%d)",d,id);
			exit(id);
		}
	}
}

int main (int argc, char **argv)
{
	int i, n, status;

	if (argc != 2)
	{
		fprintf(stderr,"usage: ring <n>\n");
		exit(1);
	}

	// Générer les n clients. Chaque client est un processus fils
	// qui appelera protocole() avec les données dans struct node
	// préremplies.
	n = atoi(argv[1]);
	genere_noeuds(n);

	// On attend la terminaison de tout le monde pour affecter le gagnant.
	for (i = 0; i < n; i++) wait(&status);
	status = WEXITSTATUS(status);
	printf("[main] %d won!!\n",status);
	exit(status);
}
