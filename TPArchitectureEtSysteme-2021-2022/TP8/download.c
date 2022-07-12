#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int p[2];
    if (pipe(p) == -1) perror("Pipe creation failed");
    pid_t downloader = fork();
    if (downloader == 0) {
        close(p[0]);
        // We do not have to read from the pipe, so we close the read end
        dup2(p[1], STDOUT_FILENO);
        /* Replace the file descriptor STDOUT_FILENO with the file descriptor
         * p[1]. Hence, STDOUT_FILENO is linked to the write end of the pipe.
         * This way, any function that use STDOUT_FILENO, like printf, writes
         * the data to the pipe (instead of the terminal, by default). */
        close(p[1]);
        /* Since the file descriptor p[1] has been copied with dup2, the
         * write-end of the pipe has two file descriptors: p[1] and
         * STDOUT_FILENO. We close the one that won't be used, p[1]. */
        execlp("curl", "curl",
               "http://www.lsv.fr/~hondet/resources/archos/shell-bootstrap.tar.gz",
               NULL);
        /* execlp replaces the current image of the programme with another one.
         * In that case, the other programme is curl. The replacing programme
         * inherits its file descriptors from the parent process. In that case,
         * the STDOUT_FILENO of curl will hence be the write end of the created
         * pipe. */
    }
    pid_t untarer = fork();
    if (untarer == 0) {
        /* The logic is close to the one of the download process, except that
         * this process reads on the pipe. */
        close(p[1]);
        // We don't have to write on the pipe here.
        dup2(p[0], STDIN_FILENO);
        /* Copy read end of the pipe and replace stdin, so that subsequent
         * programmes read on the pipe. */
        close(p[0]);
        // Close the duplicated file descriptor
        execlp("tar", "tar", "xz", NULL);
    }
    return 0;
    /* The parent process doesn't have to wait for the termination of its
       children. The pipe created will exist as long as there are file
       descriptors open on one of its ends. */
}
