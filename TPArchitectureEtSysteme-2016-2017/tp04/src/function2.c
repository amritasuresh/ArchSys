#include <stdio.h>

void main ()
{
	int n,c;
	printf("Give me a number n\n");
	printf("n = "); scanf("%d",&n);
	for (c = 0; n != 0; n &= (n-1)) { c++; }
	printf("Result: c = %d\n",c);
}
