// Ce programme correspond à la Question 3 (à compléter).

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int signe;
	int exposant;
	int mantisse;
} fc;

// décomposer un float dans ses composants
fc decompose (float f)
{
	fc a;
	unsigned int i = *(int*)(&f);

	// remplacer les trois instructions suivantes
	a.signe = 0;
	a.exposant = 0;
	a.mantisse = 0;

	return a;
}

// composer un float depuis ses composants
float compose (fc a)
{
	unsigned int i;

	// remplacer l'instruction suivante
	i = 0;

	return *(float*)(&i);
}

// cette fonction devrait réaliser l'addition virgule flottante,
// sous l'hypothèse que les opérands sont tous les deux positifs
fc addition (fc a, fc b)
{
	fc c;
	if (a.signe || b.signe) 
	{
		fprintf(stderr,"erreur: signe négatif\n");
		exit(1);
	}

	// on assure que l'exposant de a est le plus grand
	if (a.exposant < b.exposant) { c = a; a = b; b = c; }

	// insérer les bonnes instructions pour le calcul (on écrit dans c)
	// ...

	return c;
}

// afficher les composants d'une structure
void affiche (fc a)
{
	printf("signe = %d, exposant = %d (0x%x), mantisse = %d (0x%x)\n",
		a.signe, a.exposant, a.exposant, a.mantisse, a.mantisse);
}

// méthode principale
int main ()
{
	float f,g;
	fc a,b,c;

	printf("Donnez-moi une valeur : ");
	scanf("%f",&f);
	a = decompose(f);
	affiche(a);
	g = compose(a);
	printf("float value is %f\n",g);

	printf("\nEt une deuxième : ");
	scanf("%f",&g);
	b = decompose(g);
	affiche(b);

	printf("\nRésultat de l'addition addition:\n");
	c = addition(a,b);
	affiche(c);
	printf("ce qui représente %f\n",compose(c));
}
