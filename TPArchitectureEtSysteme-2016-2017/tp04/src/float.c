#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int sign;
	int exponent;
	int mantissa;
} fc;


fc decompose (float f)
{
	fc a;
	unsigned int i = *(int*)(&f);
	a.sign = (i & 0x8000000) >> 31;
	a.exponent = (i & 0x7f800000) >> 23;
	a.mantissa = i & 0x007fffff;
	return a;
}

float compose (fc a)
{
	unsigned int i = (a.sign << 31) | (a.exponent << 23) | a.mantissa;
	return *(float*)(&i);
}

fc add (fc a, fc b)
{
	fc c;
	if (a.sign || b.sign) 
	{
		fprintf(stderr,"error: negative sign\n");
		exit(1);
	}

	if (a.exponent < b.exponent) { c = a; a = b; b = c; }
	c.sign = 0; c.exponent = a.exponent;

	int ma = a.mantissa | 0x00800000, mb = b.mantissa | 0x00800000;
	int fk = a.exponent - b.exponent;
	if (fk > 24) fk = 24;
	mb = mb >> fk;
	int mc = ma + mb;

	if (mc & 0x01000000)
	{
		c.exponent++; mc = mc >> 1;
	}
	c.mantissa = mc & 0x007fffff;
	return c;
}

int main ()
{
	float f,g;
	fc a,b,c;
	printf("Give me a float: ");
	scanf("%f",&f);
	a = decompose(f);
	printf("sign = %d, exponent = %d (0x%x), mantissa = %d (0x%x)\n",
		a.sign, a.exponent, a.exponent, a.mantissa, a.mantissa);
	f = compose(a);
	printf("float value is %f\n",f);

	printf("Give me a second float: ");
	scanf("%f",&g);
	b = decompose(g);
	printf("sign = %d, exponent = %d (0x%x), mantissa = %d (0x%x)\n",
		b.sign, b.exponent, b.exponent, b.mantissa, b.mantissa);

	c = add(a,b);
	printf("\naddition:\n");
	printf("sign = %d, exponent = %d (0x%x), mantissa = %d (0x%x)\n",
		c.sign, c.exponent, c.exponent, c.mantissa, c.mantissa);
	printf("float value is %f\n",compose(c));
}
