#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "solidSphere.h"

using namespace std;

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
void check_keys(XEvent *e);
void physics(void);
void render(void);
void InitGL5(int Width, int Height);
void DrawGLScene5();
GLuint getBMP(const char *path);
void setOrth();
void menu();
void planetInfoScreen();

bool done= false;


/* ASCII code for the escape key. */
#define ESCAPE 27

/* The number of our GLUT window */
int window, 
    xres=500, yres=500; 

/* rotation angle for the triangle. */
float rtri = 0.0f, mvx =0, mvy=0, mvz=0;

float savex = 0, savey = 0;

solidSphere sp(1,32,32);

// array of textures
GLuint planTex[13], VITex, VITexAn, VITexLA, VIName;

int main(int arg, char **argc)
{
	initXWindows();
	init_opengl();

	while(!done){
		while(XPending(dpy)){
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			check_mouse(&e);
			check_keys(&e);
		}
		//physics();
		render();
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	return 0;
}
void cleanupXWindows()
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "CS335 - OpenGL FPS");
}

void setup_screen_res(const int w, const int h)
{
	xres = w;
	yres = h;
}

void initXWindows()
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
		cout <<"\n\tcannot connect to X server\n\n";
		exit(EXIT_FAILURE);
	}
	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);
	if(vi == NULL) {
		cout << "\n\tno appropriate visual found\n\n";
		exit(EXIT_FAILURE);
	} 
	//else {
	//	// %p creates hexadecimal output like in glxinfo
	//	printf("\n\tvisual %p selected\n", (void *)vi->visualid);
	//}
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPress |
		ButtonReleaseMask | EnterWindowMask | LeaveWindowMask |
		PointerMotionMask | Button1MotionMask |
		ColormapChangeMask | StructureNotifyMask | SubstructureNotifyMask;
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

void init_opengl()
{
	// setup the position, specular and shininess of the light
	GLfloat mat_spec[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shini[]= { 50.0};
	GLfloat ambiant[] =  {0.1484, 0.0, 0.5781};
	//OpenGL initialization
	// move the camera
	glViewport(0, 0, xres, yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	//glOrtho(0, xres, 0, yres, -1, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);			        // The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);		        // Enables Depth Testing
	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,900.0f);	// Calculate The Aspect Ratio Of The Window

	// apply the shinynes, specular and light position
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shini);
	GLfloat light_Pos[]= {1, 1, 1, .0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiant);

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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
	init_opengl();
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int draged =0;
	//
	if (e->type == ButtonRelease) {
		draged =0;
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			draged = 1;
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != (float)e->xbutton.x || savey != (float)e->xbutton.y) {

		if(draged ==1){
		}
		//Mouse moved
		savex = (e->xbutton.x) / xres;
		savey = (e->xbutton.y) / yres;
	}
}

void check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		switch(key){
			case XK_n:
				break;
			case XK_t:
				break;
			case XK_Escape:
				done = true;
				break;
			case XK_1:
				break;
			case XK_2:
				break;
			case XK_3:
				break;
			case XK_4:
				break;
			case XK_5:
				break;
			case XK_6:
				break;
			case XK_7:
				break;
			case XK_8:
				break;
			case XK_9:
				break;
			case XK_0:
				break;
			case XK_c:
				break;
			case XK_q:
				break;
			case XK_a:
				break;
			case XK_d:
				break;
			case XK_s:
				break;
			case XK_w:
				break;
			case XK_Left:
				mvx += 0.1;
				break;
			case XK_Right:
				mvx += -0.1;
				break;
			case XK_Up:
				mvz += 0.1;
				break;
			case XK_Down:
				mvz += -0.1;
				break;
			case XK_m:
				break;
			case XK_b:
				break;
			case XK_equal:
				break;
			case XK_minus:
				break;
			case XK_F1:
				break;
			case XK_F2:
				break;
		}
	}
}
void physics()
{
}
void render()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();	

	sp.draw(0,0,0);
	
	setOrth();

	// reinicalize perspective mode ////////////////////////////////////////////////////////////////
	// ////////////////////////////////////////////////////////////////////////////////////////////
	init_opengl();
}
////////////////////////////////////////////////////////////////////////////////////////

GLuint getBMP(const char *path)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize, wAlpha;
	int pixel;

	FILE * file = fopen(path,"rb");
	if(!file){
		printf("UNABLE TO OPEN %s\n", path);
		return 0;
	}

	if(fread(header,1,54,file) != 54){
		printf("NOT A BMP FILE\n");
		return 0;
	}
	if(header[0] != 'B' || header[1] != 'M'){
		printf("NOT A BMP FILE\n");
		return 0;
	}
	// read image header imformation
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// if the image header is corupted somehow
	if(imageSize == 0){
		imageSize = width * height * 3;
	}
	if(dataPos == 0)
		dataPos = 54;

	// get image data
	pixel = width * height;
	wAlpha = width * height * 4;
	unsigned char *data = new unsigned char[imageSize];
	unsigned char *data_A = new unsigned char[wAlpha];
	fread(data, 1, imageSize, file);
	fclose(file);

	// apply apha channel
	for(int i=0; i< pixel; i++){
		data_A[i*4] = data[i*3];
		data_A[i*4+1] = data[i*3+1];
		data_A[i*4+2] = data[i*3+2];
		if(data[i*3]== 0 and data[i*3+1]==0 and data[i*3+2]==0){
			data_A[i*4+3] = 0;
		}else
			data_A[i*4+3] = 250;
	}

	// format image to gl format
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data_A);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	delete data;
	delete data_A;
	return textureID;
}
void setOrth()
{
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, xres, yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,xres,0,yres,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
