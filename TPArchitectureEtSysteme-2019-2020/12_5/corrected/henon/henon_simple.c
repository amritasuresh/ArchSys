#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define _bound 100

// Create mutexes that indicate there is a resource available.
pthread_mutex_t xrsrc;
pthread_mutex_t yrsrc;

// Parameters
double _a = 1.4, _b = -0.3;
double x, y;
double xp = 0.5, yp = 0.5;

void printer() {
  printf("%+0.10lf %+0.10lf\n", x, y);
}

void c_xn(int bound) {
  for (int i = 1; i < bound; i++) {
    // Enclose computation in mutexes
    pthread_mutex_lock(&yrsrc);
    x = _a - _b * yp - xp * xp;
    pthread_mutex_unlock(&xrsrc);
  }
  exit(0);
}

void c_yn(int bound) {
  for (int i = 1; i < bound; i++) {
    // Enclose computation in mutexes
    pthread_mutex_lock(&xrsrc);
    y = xp;
    pthread_mutex_unlock(&yrsrc);
    xp = x; yp = y;
    printer();
  }
  exit(0);
}

int main() {
  // Setting mutexes
  pthread_mutex_init(&xrsrc, NULL);
  pthread_mutex_init(&yrsrc, NULL);
  // Lock one of two mutexes that will be unlocked by a thread
  pthread_mutex_lock(&xrsrc);

  // Creating threads
  pthread_t xn_thr, yn_thr;
  pthread_create(&yn_thr, NULL, (void *(*)(void*)) c_yn, (void*) _bound);
  pthread_create(&xn_thr, NULL, (void *(*)(void*)) c_xn, (void*) _bound);

  // Wait for termination
  pthread_join(xn_thr, NULL);
  pthread_join(yn_thr, NULL);

  exit(0);
}
