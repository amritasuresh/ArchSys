#include<stdio.h>

void printb(int n) {
  for(int i = 7 ; i >= 0 ; i--) {
    printf("%d", (n & (1 << i)) >> i);
  }
}


int removeLSOne(int n) {
  return n & (n - 1);
}

int countOnes(int n) {
  int c;
  for (c = 0; n != 0; n &= (n-1)) c++;
  return c;
}

int oneCounter(int n) {
  return ((n & 0xaaaaaaaa) >> 1) + (n & 0x55555555);
}
int main(int argc, char* argv[]) {
  for(int i = 240 ; i < 256 ; i++) {
    printb(i);
    printf(":");
    printb(oneCounter(i));
    printf("\n");
  }
  return 0;
}
