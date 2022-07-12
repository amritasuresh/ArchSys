#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* Petite modification : le fils attend un signal en appelant pause()
   ce qui va le reveiller lorsqu'un signal est livré. Du coup on évite
   la consommation inutile du CPU.

   Il reste néanmoins un problème : quand le père envoye plusieurs signaux
   d'un seul coup, ceux-ci seront perdus (sauf le premier). Ceci arrive,
   par exemple, quand l'utilisateur saisit "1010" dans une seul ligne. */

/***** le père *****/
int ready = 0;
void get_ready () { ready = 1; }

void parent (int cpid)
{
	char c;

	while (1)
	{
		for (c = 0; c != '0' && c != '1' && c != EOF; c = getchar());
		if (c == '0') kill(cpid,SIGUSR1);
		if (c == '1') kill(cpid,SIGUSR2);
		if (c == EOF) break;
	}

	// terminer le fils, sinon il reste vivant
	kill(cpid,SIGTERM);
	exit(0);
}

/***** le fils *****/
int recvd = -1;		// indique la valeur envoyée par le père
void get_zero () { recvd = 0; }
void get_one () { recvd = 1; }

void child ()
{
	int ppid = getppid();
	
	signal(SIGUSR1,get_zero);
	signal(SIGUSR2,get_one);

	printf("parent pid = %d, child pid = %d\n",ppid,getpid());
	while(1)
	{
		/* attendre la livraison d'un signal */
		pause();

		/* on affiche l'information reçue */
		if (recvd == 0) printf("got a 0\n");
		if (recvd == 1) printf("got a 1\n");
		recvd = -1;
	}
}

/***** programme principal *****/
int main ()
{
	int cpid = fork();
	if (cpid) parent(cpid);
	else child();
	return 0;
}
