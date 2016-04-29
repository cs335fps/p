//cs371 Fall 2014
//
//program: arm.c
//author:  Gordon Griesel
//date:    summer 2014
//
//This program demonstrates a robotic arm with lighting/shadows
//
//Depending on your Linux distribution,
//may have to install these packages:
// libx11-dev
// libglew1.6
// libglew1.6-dev
//
//
//
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
//#include "log.h"
#include "fonts.h"

#define rnd() (float)rand() / (float)RAND_MAX
#define PI 3.14159265358979323846264338327950
//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

void initXWindows(void);
void init_opengl(void);
//void init_textures(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
void check_keys(XEvent *e);
void physics(void);
void render(void);

int done=0;
int xres=640, yres=480;
float pos[3]={20.0,200.0,0.0};
float vel[3]={3.0,0.0,0.0};

int lesson_num=6;

float rtri = 0.0f;
float rquad = 0.0f;
float cubeRot[3]={2.0,0.0,0.0};
float cubeAng[3]={0.0,0.0,0.0};
GLfloat LightAmbient[]  = {  0.0f, 0.0f, 0.0f, 1.0f };
GLfloat LightDiffuse[]  = {  1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightSpecular[] = {  0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[] = { 100.0f, 40.0f, 40.0f, 1.0f };

int main(void)
{
	initXWindows();
	init_opengl();
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//init_textures();
	while(!done) {
		while(XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			check_mouse(&e);
			check_keys(&e);
		}
		physics();
		render();
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	return 0;
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "CS371 - Lab7 OpenGL Components");
}

void setup_screen_res(const int w, const int h)
{
	xres = w;
	yres = h;
}

void initXWindows(void)
{
	Window root;
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;

	setup_screen_res(640, 480);
	dpy = XOpenDisplay(NULL);
	if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);
	if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	//else {
	//	// %p creates hexadecimal output like in glxinfo
	//	printf("\n\tvisual %p selected\n", (void *)vi->visualid);
	//}
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
						StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
							vi->depth, InputOutput, vi->visual,
							CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
	//window has been resized.
	setup_screen_res(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	set_title();
}

void init_opengl(void)
{
	//OpenGL initialization
//	switch(lesson_num) {
//		case 0:
//		case 1:
//			glViewport(0, 0, xres, yres);
//			glDepthFunc(GL_LESS);
//			glDisable(GL_DEPTH_TEST);
//			//Initialize matrices
//			glMatrixMode(GL_PROJECTION); glLoadIdentity();
//			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
//			//This sets 2D mode (no perspective)
//			glOrtho(0, xres, 0, yres, -1, 1);
//			//Clear the screen
//			glClearColor(1.0, 1.0, 1.0, 1.0);
//			//glClear(GL_COLOR_BUFFER_BIT);
//			break;
//		case 3:
//			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//			glClearDepth(1.0);
//			glDepthFunc(GL_LESS);
//			glEnable(GL_DEPTH_TEST);
//			glShadeModel(GL_SMOOTH);
//			glMatrixMode(GL_PROJECTION);
//			glLoadIdentity();
//			gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);
//			glMatrixMode(GL_MODELVIEW);
//			break;
//		case 4:
//			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//			glClearDepth(1.0);
//			glDepthFunc(GL_LESS);
//			glEnable(GL_DEPTH_TEST);
//			glShadeModel(GL_SMOOTH);
//			glMatrixMode(GL_PROJECTION);
//			glLoadIdentity();
//			gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);
//			glMatrixMode(GL_MODELVIEW);
//			break;
//		case 5:
//		case 6:
//		case 7:
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClearDepth(1.0);
			glDepthFunc(GL_LESS);
			glEnable(GL_DEPTH_TEST);
			glShadeModel(GL_SMOOTH);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);
			glMatrixMode(GL_MODELVIEW);
			//Enable this so material colors are the same as vert colors.
			glEnable(GL_COLOR_MATERIAL);
			glEnable( GL_LIGHTING );
			glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
			glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);
			glEnable(GL_LIGHT0);
			break;
//	}
}

void check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != xres || xce.height != yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
	}
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

void check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		switch(key) {
//			case XK_1:
//				lesson_num = 1;
//				init_opengl();
//				break;
//			case XK_3:
//				lesson_num = 3;
//				init_opengl();
//				break;
//			case XK_4:
//				lesson_num = 4;
//				init_opengl();
//				break;
//			case XK_5:
//				lesson_num = 5;
//				init_opengl();
//				break;
//			case XK_6:
//				lesson_num = 6;
//				init_opengl();
//				break;
			case XK_Escape:
				done=1;
				break;
		}
	}
}

void physics(void)
{
	int addgrav = 1;
	//Update position
	pos[0] += vel[0];
	pos[1] += vel[1];
	//Check for collision with window edges
	if ((pos[0] < 0.0          && vel[0] < 0.0) ||
		(pos[0] >= (float)xres && vel[0] > 0.0)) {
		vel[0] = -vel[0];
		addgrav = 0;
	}
	if ((pos[1] < 0.0          && vel[1] < 0.0) ||
		(pos[1] >= (float)yres && vel[1] > 0.0)) {
		vel[1] = -vel[1];
		addgrav = 0;
	}
	//Gravity
	if (addgrav)
		vel[1] -= 0.4;
}

void LightedCube()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);

	glTranslatef(0.0f,0.0f,-7.0f);
	glRotatef(cubeAng[0],1.0f,0.0f,0.0f);
	glRotatef(cubeAng[1],0.0f,1.0f,0.0f);
	glRotatef(cubeAng[2],0.0f,0.0f,1.0f);

	glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_QUADS);
		//top
		//notice the normal being set
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		// bottom of cube
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
		// front of cube
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		// back of cube.
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		// left of cube
		glNormal3f( -1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		// Right of cube
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
		glEnd();
	glEnd();
	rquad -= 2.0f;
	int i;
	if (rnd() < 0.01) {
		for (i=0; i<3; i++) {
			cubeRot[i] = rnd() * 4.0 - 2.0;
		}
	}
	for (i=0; i<3; i++) {
		cubeAng[i] += cubeRot[i];
	}
}

void render(void)
{
	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
	//
	r.bot = yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00887766, "CS-371 Lab");
	//
	LightedCube();
}



