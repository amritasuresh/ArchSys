#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

int main(int argc, char *argv[]) {
  void *sp = NULL;
  int *hp = malloc(0);
  int *hp2 = malloc(4);
  printf("%p\n", (void *)&sp);
  printf("%p\n", (int *) hp);
  printf("%p\n", (int *) hp2);
  printf("%ld\n", (long int) (((intptr_t) &sp - (intptr_t) hp)/8));
  printf("%p\n", &main);
  return 0;
}
