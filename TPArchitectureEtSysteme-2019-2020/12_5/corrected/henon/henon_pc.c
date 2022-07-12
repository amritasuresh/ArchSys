#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define _bound 200

int _npts = 10;

// Producer consumer
double *pc_values;
int pc_wcursor = 0;
int pc_rcursor = 0;
sem_t pc_freespace;
sem_t pc_resource;

double _a = 1.4, _b = -0.3;


void compute_orbit() {
  double x, y;
  double xp = 0.5, yp = 0.5;
  for (int i = 1; i < _bound; i++) {
    x = _a - _b * yp -  xp * xp;
    // Assert there is free space in the queue
    if (sem_wait(&pc_freespace)) perror("semwait");
    // Put a value in the queue
    pc_values[pc_wcursor] = x;
    // Signal that there is a resource to read
    if (sem_post(&pc_resource)) perror("sempost");
    // Compute next slot in the queue
    pc_wcursor = (pc_wcursor + 1) % (2 * _npts);

    y = xp;
    if (sem_wait(&pc_freespace)) perror("semwait");
    pc_values[pc_wcursor] = y;
    pc_wcursor = (pc_wcursor + 1) % (2 * _npts);
    if (sem_post(&pc_resource)) perror("sempost");

    xp = x; yp = y;
  }
  pthread_exit(NULL);
}

void writer() {
    int fd;
    if ((fd = open("henon.dat", O_DSYNC | O_CREAT | O_RDWR | S_IRUSR)) == -1)
        perror("henon.dat");
    char buf[100];
    double x, y;
    for (int i = 1; i < _bound; i++) {
      // Wait for some resource to be available
      if (sem_wait(&pc_resource)) perror("wait for resource");
      // Read values
      x = pc_values[pc_rcursor];
      // Update read cursor
      pc_rcursor = (pc_rcursor + 1) % (2 * _npts);
      // Signal that there is more free space
      if (sem_post(&pc_freespace)) perror("sempost free space");

      if (sem_wait(&pc_resource)) perror("semwait");
      y = pc_values[pc_rcursor];
      pc_rcursor = (pc_rcursor + 1) % (2 * _npts);
      if (sem_post(&pc_freespace)) perror("sempost");

      sprintf(buf, "%+0.10lf %+0.10lf\n", x, y);
      if (write(fd, buf, sizeof(char) * strlen(buf)) == -1) perror("write");
    }
    close(fd);
    pthread_exit(NULL);
}

int main() {
  // All slots are free initially
  sem_init(&pc_freespace, PTHREAD_PROCESS_SHARED, 2 * _npts);
  sem_init(&pc_resource, PTHREAD_PROCESS_SHARED, 0);
  pc_values = malloc(2 * _npts * sizeof(double));
  pthread_t orbit_thr, writer_thr;
  pthread_create(&orbit_thr, NULL, (void *(*)(void*)) compute_orbit, NULL);
  pthread_create(&writer_thr, NULL, (void *(*)(void*)) writer, NULL);
  if (pthread_join(orbit_thr, NULL)) perror("join orbit");
  if (pthread_join(writer_thr, NULL)) perror("join writer");
  exit(0);
}
