#include<stdio.h>
#include<unistd.h>

int main (void) {
 // setbuf(stdout,NULL); // Disable buffer to printf
 while(1){
  printf("Hello ");
  write(1, "World ", 6);
 }
 return 0;
}
