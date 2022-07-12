#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/* Le programme suivant résout le problème de façon définitive :
   on remplace pause par sigsuspend. Ce dernier permet de mieux
   contrôler la livraison des signaux. Ici, ceux-ci ne peuvent
   intervenir que pendant un appel de sigsuspend ce qui élimine
   le problème précédent. */

static sigset_t newmask, zeromask;

/***** le père *****/

int ready = 0;
void get_ready () { ready = 1; }

void parent (int cpid)
{
	char c;

	/* Installer le gestionnaire pour que le fils
	   puisse signaler qu'il est prêt. */
	struct sigaction action;
	memset(&action,0,sizeof(action));
	action.sa_handler = get_ready;
	sigaction(SIGUSR1,&action,NULL);

	while (1)
	{
		/* On attend le signal du fils avant d'envoyer le prochain
		   signal. Notons que sigsuspend sera débloqué par n'importe
		   quel signal, alors on utilise ready pour assurer que c'est
		   bien le signal USR1 qui a été déclenché. */
		while (!ready) sigsuspend(&zeromask);
		ready = 0;

		/* On ignore les charactères autre que 0, 1 et EOF. */
		for (c = 0; c != '0' && c != '1' && c != EOF; c = getchar());
		if (c == '0') kill(cpid,SIGUSR1);
		if (c == '1') kill(cpid,SIGUSR2);
		if (c == EOF) break;
	}
	kill(cpid,SIGTERM);	/* terminer le fils */
	exit(0);
}

/***** le fils *****/

int recvd = -1;
void get_zero () { recvd = 0; }
void get_one () { recvd = 1; }

void child ()
{
	int ppid = getppid();

	/* installer les gestionnaires des deux signaux */
	struct sigaction action;
	memset(&action,0,sizeof(action));
	action.sa_handler = get_zero;
	sigaction(SIGUSR1,&action,NULL);
	action.sa_handler = get_one;
	sigaction(SIGUSR2,&action,NULL);

	printf("parent pid = %d, child pid = %d\n",ppid,getpid());
	while(1)
	{
		/* signaler au père qu'on est prêt */
		kill(ppid,SIGUSR1);

		/* le même principe que pour le père */
		while (recvd < 0) sigsuspend(&zeromask);
		if (recvd == 0) printf("got a 0\n");
		if (recvd == 1) printf("got a 1\n");
		recvd = -1;
	}
}

/***** programme principal *****/
int main ()
{
	/* Tout d'abord, avant le fork(), on va bloquer les deux signaux.
           Ce blocage sera hérité par le père et le fils, ce qui assure
	   que aucun signal n'est raté quand les processus démarrent. */
	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGUSR1);
	sigaddset(&newmask,SIGUSR2);
	sigprocmask(SIG_BLOCK, &newmask, NULL);

	/* Ensuite, on appelle fork. */
	int cpid = fork();
	if (cpid) parent(cpid);
	else child();
	return 0;
}
