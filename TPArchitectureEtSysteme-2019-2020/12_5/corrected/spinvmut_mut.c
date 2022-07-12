#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

pthread_mutex_t resource;

void* stupid() {
  pthread_mutex_lock(&resource);
  sleep(4);
  pthread_mutex_unlock(&resource);
  pthread_exit(NULL);
}
void* wants() {
  pthread_mutex_lock(&resource);
  printf("At last!\n");
  pthread_mutex_unlock(&resource);
  pthread_exit(NULL);
}

int main() {
  pthread_mutex_init(&resource, NULL);
  pthread_t stupid_th, wants_th;
  pthread_create(&stupid_th, NULL, stupid, NULL);
  sleep(1);
  pthread_create(&wants_th, NULL, wants, NULL);
  pthread_join(wants_th, NULL);
  pthread_join(stupid_th, NULL);
  exit(0);
}
