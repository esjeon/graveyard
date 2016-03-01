/* wcwidth-driver.c - print the width of each character
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

#define _XOPEN_SOURCE

#include <stdio.h>
#include <wchar.h>

#define IMPORTING
#include "utf8-coroutine.c"
#undef IMPORTING

int main() {
	char str[10];
	int i, c, u, w;

	for(;;) {
		i = 0;
		do {
			c = getchar();
			if(c == EOF)
				goto bye;
			str[i++] = c;
		} while((u = utf8parse((unsigned char)c)) == -1);
		str[i] = 0;

		w = wcwidth(u);
		printf("%s : %d\n", str, w);
	}

bye:
	return 0;
}
