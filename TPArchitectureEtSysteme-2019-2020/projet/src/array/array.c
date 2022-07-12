#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define SIZE 900000

int main(int argc, char* argv[]) {
  int data[SIZE] = {0};

  srand(time(NULL));

  for (unsigned int c = 0 ; c < SIZE ; c++) {
    data[c]=rand()%512;
  }

  clock_t temps;
  int sum = 0;
  for (unsigned int c = 0 ; c < SIZE ; c++) {
    if(data[c] > 256) {
      sum += data[c];
    }
  }
  temps=clock();
  printf("%f\n", (double) temps/CLOCKS_PER_SEC);

  return 0;
}
