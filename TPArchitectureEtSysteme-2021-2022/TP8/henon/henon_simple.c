#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define _bound 400
#define _a 1.25
#define _b 0.3
#define _x0 0.5
#define _y0 0.5

void gnuplot_print(int fd, double x, double y) {
    char c = ' ';
    write(fd, &x, sizeof(double));
    write(fd, &c, sizeof(char));
    write(fd, &y, sizeof(double));
    c = '\n';
    write(fd, &c, sizeof(char));
}

void c_xn(int bound, int p[], int q[]) {
    close(p[1]);
    close(q[0]);
    double xn = _x0, yn = _y0;
    for (int i = 1; i < bound; i++) {
        read(p[0], &yn, sizeof(double));
        xn = 1 - _a * xn * xn + yn;
        printf("x%d: %lf\n", i, xn);
        write(q[1], &xn, sizeof(double));
    }
    exit(0);
}

void c_yn(int bound, int p[], int q[]) {
    close(p[0]);
    close(q[1]);
    double xn = _x0, yn = _y0;
    // Start computing y1
    yn = _b * xn;
    write(p[1], &yn, sizeof(double));
    for (int i = 2; i < bound; i++) {
        sleep(1);
        read(q[0], &xn, sizeof(double));
        yn = _b * xn;
        printf("y%d: %lf\n", i, yn);
        write(p[1], &yn, sizeof(double));
    }
    exit(0);
}

int main() {
    int p[2];
    int q[2];
    if (pipe(p) == -1) perror("p pipe");
    if (pipe(q) == -1) perror("q pipe");
    // open("henon.plain", O_CREAT | O_WRONLY);
    if (!(fork())) c_xn(_bound, p, q);
    if (!(fork())) c_yn(_bound, p, q);
    wait(NULL);
    wait(NULL);
}
