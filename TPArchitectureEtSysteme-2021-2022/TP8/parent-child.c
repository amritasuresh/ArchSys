#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int p[2];
    pipe(p);
    //p[0] --> read side of the pipe
    //p[1] --> write side of the pipe

    //close(p[0]); mistake to close it before the fork

    int crea = fork();

    if(crea == -1){
        printf("Error forking process \n");
        return 1;
    }

    else if (crea) {
        //Parent process
        close(p[1]); //add the close(write-side) here
        int y;
        read(p[0], &y, sizeof(int));
        //printf("In parent process \n");
        printf("Parent says! Sum is %d\n", y);
        close(p[0]); //add the close(read-side) here after we are done.
    } else {
        //Child process
        //close(p[0]); //add the close(read-side) here
        int x,y;
        y =1;
        printf("Child process here! \n");
        printf("Enter 2 integers separated by a space \n");
        scanf("%d %d", &x, &y);
        int sum = x+y;
        //write (p[1], &sum, sizeof(int));
        //read(p[0], &y, sizeof(int));
        printf("Child says! Sum is %d\n", y);
        //close(p[0]);
        //close(p[1]); //add the close(read-side) here
    }
}
