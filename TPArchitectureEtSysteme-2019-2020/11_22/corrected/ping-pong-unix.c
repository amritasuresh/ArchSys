#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define _seq_len 4

int recvd = -1;
int ready = 0;

void display(int s) {
    if (s == SIGUSR1) {
        printf("1");
    } else {
        printf("0");
    }
}
void get_one() { recvd = 1; }
void get_zero() { recvd = 0; }
void get_ready() { ready = 1; }

int main(void) {
    short seq[_seq_len] = { 0, 1, 1, 0 };
    sigset_t zeromask, mask;
    sigemptyset(&zeromask);
    sigemptyset(&mask);

    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    pid_t pid = fork();
    if (pid == 0) {
        // Child
        struct sigaction act_one, act_zero;
        act_one.sa_handler = get_one;
        sigaction(SIGUSR1, &act_one, NULL);
        act_zero.sa_handler = get_zero;
        sigaction(SIGUSR2, &act_zero, NULL);
        pid_t father = getppid();
        printf("Ready\n");

        for (;;) {
            kill(father, SIGUSR1);
            while (recvd < 0) sigsuspend(&zeromask);
            printf("Got %d\n", recvd);
            recvd = -1;
            fflush(stdout);
        }
    } else {
        // Father
        // sleep(3);
        struct sigaction act;
        act.sa_handler = get_ready;
        sigaction(SIGUSR1, &act, NULL);

        printf("Ready\n");
        int count = 0;
        while (count < _seq_len) {
            while (!ready) sigsuspend(&zeromask);
            ready = 0;
            sleep(1);
            if (seq[count]) {
                kill(pid, SIGUSR1);
            } else {
                kill(pid, SIGUSR2);
            }
            count++;
            printf("Sent\n");
        }
        kill(pid, SIGTERM);
    }
    exit(0);
}
