// client.c - draw on external window/context
// 
// Copyright © 2013 Eon S. Jeon <esjeon@live.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#include <GL/glxext.h>

#define TRY(expr) if (!(expr))

Display			*dpy;
GLXContext		glctx;

static void
die (char *msg)
{
	fprintf (stderr, "Error: %s\n", msg);
	exit (1);
}

int
main (int argc, char *argv[])
{
	XID wid;
	XID ctxid;

	if (argc != 3)
		die("Need 2 arguments: visual id, context id");

	wid = strtoul(argv[1], NULL, 16);
	fprintf(stderr, "Window ID = %lx\n", wid);

	ctxid = strtoul(argv[2], NULL, 16);
	fprintf(stderr, "Context ID = %lx\n", ctxid);

	dpy = XOpenDisplay(NULL);

	glctx = glXImportContextEXT(dpy, ctxid);

	while (1)
	{
		static unsigned int t = 0;

		glXMakeCurrent(dpy, wid, glctx);
		glClearColor(0, 0, .001f * t, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glXMakeCurrent(dpy, None, None);

		t = (t + 1) % 1000;
		usleep(40);
	}
	return 0;
}

