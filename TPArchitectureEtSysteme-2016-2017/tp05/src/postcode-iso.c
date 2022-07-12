#include <stdio.h>

int main ()
{
	int i;
	char* villes[] = { "Saint-Étienne", "Charleville-Mézières", "Paris" };
	char* postal[] = { "42000", "08000", "75000" };

	for (i = 0; i < 3; i++)
		printf("%-22s %s\n",villes[i],postal[i]);
	return 0;
}
