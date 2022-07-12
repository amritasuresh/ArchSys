#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#define _seq_len 4

int count = 0;

void display(int s) {
    if (s == SIGUSR1) {
        printf("1");
    } else {
        printf("0");
    }
}

void increment(int s) {
    if (s == SIGUSR1)
        count++;
}

int main(void) {
    short seq[_seq_len] = { 0, 1, 1, 0 };
    pid_t pid = fork();
    if (pid == 0) {
        // Child
        pid_t father = getppid();
        signal(SIGUSR1, display);
        signal(SIGUSR2, display);
        for (;;) {
            pause();
            fflush(stdout);
            kill(father, SIGUSR1);
        }
    } else {
        // Father
        sleep(3);
        while (count < _seq_len) {
            signal(SIGUSR1, increment);
            if (seq[count]) {
                kill(pid, SIGUSR1);
            } else {
                kill(pid, SIGUSR2);
            }
            pause();
        }
        kill(pid, SIGTERM);
    }
    return 0;
}
