// Combinaison de 1.3 et 1.4

#include <stdio.h>

void main ()
{
	unsigned int n;
	printf("Donnez-moi un entier n :\n");
	printf("n = "); scanf("%d",&n);

	// Avec 32 bits, le résultat rentre dans 6 bits maximum, 32=(100000)_2
	// Du coup, on peut faire la somme avec modulo dès que les blocs
	// contiennt au moins 6 positions.
	n = ((n & 0xaaaaaaaa) >> 1) + (n & 0x55555555);
	n = ((n & 0xcccccccc) >> 2) + (n & 0x33333333);
	n = ((n & 0xf0f0f0f0) >> 4) + (n & 0x0f0f0f0f);
	n = n % 255;

	printf("Résultat: c = %d\n",n);
}
