/* utf8-coroutine.c - utf-8 parser coroutine
 *
 * Copyright © 2013 Eon S. Jeon <esjeon@live.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>

#define coBegin \
	static int coline = 0; switch(coline) { case 0:
#define coEnd \
	} coline = 0
#define coReturn(val) \
	do{ coline = __LINE__; return val; case __LINE__:; } while(0)
#define coReset(val) \
	do{ coline = 0; return val; } while(0)

int
utf8parse(unsigned char byte)
{
	static int wc = 0;
	static int len = 0;

	coBegin;

	// the first byte
	if (! (byte & 0x80)) // 0xxx xxxx
		coReset(byte);
	else if (! (byte & 0x40)) // 10xx xxxx (error)
		coReset('?');
	else if (! (byte & 0x20)) // 110x xxxx
	{
		wc = byte & 0x1f;
		len = 1;
	}
	else if (! (byte & 0x10)) // 1110 xxxx
	{
		wc = byte & 0x0f;
		len = 2;
	}
	else // 1111 0xxx
	{
		wc = byte & 0x07;
		len = 3;
	}

	// read extra bytes
	while(len--) {
		coReturn(-1);
		if ( (byte & 0xc0) != 0x80 ) /* 10xx xxxx */
			coReset('?');
		wc = (wc << 6) | (byte & 0x3f);
	}
	coReset(wc);

	coEnd;

	return -1;
}

#ifndef IMPORTING
int main()
{
	char hexc[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	unsigned char buf[256] = {0,};
	int i, j, len, ret, last;

	while(! feof(stdin)) {
		len = fread(buf, 1, 255, stdin);

		last = 0;
		for(i = 0; i < len; i++) {
			ret = utf8parse(buf[i]);
			if (ret >= 0) {
				printf("0x%04x = ", ret);
				for (j = last; j <= i; j ++)
					printf("%c", buf[j]);
				printf(" = ");
				for (j = last; j <= i; j ++)
					printf("%c%c ", hexc[buf[j] >> 4], hexc[buf[j] & 0xf]);
				printf("\n");
				last = i + 1;
			}
		}
	}
	return 0;
}
#endif
