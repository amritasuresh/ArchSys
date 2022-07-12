#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/* Dans cette version, le fils accuse reception reception d'un signal en
   envoyant un signal au père (on a donc des signaux dans les deux sens).

   Ceci élimine la plupart des problèmes mais il en reste un dans le
   scénario suivant :
	1. le fils envoye un signal au père
	2. immédiatement, le système donne la main au père
	3. le père termine sa pause et envoye le prochain signal au fils,
		puis fait pause()
	4. le fils resume, en commençant avec la livraison du signal
		envoyé par le père, puis un appel pause()
	   Du coup, les processus sont tous les deux bloqués.
   Ce scénario intervient de façon non-déterministe.			*/

/***** le père *****/
int ready = 0;
void get_ready () { }

void parent (int cpid)
{
	char c;

	while (1)
	{
		// le fils nous envoye un signal quand il est prêt
		pause();

		for (c = 0; c != '0' && c != '1' && c != EOF; c = getchar());
		if (c == '0') kill(cpid,SIGUSR1);
		if (c == '1') kill(cpid,SIGUSR2);
		if (c == EOF) break;
	}
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
		// signaler au père qu'on est prêt
		kill(ppid,SIGUSR1);

		// attendre le prochain signal
		pause();

		if (recvd == 0) printf("got a 0\n");
		if (recvd == 1) printf("got a 1\n");
	}
}

/***** programme principal *****/
int main ()
{
	struct sigaction action;
	memset(&action,0,sizeof(action));
	action.sa_handler = get_ready;
	sigaction(SIGUSR1,&action,NULL);

	int cpid = fork();
	if (cpid) parent(cpid);
	else child();
	return 0;
}
