#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[]) {
  double step1 = 1/2.;
  double step2 = 1/10.;
  double sum1, sum2 = 0;
  printf("step1: %f\n", step1);
  printf("step2: %f\n", step2);
  for(int i = 0 ; i < 50 ; i++) {
    sum1 += step1;
    sum2 += step2;
  }
  printf("sum1: %f\n", sum1);
  printf("sum2: %f\n", sum2);

  return 0;
}
