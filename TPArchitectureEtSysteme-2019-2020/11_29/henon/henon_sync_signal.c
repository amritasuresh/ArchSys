#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

// Pipes and signals for synchronisation

#define _bound 100
double _a = 1.4;
double _b = -0.3;
double _x0 = 0.5;
double _y0 = 0.5;

sigset_t mask;

void c_xn(int bound, int p[], int q[]) {
    close(p[1]);
    close(q[0]);
    double xp = _x0, yp = _y0;
    double xn, yn;
    /* Reading the pid of the process computing yn */
    pid_t y_pid;
    /* We don't really need this variable, but sigwait expects a pointer. */
    int recvd;
    if (!(read(p[0], &y_pid, sizeof(pid_t)))) perror("reading yn pid");
    for (int i = 1; i < bound; i++) {
        // Wait for it...
        sigwait(&mask, &recvd);
        // Get data
        read(p[0], &yn, sizeof(double));
        xn = _a - _b * yp - xp * xp;
        // Send data
        write(q[1], &xn, sizeof(double));
        // Signal a data has been sent
        kill(y_pid, SIGUSR1);
        // Update everything
        xp = xn, yp = yn;
    }
    exit(0);
}

void c_yn(int bound, int p[], int q[]) {
    close(p[0]);
    close(q[1]);
    double xp = _x0, yp = _y0, xn, yn;
    pid_t x_pid;
    int recvd;
    if (!(read(q[0], &x_pid, sizeof(pid_t)))) perror("reading xn pid");
    // Start computing y1
    yn = xp;
    write(p[1], &yn, sizeof(double));
    kill(x_pid, SIGUSR1);
    yp = yn;
    for (int i = 2; i < bound; i++) {
        sigwait(&mask, &recvd);
        read(q[0], &xn, sizeof(double));
        // Print to standard out here (why not)
        printf("%lf %lf\n", xp, yp);
        yn = xp;
        write(p[1], &yn, sizeof(double));
        kill(x_pid, SIGUSR1); // Anounce there is a value to read
        xp = xn, yp = yn;
    }
    exit(0);
}

int main() {
    /* Setting up signals */
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    int p[2];
    int q[2];
    if (pipe(p) == -1) perror("p pipe");
    if (pipe(q) == -1) perror("q pipe");
    pid_t xproc, yproc;
    if (!(xproc = fork())) c_xn(_bound, p, q);
    if (!(yproc = fork())) c_yn(_bound, p, q);
    /* Sending pids to the processes so that they can communicate with each
     * other. */
    write(q[1], &xproc, sizeof(pid_t));
    write(p[1], &yproc, sizeof(pid_t));
    close(p[1]); close(q[1]); close(p[0]); close(q[0]);
    wait(&xproc);
    wait(&yproc);
    return 0;
}
