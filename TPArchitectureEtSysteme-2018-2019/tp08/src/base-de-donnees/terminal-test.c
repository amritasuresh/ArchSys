#include <stdio.h>
#include <unistd.h>

// Tester si stdin/stdout sont connectés à un terminal.
// C'est comme ça que sqlite3 sait de ne pas afficher 
// son prompt interactif lors qu'il est connecté à un pipe.

int main()
{
	printf("stdin is a %s\n",isatty(0)? "terminal" : "file");
	printf("stdout is a %s\n",isatty(1)? "terminal" : "file");
	return 0;
}
