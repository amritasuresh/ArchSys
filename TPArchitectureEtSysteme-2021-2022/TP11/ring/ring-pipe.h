#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>

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
void create_node (int id, int readfd, int writefd)
{
	srand48(time(NULL) + id);

	node.id = id;
	node.fd_in = readfd;
	node.fd_out = writefd;

	// on branche des stream sur les descripteurs
	node.chan_in = fdopen(node.fd_in,"r"); 
	node.chan_out = fdopen(node.fd_out,"w"); 

	// mettre les stream en mode non-tamponné (économise des fflush)
	setlinebuf(node.chan_in); setlinebuf(node.chan_out);

	protocole(id);
}

// Générer une permutation de 1..n
int* create_perm (int n)
{
	int i, j, *p = malloc(n*sizeof(int));
	for (i = 0; i < n; i++) p[i] = i;

	for (j = 0; j < i*i; i++)
	{
		int k = (int)(drand48()*n);
		int l = (int)(drand48()*n);
		int tmp = p[k]; p[k] = p[l]; p[l] = tmp;
	}
	return p;
}

// Générer les n nœuds
void genere_noeuds (int n)
{
	int i, *perm;
	int pp[2], nextread, lastwrite;

	srand48(time(NULL));
	perm = create_perm(n);

	// On génère les n nœuds avec leur tubes
	pipe(pp);
	lastwrite = pp[1];
	nextread = pp[0];

	printf("Ring : ");
	for (i = 0; i < n; i++) printf("%d - ",perm[i]);
	printf("%d\n",perm[0]);

	for (i = 0; i < n-1; i++)
	{
		pipe(pp);
		if (!fork())
			create_node(perm[i],nextread,pp[1]);
		close(nextread);
		close(pp[1]);
		nextread = pp[0];
	}
	if (!fork()) create_node(perm[i],nextread,lastwrite);
}
