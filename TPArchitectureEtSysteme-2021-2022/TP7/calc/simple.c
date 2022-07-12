#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char ** args)
{
	int x,y;

	printf ("Give an integer : ");
	scanf("%d",&x);
	printf ("Give another : ");
	scanf("%d",&y);

	if (fork())	// parent process
	{
		int s;
		wait(&s);
		// get the sum from the child process
		printf("The result of the addition is %d\n", s);
		printf("The result of the addition is %d\n",WEXITSTATUS(s));

		exit(0); // exit
	}

	else		// child process
	{
		int z = x+y;
		exit(z);
		// communicate the result to the parent process,
	}
}
