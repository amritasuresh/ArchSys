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

    if (fork()) {
        //Parent process
        close(p[1]); //parent closes writing end of pipe
        int sum;

        read(p[0], &sum, sizeof(int));
        printf("Parent says sum is %d \n", sum);

        close(p[0]);
        
        //close(p[0]); //add the close(read-side) here
    } else {
        //Child process
        close(p[0]); //closes reading end
        int x,y;

        printf("Enter two integers separated by a space \n");
        scanf("%d %d", &x, &y);

        int sum = x+y;

        write(p[1], &sum, sizeof(int));

        close(p[1]);
    }
}
