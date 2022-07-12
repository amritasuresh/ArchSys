#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main ()
{
	int i;
	char* villes[] = { "Saint-Étienne", "Charleville-Mézières", "Paris" };
	char* postal[] = { "42000", "08000", "75000" };

	setlocale(LC_CTYPE,"fr_FR.UTF-8");
	for (i = 0; i < 3; i++)
		wprintf(L"%-22s %s\n",villes[i],postal[i]);
	return 0;
}

