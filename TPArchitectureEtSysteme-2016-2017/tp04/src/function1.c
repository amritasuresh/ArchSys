#include <stdio.h>

void main ()
{
	int x,y;
	printf("Give me two numbers x and y\n");
	printf("x = "); scanf("%d",&x);
	printf("y = "); scanf("%d",&y);
	x ^= y;
	y ^= x;
	x ^= y;
	printf("Now x = %d and y = %d\n",x,y);
}
