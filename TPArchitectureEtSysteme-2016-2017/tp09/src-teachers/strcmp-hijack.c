#include <stdio.h>
#include <string.h>

/*
  The hijacked strcmp function always returns that strings are equal.
*/

int strcmp(const char *s1, const char *s2) {
  printf("%s & %s\n", s1, s2);
  return 0;
}
