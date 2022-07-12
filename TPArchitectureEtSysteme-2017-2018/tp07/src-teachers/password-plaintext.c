/*
 * strcmp-target.c, A simple challenge that compares two strings
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  char passwd[] = "Un malheur ne vient jamais seul";
  
  if (argc < 2) {
    printf("Usage: %s <passwordphrase>\n", argv[0]);
    return 0;
  }
  
  if (!strcmp(passwd, argv[1])) {
    printf("Passphrase correct, come on in!\n");
    return 1;
  }
  
  printf("Wrong passphrase, go away!\n");
  return 0;
}
