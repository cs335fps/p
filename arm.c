//cs371 Fall 2014
//
//program: arm.cpp
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
#include "log.h"
#include "fonts.h"

typedef float Flt;
typedef Flt Vec[3];
typedef Flt	Matrix[4][4];
//some defined macros
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecNegate(a)	(a)[0]=(-(a)[0]); (a)[1]=(-(a)[1]); (a)[2]=(-(a)[2]);
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecLen(a)	((a)[0]*(a)[0]+(a)[1]*(a)[1]+(a)[2]*(a)[2])
#define VecLenSq(a)	sqrtf((a)[0]*(a)[0]+(a)[1]*(a)[1]+(a)[2]*(a)[2])
#define VecCopy(a,b)	(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];
#define VecAdd(a,b,c) (c)[0]=(a)[0]+(b)[0]; (c)[1]=(a)[1]+(b)[1]; (c)[2]=(a)[2]+(b)[2]
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; (c)[1]=(a)[1]-(b)[1]; (c)[2]=(a)[2]-(b)[2]
#define VecS(A,a,b) (b)[0]=(A)*(a)[0]; (b)[1]=(A)*(a)[1]; (b)[2]=(A)*(a)[2]
#define VecAddS(A,a,b,c) (c)[0]=(A)*(a)[0]+(b)[0]; (c)[1]=(A)*(a)[1]+(b)[1]; (c)[2]=(A)*(a)[2]+(b)[2]
#define VecCross(a,b,c) (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1]; (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2]; (c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
#define VecZero(v) (v)[0]=0.0;(v)[1]=0.0;v[2]=0.0
#define ABS(a) (((a)<0)?(-(a)):(a))
#define SGN(a) (((a)<0)?(-1):(1))
#define SGND(a) (((a)<0.0)?(-1.0):(1.0))
#define rnd() (float)rand() / (float)RAND_MAX
#define PI 3.14159265358979323846264338327950
#define MY_INFINITY 1000.0
//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

void init(void);
void initXWindows(void);
void init_opengl(void);
//void init_textures(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);

typedef struct t_global {
	int xres, yres;
	Flt aspectRatio;
	Vec armRot;
	Vec armAng;
	GLfloat lightPosition[4];
	int shadows;
} Global;
Global g;


int main(void)
{
	int done=0;
	init();
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
			done = check_keys(&e);
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
	XStoreName(dpy, win, "CS371 - Lab8 OpenGL Components");
}

void setup_screen_res(const int w, const int h)
{
	g.xres = w;
	g.yres = h;
	g.aspectRatio = (GLfloat)g.xres / (GLfloat)g.yres;
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
	if (dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
						StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
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
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	set_title();
}

void init(void)
{
	g.xres=640;
	g.yres=480;
	g.aspectRatio = (GLfloat)g.xres / (GLfloat)g.yres;
	MakeVector(2.0, 0.0, 0.0, g.armRot);
	MakeVector(0.0, 0.0, 0.0, g.armAng);
	//light is up high, right a little, toward a little
	MakeVector(140.0f, 240.0f, 40.0f, g.lightPosition);
	g.lightPosition[3] = 1.0f;
}

void init_opengl(void)
{
	//OpenGL initialization
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, g.aspectRatio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	//Enable this so material colors are the same as vert colors.
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	//Turn on a light
	glLightfv(GL_LIGHT0, GL_POSITION, g.lightPosition);
	glEnable(GL_LIGHT0);
}

void check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != g.xres || xce.height != g.yres) {
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

int check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		switch(key) {
			case XK_1:
				break;
			case XK_Escape:
				return 1;
		}
	}
	return 0;
}

void physics(void)
{
}

void box(float w1, float h1, float d1)
{
	float w=w1*0.5;
	float d=d1*0.5;
	float h=h1*0.5;
	//notice the normals being set
	glBegin(GL_QUADS);
		//top
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glVertex3f( w, h,-d);
		glVertex3f(-w, h,-d);
		glVertex3f(-w, h, d);
		glVertex3f( w, h, d);
		// bottom
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glVertex3f( w,-h, d);
		glVertex3f(-w,-h, d);
		glVertex3f(-w,-h,-d);
		glVertex3f( w,-h,-d);
		// front
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f( w, h, d);
		glVertex3f(-w, h, d);
		glVertex3f(-w,-h, d);
		glVertex3f( w,-h, d);
		// back
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glVertex3f( w,-h,-d);
		glVertex3f(-w,-h,-d);
		glVertex3f(-w, h,-d);
		glVertex3f( w, h,-d);
		// left side
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-w, h, d);
		glVertex3f(-w, h,-d);
		glVertex3f(-w,-h,-d);
		glVertex3f(-w,-h, d);
		// Right side
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glVertex3f( w, h,-d);
		glVertex3f( w, h, d);
		glVertex3f( w,-h, d);
		glVertex3f( w,-h,-d);
		glEnd();
	glEnd();
}

