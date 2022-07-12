// The program for the solution of Question 2.8.

#include <stdio.h>

#define BITS 8

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
	int tab[8] = {0,1,2,4,7,3,6,5}; //Hashtable with indices for 8 bits
	ulong s = bin_to_int("0001011100"); //De Bruijn sequence for 8 bits
	return tab[((s * (x & -x)) >> 5) & 7]; //Finds the corresponding index for LSB
}

#define GLUE(x,y) find_bit ## y (x)
#define EVAL(x,y) GLUE(x,y)
#define find(x) EVAL(x,BITS)

int main ()
{
	signed int x;
	printf("Give me an integer (%d bits) : ",BITS);
	scanf("%d",&x);
	print_binary("x=",x,BITS);
	printf("l(x)=%d\n",find(x));
}
