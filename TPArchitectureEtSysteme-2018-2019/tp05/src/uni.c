#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;	// 32 bits
typedef unsigned short ushort;	// 16 bits
typedef unsigned char uchar;	// 8 bits

#define BUFFER_SIZE 16384
#define BUFFER_MAX (BUFFER_SIZE * 3/4)
#define BUFFER_MIN (BUFFER_SIZE / 4)
#define INVALID 0xffff

int option_kompat = 0;
int option_ordering = 1;
int option_decompose = 1;
int option_compose = 0;
int option_hex = 0;

int eof = 0;
int has_starter = 0;
int lstart = 0;

#include "normalize.h"

uint get_index (uint cp)
{
	uint high = cp >> 16;
	uint middle = (cp >> 8) & 0xff;
	uint low = cp & 0xff;
	return p3[p2[p1[high]*256+middle]*256+low];
}

uint get_class (uint cp)
{
	// Obtain the Combining Class of a code point.
	return get_index(cp) >> 24;
}

uchar nextchar ()
{
	// Obtain the next byte from input (or accept end-of-file).
	uchar c;
	int r = read(0,&c,1);
	if (!r) eof = 1;
	return c;
}

uchar cont ()
{
	// Obtain a UTF-8 continuation byte. Fails in case of invalid encoding.
	uchar c = nextchar();
	assert(!eof);
	assert((c & 0xc0) == 0x80);
	return c & 0x3f;
}

// Buffering policy: We fill the buffer up to a certain size (3/4 of maximal
// content), then empty it to 1/4, or completely when all is done. That way,
// canonical ordering and recomposition can generally assume that the entire
// relevant environment of a starter is in the buffer together, at least
// unless somebody decides to put some 4000 accents behind some letter.

uint buffer[BUFFER_SIZE];
uint buffer_in = 0;
uint buffer_out = 0;

// Move to next position in buffer.
void augment (uint *u)
{
	*u = (*u + 1) % BUFFER_SIZE;
}

// Move back in buffer to previous non-deleted position.
uint backup (uint u)
{
	do { u = (u - 1) % BUFFER_SIZE; } while (buffer[u] == INVALID);
	return u;
}

// Advance to next undeleted position in buffer.
uint advance (uint u)
{
	do { u = (u + 1) % BUFFER_SIZE; } while (buffer[u] == INVALID);
	return u;
}

void buffer_push (uint cp)
{
	assert(cp != INVALID);
	buffer[buffer_in] = cp;
	augment(&buffer_in);
	assert(buffer_in != buffer_out);
}

uint buffer_pop ()
{
	assert(buffer_in != buffer_out);
	uint cp = buffer[buffer_out];
	augment(&buffer_out);
	return cp;
}

uint buffer_size ()
{
	uint dist = (buffer_in - buffer_out) % BUFFER_SIZE;
	return dist;
}

void move_back (uint cp, uint old_pos)
{
	// Move a code point (possibly a diacritic mark) from the end of the
	// buffer to its canonical position. If cp is a starter, it won't move.
	uint cca = get_class(cp);
	if (cca == 0) return;

	uint pos = old_pos;	// pos now points to where cp was inserted.

	// Move back as long as previous symbols have higher Combining Class.
	while (pos != buffer_out)
	{
		pos = backup(pos);
		uint ocp = buffer[pos];
		if (get_class(ocp) <= cca) return;
		buffer[(pos+1) % BUFFER_SIZE] = ocp;
		buffer[pos] = cp;
	}
}

uint replacement (uint first, uint last)
{
	uint idx = dpl[first & 0xfff] + (last & 0x3f);
	if (primfst[idx] != first || primlst[idx] != last) return INVALID;
	return primid[idx];
}

void compose (uint cp, uint old_pos)
{
	// Apply Canonical Composition on the code points between one
	// starter and the next (including the latter).

	// As long as we haven't arrived at the next starter yet, the ordering
	// of the content remains unstable, so we defer the composition.
	uint cca = get_class(cp);
	if (cca > 0) return;

	// If this is the first starter, there's nothing to do, just remember
	// its position.
	if (!has_starter) { lstart = old_pos; has_starter = 1; return; }

	uint prev = lstart;		// possibly blocking character
	uint next = advance(prev);	// possible diacritic character

	while (1)
	{
		uint ccp = get_class(buffer[prev]);
		uint ccn = get_class(buffer[next]);
		if (prev == lstart || ccp < ccn)
		{	// If next isn't blocked from lstart, try to replace.
			uint rp = replacement(buffer[lstart],buffer[next]);
			if (rp != INVALID)
			{
				buffer[lstart] = rp;
				buffer[next] = INVALID;	// delete position
			}
		}
		else prev = next;

		// Get out when we've arrived at the next starter, after trying
		// to combine it with the previous.
		if (next == old_pos) break;
		next = advance(next);
	}

	// Memorize this as the next starter, unless it has been composed.
	if (buffer[old_pos] != INVALID) lstart = old_pos;
}

