
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
  int parentPid=getpid();
  printf("Bonjour,\n");
  fork();
  printf("J’ai fait un ’fork’ !\n");
  

	if (getpid() == parentPid) {
        char mychar[500];
        sprintf(mychar, "pstree -p -U %d", parentPid);
        system(mychar);
    }
  return 0;
}