#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/* On vous propose l'architecture suivante :
 
 Le père accepte les saisies de l'utlisateur (un numéro de département)
 et fabrique une requête SQL qu'il envoye vers le fils. Ensuite il
 récupère la réponse.

 Le fils lance une instance de sqlite (base de données), ayant redirigé
 ses entrées et sorties.

 On utilise deux tubes. Le premier sera utilisé pour envoyer des données
 du père au fils, le deuxième dans l'autre sens.

 Il vous reste à remplir les trous (...) qui sont les opérations sur les
 tubes (pipe, dup2, read, write, close).
*/

// Code du père qui communique avec la base de données

// On fabrique la requête SQL pour obtenir les données d'un département
char* fabrique_requete (char *numero)
{
	char *modele = "SELECT nom,cheflieu FROM dep WHERE nr='%s';\n";
	char *r = malloc(strlen(modele) + strlen(numero) - 1);
	sprintf(r,modele,numero);
	return r;
}

// La réponse du serveur est de la forme nom|cheflieu\n
// On utilise sscanf pour extraire les données qu'on veut.
// La mémoire pour nom et cheflieu est alloué automatiquement par sscanf.
void decouper_reponse (char *buffer, char **nom, char **cheflieu)
{
	sscanf(buffer,"%m[^|]|%ms",nom,cheflieu);
}

void pere ()
{
	char *buffer, *nom, *cheflieu;

	// Fermer les descripteurs de tubes inutiles
	// ...

	while (1)
	{
		// On accepte un numéro de département. La mémoire sera
		// alloué automatiquement.
		printf("Numéro de département: "); fflush(stdout);
		scanf("%ms",&buffer);

		// Si la réponse est "xx", on termine.
		if (!strcmp(buffer,"xx")) return;

		// Sinon, on fabrique la bonne requête SQL.
		char *req = fabrique_requete(buffer);
		free(buffer);

		// Envoyer la requête au fils.
		// ...

		free(req);

		// Obtenir la réponse du fils et la mettre dans buffer.
		// Pour faire simple, on a droit de supposer que celle-ci
		// contient au plus 100 octets.
		// ...

		// Extraire nom et cheflieu de la réponse.
		decouper_reponse(buffer,&nom,&cheflieu);

		// Afficher la réponse
		printf("Le nom du département est %s.\n",nom);
		printf("Son chef-lieu est %s.\n\n",cheflieu);
		free(buffer); free(nom); free(cheflieu);
	}
}

// Le fils qui lance la base de données
void fils ()
{
	char *args[] = { "sqlite3", "dep.sqlite", NULL };

	// faire les bonnes manipulations pour connecter l'entrée/sortie
	// du fils aux tubes

	// ...

	execvp("sqlite3",args);	// lancer sqlite
}

// procédure principale
int main ()
{
	// code pour créer les tubes
	// ...

	// lancer père et fils
	if (fork()) pere();
	else fils();

	return 0;
}
