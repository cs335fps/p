#ifndef _VIEW_H_
#define _VIEW_H_

#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <iostream>
#include <cstdlib>
#include "game.h"

#define PI 3.14159

class View
{
private:
    int width;
    int height;
    Display *dpy;
    Window root;
    Window win;
    GLXContext glc;
    Game *game;
    
public:
    
    int zoom;
    float depth;
    
    View(Game *g);
    void InitOpenGL();
    void InitWindow();
    void cleanupXWindows(void);
    void set_title(void);
    void ShowCursor(const int onoff);
    void CenterCursor();
    Display *GetDisplay();
    int GetWidth();
    int GetHeight();
    void Render();
    void SwitchTo3D();
    void SwitchTo2D();
    void HUD();
};


#endif
