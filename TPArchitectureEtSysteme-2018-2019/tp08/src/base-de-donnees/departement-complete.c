#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/* Ce programme utilise deux tubes et deux processus (père et fils) :
 
 Le père accepte les saisies de l'utlisateur (un numéro de département)
 et fabrique une requête SQL qu'il envoye vers le fils. Ensuite il
 récupère la réponse.

 Le fils lance une instance de sqlite (base de données), ayant redirigé
 ses entrées et sorties.

 On utilise deux tubes. Le premier sera utilisé pour envoyer des données
 du père au fils, le deuxième dans l'autre sens.
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

int first[2],second[2];		// les deux tubes

void pere ()
{
	char *buffer, *nom, *cheflieu;

	// Fermer les descripteurs de tubes inutiles
	close(first[0]);
	close(second[1]);

	while (1)
	{
		// On accepte un numéro de département. La mémoire sera
		// alloué automatiquement.
		printf("Numéro de département: "); fflush(stdout);
		scanf("%ms",&buffer);

		// Si la réponse est "xx", on termine. La mort du père entraine
		// le fermeture de ses descripteurs ; la base de données
		// remarquera ce fait et terminera elle aussi.
		if (!strcmp(buffer,"xx")) return;

		// Sinon, on fabrique la bonne requête SQL.
		char *req = fabrique_requete(buffer);
		free(buffer);

		// Envoyer la requête au fils.
		write(first[1],req,strlen(req));

		free(req);

		// Obtenir la réponse du fils et la mettre dans buffer.
		// La solution suivante est acceptable dans le contexte de
		// cette exercice mais présente deux inconvenients :
		// - On alloue 100 octets sachant que dans le cas présent la
		//   réponse ne sera jamais plus longue que ça. Pourtant cette
		//   hypothèse n'est pas justifié dans d'autres contextes.
		// - Si le département n'existe pas, la réponse de Sqlite3
		//   est vide et read bloque.
		// Voir departement-stream.c pour une solution à ces deux
		// problèmes.
		buffer = malloc(100);
		read(second[0],buffer,100);

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

	dup2(first[0],0);	// connecter les entrées/sorties standards
	dup2(second[1],1);	// avec les tubes
	close(second[0]); close(second[1]);	// fermer les descripteurs
	close(first[0]); close(first[1]);	// devenus inutiles

	execvp("sqlite3",args);	// lancer sqlite
}

// procédure principale
int main ()
{
	// code pour créer les tubes
	pipe(first);
	pipe(second);

	// lancer père et fils
	if (fork()) pere();
	else fils();

	return 0;
}
