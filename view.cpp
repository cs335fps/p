#include "view.h"

View::View(Game *g, int w, int h)
{
    wOverride = w;
    hOverride = h;
    is3D = -1;
    InitWindow();
    SwitchTo3D();
    game = g;
    level1.load("Level1.obj");
    mobs.push_back(new Mob());
}

int View::GetWidth()
{
    return width;
}

int View::GetHeight()
{
    return height;
}

void View::InitWindow()
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        std::cout << "\n\tcannot connect to X server\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    root = DefaultRootWindow(dpy);

    XWindowAttributes getWinAttr;
    XGetWindowAttributes(dpy, root, &getWinAttr);

    XGrabKeyboard(dpy, root,
            False, GrabModeAsync, GrabModeAsync, CurrentTime);

    if (wOverride == 0 || hOverride == 0) {
        width = getWinAttr.width;
        height = getWinAttr.height;
    } else {
        width = wOverride;
        height = hOverride;
    }

    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if(vi == NULL) {
        std::cout << "\n\tno appropriate visual found\n" << std::endl;
        exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPress | ButtonReleaseMask |
        PointerMotionMask |
        StructureNotifyMask | SubstructureNotifyMask;
    swa.override_redirect = True;

    win = XCreateWindow(dpy, root, 0, 0, width, height,
            0, vi->depth, InputOutput, vi->visual,
            CWBorderPixel|CWColormap|CWEventMask|
            CWOverrideRedirect, &swa);

    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    CenterCursor();
}



void View::set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "First Person Strudel: Toaster Genesis");
}

void View::cleanupXWindows(void)
{
    //do not change
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void View::ShowCursor(const int onoff)
{
    // Source: Gordon's asteroids.cpp
    if (onoff) {
        //this removes our own blank cursor.
        XUndefineCursor(dpy, win);
        return;
    }
    //vars to make blank cursor
    Pixmap blank;
    XColor dummy;
    char data[1] = {0};
    Cursor cursor;
    //make a blank cursor
    blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
    if (blank == None){
        std::cout << "error: out of memory." << std::endl;
        throw 0;
    }
    cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(dpy, blank);
    //this makes you the cursor. then set it using this function
    XDefineCursor(dpy, win, cursor);
    //after you do not need the cursor anymore use this function.
    //it will undo the last change done by XDefineCursor
    //(thus do only use ONCE XDefineCursor and then XUndefineCursor):
}

void View::CenterCursor()
{
    XWarpPointer(dpy,root,root,0,0,width,height,
            width/2,height/2);
}

Display *View::GetDisplay()
{
    return dpy;
}

void View::Render()
{
    SwitchTo3D();
    for(unsigned int i = 0; i < mobs.size(); i++){
        mobs[i]->render();
    }
    float rotx = game->direction.x;
    float roty = game->direction.y - PI / 2.0;


    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    gluLookAt(
            game->position.x,
            game->position.y,
            game->position.z,
            game->position.x+sin(rotx) * sin(roty),
            game->position.y+cos(roty),
            game->position.z+cos(rotx) * sin(roty),
            0,1,0
            );

    Lighting();

    glPushMatrix();
    //level1.draw();

    for (unsigned int i = 0; i < game->walls.size(); i++) {
        game->walls[i].Draw();
    }

    glPopMatrix();
    HUD();
    //drawFloor();
    //drawLightedArm();
    // level1.loc(2,2,2);

    glFlush ();

    glXSwapBuffers(dpy, win);
}

void View::HUD()
{
    int w = width;
    int h = height;
    SwitchTo2D();
    int l = h / 1;

    glBegin(GL_LINES);
    glColor3f(1.0f,1.0f,1.0f);
    glVertex2d(w / 2 - l / 20, h / 2);
    glVertex2d(w / 2 + l / 20, h / 2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(w / 2, h / 2 - l / 20);
    glVertex2d(w / 2, h / 2 + l / 20);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2d(w / 2 - l / 20, h / 2+1);
    glVertex2d(w / 2 + l / 20, h / 2+1);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(w / 2+1, h / 2 - l / 20);
    glVertex2d(w / 2+1, h / 2 + l / 20);
    glEnd();


}

void View::Lighting()
{


    // setup the position, specular and shininess of the light
    GLfloat mat_spec[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shini[]= { 50.0};
    GLfloat ambiant[] =  {0.1484, 0.0, 0.5781};
    GLfloat diff[] =  {1.0, 1.0, 1.0, 0.5};
    //GLfloat light_Pos[]= {0, 8, 0, 1.0};

    GLfloat light_Pos[]= {0, 10, 0, 0.9};
    GLfloat light_Pos2[]= {
        game->position.x, 
        game->position.y, 
        game->position.z, 
        0.5};
    // apply the shinynes, specular and light position
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shini);
    glLightfv(GL_LIGHT0, GL_POSITION, light_Pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light_Pos2);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiant);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void View::SwitchTo3D()
{
    if (is3D == 1)
        return;


    ShowCursor(0);
    //OpenGL initialization
    // move the camera
    glViewport(0, 0, width, height);
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
    glDepthMask(GL_TRUE);
    // This Will Clear The Background Color To Black
    glClearColor(0.549f, 0.549f, 0.90588f, 0.0f);		
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);			        // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);		        // Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    //gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,900.0f);	
    glFrustum(-0.1/height*width, 0.1/height*width,
            -0.1, 0.1, game->depth, 200.0);



    is3D = 1;
}

void View::SwitchTo2D()
{
    if (is3D == 0)
        return;


    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width,0,height,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_DEPTH_BUFFER_BIT);
    is3D = 0;

}
View::~View()
{
}

