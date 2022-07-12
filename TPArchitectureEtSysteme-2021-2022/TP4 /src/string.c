#include <stdio.h>

int main()
{
    char input[100];
    printf("Type a String which will be converted to a double: ");
    scanf("%s", input);
    printf("%s", input);

    double number = 0.0;
    double divider = 1.0;
    int inFraction = 0;
    int i = 0;

    if (input[i] >= 48 && input[i] <= 57)
    {
        inFraction = 0;
        while ((input[i] >= '0' && input[i] <= '9') || input[i] == '.')
        {
            if (input[i] == '.')
            {
                i++;
                inFraction = 1;
                continue;
            }

            number = number * 10.0;
            number = number + input[i] - '0';
            i++;

            if (inFraction) divider *= 10.0;
        }

        number /= divider;

        printf("string %s -> number %g \n", input, number);
    }
    else
    {
        printf("Enter a number! \n");
    }
return 0;
}