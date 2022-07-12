// Solution

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int sign;
	int exponent;
	int mantissa;
} fc;

//decompose a float into its components
fc decompose (float f)
{
	fc a;
	unsigned int i = *(int*)(&f);
	// solution
	a.sign = (i & 0x8000000) >> 31;
	a.exponent = (i & 0x7f800000) >> 23;
	a.mantissa = i & 0x007fffff;

	return a;
}

// composer un float depuis ses composants
float compose (fc a)
{
	unsigned int i;

	// solution
	i = (a.sign << 31) | (a.exponent << 23) | a.mantissa;

	return *(float*)(&i);
}

// cette fonction devrait réaliser l'addition virgule flottante,
// sous l'hypothèse que les opérands sont tous les deux positifs
fc addition (fc a, fc b)
{
	fc c;
	if (a.sign || b.sign) 
	{
		fprintf(stderr,"erreur: signe négatif\n");
		exit(1);
	}

	// on assure que l'exposant de a est le plus grand
	if (a.exponent < b.exponent) { c = a; a = b; b = c; }

	// solution :
	c.sign = 0; c.exponent = a.exponent;

	// on rajoute le "1 implicit"
	int ma = a.mantissa | 0x00800000, mb = b.mantissa | 0x00800000;

	// on limite le décalage à 24 (sinon ça sera dec mod 32)
	int dec = a.exponent - b.exponent;
	if (dec > 24) dec = 24;

	// calcul de la mantisse
	int mc = ma + (mb >> dec);

	// normaliser en cas de débordement
	if (mc & 0x01000000) { c.exponent++; mc = mc >> 1; }
	c.mantissa = mc & 0x007fffff;

	return c;
}

// afficher les composants d'une structure
void affiche (fc a)
{
	printf("signe = %d, exposant = %d (0x%x), mantisse = %d (0x%x)\n",
		a.sign, a.exponent, a.exponent, a.mantissa, a.mantissa);
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