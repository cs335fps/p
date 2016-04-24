#include "view.h"

View::View(Game *g)
{
    InitWindow();
    InitOpenGL();
    game = g;
    depth = .15f;
    zoom = 0;
    maxZoom = .55f;
    minZoom = .15f;
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
    
    width = getWinAttr.width;
    height = getWinAttr.height;
    
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

void View::InitOpenGL()
{
    
    //OpenGL initialization
    glViewport(0, 0, width, height);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    
    glFrustum(-0.1/height*width, 0.1/height*width, -0.1, 0.1, depth, 200.0);
    //Set the screen background color
    glClearColor(1.0, 1.0, 1.0, 1.0);
    ///////////////////////////////////
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_COLOR_MATERIAL);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    ShowCursor(0);
    
    
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    //glDepthFunc(GL_LESS);
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.7, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void View::set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Test");
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
    if (blank == None)
        std::cout << "error: out of memory." << std::endl;
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
    float rotx = game->direction.x;
    float roty = game->direction.y;
    if (zoom == 1 && depth < maxZoom) {
        depth += (maxZoom - minZoom) / 15;
    }else if (zoom == 0 && depth > minZoom) {
        depth -= (maxZoom - minZoom) / 15;
    }
    
    
    float ra = 20.0;
    //Rect r;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float top = 4.0;
    float bot = -2.0;
    
    Vec v[6];
    Vec c[6];
    
    c[0] = Vec(0.5, 0.5, 1.0);
    c[1] = Vec(1.0, 1.0, 1.0);
    c[2] = Vec(1.0, 0.0, 0.0);
    c[3] = Vec(0.0, 0.0, 1.0);
    c[4] = Vec(0.0, 1.0, 0.0);
    c[5] = Vec(1.0, 1.0, 0.0);
    
    v[0].x = v[3].x = v[7].x = v[4].x = ra;
    v[1].x = v[2].x = v[5].x = v[6].x = -ra;
    
    v[0].y = v[1].y = v[2].y = v[3].y = bot;
    v[4].y = v[5].y = v[6].y = v[7].y = top;
    
    v[0].z = v[1].z = v[5].z = v[4].z = -ra;
    v[2].z = v[3].z = v[6].z = v[7].z = ra;
    
    int verts[][4] = {
        {4,5,6,7},
        {3,2,1,0},
        {0,1,5,4},
        {7,6,2,3},
        {1,2,6,5},
        {4,7,3,0}
        
    };
    glPushMatrix();
    
    glRotatef(roty / PI * 180.0,1,0,0);
    glRotatef(rotx / PI * 180.0,0,1,0);
    glTranslatef(game->position.x, game->position.y, game->position.z);
    
    for (int i = 0; i < 6; i++) {
        glColor3f(c[i].x,c[i].y,c[i].z);
        glBegin(GL_POLYGON);
        Vec N = -Normal(v[verts[i][0]],v[verts[i][1]],v[verts[i][2]]);
        glNormal3f(N.x,N.y,N.z);
        for (int j = 0; j < 4; j++){
            glVertex3f(v[verts[i][j]].x,
                       v[verts[i][j]].y,
                       v[verts[i][j]].z);
        }
        glEnd();
        glDisable(GL_LIGHTING);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f((v[verts[i][0]].x+v[verts[i][2]].x)/2.0 + N.x,
                   (v[verts[i][0]].y+v[verts[i][2]].y)/2.0 + N.y,
                   (v[verts[i][0]].z+v[verts[i][2]].z)/2.0 + N.z);
        glVertex3f((v[verts[i][0]].x+v[verts[i][2]].x)/2.0,
                   (v[verts[i][0]].y+v[verts[i][2]].y)/2.0,
                   (v[verts[i][0]].z+v[verts[i][2]].z)/2.0);
        glEnd();
        glEnable(GL_LIGHTING);
    }
    
    
    glPopMatrix();
    HUD();
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
    
    SwitchTo3D();
}

void View::SwitchTo3D()
{
    // Making sure we can render 3d again
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_LIGHTING);
    glFrustum(-0.1/height*width, 0.1/height*width, -0.1, 0.1, depth, 200.0);
    
}

void View::SwitchTo2D()
{
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);  
    glDisable(GL_LIGHTING);
    
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    
    glClear(GL_DEPTH_BUFFER_BIT);
}
