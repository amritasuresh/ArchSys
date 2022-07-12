#include <stdio.h>
#include <stdlib.h>

// write the UTF8 encoding of code point cp into buf and return buf
char* utf8 (int cp)
{
	char *buf = malloc(5);

	buf[0] = buf[1] = buf[2] = buf[3] = buf[4] = 0;
	if (cp < 0x80) {
		buf[0] = cp;
	} else if (cp <= 0x7FF) {
		buf[0] = ((cp >> 6) + 0xC0);
		buf[1] = ((cp & 0x3F) + 0x80);
	} else if (cp <= 0xFFFF) {
		buf[0] = ((cp >> 12) + 0xE0);
		buf[1] = (((cp >> 6) & 0x3F) + 0x80);
		buf[2] = ((cp & 0x3F) + 0x80);
	} else if (cp <= 0x10FFFF) {
		buf[0] = ((cp >> 18) + 0xF0);
		buf[1] = (((cp >> 12) & 0x3F) + 0x80);
		buf[2] = (((cp >> 6) & 0x3F) + 0x80);
		buf[3] = ((cp & 0x3F) + 0x80);
	} else {
		fprintf(stderr,"invalid code_point %d\n",cp);
	}
	return buf;
}

int main ()
{
	printf("\n  (%s%s%s%s%s%s)\n\n",
		utf8(0x361), utf8(0xb0),
		utf8(0x35c), utf8(0x296),
		utf8(0x361), utf8(0xb0));
	printf("     %s %s  o%s  \n\n",utf8(0x488),utf8(0x488),utf8(0x489));
}