void Floor()
{
	glPushMatrix();
	glColor3f(0.6f, 0.6f, 0.6f);
	float w=5.0*0.5;
	float d=5.0*0.5;
	float h=-1.0;
	glBegin(GL_QUADS);
		//top
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glVertex3f( w, h,-d);
		glVertex3f(-w, h,-d);
		glVertex3f(-w, h, d);
		glVertex3f( w, h, d);
	glEnd();
	glPopMatrix();
}

void LightedArm()
{
	static float ang = 0.0;

	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(g.armAng[1], 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	box(1.0, 2.0, 1.0);
	//
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.6f, 1.0f, 0.0f);
	glRotatef(ang, 1.0f, 0.0f, 0.0f);
	box(0.2, 2.2, 0.6);
	glPopMatrix();
	ang += 2.2;

	g.armAng[1] += 1;
}

Flt VecNormalize(Vec vec) {
	Flt len, tlen;
	len = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] *= tlen;
	vec[1] *= tlen;
	vec[2] *= tlen;
	return(len);
}

/*
int gluInvertMatrix(const Flt m[16], Flt invOut[16])
{
    Flt inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    if (det == 0)
        return 0;
    det = 1.0 / det;
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
    return 1;
}

void matrix_inverse_33(Matrix m, Matrix r)
{
	r[0][0] =   m[1][1]*m[2][2] - m[2][1]*m[1][2];
	r[1][0] = -(m[1][0]*m[2][2] - m[2][0]*m[1][2]);
	r[2][0] =   m[1][0]*m[2][1] - m[2][0]*m[1][1];
	r[0][1] = -(m[0][1]*m[2][2] - m[2][1]*m[0][2]);
	r[1][1] =   m[0][0]*m[2][2] - m[2][0]*m[0][2];
	r[2][1] = -(m[0][0]*m[2][1] - m[2][0]*m[0][1]);
	r[0][2] =   m[0][1]*m[1][2] - m[1][1]*m[0][2];
	r[1][2] = -(m[0][0]*m[1][2] - m[1][0]*m[0][2]);
	r[2][2] =   m[0][0]*m[1][1] - m[1][0]*m[0][1];
}

void mult_light_by_inverse_matrix(Matrix m, Flt *lp)
{
	Flt m1[16];
	Matrix mi;
	matrix_inverse_33(m, mi);
	m1[ 0] = mi[0][0];
	m1[ 1] = mi[0][1];
	m1[ 2] = mi[0][2];
	m1[ 4] = mi[1][0];
	m1[ 5] = mi[1][1];
	m1[ 6] = mi[1][2];
	m1[ 8] = mi[2][0];
	m1[ 9] = mi[2][1];
	m1[10] = mi[2][2];
	m1[ 3] = m1[ 7] = m1[11] = m1[12] = m1[13] = m1[14] = 0.0f;
	m1[15] = 1.0f;
	//VMatMult(m1, lp);
	Flt res0=m1[ 0]*lp[0]+m1[ 4]*lp[1]+m1[ 8]*lp[2]+m1[12]*lp[3];
	Flt res1=m1[ 1]*lp[0]+m1[ 5]*lp[1]+m1[ 9]*lp[2]+m1[13]*lp[3];
	Flt res2=m1[ 2]*lp[0]+m1[ 6]*lp[1]+m1[10]*lp[2]+m1[14]*lp[3];
	Flt res3=m1[ 3]*lp[0]+m1[ 7]*lp[1]+m1[11]*lp[2]+m1[15]*lp[3];
	lp[0]=res0;
	lp[1]=res1;
	lp[2]=res2;
	lp[3]=res3; //Homogenous Coordinate
}

inline void matrix_bob_to_gl(Flt m1[16], const Matrix m)
{
	//------------------------
	// 0  4  8 12  x0 y0 z0 t0
	// 1  5  9 13  x1 y1 z1 t1
	// 2  6 10 14  x2 y2 z2 t2
	// 3  7 11 15   0  0  0  1
	//------------------------
	m1[ 0] = m[0][0];
	m1[ 1] = m[0][1];
	m1[ 2] = m[0][2];
	m1[ 3] = 0.0f;
	m1[ 4] = m[1][0];
	m1[ 5] = m[1][1];
	m1[ 6] = m[1][2];
	m1[ 7] = 0.0f;
	m1[ 8] = m[2][0];
	m1[ 9] = m[2][1];
	m1[10] = m[2][2];
	m1[11] = 0.0f;
	m1[12] = 0.0f;
	m1[13] = 0.0f;
	m1[14] = 0.0f;
	m1[15] = 1.0f;
}

inline void matrix_gl_to_33(Flt m1[16], Matrix m)
{
	//------------------------
	// 0  4  8 12  x0 y0 z0 t0
	// 1  5  9 13  x1 y1 z1 t1
	// 2  6 10 14  x2 y2 z2 t2
	// 3  7 11 15   0  0  0  1
	//------------------------
	m[0][0]=m1[ 0];
	m[0][1]=m1[ 1];
	m[0][2]=m1[ 2];
	m[1][0]=m1[ 4];
	m[1][1]=m1[ 5];
	m[1][2]=m1[ 6];
	m[2][0]=m1[ 8];
	m[2][1]=m1[ 9];
	m[2][2]=m1[10];
}
*/

