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
        close(p[0]); //add the close(read-side) here
        while (1) {
            sleep(1);
            write(p[1], "b", 1);
        }
        close(p[1]); //add the close(write-side) here after we are done.
    } else {
        //Child process
        close(p[1]); //add the close(write-side) here
        char c;
        while (1) {
            read(p[0], &c, 1);
            write(STDOUT_FILENO, &c, 1);
        }
        close(p[0]); //add the close(read-side) here
    }
}
