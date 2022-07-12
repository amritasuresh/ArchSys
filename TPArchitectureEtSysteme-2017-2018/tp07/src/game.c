#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
  srand(time(NULL));
  int secret = rand() % 100;
  int guess = 101;

  printf ("Guess my number between 0 and 99!\n\n");

  while (scanf ("%d", &guess) == 1 && guess != secret) {
    if (guess > secret) { printf ("Lower!\n"); }
    else { printf ("Higher!\n"); }
  }

  if (guess == secret) { printf ("Well done!\n"); }

  return 0;
}
