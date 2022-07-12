// Le programme correspondant à la Question 1.3.

#include <stdio.h>

void main ()
{
	unsigned int n;
	printf("Donnez-moi un entier n :\n");
	printf("n = "); scanf("%d",&n);

	n = ((n & 0xaaaaaaaa) >> 1) + (n & 0x55555555);
	n = ((n & 0xcccccccc) >> 2) + (n & 0x33333333);
	n = ((n & 0xf0f0f0f0) >> 4) + (n & 0x0f0f0f0f);
	n = ((n & 0xff00ff00) >> 8) + (n & 0x00ff00ff);
	n = ((n & 0xffff0000) >> 16) + (n & 0x0000ffff);

	printf("Résultat: c = %d\n",n);
}
