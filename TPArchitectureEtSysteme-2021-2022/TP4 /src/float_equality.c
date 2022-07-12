
#include <stdio.h>
#include <stdlib.h>
#include<float.h>

int main(int argc, char *argv[]) {
    double a = 0.1;
    double b = 0.2;
    double c = 0.3;
    float overflow = FLT_MAX + 1;

    printf("\n");
    if (a + b == c) {
        printf("0.1 + 0.2 == 0.3\n");
    } else {
        printf("0.1 + 0.2 != 0.3 ??\n");
    }
    //printf("\n");
    //printf("result of a + b:\t %.18f\n", a + b);
    //printf("float c = 0.3;\t\t %.18f\n", c);
    printf("float min = %f \n", overflow);

    c = 0;

    overflow = a/c;
    printf("divide by zero = %f \n", overflow);
    b = overflow + a;
    printf("adding infinity = %f \n", b);

    b = c/c;
    printf("zero by zero = %f \n", b);

    a = overflow + b;
    printf("adding infinity to NaN = %f \n", a);



    
    return 0;
}