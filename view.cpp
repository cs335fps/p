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
    game->stPor1.assignTexA(lbmp.getBMP("STATION2portalA_tex.bmp"));
    game->stPor1.assignTexB(lbmp.getBMP("STATION2portalB_tex.bmp"));
    game->stPor2.assignTexA(lbmp.getBMP("STATIONportalA_tex.bmp"));
    game->stPor2.assignTexB(lbmp.getBMP("STATIONportalB_tex.bmp"));
    Win = lbmp.getBMP("Win.bmp");
    Lose = lbmp.getBMP("Lose.bmp");
    keys = lbmp.getBMP("keys.bmp");
    logo = lbmp.getBMP("logo.bmp");
    sheet = lbmp.getBMP("sheet.bmp");
    wallTex[0] = lbmp.getBMP("wall1.bmp");
    wallTex[1] = lbmp.getBMP("wall2.bmp");
    floorTex = lbmp.getBMP("floor1.bmp");

    game->defaultPortl.locA(-20,-20,20);
    game->defaultPortl.locA(20,-20,20);
    game->stPor2.locA(40,2,-30);
    game->stPor2.locB(-50,2,40);
    game->stPor1.locA(30,2,20);
    game->stPor1.locB(-10,2,-30);

    for (unsigned int i = 0; i < game->mobs.size(); i++) {
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
    if (blank == None) {
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
    //if (level1.isTouching(game->position.x,
    //     game->position.y, game->position.z, 2.3)) {
    //     game->position.x = ox;
    //     game->position.y = oy;
    //     game->position.z = oz;
    //    }else{
    ox = game->position.x;
    oy = game->position.y;
    oz = game->position.z;
    //    }

    glPushMatrix();


    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, skyTex);
    game->sky.draw(ox,oy,oz);

    if (game->togPortal == 1) {
        game->defaultPortl.draw();
        game->stPor1.draw();
        game->stPor2.draw();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_LIGHTING);

    for (unsigned int i = 0; i < game->walls.size(); i++) {
        game->walls[i].render(wallTex);
    }

    for (unsigned int i = 0; i < game->mobs.size(); i++) {
        if (game->mobs[i]->getTex() == 0)
            game->mobs[i]->setTex(this->mobTex);

        game->mobs[i]->render();
        //glBindTexture(GL_TEXTURE_2D, 0);
    }

    for (unsigned int i = 0; i < game->bullets.size(); i++) {
        game->bullets[i].render();
    }

    for (unsigned int i = 0; i < game->bulletHoles.size(); i++) {
        if (game->partyMode)
            game->bulletHoles[i].render2();
        else
            game->bulletHoles[i].render();
    }

    float fl = 200.0;
    glColor3f(.5,.5,.5);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, floorTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_POLYGON);
    glTexCoord2f(fl/4.0,fl/4.0);
    glVertex3f(fl,0,fl);
    glTexCoord2f(-fl/4.0,fl/4.0);
    glVertex3f(-fl,0,fl);
    glTexCoord2f(-fl/4.0,-fl/4.0);
    glVertex3f(-fl,0,-fl);
    glTexCoord2f(fl/4.0,-fl/4.0);
    glVertex3f(fl,0,-fl);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_BLEND);
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
    if (game->dmgAnim > 0) {
        glColor4f(1,0,0,((float)game->dmgAnim / 20.0) * 0.5);
        glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(0, height);
        glVertex2f(width, height);
        glVertex2f(width, 0);
        glEnd( );
    }

    if (game->gameRunning == 0) {
        float imgw = width * 0.8;
        float imgh = width * 0.4;
        float iox = (width - imgw) / 2.0;
        float ioy = (height - imgh) / 2.0;
        for (int i = 0; i < 2; i++) {
            float tr = (game->timer->Get() - game->lastTime - 5.0) / 2.0;
            if (tr < 0.0) tr = 0.0;
            if (tr > 1.0) tr = 1.0;
            if (i == 0) {
                if (tr == 0.0)
                    continue;
                glBindTexture(GL_TEXTURE_2D, keys);
                glColor4f(1.0f,1.0f,1.0f,1.0f);
            } else { 
                glBindTexture(GL_TEXTURE_2D, logo);
                glColor4f(1.0f,1.0f,1.0f,1.0f-tr);
            }
            
            glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(imgw + iox, imgh + ioy);

            glTexCoord2f(1.0f, 0.0f);
            glVertex2f( imgw + iox, ioy);

            glTexCoord2f(0.0f, 0.0f);
            glVertex2f( iox, ioy);

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f( iox, imgh + ioy);

            glBindTexture(GL_TEXTURE_2D, 0);
            glEnd();
        }
        // display that the user won or lost
    } else if (game->mobs.size() == 0 || game->playerHP == 0) {
        if (game->playerHP == 0) {
            //game->renderGameOver(width, height, Lose);
        }else{
            game->renderGameOver(width, height, Win);
        }
        glColor4f(0,0,0,.5);
        glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(0,height);
        glVertex2f(width,height);
        glVertex2f(width,0);
        glEnd();
        PrintText("GAME OVER!", width / 2,
                height * 0.8, height * 0.1, sheet, 1);
        if (game->servMessage.size() > 1) {

            for (unsigned int i = 0; i < game->servMessage.size(); i++) {
                vector<string> v = Split(game->servMessage[i],"~");
                if (v.size() == 2) {
                    PrintText(v[0], width / 2,
                            height * 0.7 - (double)0.075 * (i) * height, 
                            height * 0.05, sheet, 2);
                    PrintText(v[1], width / 2,
                            height * 0.7 - (double)0.075 * (i) * height, 
                            height * 0.05, sheet, 0);
                } else {
                    PrintText(v[0], width / 2,
                            height * 0.7 - (double)0.075 * (i) * height, 
                            height * 0.05, sheet, 1);
                }
            }
            return;
        }
    } else {
        DrawHealth(game, width, height);
        DrawAmmo(game, width, height);

        if (game->lkey == 1)
            Lizandrokey(game,width, height);
        else
            DrawCrosshairs(game,width,height);
        GameMenu(game,width,height);
        reloadMessage(game,width,height);


    }
    char buf[16];
    sprintf(buf,"%d",game->nkills);
    string s = "KILLS:" + string(buf);
    PrintText(s,height * .05,height * .05,height * 0.05,sheet);
    if (game->guntype == 0)
        s = "9MM";
    else if (game->guntype == 1)
        s = "SNIPER";
    else
        s = "SHOTGUN";
    PrintText(s,width / 2,height * .05,height * 0.05,sheet,1);
    s = string(game->name);
    PrintText(s,width - height * .05,height * .05,height * 0.05,sheet,2);
}

void View::Lighting()
{
    // setup the position, specular and shininess of the light
    GLfloat mat_spec[] = {1.0, 1.0, 1.0, 0.2};
    GLfloat mat_shini[]= { 5.0};
    GLfloat ambiant[] =  {0.3, 0.3, 0.3};
    GLfloat diff[] =  {1.0, 1.0, 1.0, 0.5};
    GLfloat spec[] = { 1.0, 1.0, 1.0, 0.5 };
    //GLfloat light_Pos[]= {0, 8, 0, 1.0};

    GLfloat light_Pos[]= {100, 100, 100, 0.9};
    GLfloat light_Pos2[]= {-100, 100, -100, 0.9};
    /*GLfloat light_Pos2[]= {
      game->position.x, 
      game->position.y, 
      game->position.z, 
      0.5};*/
    // apply the shinynes, specular and light position
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shini);
    glLightfv(GL_LIGHT0, GL_POSITION, light_Pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light_Pos2);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiant);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambiant);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec);

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
    glClearDepth(1.0);              // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);           // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);        // Enables Depth Testing
    glShadeModel(GL_SMOOTH);        // Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();               // Reset The Projection Matrix

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




