#include<stdlib.h>
#include<stdio.h>
#include <time.h>

#define SIZE 10000

int main(int argc, char *argv[]) {

  int **array = (int **) malloc(SIZE * sizeof(int *));

  for (unsigned int i=0; i < SIZE; i++)
    array[i] = (int *) malloc(SIZE * sizeof(int));
  int sum;
  srand(time(NULL));

  for (unsigned int x = 0 ; x < SIZE ; x++) {
    for (unsigned int y = 0 ; y < SIZE ; y++) {
      array[x][y]=rand()%2;
    }
  }

  sum = 0;
  clock_t start_1,end_1;
  start_1=clock();
  for (unsigned int x = 0 ; x < SIZE ; x++) {
    for (unsigned int y = 0 ; y < SIZE ; y++) {
      sum+=array[x][y];
    }
  }

  end_1=clock();

  sum = 0;
  clock_t start_2, end_2;
  start_2=clock();
  for (unsigned int y = 0 ; y < SIZE ; y++) {
    for (unsigned int x = 0 ; x < SIZE ; x++) {
      sum+=array[x][y];
    }
  }

  end_2=clock();
  printf("temps1: %f\n", (double) (end_1-start_1)/CLOCKS_PER_SEC);
  printf("temps2: %f\n", (double) (end_2-start_2)/CLOCKS_PER_SEC);
}