void buffer_insert (uint cp)
{
	// Insert a decomposed code point at last position in buffer.
	uint old_pos = buffer_in;
	buffer_push(cp);

	// Then move it back in accordance with Canonical Ordering.
	if (option_ordering) move_back(cp,old_pos);

	// Re-compose with Primary Composites.
	if (option_compose) compose(cp,old_pos);
}

char* encode_utf8 (uint cp)
{
	// Computes the UTF-8 encoding of a code point. Returns pointer to
	// static buffer that will be overwritten on next usage.

	static uchar buf[5];

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
		assert(!"invalid code point");
	}
	return (char*)buf;
}

void buffer_flush (uint target)
{
	// Empty the buffer up to a given size.
	while (buffer_size() > target)
	{
		uint cp = buffer_pop();
		if (cp == INVALID) continue;
		if (option_hex) printf("%04x ",cp);
		if (!option_hex) fputs(encode_utf8(cp),stdout);
	}
	if (option_hex) printf("\n");
}

uint decode_utf16 (ushort first, ushort last)
{
	return (((first & 0x3ff) + 0x40) << 10) + (last & 0x3ff);
}

void code_push (uint cp)
{
	// Take a code point, decompose it recursively.
	uint idx = get_index(cp);
	uint len = (idx >> 16) & 0x3f;

	// If there is nothing to decompose (or we don't want to),
	// then this code point is final.
	if (len == 0 || !option_decompose) { buffer_insert(cp); return; }

	// Take compatibility decomposition into account if the user wants.
	uint cpt = (idx & (1<<22));
	if (cpt && !option_kompat) { buffer_insert(cp); return; }

	// Iterate over the decomposition. The length is in terms of code
	// points, each of which corresponds to one or two UTF-16 code units.
	uint pos = (idx & 0xffff);
	for (; len > 0; len--)
	{
		uint first = txt[pos++];
		if ((first & 0xf800) == 0xd800)
		{
			uint last = txt[pos++];
			code_push(decode_utf16(first,last));
		}
		else
			code_push(first);
	}
}

void decode_utf8 (uchar c)
{
	// Obtain the next UTF-8-encoded code point from input for processing.
	// Invalid encodings are caught by assertions.

	uint cp;
	assert((c & 0xc0) != 0x80);
	assert((c & 0xf8) != 0xf8);

	if (c < 0x80)
		code_push(c);
	else if ((c & 0xe0) == 0xc0)
	{
		cp = (c & 0x1f) << 6;
		code_push(cp + cont());
	}
	else if ((c & 0xf0) == 0xe0)
	{
		cp = (c & 0x0f) << 12;
		cp += cont() << 6;
		code_push(cp + cont());
	}
	else if ((c & 0xf8) == 0xf0)
	{
		cp = (c & 0x07) << 18;
		cp += cont() << 12;
		cp += cont() << 6;
		code_push(cp + cont());
	}
}

void read_input ()
{
	while (1)
	{
		uchar c = nextchar();
		if (eof) break;

		decode_utf8(c);
		if (buffer_size() > BUFFER_MAX) buffer_flush(BUFFER_MIN);
	}
	buffer_flush(0);
}

void usage ()
{
	fprintf(stderr,
"Reads UTF-8 stream from standard input, performs Unicode normalization,\n"
"and prints resulting UTF-8 stream to standard output. Options:\n\n"

"-c : produce composed forms\n"
"-k : apply compatibility decompositions\n"
"-x : print result in hexadecimal form instead\n"
"-D : (non-standard) do not decompose code points in input\n"
"-O : (non-standard) do not perform canonical ordering\n\n"

"When no option is given, produces NFD. For NFC, use -c. For NFKD/NFKC, add -k."
"\nWith -x -D -O, produces an unprocessed hexdump of the UTF-8 code points.\n"
);

	exit(1);
}

int main (int argc, char **argv)
{
	assert(sizeof(uint) == 4);
	assert(sizeof(ushort) == 2);
	assert(sizeof(uchar) == 1);

	int i;
	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i],"-k")) option_kompat = 1;
		else if (!strcmp(argv[i],"-c")) option_compose = 1;
		else if (!strcmp(argv[i],"-x")) option_hex = 1;
		else if (!strcmp(argv[i],"-O")) option_ordering = 0;
		else if (!strcmp(argv[i],"-D")) option_decompose = 0;
		else if (!strcmp(argv[i],"-?")) usage();
		else assert(!"invalid argument");
	}

	read_input();
}
