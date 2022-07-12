#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int sign;
	int exponent;
	int mantissa;
} fc;

// decompose a float into its components
fc decompose (float f)
{
	fc a;
	unsigned int i = *(int*)(&f);

	// the following three operations should be replaced!
	a.sign = 0;
	a.exponent = 0;
	a.mantissa = 0;

	return a;
}

// compose a float value from its components
float compose (fc a)
{
	unsigned int i;

	// the following operation should be replaced
	i = 0;

	return *(float*)(&i);
}

// implement floating-point addition
fc add (fc a, fc b)
{
	fc c;
	if (a.sign || b.sign) 
	{
		fprintf(stderr,"error: negative sign\n");
		exit(1);
	}

	if (a.exponent < b.exponent) { c = a; a = b; b = c; }

	// the following should be replaced
	return a;
}

// outputs the components of an fc structure
void output (fc a)
{
	printf("sign = %d, exponent = %d (0x%x), mantissa = %d (0x%x)\n",
		a.sign, a.exponent, a.exponent, a.mantissa, a.mantissa);
}

// example program that uses the functions above
int main ()
{
	float f,g;
	fc a,b,c;

	printf("Give me a float: ");
	scanf("%f",&f);
	a = decompose(f);
	output(a);
	g = compose(a);
	printf("float value is %f\n",g);

	printf("\nGive me a second float: ");
	scanf("%f",&g);
	b = decompose(g);
	output(b);

	printf("\nresult of addition:\n");
	c = add(a,b);
	output(c);
	printf("float value is %f\n",compose(c));
}