void identity33(Matrix m)
{
	m[0][0] = m[1][1] = m[2][2] = 1.0f;
	m[0][1] = m[0][2] = m[1][0] = m[1][2] = m[2][0] = m[2][1] = 0.0f;
}

void yy_transform(const Vec rotate, Matrix a)
{
	//This function applies a rotation to a matrix.
	//Call this function first, then call trans_vector() to apply the
	//rotations to an object or vertex.
	//
	if (rotate[0] != 0.0f) {
		Flt ct = cos(rotate[0]), st = sin(rotate[0]);
		Flt t10 = ct*a[1][0] - st*a[2][0];
		Flt t11 = ct*a[1][1] - st*a[2][1];
		Flt t12 = ct*a[1][2] - st*a[2][2];
		Flt t20 = st*a[1][0] + ct*a[2][0];
		Flt t21 = st*a[1][1] + ct*a[2][1];
		Flt t22 = st*a[1][2] + ct*a[2][2];
		a[1][0] = t10;
		a[1][1] = t11;
		a[1][2] = t12;
		a[2][0] = t20;
		a[2][1] = t21;
		a[2][2] = t22;
		return;
	}
	if (rotate[1] != 0.0f) {
		Flt ct = cos(rotate[1]), st = sin(rotate[1]);
		Flt t00 = ct*a[0][0] - st*a[2][0];
		Flt t01 = ct*a[0][1] - st*a[2][1];
		Flt t02 = ct*a[0][2] - st*a[2][2];
		Flt t20 = st*a[0][0] + ct*a[2][0];
		Flt t21 = st*a[0][1] + ct*a[2][1];
		Flt t22 = st*a[0][2] + ct*a[2][2];
		a[0][0] = t00;
		a[0][1] = t01;
		a[0][2] = t02;
		a[2][0] = t20;
		a[2][1] = t21;
		a[2][2] = t22;
		return;
	}
	if (rotate[2] != 0.0f) {
		Flt ct = cos(rotate[2]), st = sin(rotate[2]);
		Flt t00 = ct*a[0][0] - st*a[1][0];
		Flt t01 = ct*a[0][1] - st*a[1][1];
		Flt t02 = ct*a[0][2] - st*a[1][2];
		Flt t10 = st*a[0][0] + ct*a[1][0];
		Flt t11 = st*a[0][1] + ct*a[1][1];
		Flt t12 = st*a[0][2] + ct*a[1][2];
		a[0][0] = t00;
		a[0][1] = t01;
		a[0][2] = t02;
		a[1][0] = t10;
		a[1][1] = t11;
		a[1][2] = t12;
		return;
	}
}

void trans_vector(Matrix mat, const Vec in, Vec out)
{
	Flt f0 = mat[0][0] * in[0] + mat[1][0] * in[1] + mat[2][0] * in[2];
	Flt f1 = mat[0][1] * in[0] + mat[1][1] * in[1] + mat[2][1] * in[2];
	Flt f2 = mat[0][2] * in[0] + mat[1][2] * in[1] + mat[2][2] * in[2];
	out[0] = f0;
	out[1] = f1;
	out[2] = f2;
}



#define SHOW_TEST_QUAD

#ifdef SHOW_TEST_QUAD
#define setv(a,b) \
a[0]=MY_INFINITY*(b[0]-g.lightPosition[0])+g.lightPosition[0];\
a[1]=MY_INFINITY*(b[1]-g.lightPosition[1])+g.lightPosition[1];\
a[2]=MY_INFINITY*(b[2]-g.lightPosition[2])+g.lightPosition[2]
Vec tquad[4]={
{0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 2.0f},
{2.0f, 0.0f, 2.0f},
{2.0f, 0.0f, 0.0f }};
Flt r1=0.0;
static float mdir=0.02, m1=0.0;
Matrix qmat;
Vec qtrans;
Flt qlight[4];
Vec qvert[4];
Vec aquad[4];
Vec bquad[4];

