#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#define _a 1.4
#define _b (-0.3)
#define _x0 0.5
#define _y0 0.5

int bound = 100;

void c_xn(int p[], int q[], int wp[]) {
    close(p[1]);
    close(q[0]);
    close(wp[0]);
    double xn = _x0, yn = _y0;
    for (int i = 1; i < bound; i++) {
        xn = _a - _b * yn - xn * xn;
        write(q[1], &xn, sizeof(double));
        read(p[0], &yn, sizeof(double));
        write(wp[1], &xn, sizeof(double));
    }
    exit(0);
}

void c_yn(int p[], int q[], int wp[]) {
    close(p[0]);
    close(q[1]);
    close(wp[0]);
    double xn = _x0, yn = _y0;
    for (int i = 1; i < bound; i++) {
        sleep(1);
        printf("\r%d", i);
        fflush(stdout);
        yn = xn;
        // Sending to colleague
        write(p[1], &yn, sizeof(double));
        // Update x
        read(q[0], &xn, sizeof(double));
        // Sending to writer
        write(wp[1], &yn, sizeof(double));
    }
    exit(0);
}

void writer(int px[], int py[]) {
    close(px[1]);
    close(py[1]);
    double xn, yn;
    int fd;
    if ((fd = open("henon.dat", O_DSYNC | O_CREAT | O_RDWR | S_IRUSR)) == -1)
        perror("henon.dat");
    char buf[20];
    while (read(px[0], &xn, sizeof(double)) &
           read(py[0], &yn, sizeof(double))) {
        printf("%+0.4lf %+0.4lf\n", xn, yn);
        sprintf(buf, "%lf %lf\n", xn, yn);
        if (write(fd, buf, strlen(buf)) == -1) perror("write");
    }
    close(fd);
    exit(0);
}

int main() {
    int px[2], py[2];
    pipe(px);
    pipe(py);
    int p[2], q[2];
    if (pipe(p) == -1) perror("p pipe");
    if (pipe(q) == -1) perror("q pipe");
    if (!fork()) writer(px, py);
    if (!fork()) c_xn(p, q, px);
    if (!fork()) c_yn(p, q, py);
    wait(NULL);
    wait(NULL);
    // Close writers so that writer receives a SIGPIPE
    close(px[1]);
    close(py[1]);
    wait(NULL);
    exit(0);
}
