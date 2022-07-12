#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define SIZE 100000

int comp (const void * elem1, const void * elem2)
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int main(int argc, char* argv[]) {
  int data[SIZE];
  int data2[SIZE];

  srand(time(NULL));
  for (unsigned int c = 0 ; c < SIZE ; c++) {
    data[c]=rand()%512;
    data2[c]=rand()%512;
  }

  qsort (data2, sizeof(data2)/sizeof(*data2), sizeof(*data2), comp);

  int sum = 0;
  clock_t start_1, end_1;
  start_1=clock();
  for(unsigned int i = 0 ; i < 10000 ; i++) {
    for (unsigned int c = 0 ; c < SIZE ; c++) {
      if(data[c] < 256) {
	sum += data[c];
      }
    }
  }
  end_1=clock();

  sum=0;
  clock_t start_2, end_2;
  start_2=clock();
  for(unsigned int i = 0 ; i < 10000 ; i++) {
    for (unsigned int c = 0 ; c < SIZE ; c++) {
      if(data2[c] < 256) {
	sum += data2[c];
      }
    }
  }
  end_2=clock();

  printf("not sorted: %f\n", (double) (end_1-start_1)/CLOCKS_PER_SEC);
  printf("sorted: %f\n", (double) (end_2-start_2)/CLOCKS_PER_SEC);

  return 0;
}
