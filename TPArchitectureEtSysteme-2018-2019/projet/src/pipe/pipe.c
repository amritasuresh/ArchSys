#include<unistd.h>
#include<stdio.h>

int main (int argc, char *argv[]) {
  int p[2];
  pipe(p);
  close(p[0]);

  if (fork()) {
    while (1) {
      sleep(1);
      write(p[1],"a",1);
    }
   } else {
     char c = 'a';
     while (1) {
       read(p[0],&c,1);
       write(1,&c,1);
     }
   }
   return 0;
}
