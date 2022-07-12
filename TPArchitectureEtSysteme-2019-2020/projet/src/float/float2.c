#include<stdlib.h>
#include<stdio.h>
#include<float.h>
int main(int argc, char* argv[]) {
  double y = FLT_MAX;
  if (y == y+1.) {
    printf("true\n");
  }
  else {
    printf("false\n");
  }

  return 0;
}
