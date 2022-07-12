#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int p[2];
    pipe(p);
    close(p[0]);

    if (fork()) {
        while (1) {
            sleep(1);
            write(p[1], "b", 1);
        }
    } else {
        char c;
        while (1) {
            read(p[0], &c, 1);
            write(STDOUT_FILENO, &c, 1);
        }
    }
}
