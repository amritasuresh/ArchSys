#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//======================================================//
//=================== bit operations ===================//
//======================================================//

/* returns bit of n at position pos */
int getBit(int n, int pos) {
  return (n>>pos)&1;
}

/* returns n with bit at position pos set to 1 */
int setBit(int n, int pos) { 
  return (n | (1 << pos)); 
}

/* returns n with bit at position pos set to 0 */
int clearBit(int n, int pos) {
  return (n & ~(1 << pos));
}

/* returns n with bit at position pos flipped */
int flipBit(int n, int pos) {
  return (n ^ (1 << pos));
}
