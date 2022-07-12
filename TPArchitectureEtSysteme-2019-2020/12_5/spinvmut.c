#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

pthread_spinlock_t resource;

void* stupid() {
  sleep(4);
  pthread_exit(NULL);
}
void* wants() {
  printf("At last!\n");
  pthread_exit(NULL);
}

int main() {
  pthread_spin_init(&resource, PTHREAD_PROCESS_SHARED);
  pthread_t stupid_th, wants_th;
  pthread_create(&stupid_th, NULL, stupid, NULL);
  sleep(1);
  pthread_create(&wants_th, NULL, wants, NULL);
  pthread_join(wants_th, NULL);
  pthread_join(stupid_th, NULL);
  exit(0);
}
