#include <stdio.h>
#include <string.h>

/*
  The hijacked puts function filters out "bad" strings.
*/

int puts(const char *s)
{
  
	if (!strcmp(s,"bad")) return 0;
	return printf("%s\n",s);
}
