// server.c - create a window and a glx context, and print their XIDs
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

#define NOFAIL(expr) \
	if(! (expr)) do { \
		fprintf(stderr, "FAIL %s:%4d\t" #expr "\n", __FILE__, __LINE__); \
		exit(1); \
	} while(0)

Display 	*dpy;
int		scr;
Window		root;
Window		win;
GLXContext	ctx;

int glx_attr_list[] = {
	GLX_RGBA,
	GLX_RED_SIZE, 8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE, 8,
	GLX_ALPHA_SIZE, 8,
	GLX_DEPTH_SIZE, 24,
	None
};


void
create_window ()
{
	XVisualInfo *vi;
	XSetWindowAttributes attr;

	NOFAIL(vi = glXChooseVisual(dpy, scr, glx_attr_list));

	ctx = glXCreateContext(dpy, vi, NULL, False);

	attr.background_pixel = 0;
	attr.colormap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	attr.event_mask = KeyPressMask;

	win = XCreateWindow (
		dpy, root, 0, 0, 300, 300, 0,
		vi->depth, InputOutput, vi->visual,
		CWBackPixel | CWColormap | CWEventMask,
		&attr);

	XMoveWindow(dpy, win, 500, 300);
	
	XMapWindow(dpy, win);
}

int
main ()
{
	NOFAIL (dpy = XOpenDisplay(NULL));
	scr = DefaultScreen(dpy);
	root = RootWindow(dpy, scr);

	create_window();

	{
		XID ctxid = glXGetContextIDEXT(ctx);
		printf("ctx %lx\nwin %lx\n", ctxid, win);
	}

	/* clear the window first */
	glXMakeCurrent(dpy, win, ctx);
	glClearColor(0, 0, 1.f, 1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
	glXMakeCurrent(dpy, None, None);

	for(;;) {
		XSync(dpy, False);
		usleep(100);
	}

	return 0;
}

