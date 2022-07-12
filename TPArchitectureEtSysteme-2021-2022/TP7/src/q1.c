#include <stdio.h>
#include <sys/types.h>
#include<unistd.h>
#define BOUND 5

int main()
{
	int i=0;
	for(i=0; i<BOUND; i++){
		fork();
	}

	//fork();
	//fork();
	//fork();
	printf("Hello World\n");
	return 0;
}