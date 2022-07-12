#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>

#define POP_B 10
float _baboon_repro = 0.5;
float _cheetah_attack = 0.4;
int period = 1;

char* shm_name = "/baboons";
int shm_flags = O_CREAT | O_RDWR;
mode_t shm_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
size_t shm_size = POP_B * sizeof(pid_t);
int mmap_flags = PROT_READ | PROT_WRITE;

char* sem_pop_name = "/baboonremain";
mode_t sem_pop_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

char* sem_name = "/memwritesem";
mode_t sem_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
unsigned int sem_val = 1;

// Returns 1 with probability [p] and 0 with probability 1 - [p].
int unif(float p) {
  return ((float) rand()) / ((float) RAND_MAX) < p;
}

// Add [newborn] to the end of [mem]. If [mem] is full, nothing is done.
void fill_mem(pid_t newborn, pid_t* mem, sem_t* sem) {
  // Get first available index
  int i = 0;
  while (i < POP_B && *(mem + i) == 0) i++;
  if (i == POP_B - 1) return;
  sem_wait(sem);
  mem[i] = newborn;
  sem_post(sem);
}

// Removes [dead] from memory [mem]. Does nothing if not found.
void clear_mem(pid_t dead, pid_t* mem, sem_t* sem) {
  // Find index of dead
  int i = 0;
  for (i = 0; i < POP_B; i++) {
    if (mem[i] == dead) {
      sem_wait(sem);
      mem[i] = 0;
      sem_post(sem);
      return;
    }
  }
}

pid_t pick_mem(pid_t* baboons) {
  return baboons[rand() % POP_B];
}

// Triggered when a baboon is attacked
void kill_baboon(int signo) {
  // Get shared memory
  int shd = shm_open(shm_name, shm_flags, shm_mode);
  pid_t* baboons =
    mmap(NULL, shm_size, mmap_flags, MAP_SHARED, shd, 0);
  // Get semaphore (don't try to create it)
  sem_t *sem_baboon = sem_open(sem_name, O_RDWR);
  sem_t *sem_pop = sem_open(sem_pop_name, O_RDWR);
  if (baboons == MAP_FAILED) perror("mmap baboon");

  pid_t meal = getpid();
  printf("Killing %d\n", meal);
  clear_mem(meal, baboons, sem_baboon);
  sem_post(sem_pop);
  return;
}

void baboon_birth() {
  // Get shared memory
  int shd = shm_open(shm_name, shm_flags, shm_mode);
  pid_t* baboons =
    mmap(NULL, shm_size, mmap_flags, MAP_SHARED, shd, 0);
  // Get semaphore (don't try to create it)
  sem_t *sem_baboon = sem_open(sem_name, O_RDWR);
  sem_t *sem_pop = sem_open(sem_pop_name, O_RDWR);
  if (sem_pop == SEM_FAILED) perror("sem");
  if (baboons == MAP_FAILED) perror("mmap baboon");

  sigset_t sig_mask;
  sigemptyset(&sig_mask);
  sigaddset(&sig_mask, SIGTERM);
  struct sigaction under_attack;
  under_attack.sa_handler = kill_baboon;
  sigaction(SIGTERM, &under_attack, NULL);

  for (;;) {
    if (unif(_baboon_repro)) {
      sem_wait(sem_pop);
      pid_t baboon = fork();
      if (!baboon) {
        baboon_birth();
      } else {
        printf("Baboon %d is born.\n", baboon);
        fill_mem(baboon, baboons, sem_baboon);
      }
    } else {
    }
    sleep(period);
  }
}

void cheetah_birth() {
  printf("Cheetah entered the chat\n");
  int shd = shm_open(shm_name, shm_flags, shm_mode);
  pid_t* baboons =
    mmap(NULL, shm_size, mmap_flags, MAP_SHARED, shd, 0);
  if (baboons == MAP_FAILED) perror("mmap baboon");
  for (;;) {
    if (unif(_cheetah_attack)) {
      pid_t meal = pick_mem(baboons);
      printf("Attacking %d\n", meal);
      kill(meal, SIGTERM);
    }
    sleep(period);
  }
}

int main() {
  // Initialise prng
  srand(time(NULL));

  int shd = shm_open(shm_name, shm_flags, shm_mode);
  ftruncate(shd, shm_size); // Initialised to 0
  pid_t* baboons =
    mmap(NULL, shm_size, PROT_READ | PROT_WRITE | PROT_EXEC,
         MAP_SHARED,
         shd, 0);
  if (baboons == MAP_FAILED) perror("mmap");
  sem_t* sem_baboon = sem_open(sem_name, O_CREAT | O_RDWR, sem_mode, sem_val);

  sem_t* sem_baboon_pop =
    sem_open(sem_pop_name, O_CREAT | O_RDWR, sem_pop_mode, POP_B);
  if (sem_baboon_pop == SEM_FAILED) perror("sem failed");

  // Initialisation value does not work properly
  for (int i = 0; i < POP_B; i++) {
    sem_post(sem_baboon_pop);
  }
  sem_post(sem_baboon);
  //
  pid_t baboon = fork();
  if (!baboon) {
    baboon_birth();
  } else {
    fill_mem(baboon, baboons, sem_baboon);
    printf("baboon %d is born\n", baboons[0]);
  }
  if (!fork()) {
    cheetah_birth();
  }
  printf("Cheetah born\n");
  return 0;
}
