// Le programme correspondant à la Question 1.2.

#include <stdio.h>

void main ()
{
	unsigned int n,c;
	printf("Donnez-moi un entier n :\n");
	printf("n = "); scanf("%d",&n);

	for (c = 0; n != 0; n &= (n-1)) { c++; }
	printf("Résultat: c = %d\n",c);
}
