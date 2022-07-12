#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char ** args)
{
	int x,y;

	printf ("Donnez un entier : ");
	scanf("%d",&x);
	printf ("Donnez un autre : ");
	scanf("%d",&y);

	if (fork())	// père
	{
		int s;
		// obtenir la somme du fils ... à compléter
		printf("Le resultat de l'addition est %d\n",s);

		exit(0); // terminer
	}

	else		// fils
	{
		int z = x+y;
		// communiquer le résultat au père par code de sortie
	}
}
