#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
	int x,y;

	printf ("Donnez un entier : ");
	scanf("%d",&x);
	printf ("Donnez un autre : ");
	scanf("%d",&y);

	if (fork()) {
		pid_t s = 0;
		// obtenir la somme du fils ... à compléter
		wait(&s);
		printf("Le resultat de l'addition est %d\n", WEXITSTATUS(s));

		exit(0); // terminer
	} else {
		// Child
		int z = x+y;
		exit(z);
		// communiquer le résultat au père par code de sortie
	}
}
