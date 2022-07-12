/*
 * strcmp-target.c, A simple challenge that compares two strings
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  char nonsense1[] = "L4f$T80t8yNA~]zM^:d)$LvEQ+y0;$5xLLR9Nmki$oh.*KwAU?Qj?(:S";
  char word3[] = "ne ";
  char nonsense2[] = "#hKQp%fsyWpeb2+]1~Mo6AFXegkz/xHlp!F$@!,IzEmqXW5b/Shg4y";
  char word1[] = "Un ";
  char nonsense3[] = "t[r*zN@p[^%Ne+v{5s_A9r3)r7Ge}eI[}sD9U>QOJ+>{>!#Np";
  char word4[] = "vient ";
  char nonsense4[] = "~BsTND<yXn+fN/+;5|JPu+ANN>,d&_yz<E|TEUMU|#k";
  char word2[] = "malheur ";
  char nonsense5[] = "m|cKo'8PZToJzi=?5$gZ`!{3HezX*[A^2=yvf?zuNQ7fOlNX'w@$Y";
  char word6[] = "seul";
  char nonsense6[] = "i_:=6oc8oT1qo7-Pm#V7Ouz)ns(zWyLens`S^+f3(<KF]FAEWO,&'";
  char word5[] = "jamais ";
  char nonsense7[] = "T(@7Y{ojz2a10K)QNlq_6P&Q|;62X,=payg%8Xu`fEiW,pF5Dt5G!c>";

  char passwd[32] = "";

  strcat(passwd, word1);
  strcat(passwd, word2);
  strcat(passwd, word3);
  strcat(passwd, word4);
  strcat(passwd, word5);
  strcat(passwd, word6);
  
  if (argc < 2) {
    printf("Usage: %s <passphrase>\n", argv[0]);
    return 0;
  }
  
  if (!strcmp(passwd, argv[1])) {
    printf("Passphrase correct, come on in!\n");
    return 1;
  }
  
  printf("Wrong passphrase, go away!\n");
  return 0;
}
