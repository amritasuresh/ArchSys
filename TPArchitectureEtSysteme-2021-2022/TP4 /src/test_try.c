

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

bool is_correct(float response, float target) {
    /* Is student response within acceptable tolerance of target?

        - response : student's answer (string)
        - target  : instructor result (number)
    */
    const float TOLERANCE = 0.01;

    float val = response;
    float at_most = TOLERANCE * fmax(fabs(val), fabs(target));
    printf("Value %f\n", fabs(val) );
    printf("Target %f\n", fabs(target) );
    printf("At most %f\n", fmax(fabs(val), fabs(target)) );
    float fab = fabs(val - target);
    printf("%f\n", fab );
    return fabs(val - target) <= at_most;
}

bool is_correctWrong(const char *response, float target) {
    /* Is student response within acceptable tolerance of target?

        - response : student's answer (string)
        - target  : instructor result (number)
    */
    const float TOLERANCE = 0.01;

    float val = strtof(response, NULL);
    float at_most = TOLERANCE * fmax(fabs(val), fabs(target));
    printf("Value %f\n", fabs(val) );
    printf("Target %f\n", fabs(target) );
    printf("At most %f\n", fmax(fabs(val), fabs(target)) );
    float fab = fabs(val - target);
    printf("Val - Target %f\n", fab );
    return fabs(val - target) <= at_most;
}

// main method
int main ()
{
	float resp;
	printf("What is your guess?");
	//scanf("%f",&resp);
	float target = 23.1;
	char input[100];
	scanf("%s", input);
    //printf("%s", input);
   	

	//printf("What is the correct answer (to be entered by the admin)");
	//scanf("%f",&target);

	//bool isCorrect = is_correct(resp, target);
	//float g = 2-2;
	//float f = FLT_MIN;
	bool isCorrect = is_correctWrong(input, target);
	if(isCorrect!=false){
		printf("\nWell done\n");
	}
	else{
		printf("\nWrong answer\n");
	}
	
}
