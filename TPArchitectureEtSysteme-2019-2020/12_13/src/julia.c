#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

complex double
inverse(complex double c, complex double z) {
  complex double re = creal(z - c);
  complex double im = cimag(z - c);
  complex double mod = sqrt(sqrt(re * re + im * im));
  complex double arg = (double) re > 0 ? atan(im/re) : M_PI + atan(im/re);
  arg = arg / 2 + (rand() % 2 * M_PI);
  return mod * (cos(arg) + _Complex_I * sin(arg));
}

int main() {
  complex double z = 1 + 2 * _Complex_I;
  complex double c = -0.4 - 0.6 * _Complex_I;
  unsigned int i0 = 50;
  unsigned int bound = 15000;
  for (unsigned int i = 0; i < bound; i++) {
    if (i >= i0) printf("%lf %lf\n", creal(z), cimag(z));
    z = inverse(c, z);
  }
  return 0;
}
