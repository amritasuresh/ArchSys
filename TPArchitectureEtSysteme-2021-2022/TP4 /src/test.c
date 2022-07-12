#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

bool is_correct(const char *response, float target) {
    /* Is student response within acceptable tolerance of target?

        - response : student's answer (string)
        - target  : instructor result (number)
    */
    const float TOLERANCE = 0.01;

    float val = strtof(response, NULL);
    float at_most = TOLERANCE * fmax(fabs(val), fabs(target));
    return fabs(val - target) <= at_most;
}

// main method
int main ()
{
    //Input a single guess that could give you the right answer no matter what the target value
	printf("What is your guess (for any possible target solution)?");
    char input[100];
    scanf("%s", input);
    //Some targets hardcoded
	float target[5] = {23.1, 200.7, -30, 0, 1.414};
	
    int i;
    bool isCorrect = false;
    
    for(i=0; i<5; i++){
        isCorrect = is_correct(input, target[i]);
        if(isCorrect==false){
            printf("Wrong answer!\n");
            break;
        }
    
    }
    if(isCorrect!=false){
        printf("Well done!\n");
    }
	
	
}
