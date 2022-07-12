#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
void forkexample()
{
// child process because return value zero

}
int main(){
	int a = 5;
	if (fork() == 0) {
		a = a + 5;
		printf("a = %d, &a = %d\n", a, &a);
	}
	else {
		a = a - 5;
		printf("a = %d, &a = %d\n", a, &a);
	}
return 0;
}