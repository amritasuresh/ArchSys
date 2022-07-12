#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* Pingpong : le père communique les '0' et '1' dans la console au fils
	en lui envoyant des signaux. On souhaite que le fils affiche tous
	les '0' et '1' rencontrés. Ctrl+D termine le programme.		    */

/* Une première solution, très simple: Le parent envoie des signaux au fils
   qui attend dans une boucle while. Le grand inconvenient de cette solution
   est la consommation du processeur (à 100%, en raison de l'attente actif
   du fils). */

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
		/* attente active (consommation de CPU énorme) */
		while (recvd < 0);

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
