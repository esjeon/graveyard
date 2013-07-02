/* dumper.c - show hexdump of a file
 * 
 * Copyright © 2013 Eon S. Jeon <esjeon@live.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#define BTW(a,n,b) ((a) <= (n) && (n) <= (b))

#define MAXCOL 40

void
dump_buffer (int size, int *buf) {
	int i;

	if (size == 0) return;

	for (i = 0; i < size; i ++) {
		printf ("%02X", buf[i]);
		if ((i+1) % 2 == 0)
			printf (" ");
	}
	printf ("\n");

	for (i = 0; i < size; i ++) {
		if (buf[i] == ' ')
			printf (" !");
		else if ( BTW(' ', buf[i], '~') )
			printf ("%c ", buf[i]);
		else
			printf ("  ");

		if ((i+1) % 2 == 0)
			printf (" ");
	}
	printf ("\n");
}

void
usage (name)
	char * name;
{
	fprintf (stderr, "Usage: %s file-name [10 <= width <= %d]\n", name, MAXCOL);
}

int
main (argc, argv)
	int argc;
	char *argv[];
{
	FILE *fp;
	int buf[MAXCOL];
	int col, user_col;
	int c;

	user_col = 20;

	switch (argc) {
		case 3:
			{
				char *end;
				user_col = strtol(argv[2], &end, 10);
				if (end == argv[2] || user_col < 10 || user_col > MAXCOL) {
					usage (argv[0]);
					return 1;
				}
			}
		case 2:
			fp = fopen (argv[1], "r");
			if (! fp) {
				fprintf (stderr, "Failed to open the input file");
				return 1;
			}
			break;
		default:
			usage (argv[0]);
			return 0;
	}

	flockfile (fp);
	col = 0;

	while (1) {
		c = fgetc_unlocked (fp);
		if (c == EOF)
			break;

		buf[col++] = c;

		if (col == user_col) {
			dump_buffer (col, buf);
			printf ("\n");
			col = 0;
		}
	}

	dump_buffer (col, buf);

	funlockfile (fp);

	return 0;
}