void quad_shadow_volume(void)
{
	glPushMatrix();
	glBegin(GL_QUADS);
		//back cap
		glVertex3fv(aquad[0]);
		glVertex3fv(aquad[3]);
		glVertex3fv(aquad[2]);
		glVertex3fv(aquad[1]);
		//front cap
		glVertex3fv(bquad[0]);
		glVertex3fv(bquad[1]);
		glVertex3fv(bquad[2]);
		glVertex3fv(bquad[3]);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex3fv(bquad[0]); glVertex3fv(aquad[0]);
		glVertex3fv(bquad[1]); glVertex3fv(aquad[1]);
		glVertex3fv(bquad[2]); glVertex3fv(aquad[2]);
		glVertex3fv(bquad[3]); glVertex3fv(aquad[3]);
		glVertex3fv(bquad[0]); glVertex3fv(aquad[0]);
	glEnd();
	glPopMatrix();
}

void draw_shadow(void)
{
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 1, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	//glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, 1);
		glVertex2i(1, 1);
		glVertex2i(1, 0);
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void render_shadows(void)
{
	//disable writing of frame buffer color components.
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//disable writing into the depth buffer.
	glDepthMask(GL_FALSE);
	//enable culling of back-facing polygons. 	
	glEnable(GL_CULL_FACE);
	//enable stencil testing.
	glEnable(GL_STENCIL_TEST);
	//specify a depth offset for polygon rendering.
	glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(-0.002f, 1.0f);
	glPolygonOffset(0.01f, 1.0f);
	//enable culling of front-facing polygons. 	
	glCullFace(GL_FRONT);
	//set front and back function and reference value for stencil testing.
	glStencilFunc(GL_ALWAYS, 0x0, 0xff);
	//set front and back stencil test actions.
	glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
	//-------------------------------------------------------------------------
	//render the shadow surfaces.
	quad_shadow_volume();
	//-------------------------------------------------------------------------
	//enable culling of back-facing polygons. 	
	glCullFace(GL_BACK);
	//set front and back function and reference value for stencil testing.
	glStencilFunc(GL_ALWAYS, 0x0, 0xff);
	//set front and back stencil test actions.
	glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
	//-------------------------------------------------------------------------
	//render the shadow surfaces.
	quad_shadow_volume();
	//-------------------------------------------------------------------------
	//reset for rendering to screen
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_CULL_FACE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	//
	//set front and back function and reference value for stencil testing.
	glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);
	//set front and back stencil test actions.
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	//draw the whole stencil buffer to the screen, revealing the shadows.
	draw_shadow();
	//disable stencil testing.
	glDisable(GL_STENCIL_TEST);
}
#endif //SHOW_TEST_QUAD


void render(void)
{
	Rect r;
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//
	//3D mode
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	gluPerspective(45.0f, g.aspectRatio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,5,8,  0,0,0,  0,1,0);
	glLightfv(GL_LIGHT0, GL_POSITION, g.lightPosition);
	//
	Floor();
	LightedArm();
	//
	#ifdef SHOW_TEST_QUAD
		int i;
		glPushMatrix();
		//save translation in a variable.
		MakeVector(m1, 2.4f, 0.0f, qtrans);
		//apply opengl translation.
		glTranslatef(qtrans[0],qtrans[1],qtrans[2]);
		//apply opengl rotation, for drawing the quad itself.
		glRotatef(r1, 0.0f, 1.0f, 0.0f);
		//build our own rotation matrix for rotating the shadow.
		Vec rot={0.0, r1/180.0*PI, 0.0};
		identity33(qmat);
		//transform the matrix
		yy_transform(rot, qmat);
		//transform the vertices of the quad, to use for the shadow
		for (i=0; i<4; i++) {
			//save original vertices.
			VecCopy(tquad[i], qvert[i]);
			//transform to new vertices.
			trans_vector(qmat, qvert[i], qvert[i]);
			//add in the translation.
			VecAdd(qvert[i], qtrans, qvert[i]);
			//form the shadow quads, using the light source.
			VecCopy(qvert[i], bquad[i]);
			setv(aquad[i], bquad[i]);
		}
		//
		//adjust the movement. (this should be moved to physics)
		m1 += mdir;
		if (m1 > 1.5) mdir *= -1.0;
		if (m1 < -1.5) mdir *= -1.0;
		//continue the rotation going.
		r1 += 0.2;
		//
		//now, draw the opengl object on screen.
		glColor3ub(220, 220, 220);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);
			for (i=0; i<4; i++)
				glVertex3fv(tquad[i]);
		glEnd();
		glPopMatrix();
		//
		//now, render all shadows.
		render_shadows();
	#endif

	//switch to 2D mode
	//
	glViewport(0, 0, g.xres, g.yres);
	glMatrixMode(GL_MODELVIEW);   glLoadIdentity();
	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	gluOrtho2D(0, g.xres, 0, g.yres);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	r.bot = g.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00887766, "CS-371 Lab");
	ggprint8b(&r, 16, 0x00ffff00, "S Shadows ");
	//
	glPopAttrib();
}



