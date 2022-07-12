// Ce programme représente la solution de la Question 2.

#include <stdio.h>

#define BITS 8	// ou 16

typedef unsigned long long ulong;
typedef signed char schar;
typedef signed short sshort;

void print_binary (char *prefix, ulong x, int bits)
{
	ulong b = 1 << (bits-1);
	printf("%s",prefix);
	while (b)
	{
		printf("%c",(x & b)? '1' : '0');
		b = b >> 1;
	}
	printf("\n");
}

ulong bin_to_int (char *s)
{
	ulong result = 0;
	while (*s)
	{
		result = result << 1;
		if (*s++ == '1') result++;
	}
	return result;
}

int find_bit8 (schar x)
{
	int tab[8] = {0,1,2,4,7,3,6,5};
	ulong s = bin_to_int("0001011100");
	return tab[((s * (x & -x)) >> 7) & 7];
}

int find_bit16 (sshort x)
{
	int tab[16] = {0,1,2,5,3,9,6,11,15,4,8,10,14,7,13,12};
	ulong s = bin_to_int("0000100110101111000");
	return tab[((s * (x & -x)) >> 15) & 15];
}

#define GLUE(x,y) find_bit ## y (x)
#define EVAL(x,y) GLUE(x,y)
#define find(x) EVAL(x,BITS)

int main ()
{
	signed int x;
	printf("Donnez-moi un entier (%d bits) : ",BITS);
	scanf("%d",&x);
	print_binary("x=",x,BITS);
	printf("l(x)=%d\n",find(x));
}
