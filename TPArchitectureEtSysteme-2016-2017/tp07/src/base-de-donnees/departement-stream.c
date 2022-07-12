#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int first[2],second[2];

/* Ce programme utilise deux tubes et deux processus (père et fils) :
 
 Le père accepte les saisies de l'utlisateur (un numéro de département)
 et fabrique une requête SQL qu'il envoye vers le fils. Ensuite il
 récupère la réponse.

 Le fils lance une instance de sqlite (base de données), ayant redirigé
 ses entrées et sorties.

 On utilise deux tubes, first et second.
 first est utilisé pour envoyer des données du père au fils.
 second est utilisé dans l'autre sens.  */

// Code du père qui communique avec la base de données

void pere ()
{
	char *buffer, *nom, *cheflieu;

	// On crée des FILE sur les descripteur ce qui permet d'utiliser
	// fprintf et fscanf.
	FILE *dbout = fdopen(second[0],"r");
	FILE *dbin = fdopen(first[1],"w");
	setlinebuf(dbin);	// pour flush automatique apres newline

	// on ferme les descripteurs inutiles
	close(first[0]);
	close(second[1]);

	// Avec "timer ON", chaque réponse du serveur BDD sera terminé par
	// une ligne avec des statistiques. On ne s'intéresse pas aux
	// statistiques, mais cela assure que la réponse n'est jamais vide...
	fprintf(dbin,".timer ON\n");

	while (1)
	{
		// On accepte un numéro de département.
		printf("Numéro de département: "); fflush(stdout);
		scanf("%ms",&buffer);

		// Si la réponse est "xx", on termine. La mort du père entraine
		// le fermeture de ses descripteurs ; la base de données
		// remarquera ce fait et terminera aussi.
		if (!strcmp(buffer,"xx")) return;

		// Envoyer la requête au fils.
		fprintf(dbin,"SELECT nom,cheflieu FROM dep WHERE nr='%s';\n",
				buffer);
                free(buffer);

		// On obtient la réponse pour en extraire les données utiles.
		// Si la première ligne commence avec "Run Time", c'est parce
		// que la recherche n'a rien donné.

		// obtenir une ligne complète
		fscanf(dbout," %m[^\n]",&buffer);
		if (!strncmp(buffer,"Run Time:",9))
		{
			printf("Ce département n'existe pas !\n\n");
			free(buffer);
			continue;
		}

		// La réponse du serveur est de la forme nom|cheflieu
		sscanf(buffer,"%m[^|]|%ms",&nom,&cheflieu);

		// Afficher la réponse
		printf("Le nom du département est %s.\n",nom);
		printf("Son chef-lieu est %s.\n\n",cheflieu);
		free(buffer); free(nom); free(cheflieu);

		// Consommer la ligne "Run Time"
		fscanf(dbout," %m[^\n]",&buffer); free(buffer);
	}
}

// Le fils qui lance la base de données
void fils ()
{
	char *args[] = { "sqlite3", "dep.sqlite", NULL };
	dup2(first[0],0);	// connecter les entrées/sorties standards
	dup2(second[1],1);	// avec les tubes
	close(second[0]); close(second[1]);	// fermer les descripteurs
	close(first[0]); close(first[1]);	// devenus inutiles
	execvp("sqlite3",args);	// lancer sqlite
}

// procédure principale
int main ()
{
	// créer les deux tubes
	pipe(first);
	pipe(second);

	// lancer père et fils
	if (fork()) pere();
	else fils();

	return 0;
}
