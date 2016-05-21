#include "view.h"

View::View(Game *g, int w, int h)
{
    loadBMP lbmp;
    wOverride = w;
    hOverride = h;
    is3D = -1;
    InitWindow();
    SwitchTo3D();
    game = g;
    depth = game->depth;
    ox = oy = oz =0;
    this->mobTex = lbmp.getBMP("enemy.bmp");
    game->sky.redraw(100,30,30,true );
    skyTex = lbmp.getBMP("sky.bmp");
    game->defaultPortl.assignTexA(lbmp.getBMP("portalA_tex.bmp"));
    game->defaultPortl.assignTexB(lbmp.getBMP("portalB_tex.bmp"));


    for(unsigned int i = 0; i < game->mobs.size(); i++){
        game->mobs[i]->setTex(mobTex);
    }
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
    if (vi == NULL) {
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
    depth = game->depth;
    SwitchTo3D();

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
    //	if (level1.isTouching(game->position.x,
    //       game->position.y, game->position.z, 2.3)){
    //     	game->position.x = ox;
    //     	game->position.y = oy;
    //     	game->position.z = oz;
    //    }else{
    ox = game->position.x;
    oy = game->position.y;
    oz = game->position.z;
    //    }

    glPushMatrix();
    if (game->togPortal == 1){
        game->defaultPortl.draw();
    }

    glBindTexture(GL_TEXTURE_2D, skyTex);
    game->sky.draw(ox,oy,oz);
    glBindTexture(GL_TEXTURE_2D, 0);

    for (unsigned int i = 0; i < game->walls.size(); i++) {
        game->walls[i].render();
    }

    for(unsigned int i = 0; i < game->mobs.size(); i++){
        // These textures should be set in the mob draw
        // function where it knows what itself is.
	if(game->mobs[i]->getTex() == 0)
	    game->mobs[i]->setTex(this->mobTex);
        game->mobs[i]->render();
        //glBindTexture(GL_TEXTURE_2D, 0);
    }

    for (unsigned int i = 0; i < game->bullets.size(); i++) {
        game->bullets[i].render();
    }
    
    for (unsigned int i = 0; i < game->bulletHoles.size(); i++) {
        game->bulletHoles[i].render();
    }

    float fl = 200.0;
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(fl,0,fl);
    glVertex3f(-fl,0,fl);
    glVertex3f(-fl,0,-fl);
    glVertex3f(fl,0,-fl);
    glEnd();

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
    SwitchTo2D();

    DrawCrosshairs(game,width,height);
    GameMenu(game,width,height);
    reloadMessage(game,width,height);
}

void View::Lighting()
{
    // setup the position, specular and shininess of the light
    GLfloat mat_spec[] = {1.0, 1.0, 1.0, 0.2};
    GLfloat mat_shini[]= { 5.0};
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
            -0.1, 0.1, depth, 200.0);

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

